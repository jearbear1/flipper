#include <flipper.h>
#include "fmrTest.h"

int my_lf_int8_argument_test(int8_t x) {
    lf_info("Received int8: %02x", x);
    return lf_success;
}

int my_lf_int16_argument_test(int16_t x) {
    lf_info("Received int16: %04x", x);
    return lf_success;
}

int my_lf_int32_argument_test(int32_t x) {
    lf_info("Received int32: %08x", x);
    return lf_success;
}

int my_lf_int_argument_test(int x) {
    lf_info("Received int: %08x", x);
    return lf_success;
}

int my_lf_ptr32_argument_test(uint32_t *x) {
    lf_info("Received ptr32 value: %08x", *x);
    return lf_success;
}

int my_lf_ptr64_argument_test(uint64_t *x) {
    lf_info("Received ptr64 value: %016llx", *x);
    return lf_success;
}

int my_lf_int64_argument_test(int64_t x) {
    lf_info("Received int64: %016llx", x);
    return lf_success;
}

int my_lf_int8_result_test(int8_t x) {
    lf_info("Returning int8 result for: %02x", x);
    return lf_success;
}

int my_lf_int16_result_test(int16_t x) {
    lf_info("Returning int16 result for: %04x", x);
    return lf_success;
}

int my_lf_int32_result_test(int32_t x) {
    lf_info("Returning int32 result for: %08x", x);
    return lf_success;
}

int my_lf_int_result_test(int x) {
    lf_info("Returning int result for: %08x", x);
    return lf_success;
}

int my_lf_ptr32_result_test(uint32_t *x) {
    lf_info("Returning ptr32 result for: %08x", *x);
    return lf_success;
}

int my_lf_ptr64_result_test(uint64_t *x) {
    lf_info("Returning ptr64 result for: %016llx", *x);
    return lf_success;
}

int my_lf_int64_result_test(int64_t x) {
    lf_info("Returning int64 result for: %016llx", x);
    return lf_success;
}
