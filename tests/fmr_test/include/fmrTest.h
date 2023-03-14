#ifndef __fmrTest_h__
#define __fmrTest_h__

#include <stdio.h>
#include <flipper.h>

int my_lf_int8_argument_test(uint8_t a);
int my_lf_int16_argument_test(uint16_t b);
int my_lf_int32_argument_test(uint32_t c);
int my_lf_int_argument_test(uint32_t d);
void my_lf_ptr32_argument_test(uint32_t *e);
void my_lf_ptr64_argument_test(uint64_t *f);
int my_lf_int64_argument_test(uint64_t g);

int my_lf_int8_result_test(uint8_t h);
int my_lf_int16_result_test(uint16_t i);
int my_lf_int32_result_test(uint32_t j);
int my_lf_int_result_test(uint32_t k);
void my_lf_ptr32_result_test(uint32_t *l);
void my_lf_ptr64_result_test(u_int64_t *m);
int my_lf_int64_result_test(u_int64_t n);

#endif
