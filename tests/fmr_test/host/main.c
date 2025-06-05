#include <flipper.h>
#include <fmr.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#ifdef FLIPPER_PLATFORM_POSIX
#include <network.h>
#endif

int read_fvm_port(void) {
    FILE *portfile = fopen("/tmp/fvm.port", "r");
    if (!portfile) {
        fprintf(stderr, "[client] Failed to open /tmp/fvm.port: %s\n", strerror(errno));
        return -1;
    }
    int port;
    if (fscanf(portfile, "%d", &port) != 1) {
        fprintf(stderr, "[client] Failed to read port from /tmp/fvm.port\n");
        fclose(portfile);
        return -1;
    }
    fclose(portfile);
    return port;
}

void debug_packet(const char *label, const uint8_t *data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3 || strcmp(argv[1], "--device") != 0) {
        fprintf(stderr, "Usage: %s --device <fvm|usb|uart> <module_name>\n", argv[0]);
        return 1;
    }

    const char *device_type = argv[2];
    const char *module_name = (argc > 3) ? argv[3] : NULL;
    if (!module_name) {
        fprintf(stderr, "[client] Error: Module name required.\n");
        return 1;
    }
    printf("[client] Using module name: %s\n", module_name);

    // Create and select device based on type
    struct _lf_device *device = NULL;
    int port = LF_UDP_PORT; // Default port

    if (strcmp(device_type, "fvm") == 0) {
#ifdef FLIPPER_PLATFORM_POSIX
        // Read dynamic port for FVM
        port = read_fvm_port();
        if (port < 0) {
            fprintf(stderr, "[client] Failed to determine FVM port, falling back to default %d\n", LF_UDP_PORT);
            port = LF_UDP_PORT;
        }
        printf("[client] Using FVM port: %d\n", port);

        device = lf_create_virtual_device();
        if (!device) {
            fprintf(stderr, "[client] Failed to create FVM device.\n");
            return 1;
        }

        // Initialize network context and connect to FVM
        device->_ep_ctx = calloc(1, sizeof(struct _lf_network_context));
        if (!device->_ep_ctx) {
            fprintf(stderr, "[client] Failed to allocate network context for FVM.\n");
            lf_device_release(device);
            return 1;
        }
        if (lf_network_connect(device, "localhost", port) != lf_success) {
            fprintf(stderr, "[client] Failed to connect to FVM on localhost:%d\n", port);
            lf_device_release(device);
            return 1;
        }
#else
        fprintf(stderr, "[client] FVM not supported on this platform.\n");
        return 1;
#endif
    } else if (strcmp(device_type, "usb") == 0) {
        fprintf(stderr, "[client] USB device support is not implemented.\n");
        return 1;
    } else if (strcmp(device_type, "uart") == 0) {
        fprintf(stderr, "[client] UART device support is not implemented.\n");
        return 1;
    } else {
        fprintf(stderr, "[client] Invalid device type: %s. Use 'fvm', 'usb', or 'uart'.\n", device_type);
        return 1;
    }

    if (lf_select(device) != lf_success) {
        fprintf(stderr, "[client] Failed to select device.\n");
        lf_device_release(device);
        return 1;
    }

    printf("[client] Connected to device '%s'.\n", device->name);


        // Lookup module index
        uint16_t module_idx;
        if (lf_dyld_dyn_sym_lookup(device, module_name, &module_idx) != lf_success) {
            fprintf(stderr, "[client] Failed to lookup module '%s' on device '%s'.\n", module_name, device->name);
            lf_detach(device);
            lf_device_release(device);
            lf_exit();
            return 1;
        }
        printf("[client] Module index for '%s': %u\n", module_name, module_idx);

        int result;
        struct _lf_ll *args = NULL;
        lf_return_t ret_value;
        int64_t expected;

        // Function 0: my_lf_uint8_argument_test
        printf("[client] Sending RPC packet to invoke '%s' function 0\n", module_name);
        args = NULL;
        uint8_t arg0_val = 16;
        struct _lf_arg *arg0 = lf_arg_create(lf_uint8_t, (lf_arg)arg0_val); // Use lf_uint8_t (16)
        if (!arg0 || lf_ll_append(&args, arg0, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 0\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = 0;
        result = lf_invoke_by_index(device, module_name, 0, lf_uint8_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 0 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 0: error %d\n", module_name, result);
        }
        lf_ll_release(&args);

        // Function 1: my_lf_uint16_argument_test
        printf("[client] Sending RPC packet to invoke '%s' function 1\n", module_name);
        args = NULL;
        uint16_t arg1_val = 4931;
        struct _lf_arg *arg1 = lf_arg_create(lf_uint16_t, (lf_arg)arg1_val); // Use lf_uint16_t (17)
        if (!arg1 || lf_ll_append(&args, arg1, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 1\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = 0;
        result = lf_invoke_by_index(device, module_name, 1, lf_uint16_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 1 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 1: error %d\n", module_name, result);
        }
        lf_ll_release(&args);

        // Function 2: my_lf_uint32_argument_test
        printf("[client] Sending RPC packet to invoke '%s' function 2\n", module_name);
        args = NULL;
        uint32_t arg2_val = 70000;
        struct _lf_arg *arg2 = lf_arg_create(lf_int32_t, (lf_arg)arg2_val); // Use lf_int32_t (27)
        if (!arg2 || lf_ll_append(&args, arg2, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 2\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = 0;
        result = lf_invoke_by_index(device, module_name, 2, lf_int32_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 2 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 2: error %d\n", module_name, result);
        }
        lf_ll_release(&args);

        // Function 3: my_lf_int_argument_test
        printf("[client] Sending RPC packet to invoke '%s' function 3\n", module_name);
        args = NULL;
        int arg3_val = 111;
        struct _lf_arg *arg3 = lf_arg_create(lf_int_t, (lf_arg)arg3_val); // Use lf_int_t (4)
        if (!arg3 || lf_ll_append(&args, arg3, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 3\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = 0;
        result = lf_invoke_by_index(device, module_name, 3, lf_int_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 3 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 3: error %d\n", module_name, result);
        }
        lf_ll_release(&args);

        // Function 4: my_lf_uint8_result_test
        printf("[client] Sending RPC packet to invoke '%s' function 4\n", module_name);
        args = NULL;
        uint8_t arg4_val = 16;
        struct _lf_arg *arg4 = lf_arg_create(lf_uint8_t, (lf_arg)arg4_val); // Use lf_uint8_t (16)
        if (!arg4 || lf_ll_append(&args, arg4, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 7\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = (int64_t)(arg4_val * 2);
        result = lf_invoke_by_index(device, module_name, 4, lf_uint8_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 4 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 4: error %d\n", module_name, result);
        }
        lf_ll_release(&args);

        // Function 5: my_lf_uint16_result_test
        printf("[client] Sending RPC packet to invoke '%s' function 5\n", module_name);
        args = NULL;
        uint16_t arg5_val = 4931;
        struct _lf_arg *arg5 = lf_arg_create(lf_uint16_t, (lf_arg)arg5_val); // Use lf_uint16_t (17)
        if (!arg5 || lf_ll_append(&args, arg5, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 8\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = (int64_t)(arg5_val * 2);
        result = lf_invoke_by_index(device, module_name, 5, lf_uint16_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 5 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 5: error %d\n", module_name, result);
        }
        lf_ll_release(&args);

        // Function 6: my_lf_uint32_result_test
        printf("[client] Sending RPC packet to invoke '%s' function 6\n", module_name);
        args = NULL;
        uint32_t arg6_val = 70000;
        struct _lf_arg *arg6 = lf_arg_create(lf_uint32_t, (lf_arg)arg6_val); // Use lf_uint32_t (19)
        if (!arg6 || lf_ll_append(&args, arg6, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 9\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = (int64_t)(arg6_val * 2);
        result = lf_invoke_by_index(device, module_name, 6, lf_uint32_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 6 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 6: error %d\n", module_name, result);
        }
        lf_ll_release(&args);

        // Function 7: my_lf_int_result_test
        printf("[client] Sending RPC packet to invoke '%s' function 7\n", module_name);
        args = NULL;
        int arg7_val = 111;
        struct _lf_arg *arg7 = lf_arg_create(lf_int_t, (lf_arg)arg7_val); // Use lf_int_t (4)
        if (!arg7 || lf_ll_append(&args, arg7, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 10\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = (int64_t)(arg7_val * 2);
        result = lf_invoke_by_index(device, module_name, 7, lf_int_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 7 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 7: error %d\n", module_name, result);
        }
        lf_ll_release(&args);
    
        // Function 8: my_lf_int8_argument_test
        printf("[client] Sending RPC packet to invoke '%s' function 8\n", module_name);
        args = NULL;
        int8_t arg8_val = -16;
        struct _lf_arg *arg8 = lf_arg_create(lf_int8_t, (lf_arg)arg8_val); // Use lf_uint8_t (24)
        if (!arg8 || lf_ll_append(&args, arg8, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 8\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = 0;
        result = lf_invoke_by_index(device, module_name, 8, lf_int8_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 8 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 8: error %d\n", module_name, result);
        }
        lf_ll_release(&args);
    
        // Function 9: my_lf_int8_result_test
        printf("[client] Sending RPC packet to invoke '%s' function 9\n", module_name);
        args = NULL;
        int8_t arg9_val = -16;
        struct _lf_arg *arg9 = lf_arg_create(lf_int8_t, (lf_arg)arg9_val); // Use lf_int8_t (24)
        if (!arg9 || lf_ll_append(&args, arg9, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 9\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = (int64_t)(arg9_val * 2);
        result = lf_invoke_by_index(device, module_name, 9, lf_int8_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 9 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 9: error %d\n", module_name, result);
        }
        lf_ll_release(&args);

    
        // Function 10: my_lf_int16_argument_test
        printf("[client] Sending RPC packet to invoke '%s' function 10\n", module_name);
        args = NULL;
        int16_t arg10_val = -4931;
        struct _lf_arg *arg10 = lf_arg_create(lf_int16_t, (lf_arg)arg10_val); // Use lf_int16_t (25)
        if (!arg10 || lf_ll_append(&args, arg10, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 10\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = 0;
        result = lf_invoke_by_index(device, module_name, 10, lf_int8_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 10 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 10: error %d\n", module_name, result);
        }
        lf_ll_release(&args);

        // Function 11: my_lf_int16_result_test
        printf("[client] Sending RPC packet to invoke '%s' function 11\n", module_name);
        args = NULL;
        int16_t arg11_val = -4931;
        struct _lf_arg *arg11 = lf_arg_create(lf_int16_t, (lf_arg)arg11_val); // Use lf_int16_t (25)
        if (!arg11 || lf_ll_append(&args, arg11, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 11\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = (int64_t)(arg11_val * 2);
        result = lf_invoke_by_index(device, module_name, 11, lf_int16_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 11 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 11: error %d\n", module_name, result);
        }
        lf_ll_release(&args);
    
        // Function 12: my_lf_int32_argument_test
        printf("[client] Sending RPC packet to invoke '%s' function 12\n", module_name);
        args = NULL;
        int32_t arg12_val = -70000;
        struct _lf_arg *arg12 = lf_arg_create(lf_int32_t, (lf_arg)arg12_val); // Use lf_int32_t (27)
        if (!arg12 || lf_ll_append(&args, arg12, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 12\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = 0;
        result = lf_invoke_by_index(device, module_name, 12, lf_int32_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 12 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 12: error %d\n", module_name, result);
        }
        lf_ll_release(&args);

        // Function 13: my_lf_int32_result_test
        printf("[client] Sending RPC packet to invoke '%s' function 13\n", module_name);
        args = NULL;
        int32_t arg13_val = -70000;
        struct _lf_arg *arg13 = lf_arg_create(lf_int32_t, (lf_arg)arg13_val); // Use lf_int32_t (27)
        if (!arg13 || lf_ll_append(&args, arg13, free) != lf_success) {
            fprintf(stderr, "[client] Failed to create or append arg for function 13\n");
            lf_ll_release(&args);
            goto cleanup;
        }
        expected = (int64_t)(arg13_val * 2);
        result = lf_invoke_by_index(device, module_name, 13, lf_int16_t, &ret_value, args);
        if (result == lf_success) {
            printf("[client] %s() function 13 returned: %llu\n", module_name, ret_value);
            printf("[client] Expected return: %lld, Actual: %lld, %s\n", expected, (int64_t)ret_value,
                   expected == (int64_t)ret_value ? "PASS" : "FAIL");
        } else {
            fprintf(stderr, "[client] Failed to invoke %s function 13: error %d\n", module_name, result);
        }
        lf_ll_release(&args);
    
        // Success Cleanup
        printf("[client] FMR tests complete.\n");
        lf_detach(device);
        lf_device_release(device);
        lf_exit();
        return 0;

    cleanup:
        lf_detach(device);
        lf_device_release(device);
        lf_exit();
        return 1;
    }
