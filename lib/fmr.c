#include "libflipper.h"
#include "fmr.h"
#include <string.h>
#include <stdlib.h>

#define MAX_DISPATCHERS FMR_CLASS_COUNT

/* Dispatcher table, initialized to NULLs */
static int (*fmr_registry[MAX_DISPATCHERS])(struct _lf_device *, const struct _fmr_packet *, lf_return_t *) = { 0 };

/* Register a handler for a given FMR class */
int fmr_register(fmr_class type, int (*handler)(struct _lf_device *, const struct _fmr_packet *, lf_return_t *)) {
    if (type >= MAX_DISPATCHERS || !handler) return lf_error;
    fmr_registry[type] = handler;
    return lf_success;
}

/* Forward declarations of built-in handlers */
static int fmr_rpc(struct _lf_device *, const struct _fmr_packet *, lf_return_t *);
static int fmr_push(struct _lf_device *, const struct _fmr_packet *, lf_return_t *);
static int fmr_pull(struct _lf_device *, const struct _fmr_packet *, lf_return_t *);
static int fmr_dyld(struct _lf_device *, const struct _fmr_packet *, lf_return_t *);
static int fmr_malloc(struct _lf_device *, const struct _fmr_packet *, lf_return_t *);
static int fmr_free(struct _lf_device *, const struct _fmr_packet *, lf_return_t *);

/* Register built-in dispatchers */
void fmr_init(void) {
    fmr_register(fmr_rpc_class,    fmr_rpc);
    fmr_register(fmr_push_class,   fmr_push);
    fmr_register(fmr_pull_class,   fmr_pull);
    fmr_register(fmr_dyld_class,   fmr_dyld);
    fmr_register(fmr_malloc_class, fmr_malloc);
    fmr_register(fmr_free_class,   fmr_free);
}

int fmr_perform(struct _lf_device *device, struct _fmr_packet *packet) {
    struct _fmr_header *hdr = &packet->hdr;
    struct _fmr_result result;
    lf_return_t retval = -1;
    lf_crc_t _crc, crc;
    int e = E_UNIMPLEMENTED;

    lf_assert(hdr->magic == FMR_MAGIC_NUMBER, E_CHECKSUM, "invalid magic number");

    _crc = hdr->crc;
    hdr->crc = 0;
    lf_crc(packet, hdr->len, &crc);
    lf_assert(!memcmp(&_crc, &crc, sizeof(crc)), E_CHECKSUM, "checksums do not match (0x%04x/0x%04x)", _crc, crc);

    lf_error_set(E_OK);

    lf_assert(hdr->type < MAX_DISPATCHERS, E_FMR, "unknown header type '%d'", hdr->type);
    if (!device || !device->modules) {
        fprintf(stderr, "[fmr_perform] ERROR: device or device->modules is NULL\n");
    }

    if (!fmr_registry[hdr->type]) {
        fprintf(stderr, "[fmr_perform] ERROR: no registered handler for type %d\n", hdr->type);
        goto fail;
    }

    int dispatch_result = fmr_registry[hdr->type](device, packet, &retval);
    if (!dispatch_result) {
        fprintf(stderr, "[fmr_perform] Dispatcher for type %d failed.\n", hdr->type);
    }
    lf_assert(dispatch_result, E_FMR, "failed to dispatch call");

fail:
    result.error = lf_error_get();
    result.value = retval;
    e = device->write(device, &result, sizeof(result));
    lf_debug_result(&result);
    return e;
}

/* Helper struct definitions */
typedef uint32_t lf_size_t;

struct _fmr_rpc {
    uint8_t module;
    uint8_t index;
};

struct _fmr_push {
    uint8_t module;
    uint8_t index;
    uint8_t length;
};

struct _fmr_pull {
    uint8_t module;
    uint8_t index;
    uint8_t length;
};

/* External function declarations */
extern struct _lf_module *lf_module_get_by_name(struct _lf_device *device, const char *name);

/* fmr_rpc: Dispatches a remote procedure call to a target module and function */
static int fmr_rpc(struct _lf_device *device, const struct _fmr_packet *packet, lf_return_t *retval) {
    const uint8_t *data = (const uint8_t *)(packet + 1);
    struct _fmr_rpc *rpc = (struct _fmr_rpc *)data;

    struct _lf_ll *node = device->modules;
    for (uint8_t i = 0; node && i < rpc->module; i++) node = node->next;
    struct _lf_module *module = node ? (struct _lf_module *)node->item : NULL;

    if (!module || !module->table || rpc->index >= module->length || !module->table[rpc->index]) {
        fprintf(stderr, "[fmr_rpc] Invalid module or index %d (max %zu)\n", rpc->index, module ? module->length : 0);
        return lf_error;
    }
    lf_function fn = module->table[rpc->index];
    *retval = fn(data + sizeof(struct _fmr_rpc));
    return lf_success;
}

/* fmr_push: Receives and stores a value into a module's state */
static int fmr_push(struct _lf_device *device, const struct _fmr_packet *packet, lf_return_t *retval) {
    const uint8_t *data = (const uint8_t *)(packet + 1);
    struct _fmr_push *push = (struct _fmr_push *)data;

    struct _lf_ll *node = device->modules;
    for (uint8_t i = 0; node && i < push->module; i++) node = node->next;
    struct _lf_module *module = node ? (struct _lf_module *)node->item : NULL;

    if (!module || !module->state) return lf_error;
    memcpy((uint8_t *)module->state + push->index, data + sizeof(struct _fmr_push), push->length);
    *retval = 0;
    return lf_success;
}

/* fmr_pull: Sends a requested state value from a module back to the host */
static int fmr_pull(struct _lf_device *device, const struct _fmr_packet *packet, lf_return_t *retval) {
    const uint8_t *data = (const uint8_t *)(packet + 1);
    struct _fmr_pull *pull = (struct _fmr_pull *)data;

    struct _lf_ll *node = device->modules;
    for (uint8_t i = 0; node && i < pull->module; i++) node = node->next;
    struct _lf_module *module = node ? (struct _lf_module *)node->item : NULL;

    if (!module || !module->state) return lf_error;
    int e = device->write(device, (uint8_t *)module->state + pull->index, pull->length);
    *retval = (e == lf_success) ? 0 : -1;
    return e;
}

/* fmr_dyld: Dynamically resolves a module by name at runtime and returns its index */
static int fmr_dyld(struct _lf_device *device, const struct _fmr_packet *packet, lf_return_t *retval) {
    const char *name = (const char *)(packet + 1);
    struct _lf_module *module = lf_module_get_by_name(device, name);
    if (!module) return lf_error;
    *retval = module->idx;
    return lf_success;
}

/* fmr_malloc: Allocates memory on the device and returns the pointer address */
static int fmr_malloc(struct _lf_device *device, const struct _fmr_packet *packet, lf_return_t *retval) {
    const uint8_t *data = (const uint8_t *)(packet + 1);
    size_t *len = (size_t *)data;
    void *ptr = malloc(*len);
    if (!ptr) return lf_error;
    *retval = (uintptr_t)ptr;
    return lf_success;
}

/* fmr_free: Frees a previously allocated memory block on the device */
static int fmr_free(struct _lf_device *device, const struct _fmr_packet *packet, lf_return_t *retval) {
    const uint8_t *data = (const uint8_t *)(packet + 1);
    void *ptr = *(void **)data;
    free(ptr);
    *retval = 0;
    return lf_success;
}
