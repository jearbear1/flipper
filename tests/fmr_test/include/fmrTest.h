#ifndef __fmrTest_h__
#define __fmrTest_h__

#include <stdio.h>
#include <stdint.h>  
#include <flipper.h>

LF_FUNC int my_lf_int8_argument_test(uint8_t a);
LF_FUNC int my_lf_int16_argument_test(uint16_t b);
LF_FUNC int my_lf_int32_argument_test(uint32_t c);
LF_FUNC int my_lf_int_argument_test(int d);
LF_FUNC void my_lf_ptr32_argument_test(uint32_t *e);
LF_FUNC void my_lf_ptr64_argument_test(uint64_t *f);
LF_FUNC int my_lf_int64_argument_test(uint64_t g);

LF_FUNC int my_lf_int8_result_test(uint8_t h);
LF_FUNC int my_lf_int16_result_test(uint16_t i);
LF_FUNC int my_lf_int32_result_test(uint32_t j);
LF_FUNC int my_lf_int_result_test(int k);
LF_FUNC void my_lf_ptr32_result_test(uint32_t *l);
LF_FUNC void my_lf_ptr64_result_test(uint64_t *m);
LF_FUNC int my_lf_int64_result_test(uint64_t n);

#endif
