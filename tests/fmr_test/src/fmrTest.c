#include <flipper.h>
#include <fmrTest.h>


LF_FUNC my_lf_int8_argument_test(uint8_t a) {

    if (a == 0x10 ) {
        printf("argument = %d",a);
        return lf_success;
    } else {
        printf("argument is not of type uint8");
         return lf_error;
    }
}

LF_FUNC my_lf_int16_argument_test(uint16_t b) {

     if (b == 0xECBD) {
        printf("argument = %d",b);
        return lf_success;
    } else {
        printf("argument is not of type uint16");
         return lf_error;
    }
}

LF_FUNC my_lf_int32_argument_test(uint32_t c) {

     if (c == 0xFFFFFFFF) {
        printf("argument = %d",c);
        return lf_success;
    } else {
        printf("argument is not of type uint32");
         return lf_error;
    }
}

LF_FUNC my_lf_int_argument_test(uint32_t d) {

    if (d == 0xF7FBEF6F) {
        printf("argument = %d",d);
        return lf_success;
    } else {
        printf("argument is not of type uint32");
         return lf_error;
    }

}

/*
LF_FUNC my_lf_ptr32_argument_test(uint32_t *e) {
    if (*e ==  ) {
        return lf_success;
    } else {
        return lf_error;
    }
}
*/

/*
LF_FUNC my_lf_ptr64_argument_test(uint64_t *f) {
  if (*f ==  ) {
        return lf_success;
    } else {
        return lf_error;
    }
}
*/

LF_FUNC my_lf_int64_argument_test(u_int64_t g) {

    if (g == 0x17FFFFFFE) {
        printf("argument = %d",g);
        return lf_success;
    } else {
        printf("argument is not of type uint64");
        return lf_error;
    }

}

LF_FUNC my_lf_int8_result_test(uint8_t h) {

    if (h ==  0x10) {
        return h;
    } else {
        return lf_error;
    }
}

LF_FUNC my_lf_int16_result_test(uint16_t i){

     if (i ==  0xECBD) {
        return i;
    } else {
        return lf_error;
    }
}

LF_FUNC my_lf_int32_result_test(uint32_t j) {

   if (j ==  0xFFFFFFFF) {
        return j;
    } else {
        return lf_error;
    }

}

LF_FUNC my_lf_int_result_test(_uint32_t k) {

    if (k ==  0xF7FBEF6F) {
        return k;
    } else {
        return lf_error;
    }
}

/*
LF_FUNC my_lf_ptr32_result_test(_uint32_t *l)
my_lf_ptr64_result_test(_uint64_t *m);
*/

LF_FUNC my_lf_int64_result_test(u_int64_t n) { 

    if (n == 0x17FFFFFFE) {
        return n;
    } else {
        return lf_error;
    }
}
