#include <flipper.h>
#include <fmr.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

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

// Helper function to extract a single argument from the args linked list
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

LF_FUNC lf_return_t my_lf_int8_argument_test(struct _lf_ll *args) {
    char msg[128];
    uint8_t x;
    if (extract_arg(args, lf_uint8_t, &x) != 0) return lf_error;
    debug_bytes("Raw int8 bytes", &x, sizeof(x));
    snprintf(msg, sizeof(msg), "Received int8: %u (0x%02x)\n", x, x);
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

LF_FUNC lf_return_t my_lf_int16_argument_test(struct _lf_ll *args) {
    char msg[128];
    uint16_t x;
    if (extract_arg(args, lf_uint16_t, &x) != 0) return lf_error;
    debug_bytes("Raw int16 bytes", &x, sizeof(x));
    snprintf(msg, sizeof(msg), "Received int16: %u (0x%04x)\n", x, x);
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

LF_FUNC lf_return_t my_lf_int32_argument_test(struct _lf_ll *args) {
    char msg[128];
    uint32_t x;
    if (extract_arg(args, lf_uint32_t, &x) != 0) return lf_error;
    debug_bytes("Raw int32 bytes", &x, sizeof(x));
    snprintf(msg, sizeof(msg), "Received int32: %u (0x%08x)\n", x, x);
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

LF_FUNC lf_return_t my_lf_ptr32_argument_test(struct _lf_ll *args) {
    char msg[128];
    uint32_t x;
    if (extract_arg(args, lf_uint32_t, &x) != 0) return lf_error;
    debug_bytes("Raw ptr32 bytes", &x, sizeof(x));
    snprintf(msg, sizeof(msg), "Received ptr32 value: %u (0x%08x)\n", x, x);
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

LF_FUNC lf_return_t my_lf_ptr64_argument_test(struct _lf_ll *args) {
    char msg[128];
    uint64_t x;
    if (extract_arg(args, lf_uint64_t, &x) != 0) return lf_error;
    debug_bytes("Raw ptr64 bytes", &x, sizeof(x));
    snprintf(msg, sizeof(msg), "Received ptr64 value: %llu (0x%016llx)\n", x, x);
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

LF_FUNC lf_return_t my_lf_int64_argument_test(struct _lf_ll *args) {
    char msg[128];
    uint64_t x;
    if (extract_arg(args, lf_uint64_t, &x) != 0) return lf_error;
    debug_bytes("Raw int64 bytes", &x, sizeof(x));
    snprintf(msg, sizeof(msg), "Received int64: %llu (0x%016llx)\n", x, x);
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

LF_FUNC lf_return_t my_lf_int8_result_test(struct _lf_ll *args) {
    char msg[128];
    uint8_t x;
    if (extract_arg(args, lf_uint8_t, &x) != 0) return lf_error;
    debug_bytes("Raw int8 bytes (result)", &x, sizeof(x));
    uint8_t doubled = x * 2;
    snprintf(msg, sizeof(msg), "Returning int8 result for: %u (0x%02x), doubled: %u (0x%02x)\n", x, x, doubled, doubled);
    write(STDOUT_FILENO, msg, strlen(msg));
    return (lf_return_t)doubled;
}

LF_FUNC lf_return_t my_lf_int16_result_test(struct _lf_ll *args) {
    char msg[128];
    uint16_t x;
    if (extract_arg(args, lf_uint16_t, &x) != 0) return lf_error;
    debug_bytes("Raw int16 bytes (result)", &x, sizeof(x));
    int16_t doubled = (int16_t)x * 2; // Preserve sign for intermediate calculation
    snprintf(msg, sizeof(msg), "Returning int16 result for: %u (0x%04x), doubled: %d (0x%04x)\n", x, x, doubled, (uint16_t)doubled);
    write(STDOUT_FILENO, msg, strlen(msg));
    return (lf_return_t)(uint16_t)doubled; // Cast to unsigned for return
}

LF_FUNC lf_return_t my_lf_int32_result_test(struct _lf_ll *args) {
    char msg[128];
    uint32_t x;
    if (extract_arg(args, lf_uint32_t, &x) != 0) return lf_error;
    debug_bytes("Raw int32 bytes (result)", &x, sizeof(x));
    int32_t doubled = (int32_t)x * 2; // Preserve sign for intermediate calculation
    snprintf(msg, sizeof(msg), "Returning int32 result for: %u (0x%08x), doubled: %d (0x%08x)\n", x, x, doubled, (uint32_t)doubled);
    write(STDOUT_FILENO, msg, strlen(msg));
    return (lf_return_t)(uint32_t)doubled; // Cast to unsigned for return
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

LF_FUNC lf_return_t my_lf_ptr32_result_test(struct _lf_ll *args) {
    char msg[128];
    uint32_t x;
    if (extract_arg(args, lf_uint32_t, &x) != 0) return lf_error;
    debug_bytes("Raw ptr32 bytes (result)", &x, sizeof(x));
    uint32_t doubled = x * 2;
    snprintf(msg, sizeof(msg), "Returning ptr32 result for: %u (0x%08x), doubled: %u (0x%08x)\n", x, x, doubled, doubled);
    write(STDOUT_FILENO, msg, strlen(msg));
    return (lf_return_t)doubled;
}

LF_FUNC lf_return_t my_lf_ptr64_result_test(struct _lf_ll *args) {
    char msg[128];
    uint64_t x;
    if (extract_arg(args, lf_uint64_t, &x) != 0) return lf_error;
    debug_bytes("Raw ptr64 bytes (result)", &x, sizeof(x));
    uint64_t doubled = x * 2;
    snprintf(msg, sizeof(msg), "Returning ptr64 result for: %llu (0x%016llx), doubled: %llu (0x%016llx)\n", x, x, doubled, doubled);
    write(STDOUT_FILENO, msg, strlen(msg));
    return (lf_return_t)doubled;
}

LF_FUNC lf_return_t my_lf_int64_result_test(struct _lf_ll *args) {
    char msg[128];
    uint64_t x;
    if (extract_arg(args, lf_uint64_t, &x) != 0) return lf_error;
    debug_bytes("Raw int64 bytes (result)", &x, sizeof(x));
    int64_t doubled = (int64_t)x * 2; // Preserve sign for intermediate calculation
    snprintf(msg, sizeof(msg), "Returning int64 result for: %llu (0x%016llx), doubled: %lld (0x%016llx)\n", x, x, doubled, (uint64_t)doubled);
    write(STDOUT_FILENO, msg, strlen(msg));
    return (lf_return_t)(uint64_t)doubled; // Cast to unsigned for return
}

__attribute__((visibility("default")))
lf_function lf_module_fmrTest_table[] = {
    (lf_function)my_lf_int8_argument_test,
    (lf_function)my_lf_int16_argument_test,
    (lf_function)my_lf_int32_argument_test,
    (lf_function)my_lf_int_argument_test,
    (lf_function)my_lf_ptr32_argument_test,
    (lf_function)my_lf_ptr64_argument_test,
    (lf_function)my_lf_int64_argument_test,
    (lf_function)my_lf_int8_result_test,
    (lf_function)my_lf_int16_result_test,
    (lf_function)my_lf_int32_result_test,
    (lf_function)my_lf_int_result_test,
    (lf_function)my_lf_ptr32_result_test,
    (lf_function)my_lf_ptr64_result_test,
    (lf_function)my_lf_int64_result_test
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
