#ifndef __fmrTest_h__
#define __fmrTest_h__

#include <flipper.h>
#include <stdint.h>

// Argument-only tests
lf_return_t my_lf_uint8_argument_test(struct _lf_ll *args);
lf_return_t my_lf_uint16_argument_test(struct _lf_ll *args);
lf_return_t my_lf_uint32_argument_test(struct _lf_ll *args);
lf_return_t my_lf_int_argument_test(struct _lf_ll *args);
lf_return_t my_lf_int8_argument_test(struct _lf_ll *args);
lf_return_t my_lf_int16_argument_test(struct _lf_ll *args);
lf_return_t my_lf_int32_argument_test(struct _lf_ll *args);

// Return tests (functions return double the input value encoded in lf_return_t)
lf_return_t my_lf_uint8_result_test(struct _lf_ll *args);      // returns uint8_t doubled
lf_return_t my_lf_uint16_result_test(struct _lf_ll *args);     // returns uint16_t doubled
lf_return_t my_lf_uint32_result_test(struct _lf_ll *args);     // returns uint32_t doubled
lf_return_t my_lf_int_result_test(struct _lf_ll *args);        // returns int doubled
lf_return_t my_lf_int8_result_test(struct _lf_ll *args);      // returns int8_t doubled
lf_return_t my_lf_int16_result_test(struct _lf_ll *args);      // returns int16_t doubled
lf_return_t my_lf_int32_result_test(struct _lf_ll *args);      // returns int132_t doubled



#endif /* __fmrTest_h__ */
