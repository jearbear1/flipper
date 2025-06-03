#ifndef __fmrTest_h__
#define __fmrTest_h__

#include <flipper.h>

lf_return_t my_lf_int8_argument_test(int8_t a);
lf_return_t my_lf_int16_argument_test(int16_t b);
lf_return_t my_lf_int32_argument_test(int32_t c);
lf_return_t my_lf_int_argument_test(int d);
lf_return_t my_lf_ptr32_argument_test(uint32_t *e);
lf_return_t my_lf_ptr64_argument_test(uint64_t *f);
lf_return_t my_lf_int64_argument_test(int64_t g);

lf_return_t my_lf_int8_result_test(int8_t h);
lf_return_t my_lf_int16_result_test(int16_t i);
lf_return_t my_lf_int32_result_test(int32_t j);
lf_return_t my_lf_int_result_test(int k);
lf_return_t my_lf_ptr32_result_test(uint32_t *l);
lf_return_t my_lf_ptr64_result_test(uint64_t *m);
lf_return_t my_lf_int64_result_test(int64_t n);

#endif
