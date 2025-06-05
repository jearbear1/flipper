.syntax unified
.thumb
.global fmr_call
.thumb_func

fmr_call:
    push {r4-r11, lr}

    mov r6, r0      // r6 = function
    mov r7, r1      // r7 = return type
    mov r8, r2      // r8 = argc
    mov r9, r4      // r9 = pointer to _fmr_call (for arg_types)
    mov r10, r5     // r10 = pointer to _lf_ll (argv linked list)
    mov r11, #0     // r11 = argi

load_args:
    cmp r8, #0
    beq call_func

    // Load arg pointer: _lf_ll->item -> _lf_arg
    ldr r4, [r10]       // r4 = _lf_ll->item
    add r10, r10, #12   // advance to next _lf_ll

    ldr r5, [r4, #4]    // r5 = arg->value (offset 4 bytes)
    
    // Load arg type: r9 (call) + 4 (offset) + argi
    mov r0, r9
    add r0, r0, #4      // point to arg_types[0]
    add r0, r0, r11
    ldrb r1, [r0]       // r1 = type

    // Decode by type
    cmp r11, #0
    beq write_r0
    cmp r11, #1
    beq write_r1
    cmp r11, #2
    beq write_r2
    cmp r11, #3
    beq write_r3
    b fail              // more than 4 args not supported

write_r0:
    mov r0, r5
    b next_arg
write_r1:
    mov r1, r5
    b next_arg
write_r2:
    mov r2, r5
    b next_arg
write_r3:
    mov r3, r5
    b next_arg

next_arg:
    add r11, r11, #1
    sub r8, r8, #1
    b load_args

call_func:
    blx r6

    // Return processing
    cmp r7, #2
    beq ret_void
    cmp r7, #16
    beq ret_u8
    cmp r7, #24
    beq ret_s8
    cmp r7, #17
    beq ret_u16
    cmp r7, #25
    beq ret_s16
    cmp r7, #19
    beq ret_u32
    cmp r7, #27
    beq ret_s32
    cmp r7, #3
    beq ret_64
    cmp r7, #4
    beq ret_64
    cmp r7, #6
    beq ret_64
    cmp r7, #7
    beq ret_64
    b fail

ret_void:
    mov r0, #0
    mov r1, #0
    b done
ret_u8:
    and r0, r0, #0xFF
    b done
ret_s8:
    sxtb r0, r0
    b done
ret_u16:
    ldr r4, =0xFFFF // Use a constant loaded into a temporary register r4, then AND with it.
    and r0, r0, r4
    b done
ret_s16:
    sxth r0, r0
    b done
ret_u32:
    b done
ret_s32:
    b done
ret_64:
    // EABI dictates:
    // - r0 = lower 32 bits
    // - r1 = upper 32 bits
    // The function we call via `blx r6` is responsible for populating both.

    b done


fail:
    mov r0, #-1
    mov r1, #-1

done:
    pop {r4-r11, pc}
