#include "libflipper.h"
#include "fmr.h"
#include <string.h>
#include <stdlib.h>
#include <error.h>
#include <stdio.h>

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

struct _fmr_packet *current_packet = NULL;

int fmr_perform(struct _lf_device *device, struct _fmr_packet *packet) {
    struct _fmr_header *hdr = &packet->hdr;
    struct _fmr_result result = {0};
    lf_return_t retval = 0;
    lf_crc_t crc, _crc;
    int e = E_UNIMPLEMENTED;

    lf_assert(hdr->magic == FMR_MAGIC_NUMBER, E_CHECKSUM, "invalid magic number");

    _crc = hdr->crc;
    hdr->crc = 0;
    lf_crc(packet, hdr->len, &crc);
    if (memcmp(&_crc, &crc, sizeof(crc))) {
        lf_error_set(E_CHECKSUM);
        fprintf(stderr, "[fmr_perform] Checksums do not match (0x%04x/0x%04x)\n", _crc, crc);
        goto fail;
    }

    lf_error_set(E_OK);

    lf_assert(hdr->type < MAX_DISPATCHERS, E_FMR, "unknown header type '%d'", hdr->type);
    if (!device || !device->modules) {
        lf_debug("[fmr_perform] ERROR: device or device->modules is NULL");
        result.error = E_NULL;
        goto send_result;
    }

    if (!fmr_registry[hdr->type]) {
        lf_debug("[fmr_perform] ERROR: no registered handler for type %d", hdr->type);
        result.error = E_FMR;
        goto send_result;
    }

    fprintf(stderr, "[fmr_perform] Dispatching packet type=%d\n", hdr->type);
    current_packet = packet; // Set before dispatching
    int dispatch_result = fmr_registry[hdr->type](device, packet, &retval);
    current_packet = NULL; // Clear after dispatching
    if (dispatch_result != lf_success) {
        lf_debug("[fmr_perform] Dispatcher for type %d failed", hdr->type);
        result.error = E_FMR;
    } else {
        result.error = E_OK;
    }
    result.value = retval;

fail:
    result.error = lf_error_get();
send_result:
    fprintf(stderr, "[fmr_perform] Sending result: value=%llu, error=%d\n", result.value, result.error);
    e = device->write(device, &result, sizeof(result));
    lf_debug_result(&result);
    return e;
}

// Helper struct definitions
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
    struct _fmr_call *call = &((struct _fmr_call_packet *)packet)->call;
    fprintf(stderr, "[fmr_rpc] Module index: %u, Function index: %u\n", call->module, call->function);
    struct _lf_module *module = lf_ll_item(device->modules, call->module);
    if (!module || call->function >= module->length) {
        fprintf(stderr, "[fmr_rpc] Invalid module index %u or function index %u (max %zu)\n",
                call->module, call->function, module ? module->length - 1 : 0);
        return lf_error;
    }
    uint8_t *argv = call->argv;
    uint8_t i;
    struct _lf_ll *args = NULL;
    for (i = 0; i < call->argc; i++) {
        lf_type type = call->arg_types[i]; // Use the full lf_type from the array
        uint8_t size = lf_sizeof(type);
        struct _lf_arg *arg = malloc(sizeof(struct _lf_arg));
        if (!arg) {
            fprintf(stderr, "[fmr_rpc] Failed to allocate argument\n");
            lf_error_set(E_NULL);
            lf_ll_release(&args);
            return lf_error;
        }
        arg->type = type;
        memcpy(&arg->value, argv, size);
        argv += size;
        if (lf_ll_append(&args, arg, free) != lf_success) {
            fprintf(stderr, "[fmr_rpc] Failed to append argument %u\n", i);
            free(arg);
            lf_ll_release(&args);
            return lf_error;
        }
    }
    *retval = module->table[call->function](args);
    lf_ll_release(&args);
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
    fprintf(stderr, "[fmr_dyld] Raw packet module name (len=%zu): ", strlen(name) + 1);
    for (size_t i = 0; i < strlen(name) + 1; i++) {
        fprintf(stderr, "0x%02x ", ((uint8_t *)name)[i]);
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "[fmr_dyld] Looking up module '%s'\n", name);
    struct _lf_module *module = lf_module_get_by_name(device, name);
    if (!module) {
        fprintf(stderr, "[fmr_dyld] Module '%s' not found\n", name);
        *retval = 0;
        return lf_error;
    }
    *retval = module->idx;
    fprintf(stderr, "[fmr_dyld] Found module '%s' with idx=%u\n", name, module->idx);
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
