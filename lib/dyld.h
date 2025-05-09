#ifndef __lf_dyld_h__
#define __lf_dyld_h__

#include "device.h"
#include "module.h"
#include "fmr.h"    // For lf_dyld()

/* Registers a module on the device's module list. */
int dyld_register(struct _lf_device *device, struct _lf_module *module);

/* Loads a module image onto the device (not yet implemented). */
int dyld_load(struct _lf_device *device, void *src, size_t len);

/* Returns a pointer to the module structure with the given name, resolving from device. */
struct _lf_module *dyld_module(struct _lf_device *device, const char *module);

/* Searches the device's module list for a module by name. */
struct _lf_module *lf_module_get_by_name(struct _lf_device *device, const char *name);

/* Prints all modules currently registered on the device. */
void print_module_list(struct _lf_ll *list);

/* Removes a module from the device's module list. */
int dyld_unload(struct _lf_device *device, char *module);

/* Performs dynamic symbol lookup for a module by name and retrieves its index. */
int  lf_dyld_dyn_sym_lookup(struct _lf_device *device, const char *module, uint16_t *idx);

// return a pointer to the currently selected struct _lf_device.
struct _lf_device *dyld_lf_get_selected(void);


#endif /* __lf_dyld_h__ */
