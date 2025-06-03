#include "libflipper.h"
#include <string.h> // For strncpy
#include "error.h" // for LF_ERROR(E_TYPE, "type error")
// Ensure Network.C is only included when compiling for host/posix
#ifdef FLIPPER_PLATFORM_POSIX
#include "network.h"
#endif

// Creates a new libflipper device with an optional name. This is for FVM
struct _lf_device *lf_device_create_named(const char *name,
                                          int (*read)(struct _lf_device *device, void *dst, uint32_t length),
                                          int (*write)(struct _lf_device *device, void *src, uint32_t length),
                                          int (*release)(void *device)) {
    struct _lf_device *device = NULL;
    device = (struct _lf_device *)calloc(1, sizeof(struct _lf_device));
    lf_assert(device, E_MALLOC, "Failed to allocate memory for new device.");
    device->read = read;
    device->write = write;
    device->release = release;

    /*
     zero Initialize the name to ensure that the Client and FVM
     refer to the same thing. when trying to communicate
     */
    if (name) {
        memset(device->name, 0, sizeof(device->name));
        strncpy(device->name, name, sizeof(device->name) - 1);
        device->name[sizeof(device->name) - 1] = '\0';
    }

    return device;
fail:
    free(device);
    return NULL;
}

// Creates a new libflipper device. This is for Physical Devices
struct _lf_device *lf_device_create(int (*read)(struct _lf_device *device, void *dst, uint32_t length),
                                    int (*write)(struct _lf_device *device, void *src, uint32_t length),
                                    int (*release)(void *device)) {
    return lf_device_create_named(NULL, read, write, release);
}

void lf_device_release(void *_device) {
    struct _lf_device *device = (struct _lf_device *)_device;
    lf_assert(device, E_NULL, "invalid device");
    free(device);
fail:
    return;
}

/* Creates a virtual device for FVM usage over network */
#ifdef FLIPPER_PLATFORM_POSIX
struct _lf_device *lf_create_virtual_device() {
    return lf_device_create_named("fvm", lf_network_read, lf_network_write, lf_network_release);
}
#endif


/* Creates a physical device. Can be extended to support multiple transport layers */

/*
struct _lf_device *lf_create_physical_device(const char *transport) {
    if (strcmp(transport, "usb") == 0) {
        return lf_device_create_named(NULL, usb_read, usb_write, usb_release);
    } else if (strcmp(transport, "uart") == 0) {
        return lf_device_create_named(NULL, uart_read, uart_write, uart_release);
    } else if (strcmp(transport, "usart") == 0) {
        return lf_device_create_named(NULL, usart_read, usart_write, usart_release);
    } else {
        LF_ERROR(E_TYPE, "type error");
        return NULL;
    }
}
*/
