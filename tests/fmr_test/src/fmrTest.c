#include <flipper.h>
#include <fmr.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <fmrTest.h>

void debug_bytes(const char *label, const void *data, size_t len) {
    char msg[128];
    char *ptr = msg;
    ptr += snprintf(ptr, sizeof(msg) - (ptr - msg), "%s: ", label);
    for (size_t i = 0; i < len && (ptr - msg) < sizeof(msg) - 4; i++) {
        ptr += snprintf(ptr, sizeof(msg) - (ptr - msg), "%02x ", ((unsigned char *)data)[i]);
    }
    *ptr++ = '\n';
    *ptr = '\0';
    write(STDOUT_FILENO, msg, strlen(msg));
}

int extract_arg(struct _lf_ll *args, lf_type expected_type, void *value) {
    if (!args || !args->item) {
        write(STDOUT_FILENO, "Error: args is NULL or empty\n", 29);
        return -1;
    }
    struct _lf_arg *arg = (struct _lf_arg *)args->item;
    if (arg->type != expected_type) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Error: expected arg type %u, got %u\n", expected_type, arg->type);
        write(STDOUT_FILENO, msg, strlen(msg));
        return -1;
    }
    memcpy(value, &arg->value, lf_sizeof(arg->type));
    return 0;
}

LF_FUNC lf_return_t my_lf_uint8_argument_test(struct _lf_ll *args) {
    char msg[128];
    uint8_t x;
    if (extract_arg(args, lf_uint8_t, &x) != 0) return lf_error;
    debug_bytes("Raw uint8 bytes", &x, sizeof(x));
    snprintf(msg, sizeof(msg), "Received uint8: %u (0x%02x)\n", x, x);
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

LF_FUNC lf_return_t my_lf_uint16_argument_test(struct _lf_ll *args) {
    char msg[128];
    uint16_t x;
    if (extract_arg(args, lf_uint16_t, &x) != 0) return lf_error;
    debug_bytes("Raw uint16 bytes", &x, sizeof(x));
    snprintf(msg, sizeof(msg), "Received uint16: %u (0x%04x)\n", x, x);
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

LF_FUNC lf_return_t my_lf_uint32_argument_test(struct _lf_ll *args) {
    char msg[128];
    int32_t x;
    if (extract_arg(args, lf_int32_t, &x) != 0) return lf_error;
    debug_bytes("Raw int32 bytes", &x, sizeof(x));
    snprintf(msg, sizeof(msg), "Received int32: %d (0x%08x)\n", x, (uint32_t)x);
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

LF_FUNC lf_return_t my_lf_int_argument_test(struct _lf_ll *args) {
    char msg[128];
    int x;
    if (extract_arg(args, lf_int_t, &x) != 0) return lf_error;
    debug_bytes("Raw int bytes", &x, sizeof(x));
    snprintf(msg, sizeof(msg), "Received int: %d (0x%08x)\n", x, (unsigned int)x);
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

LF_FUNC lf_return_t my_lf_uint8_result_test(struct _lf_ll *args) {
    char msg[128];
    uint8_t x;
    if (extract_arg(args, lf_uint8_t, &x) != 0) return lf_error;
    debug_bytes("Raw uint8 bytes (result)", &x, sizeof(x));
    uint8_t doubled = x * 2;
    snprintf(msg, sizeof(msg), "Returning uint8 result for: %u (0x%02x), doubled: %u (0x%02x)\n", x, x, doubled, doubled);
    write(STDOUT_FILENO, msg, strlen(msg));
    return (lf_return_t)doubled;
}

LF_FUNC lf_return_t my_lf_uint16_result_test(struct _lf_ll *args) {
    char msg[128];
    uint16_t x;
    if (extract_arg(args, lf_uint16_t, &x) != 0) return lf_error;
    debug_bytes("Raw uint16 bytes (result)", &x, sizeof(x));
    uint16_t doubled = x * 2;
    snprintf(msg, sizeof(msg), "Returning uint16 result for: %u (0x%04x), doubled: %u (0x%04x)\n", x, x, doubled, doubled);
    write(STDOUT_FILENO, msg, strlen(msg));
    return (lf_return_t)doubled;
}

LF_FUNC lf_return_t my_lf_uint32_result_test(struct _lf_ll *args) {
    char msg[128];
    uint32_t x;
    if (extract_arg(args, lf_uint32_t, &x) != 0) return lf_error;
    debug_bytes("Raw uint32 bytes (result)", &x, sizeof(x));
    uint32_t doubled = x * 2;
    snprintf(msg, sizeof(msg), "Returning uint32 result for: %u (0x%08x), doubled: %u (0x%08x)\n", x, x, doubled, doubled);
    write(STDOUT_FILENO, msg, strlen(msg));
    return (lf_return_t)doubled;
}

LF_FUNC lf_return_t my_lf_int_result_test(struct _lf_ll *args) {
    char msg[128];
    int x;
    if (extract_arg(args, lf_int_t, &x) != 0) return lf_error;
    debug_bytes("Raw int bytes (result)", &x, sizeof(x));
    int doubled = x * 2;
    snprintf(msg, sizeof(msg), "Returning int result for: %d (0x%08x), doubled: %d (0x%08x)\n", x, (unsigned int)x, doubled, (unsigned int)doubled);
    write(STDOUT_FILENO, msg, strlen(msg));
    return (lf_return_t)(unsigned int)doubled;
}

__attribute__((visibility("default")))
lf_function lf_module_fmrTest_table[] = {
    (lf_function)my_lf_uint8_argument_test,
    (lf_function)my_lf_uint16_argument_test,
    (lf_function)my_lf_uint32_argument_test,
    (lf_function)my_lf_int_argument_test,
    (lf_function)my_lf_uint8_result_test,
    (lf_function)my_lf_uint16_result_test,
    (lf_function)my_lf_uint32_result_test,
    (lf_function)my_lf_int_result_test,
};

__attribute__((visibility("default")))
struct _lf_module fmrTest = {
    .name = "fmrTest",
    .version = 1,
    .idx = UINT16_MAX,
    .interface = NULL,
    .table = lf_module_fmrTest_table,
    .length = sizeof(lf_module_fmrTest_table) / sizeof(lf_module_fmrTest_table[0])
};
