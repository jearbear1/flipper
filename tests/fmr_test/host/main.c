#include <flipper.h>
#include "fmrTest.h"

int main(int argc, char *argv[]) {

    // flipper_attach();
    struct _lf_device *fvm = flipper_attach_network("localhost");

    // 16
    my_lf_int8_argument_test(0x10);

    // 60605
    my_lf_int16_argument_test(0xECBD);

    // 4294967295
    my_lf_int32_argument_test(0xFFFFFFFF);

    // 4160483183
    my_lf_int_argument_test(0xF7FBEF6F);

    /*
    // pointer to 32 bit register
    my_lf_ptr32_argument_test(*e);
    // pointer to 64 bit register
    my_lf_ptr64_argument_test(*f);
    */
    
    // 6442450942
    my_lf_int64_argument_test(0x17FFFFFFE);

    // 16
    my_lf_int8_result_test(0x10);

    // 60605
    my_lf_int16_result_test(0xECBD);

    // 4294967295
    my_lf_int32_result_test(0xFFFFFFFF);

    // 4160483183
    my_lf_int_result_test(0xF7FBEF6F);

    /*
    my_lf_ptr32_result_test(uint32_t *l)
    my_lf_ptr64_result_test(u_int64_t *m);
    my_lf_int64_result_test(u_int64_t n);
    */

    return lf_success;

}
