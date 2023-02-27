#define _function x19
#define retv x20
#define argc x21
#define argt x22
#define _argv x18
#define argi x23
#define temp x24

/* lf_return_t fmr_call(lf_return_t (* function)(void), lf_type ret, uint8_t argc, uint16_t argt, void *argv); */

.global _fmr_call
.p2align    2

_fmr_call:
    /* Push Platform Specific Register, Callee Save Registers and Link Register onto stack*/
    STUR     x18, [sp, #-16]
    STUR     x19, [sp, #-32]
    STUR     x20, [sp, #-48]
    STUR     x21, [sp, #-64]
    STUR     x22, [sp, #-80]
    STUR     x23, [sp, #-96]
    STUR     x24, [sp, #-112]
    STUR     LR, [sp, #-128]

    /* Save the function address into x19. */
    MOV _function, X0

    /* Load the return type. */
    MOV retv, X1

    /* Load the argument count into x21. */
    MOV argc, X2

    /* Load the argument types into x22. */
    MOV argt, X3

    /* Load the address of the argument list into x18. */
    LDUR _argv, [sp, #16] /* 64 bytes of stack */

    /* Clear the argument index register. */
    MOV argi, #0

_load:
    CMP argc, #0
    BEQ _call

    /* move X22 into temp */
    MOV temp, argt
    /* encode type with 4 bits register is 64 bits so and with 15 to remove unwanted bits */
    AND temp, temp, 0xF

    # lf_int8_t
    CMP temp, #0
        BEQ _load_8
    # lf_int16_t
    CMP temp, #1
        BEQ _load_16
    # lf_int32_t
    CMP temp, #3
        BEQ _load_32
    # lf_int_t
    CMP temp, #4
        BEQ _load_ptr
    # lf_ptr_t
    CMP temp, #6
        BEQ _load_int
    # lf_int64_t
    CMP temp, #7
        BEQ _load_64


_load_8:
    /* Load a byte from the argument list. */
    LDURSB temp, [_argv, #0]
    /* Increment the argv pointer by one. */
    ADD _argv, _argv, #1
    B _load_done
_load_16:
    /* Load a halfword from the argument list. */
    LDURSH temp, [_argv, #0]
    /* Increment the argv pointer by two. */
    ADD _argv, _argv, #2
    B _load_done
_load_32:
    /* Load a word from the argument list. */
    LDURSW temp, [_argv, #0]
    /* Increment the argv pointer by four. */
    ADD _argv, _argv, #4
    B _load_done
_load_64:
    /* Load a double-word from the argument list. */
    LDUR temp, [_argv, #0]
    /* Increment the argv pointer by eight. */
    ADD _argv, _argv, #8
    B _load_done

_load_int:
    LDUR temp, [_argv, #0]
    ADD _argv, _argv,  #8
_load_ptr:
    LDUR temp, [_argv, #0]
    ADD _argv, _argv, #16

_load_done:
    /* If the argument index is less than 8, load the argument into the appropriate register. */
    CMP argi, #0
        BEQ _write_x0
    CMP argi, #1
        BEQ _write_x1
    CMP argi, #2
        BEQ _write_x2
    CMP argi, #3
        BEQ _write_x3
    B _write_stack

_write_x0:
    MOV x0, temp
    B _write_done
_write_x1:
    MOV x1, temp
    B _write_done
_write_x2:
    MOV x2, temp
    B _write_done
_write_x3:
    MOV x3, temp
    B _write_done
_write_stack:
    B _fail

_write_done:
    /* bit packed argument types where argi is index */
    /* shift argument types right */
    LSR argt, argt, #4
    /* subtract argument count by 1 */
    SUB argc, argc, #1
    /* increase argument index by one */
    ADD argi, argi, #1
    B _load

_call:
    /* Jump to the function. */
    BLR _function

_ret:
    # lf_int8_t
    CMP retv, #0
        BEQ _ret_8
    # lf_int16_t
    CMP retv, #1
        BEQ _ret_16
    # lf_void_t
    CMP retv, #2
        BEQ _ret_void
    # lf_int32_t
    CMP retv, #3
        BEQ _ret_32
    # lf_int_t
    CMP retv, #4
        BEQ _ret_int
    # lf_int_t
    CMP retv, #6
        BEQ _ret_ptr
    # fmr_int
    CMP retv, #7
        BEQ _ret_64


_ret_void:
    /* clear x0 and x1 */
    MOV x0, #0
    MOV x1, #0
    B _call_done
_ret_8:
    B _call_done
_ret_16:
    B _call_done
_ret_int:
_ret_32:
    B _call_done
_ret_ptr:
_ret_64:
    B _call_done

_fail:
    MOV x0, #-1
    MOV x1, #-1
_call_done:
    /* take used registers and program counter off of the stack */
    LDUR     x18, [sp, #16]
    LDUR     x19, [sp, #32]
    LDUR     x20, [sp, #48]
    LDUR     x21, [sp, #64]
    LDUR     x22, [sp, #80]
    LDUR     x23, [sp, #96]
    LDUR     x24, [sp, #112]
    BR       LR


.end
