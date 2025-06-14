/* fmr_helper.c - Runtime support helpers for Flipper Message Runtime */

#include "libflipper.h"
#include "fmr.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/* Append a new argument to a linked list of arguments */
int fmr_append_arg(struct _lf_ll **list, lf_type type, lf_arg value) {
    struct _lf_arg *arg = malloc(sizeof(struct _lf_arg));
    if (!arg) return lf_error;
    arg->type = type;
    arg->value = value;
    return lf_ll_append(list, arg, NULL);  // Wrap in address and no deconstructothis uses lf_ll_append directly
}

/* Creates a single argument object */
struct _lf_arg *lf_arg_create(lf_type type, lf_arg value) {
    struct _lf_arg *arg = malloc(sizeof(struct _lf_arg));
    if (!arg) return NULL;
    arg->type = type;
    arg->value = value;
    return arg;
}

/* Computes the size in bytes of the lf_type */
int lf_sizeof(lf_type type) {
    switch (type) {
        case lf_uint8_t:
        case lf_int8_t: return 1;
        case lf_uint16_t:
        case lf_int16_t: return 2;
        case lf_uint32_t:
        case lf_int_t:
        case lf_int32_t: return 4;
        case lf_uint64_t:
        case lf_int64_t:
        case lf_ptr_t: return 8;
        default: return 0;
    }
}

/* Builds a linked list of arguments from variadic arguments */
struct _lf_ll *fmr_build(int argc, ...) {
    va_list ap;
    va_start(ap, argc);

    struct _lf_ll *args = lf_ll_create();
    if (!args) return NULL;

    for (int i = 0; i < argc; i++) {
        lf_type type = va_arg(ap, int);
        lf_arg value = va_arg(ap, lf_arg);
        if (fmr_append_arg(&args, type, value) != lf_success) {
            lf_ll_release(&args);
            return NULL;
        }
    }

    va_end(ap);
    return args;
}

/* Generate call header and body from module and args */
int lf_create_call(lf_module module, lf_function function, lf_type ret, struct _lf_ll *args, struct _fmr_header *header, struct _fmr_call *call) {
    if (!call || !header) return lf_error;

    call->module = module;
    call->function = 0; // Placeholder for resolved index elsewhere
    call->ret = ret;
    call->argc = args ? lf_ll_count(args) : 0;

    uint8_t *dst = call->argv;
    size_t payload_size = sizeof(struct _fmr_call) + (call->argc * sizeof(lf_type));
    struct _lf_ll *n = args;
    for (lf_argc i = 0; i < call->argc; i++) {
        struct _lf_arg *arg = (struct _lf_arg *)n->item;
        call->arg_types[i] = arg->type; // Store the full lf_type
        size_t arg_size = lf_sizeof(arg->type);
        if (payload_size + arg_size > FMR_PACKET_SIZE - sizeof(struct _fmr_header)) {
            lf_debug("Argument list too large for packet");
            return lf_error;
        }
        memcpy(dst, &arg->value, arg_size);
        dst += arg_size;
        payload_size += arg_size;
        n = n->next;
    }

    header->type = fmr_rpc_class;
    header->len = payload_size;
    return lf_success;
}

/* Retrieve a module pointer by its index */
struct _lf_module *lf_module_get_by_index(struct _lf_device *device, uint8_t index) {
    if (!device || !device->modules) return NULL;
    struct _lf_ll *node = device->modules;
    for (uint8_t i = 0; node && i < index; i++) {
        node = node->next;
    }
    return node ? (struct _lf_module *)node->item : NULL;
}
