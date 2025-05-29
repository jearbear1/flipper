#include <flipper.h>  // flipper operating system
#include <fmr.h>      // for brokering remote procedure calls between host and device
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>   // for write(), STDOUT_FILENO
#include <string.h>   // for strlen()

__attribute__((visibility("default")))
LF_FUNC lf_return_t fvm_print_hello_function(void) {
    const char *msg = "Running inside FVM! Hello World from Flipper Engineering!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    return 123;
}

__attribute__((visibility("default")))
lf_function lf_module_fvm_test_table[] = {
    (lf_function)fvm_print_hello_function
};

__attribute__((visibility("default")))
struct _lf_module fvm_test = {
    .name = "fvm_test",
    .version = 1,
    .idx = UINT16_MAX,
    .interface = NULL,
    .table = lf_module_fvm_test_table,
    .length = sizeof(lf_module_fvm_test_table) / sizeof(lf_module_fvm_test_table[0])
};
