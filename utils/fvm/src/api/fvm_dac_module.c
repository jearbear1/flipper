#include <flipper/flipper.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <fmr.h>
#include "dac.h"

#ifdef __APPLE__
#undef LF_FUNC
#define LF_FUNC __attribute__((used))
#endif

// Emulates configuring the DAC
LF_FUNC lf_return_t fvm_dac_configure(const void *args) {
    const char *msg = "[FVM] Called fvm_dac_configure()\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

// Emulates enabling the DAC channels
LF_FUNC lf_return_t fvm_dac_enable_channels(const void *args) {
    const char *msg = "[FVM] Called fvm_dac_enable_channels()\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

// Emulates setting the DAC channels
LF_FUNC lf_return_t fvm_dac_set_channels(const void *args) {
    const char *msg = "[FVM] Called fvm_dac_set_channels()\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

// Emulates writing to the DAC
LF_FUNC lf_return_t fvm_dac_write(const void *args) {
    const char *msg = "[FVM] Called fvm_dac_write(channel)\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

// Emulates cleaning up the DAC
LF_FUNC lf_return_t fvm_dac_cleanup(const void *args) {
    const char *msg = "[FVM] Called fvm_dac_cleanup()\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    return lf_success;
}

// Define the function table
__attribute__((visibility("default")))
lf_function lf_module_dac_table[] = {
    fvm_dac_configure,
    fvm_dac_enable_channels,
    fvm_dac_set_channels,
    fvm_dac_write,
    fvm_dac_cleanup,
};

// Register the module
__attribute__((visibility("default")))
struct _lf_module dac_module = {
    .name = "dac_module",
    .version = 1,
    .idx = UINT16_MAX,
    .interface = NULL,
    .table = lf_module_dac_table,
    .length = sizeof(lf_module_dac_table) / sizeof(lf_module_dac_table[0])
};
