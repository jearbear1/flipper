// os/arch/arm64/fmr.S
// Function call wrapper for ARM64, handling dynamic argument loading and return type processing.

.text
.arch armv8-a                     // Explicitly specify AArch64 architecture
.global _fmr_call
.p2align 2                        // Align to 4-byte boundary

// Register aliases
#define _function   x19
#define retv        x20
#define argc        x21
#define _fmr_call   x22
#define _argv       x18
#define argi        x23
#define temp        x24
#define arg_ptr     x25
#define type        x26

_fmr_call:
    stp     _argv, _function, [sp, #-16]!   // Save x18, x19
    stp     retv, argc, [sp, #-16]!         // Save x20, x21
    stp     _fmr_call, argi, [sp, #-16]!    // Save x22, x23
    stp     temp, arg_ptr, [sp, #-16]!      // Save x24, x25
    stp     type, x27, [sp, #-16]!          // Save x26, x27
    stp     lr, xzr, [sp, #-16]!            // Save lr, pad with xzr

    mov     _function, x0       // function pointer
    mov     retv, x1            // return type
    mov     argc, x2            // argument count
    mov     _fmr_call, x3       // pointer to _fmr_call structure
    mov     _argv, x4           // pointer to argument linked list
    mov     argi, #0            // argument index

_load:
    cmp     argc, #0
    beq     _call

    ldr     arg_ptr, [_argv]           // arg_ptr = _argv->item
    add     _argv, _argv, #24          // Move to next node
    add     arg_ptr, arg_ptr, #8       // arg_ptr points to arg->value

    add     temp, _fmr_call, #4        // Base of arg_types (offset 4)
    add     temp, temp, argi           // temp = base + index
    ldrb    w26, [temp, #0]           // loads 8-bit unsigned values into a 32-bit W register,

    cmp     type, #16
    beq     _load_u8
    cmp     type, #24
    beq     _load_s8
    cmp     type, #17
    beq     _load_u16
    cmp     type, #25
    beq     _load_s16
    cmp     type, #19
    beq     _load_u32
    cmp     type, #27
    beq     _load_s32
    cmp     type, #4
    beq     _load_64
    cmp     type, #6
    beq     _load_64
    cmp     type, #3
    beq     _load_64
    cmp     type, #7
    beq     _load_64
    b       _fail

_load_u8:
    ldrb    w24, [arg_ptr, #0]
    b       _load_done

_load_s8:
    ldrsb   temp, [arg_ptr, #0]
    b       _load_done

_load_u16:
    ldrh    w24, [arg_ptr, #0]
    b       _load_done

_load_s16:
    ldrsh   temp, [arg_ptr, #0]
    b       _load_done

_load_u32:
    ldr     temp, [arg_ptr]
    b       _load_done

_load_s32:
    ldrsw   temp, [arg_ptr]
    b       _load_done

_load_64:
    ldr     temp, [arg_ptr]
    b       _load_done

_load_done:
    cmp     argi, #0
    beq     _write_x0
    cmp     argi, #1
    beq     _write_x1
    cmp     argi, #2
    beq     _write_x2
    cmp     argi, #3
    beq     _write_x3
    b       _write_stack

_write_x0:
    mov     x0, temp
    b       _write_done

_write_x1:
    mov     x1, temp
    b       _write_done

_write_x2:
    mov     x2, temp
    b       _write_done

_write_x3:
    mov     x3, temp
    b       _write_done

_write_stack:
    b       _fail                 // Stack arguments not supported

_write_done:
    add     argi, argi, #1
    sub     argc, argc, #1
    b       _load

_call:
    blr     _function

_ret:
    cmp     retv, #2
    beq     _ret_void
    cmp     retv, #16
    beq     _ret_u8
    cmp     retv, #24
    beq     _ret_s8
    cmp     retv, #17
    beq     _ret_u16
    cmp     retv, #25
    beq     _ret_s16
    cmp     retv, #19
    beq     _ret_u32
    cmp     retv, #27
    beq     _ret_s32
    cmp     retv, #4
    beq     _ret_64
    cmp     retv, #6
    beq     _ret_64
    cmp     retv, #3
    beq     _ret_64
    cmp     retv, #7
    beq     _ret_64
    b       _fail

_ret_void:
    mov     x0, #0
    mov     x1, #0
    b       _call_done

_ret_u8:
    and     x0, x0, #0xFF
    b       _call_done

_ret_s8:
    sxtb    x0, x0
    b       _call_done

_ret_u16:
    and     x0, x0, #0xFFFF
    b       _call_done

_ret_s16:
    sxth    x0, x0
    b       _call_done

_ret_u32:
    uxtw    x0, x0
    b       _call_done

_ret_s32:
    sxtw    x0, x0
    b       _call_done

_ret_64:
    b       _call_done

_fail:
    mov     x0, #-1
    mov     x1, #-1

_call_done:
    ldp     lr, xzr, [sp], #16
    ldp     type, x27, [sp], #16
    ldp     temp, arg_ptr, [sp], #16
    ldp     _fmr_call, argi, [sp], #16
    ldp     retv, argc, [sp], #16
    ldp     _argv, _function, [sp], #16
    ret

.end
