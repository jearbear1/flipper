#include <flipper.h>
#include "fmrTest.h"

int main(int argc, char *argv[]) {
    
    printf("attaching to flipper device. \n");
    
    // flipper_attach();
    struct _lf_device *fvm = flipper_attach_network("localhost");

    printf("Running FMR tests...\n");
    
    // 8-bit argument test:  16
    my_lf_int8_argument_test(0x10);

    // 16-bit argument test: 60605
    my_lf_int16_argument_test(0xECBD);

    // 32-bit argument test: 4294967295
    my_lf_int32_argument_test(0xFFFFFFFF);

    // 32-bit int argument test: 4160483183
    my_lf_int_argument_test(0xF7FBEF6F);

    // Pointer-based argument tests
    uint32_t ptr32_val = 0xCAFEBABE;
    uint64_t ptr64_val = 0x123456789ABCDEF0;
    my_lf_ptr32_argument_test(&ptr32_val);
    my_lf_ptr64_argument_test(&ptr64_val);
    
    // 64-bit argument test: 6442450942
    my_lf_int64_argument_test(0x17FFFFFFE);

    // 8-bit result test: 16
    my_lf_int8_result_test(0x10);

    // 16-bit result test 60605
    my_lf_int16_result_test(0xECBD);

    // 32-bit result test 4294967295
    my_lf_int32_result_test(0xFFFFFFFF);

    // 32-bit int result test 4160483183
    my_lf_int_result_test(0xF7FBEF6F);

    // Pointer-based result tests
    my_lf_ptr32_result_test(&ptr32_val);
    my_lf_ptr64_result_test(&ptr64_val);

    // 64-bit result test
    my_lf_int64_result_test(0x17FFFFFFE);
    
    printf("FMR tests complete.\n");
    
    return lf_success;

}
