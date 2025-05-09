#include "dac.h"

// Wrapper functions (convert from raw args to strongly typed calls)

LF_FUNC lf_return_t _dac_configure(const void *args) {
    return dac_configure();
}

LF_FUNC lf_return_t _dac_enable_channels(const void *args) {
    int i = *(int *)args;
    return dac_enbale_channels(i);
}

LF_FUNC lf_return_t _dac_set_channels(const void *args) {
    int i = *(int *)args;
    return dac_set_channels(i);
}

LF_FUNC lf_return_t _dac_write(const void *args) {
    return dac_write();
}

LF_FUNC lf_return_t _dac_cleanup(const void *args) {
    return dac_cleanup();
}

// Function table : array of function pointers
lf_function dac_table[] = {
    _dac_configure,
    _dac_enable_channels,
    _dac_set_channels,
    _dac_write,
    _dac_cleanup
};
