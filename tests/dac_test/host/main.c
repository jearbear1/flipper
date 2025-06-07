#include <flipper.h>
#include <fmr.h>
#include <network.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int read_fvm_port(void) {
    FILE *f = fopen("/tmp/fvm.port", "r");
    int port = LF_UDP_PORT;
    if (f && fscanf(f, "%d", &port) == 1) fclose(f);
    return port;
}

int main(void) {
    int port = read_fvm_port();

    struct _lf_device *device = lf_create_virtual_device();
    if (!device) {
        fprintf(stderr, "Failed to create virtual device.\n");
        return 1;
    }

    // Allocate endpoint context before connecting
    device->_ep_ctx = calloc(1, sizeof(struct _lf_network_context));
    if (!device->_ep_ctx) {
        fprintf(stderr, "Failed to allocate network context.\n");
        lf_device_release(device);
        return 1;
    }

    // Now connection can be made
    if (lf_network_connect(device, "localhost", port) != lf_success) {
        fprintf(stderr, "Failed to connect to FVM.\n");
        lf_device_release(device);
        return 1;
    }

    if (lf_select(device) != lf_success) {
        fprintf(stderr, "Failed to select device\n");
        return 1;
    }

    uint16_t module_index;
    if (lf_dyld_dyn_sym_lookup(device, "dac_module", &module_index) != lf_success) {
        fprintf(stderr, "Module lookup failed\n");
        return 1;
    }

    // Call fvm_dac_configure it's function index 0
    lf_return_t ret;
    if (lf_invoke_by_index(device, "dac_module", 0, lf_void_t, &ret, NULL) != lf_success) {
        fprintf(stderr, "Failed to invoke dac_module [0]\n");
        return 1;
    }
    printf("DAC configured successfully\n");
    
    // Call fvm_dac_enable_channels it's function index 1

    if (lf_invoke_by_index(device, "dac_module", 1, lf_void_t, &ret, NULL) != lf_success) {
        fprintf(stderr, "Failed to invoke dac_module [1]\n");
        return 1;
    }
    printf("DAC enableld channels successfully\n");
    
    // Call fvm_dac_set_channels it's function index 2
    
    if (lf_invoke_by_index(device, "dac_module", 2, lf_void_t, &ret, NULL) != lf_success) {
        fprintf(stderr, "Failed to invoke dac set channel [2]\n");
        return 1;
    }
    printf("DAC set channels successfully\n");
    
    // Call set channel it's function index 3
    
    if (lf_invoke_by_index(device, "dac_module", 3, lf_void_t, &ret, NULL) != lf_success) {
        fprintf(stderr, "Failed to invoke fvm_dac_write [3]\n");
        return 1;
    }
    printf("DAC write called successfully\n");
    
    // Call fvm dac cleanup it's function index 4
    
    if (lf_invoke_by_index(device, "dac_module", 4, lf_void_t, &ret, NULL) != lf_success) {
        fprintf(stderr, "Failed to invoke dac_module[0]\n");
        return 1;
    }
    printf("DAC cleanup called successfully\n");
    
   
    lf_exit();
    return 0;
}
