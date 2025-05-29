#include "libflipper.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>

/* Debug helper to print all registered modules */
void print_module_list(struct _lf_ll *list) {
    fprintf(stderr, "[dyld_register] Current module list:\n");
    int i = 0;
    while (list) {
        struct _lf_module *m = list->item;
        fprintf(stderr, "  [%d] name=%s idx=%u\n", i++, m ? m->name : "(null)", m ? m->idx : 0);
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
    fprintf(stderr, "[dyld_module] Module list count: %zu\n", lf_ll_count(device->modules));

    size_t count = lf_ll_count(device->modules);
    for (size_t i = 0; i < count; i++) {
        struct _lf_module *m = lf_ll_item(device->modules, i);
        fprintf(stderr, "[dyld_module] Comparing '%s' to '%s'\n", m->name, module);
        if (m && strcmp(m->name, module) == 0)
            return m;
    }

    uint16_t idx;
    int e = lf_dyld_dyn_sym_lookup(device, module, &idx);
    if (e != lf_success) {
        lf_error_set(E_MODULE);
        fprintf(stderr, "[dyld_module] Failed to find counterpart for module '%s' on device '%s'.\n", module, device->name);
        return NULL;
    }

    struct _lf_module *m = lf_module_create(module, idx);
    lf_assert(m, E_NULL, "failed to create module for '%s'", module);

    memset(m->name, 0, sizeof(m->name));
    strncpy(m->name, module, sizeof(m->name) - 1);
    m->name[sizeof(m->name) - 1] = '\0';

    e = dyld_register(device, m);
    if (e != lf_success) {
        fprintf(stderr, "[dyld_module] Failed to register module '%s'.\n", module);
        free(m);
        lf_error_set(E_MODULE);
        return NULL;
    }

    fprintf(stderr, "[dyld_module] Registered module '%s' with idx=%u\n", module, idx);
    return m;

fail:
    return NULL;
}

struct _lf_module *lf_module_get_by_name(struct _lf_device *device, const char *name) {
    lf_assert(device, E_NULL, "Device is NULL");
    lf_assert(name, E_NULL, "Module name is NULL");

    fprintf(stderr, "[lf_module_get_by_name] Looking for '%s'\n", name);
    size_t count = lf_ll_count(device->modules);
    for (size_t i = 0; i < count; i++) {
        struct _lf_module *mod = lf_ll_item(device->modules, i);
        fprintf(stderr, "[lf_module_get_by_name] Checking '%s'\n", mod ? mod->name : "(null)");
        if (mod && strcmp(mod->name, name) == 0) {
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
    memset(&_packet, 0, sizeof(_packet)); // Clear the entire packet buffer

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
        fprintf(stderr, "[lf_dyld_dyn_sym_lookup] Warning: device->name was empty, defaulted to 'fvm'.\n");
    }

    hdr->magic = FMR_MAGIC_NUMBER;
    hdr->len = sizeof(*hdr);
    hdr->type = fmr_dyld_class;

    // Copy module name and ensure null-termination
    size_t module_len = strlen(module);
    if (module_len >= LF_MODULE_NAME_MAX) {
        fprintf(stderr, "[lf_dyld_dyn_sym_lookup] Module name '%s' too long\n", module);
        return lf_error;
    }
    strncpy(packet->module, module, module_len + 1); // Copy including null terminator
    hdr->len += module_len + 1;

    lf_crc(packet, hdr->len, &crc);
    hdr->crc = crc;

    lf_debug_packet(&_packet);

    e = device->write(device, packet, sizeof(_packet));
    lf_assert(e == lf_success, E_ENDPOINT, "Failed to send dyld packet to device '%s'.", device->name);

    e = device->read(device, &result, sizeof(struct _fmr_result));
    lf_assert(e == lf_success, E_ENDPOINT, "Failed to receive response from device %s: error %d", device->name, errno);

    fprintf(stderr, "[lf_dyld_dyn_sym_lookup] Received result: value=%llu, error=%d\n", result.value, result.error);

    lf_debug("fmr_result=%d", result.error);
    lf_assert(result.error == E_OK, result.error, "Device '%s' returned error %d for module '%s'.", device->name, result.error, module);

    if (result.value > UINT16_MAX) {
        fprintf(stderr, "[lf_dyld_dyn_sym_lookup] Warning: module index '%llu' exceeds UINT16_MAX for '%s'\n", result.value, module);
    }
    *idx = (uint16_t)result.value;

    lf_debug("lf_dyld_dyn_sym_lookup: Retrieved index %d for module '%s'.", *idx, module);
    return lf_success;

fail:
    return lf_error;
}

/* Unload a function from the device. */
int dyld_unload(struct _lf_device *device, char *module) {
    lf_assert(device, E_NULL, "invalid device");
    lf_assert(module, E_NULL, "invalid module");

    struct _lf_module *f = dyld_module(device, module);
    lf_assert(f == NULL, E_NULL, "no module '%s' loaded on device '%s'.", module, device->name);
    lf_ll_remove(&device->modules, f);
    return lf_success;

fail:
    return lf_error;
}

/* Returns the selected device, safely wrapped for dyld users. */
struct _lf_device *lf_dyld_get_selected(void) {
    return lf_get_selected();
}
