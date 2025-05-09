#ifndef __lf_module_h__
#define __lf_module_h__

#include "defines.h"  
#include "fmr.h"

#define LF_MODULE_NAME_MAX 32

struct _lf_module {
    /* A string containing the module's name. */
    char name[LF_MODULE_NAME_MAX]; 

    /* The version of the module. */
    lf_version_t version;

    /* The loaded index of the module on the device. */
    uint16_t idx;

    /* The module's interface (legacy/optional). */
    void **interface;

    /* Pointer to the function table (used by FVM). */
    lf_function *table;

    /* Number of functions in the table. */
    size_t length;

    /* Pointer to the module's state memory. */
    void *state;
};

struct _lf_module *lf_module_create(const char *name, uint16_t idx);
void lf_module_release(void *module);

#endif
