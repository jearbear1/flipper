#include "libflipper.h"
#include <string.h> // Needed for strcmp, strdup

/* Debug helper to print all registered modules */
void print_module_list(struct _lf_ll *list) {
    fprintf(stderr, "[dyld_register] Current module list:\n");
    int i = 0;
    while (list) {
        struct _lf_module *m = list->item;
        fprintf(stderr, "  [%d] name=%s idx=%d\n", i++, m ? m->name : "(null)", m ? m->idx : -1);
        list = list->next;
    }
}

int dyld_register(struct _lf_device *device, struct _lf_module *module) {
    lf_assert(device, E_NULL, "invalid device");
    lf_assert(module, E_NULL, "invalid module");

    // Prevent duplicate module registration
    struct _lf_module *existing = lf_module_get_by_name(device, module->name);
    if (existing) {
        fprintf(stderr, "[dyld_register] Module '%s' already registered.\n", module->name);
        return lf_success;
    }

    if (module->idx == UINT16_MAX) {
        int count = lf_ll_count(device->modules);
        fprintf(stderr, "[dyld_register] module->idx is UINT16_MAX, setting to %d\n", count);
        module->idx = count;
    }

    fprintf(stderr, "[dyld_register] Registering module '%s' with index %d\n", module->name, module->idx);
    fprintf(stderr, "[dyld_register] Registering module '%s' (strlen=%zu)\n", module->name, strlen(module->name));

    int result = lf_ll_append(&device->modules, module, lf_module_release);
    fprintf(stderr, "[dyld_register] lf_ll_append returned %d\n", result);

    if (result != lf_success) {
        fprintf(stderr, "[dyld_register] Failed to append module '%s' to list\n", module->name);
        goto fail;
    }

    print_module_list(device->modules);
    return lf_success;

fail:
    return lf_error;
}

/* Load a module onto the device. */
int dyld_load(struct _lf_device *device, void *src, size_t len) {
    return lf_success;
}

/* Get the module index on the device. */
struct _lf_module *dyld_module(struct _lf_device *device, const char *module) {
    lf_assert(device, E_NULL, "invalid device");
    lf_assert(module, E_NULL, "invalid module");

    fprintf(stderr, "[dyld_module] Incoming query for module = '%s'\n", module);

    size_t count = lf_ll_count(device->modules);
    for (size_t i = 0; i < count; i++) {
        struct _lf_module *m = lf_ll_item(device->modules, i);
        fprintf(stderr, "[dyld_module] Comparing '%s' to '%s'\n", m->name, module);
        fprintf(stderr, "[dyld_module] Comparing '%s' (strlen=%zu) to '%s' (strlen=%zu)\n",
                m->name, strlen(m->name), module, strlen(module));
        if (m && m->name && strcmp(m->name, module) == 0)
            return m;
    }

    // Fallback: Attempt to resolve and register
    uint16_t idx;
    lf_assert( lf_dyld_dyn_sym_lookup(device, module, &idx), E_MODULE,
              "Failed to find counterpart for module '%s' on device '%s'.", module, device->name);

    struct _lf_module *m = lf_module_create(module, idx);
    lf_assert(m, E_NULL, "Failed to create new module '%s'.", module);

    memset(m->name, 0, sizeof(m->name));
    strncpy(m->name, module, sizeof(m->name) - 1);
    m->name[sizeof(m->name) - 1] = '\0';

    int e = dyld_register(device, m);
    lf_assert(e, E_MODULE, "Failed to register module '%s'.", module);
    return m;

fail:
    return NULL;
}

struct _lf_module *lf_module_get_by_name(struct _lf_device *device, const char *name) {
    lf_assert(device, E_NULL, "Device is NULL");
    lf_assert(name, E_NULL, "Module name is NULL");

    size_t count = lf_ll_count(device->modules);
    for (size_t i = 0; i < count; i++) {
        struct _lf_module *mod = lf_ll_item(device->modules, i);
        if (mod && mod->name && strcmp(mod->name, name) == 0) {
            return mod;
        }
    }

fail:
    return NULL;
}

/*
 * Retrieves the index of a module by name and writes it into *idx.
 */
int lf_dyld_dyn_sym_lookup(struct _lf_device *device, const char *module, uint16_t *idx) {
    struct _fmr_packet _packet;
    memset(&_packet, 0, sizeof(_packet));

    struct _fmr_dyld_packet *packet = (struct _fmr_dyld_packet *)&_packet;
    struct _fmr_header *hdr = &packet->hdr;
    struct _fmr_result result;
    int e;
    lf_crc_t crc;

    lf_assert(device, E_NULL, "invalid device");
    lf_assert(module, E_NULL, "invalid module name");

    if (device->name[0] == '\0') {
        strncpy(device->name, "fvm", sizeof(device->name) - 1);
        device->name[sizeof(device->name) - 1] = '\0';
        fprintf(stderr, "[// lf_dyld_dyn_sym_lookup] Warning: device->name was empty, defaulted to 'fvm'.\n");
    }

    hdr->magic = FMR_MAGIC_NUMBER;
    hdr->len = sizeof(hdr);
    hdr->type = fmr_dyld_class;

    strncpy(packet->module, module, sizeof(struct _fmr_packet) - sizeof(struct _fmr_dyld_packet));
    hdr->len += strlen(module) + 1;

    lf_crc(packet, hdr->len, &crc);
    hdr->crc = crc;

    lf_debug_packet(&_packet);

    e = device->write(device, packet, sizeof(_packet));
    lf_assert(e, E_ENDPOINT, "Failed to send message to device '%s'.", device->name);

    e = device->read(device, &result, sizeof(struct _fmr_result));
    lf_assert(e, E_ENDPOINT, "Failed to receive message from the device '%s'.", device->name);

    lf_debug_result(&result);
    lf_assert(result.error == E_OK, result.error, "An error occurred on the device '%s'.", device->name);

    lf_assert(result.value <= UINT16_MAX, E_MODULE, "Module index '%llu' out of bounds", result.value);
    *idx = (uint16_t)result.value;

    return lf_success;

fail:
    return lf_error;
}

/* Unload a module from the device. */
int dyld_unload(struct _lf_device *device, char *module) {
    lf_assert(device, E_NULL, "invalid device");
    lf_assert(module, E_NULL, "invalid module");

    struct _lf_module *m = dyld_module(device, module);
    lf_assert(m, E_NULL, "no module '%s' loaded on device '%s'.", module, device->name);
    lf_ll_remove(&device->modules, m);
    return lf_success;

fail:
    return lf_error;
}

/* Returns the currently selected device, safely wrapped for dyld users. */
struct _lf_device *dyld_lf_get_selected(void) {
    return lf_get_selected();
}
