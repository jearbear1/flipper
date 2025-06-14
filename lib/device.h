#ifndef __lf_device_h__
#define __lf_device_h__

/* Macros that quantify device attributes. */
#define lf_device_8bit (1 << 1)
#define lf_device_16bit (1 << 2)
#define lf_device_32bit (1 << 3)
#define lf_device_big_endian 1
#define lf_device_little_endian 0

typedef enum { FLIPPER_DEVICE_CARBON, FLIPPER_DEVICE_INVALID } lf_device_t;

/* Describes a device capible of responding to FMR packets. */
struct _lf_device {
    /* The human readable name of the device. */
    char name[32];
    /* The device's firmware version. */
    lf_version_t version;
    /* The type of device */
    lf_device_t type;
    /* The modules loaded on the device. */
    struct _lf_ll *modules;
    /* Receives arbitrary data from the device. */
    int (*read)(struct _lf_device *device, void *dst, uint32_t length);
    /* Transmits arbitrary data to the device. */
    int (*write)(struct _lf_device *device, void *src, uint32_t length);
    /* Releases device state. */
    int (*release)(void *device);
    /* The device's context. */
    void *_dev_ctx;
    /* The endpoint's context. */
    void *_ep_ctx;
};

struct _lf_device *lf_device_create(int (*read)(struct _lf_device *device, void *dst, uint32_t length),
                                    int (*write)(struct _lf_device *device, void *src, uint32_t length),
                                    int (*release)(void *device));

struct _lf_device *lf_device_create_named(const char *name,
                                          int (*read)(struct _lf_device *device, void *dst, uint32_t length),
                                          int (*write)(struct _lf_device *device, void *src, uint32_t length),
                                          int (*release)(void *device));

struct _lf_device *lf_create_virtual_device(void);

struct _lf_device *lf_create_physical_device(void);


void lf_device_release(void *device);


#endif
