#include <flipper.h>
#include <stdio.h>
#include <stdint.h>
#include <fmr.h>

__attribute__((visibility("default")))
LF_FUNC lf_return_t fvm_test(void) {
    printf("Running inside FVM! Hello World from Flipper Engineering!\n");
    fflush(stdout);
    return 0;
}

__attribute__((used))
static void *__fvm_test_ptr = fvm_test;

__attribute__((visibility("default")))
lf_function lf_module_fvm_test_table[] = {
    (lf_function)fvm_test
};

__attribute__((visibility("default")))
struct _lf_module fvm_test_module = {
    .name = "fvm_test",
    .version = 1,
    .idx = UINT16_MAX,
    .interface = (void **)lf_module_fvm_test_table,
    .table = lf_module_fvm_test_table,
    .length = 1
};
