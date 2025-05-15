#include "libflipper.h"
#include <string.h> // Needed for strlen, strncpy

struct _lf_module *lf_module_create(const char *name, uint16_t idx) {
    lf_assert(name, E_NULL, "No name");
    lf_assert(strlen(name) < LF_MODULE_NAME_MAX, E_OVERFLOW,
        "Module name '%s' is invalid. Module names must be < %d characters.",
        name, LF_MODULE_NAME_MAX);

    struct _lf_module *module = calloc(1, sizeof(struct _lf_module));
    lf_assert(module, E_MALLOC, "Failed to allocate memory for new _lf_module.");

    // Safely copy name into fixed-size array
    strncpy(module->name, name, sizeof(module->name) - 1);
    module->name[sizeof(module->name) - 1] = '\0';

    module->idx = idx;
    module->length = 0;
    return module;

fail:
    return NULL;
}

void lf_module_release(void *_module) {
    if (!_module) return;

    struct _lf_module *module = (struct _lf_module *)_module;

    // No need to free fixed-size array 'name'
    // Just zero it out if you want to clear it (optional)
    memset(module->name, 0, sizeof(module->name));

    free(module);
}
