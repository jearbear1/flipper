#include "libflipper.h"
#include "fmr.h"

static struct _lf_ll *lf_attached_devices;
static struct _lf_device *lf_current_device;

extern const char lf_git_hash[];

const char *lf_get_git_hash(void) {
    return lf_git_hash;
}

/* Setter for the lf_current_device global. */
static void lf_set_current_device(struct _lf_device *device) {
    lf_current_device = device;
}

/* Getter for the lf_current_device global. */
static struct _lf_device *lf_get_current_device(void) {
    return lf_current_device;
}

int lf_attach(void) {
#ifdef LF_POSIX
    struct _lf_ll *usb_devices = lf_libusb_get_devices();
    lf_ll_concat(&lf_attached_devices, usb_devices);
    return lf_success;
fail:
#endif
    return lf_error;
}

int lf_select(struct _lf_device *device) {
    lf_assert(device, E_NULL, "invalid device");
    lf_set_current_device(device);
    return lf_success;
fail:
    return lf_error;
}

struct _lf_device *lf_get_selected(void) {
    return lf_get_current_device();
}

int lf_detach(struct _lf_device *device) {
    lf_assert(device, E_NULL, "invalid device provided to detach.");
    lf_ll_remove(&lf_attached_devices, device);
    return lf_success;
fail:
    return lf_error;
}

int __attribute__((__destructor__)) lf_exit(void) {
    lf_ll_release(&lf_attached_devices);
    return lf_success;
}

/*
 Used when host code calls a known function pointer.
 */
int lf_invoke(struct _lf_device *device, const char *module, lf_function function, lf_type ret, lf_return_t *retval, struct _lf_ll *args) {
    struct _fmr_packet _packet;
    struct _fmr_call_packet *packet = (struct _fmr_call_packet *)&_packet;
    struct _fmr_header *hdr = &packet->hdr;
    struct _fmr_result result;
    struct _lf_module *m = NULL;
    int e;
    lf_crc_t crc;

    lf_assert(device, E_NULL, "invalid device");
    lf_assert(module, E_NULL, "invalid module");

    memset(&_packet, 0, sizeof(_packet));
    hdr->magic = FMR_MAGIC_NUMBER;
    hdr->len = sizeof(packet->hdr);

    m = dyld_module(device, module);
    lf_assert(m, E_MODULE, "No counterpart found for module '%s'.", module);

    e = lf_create_call(m->idx, function, ret, args, hdr, &packet->call);
    lf_assert(e, E_NULL, "Failed to generate a valid call to module '%s'.", module);

    lf_crc(packet, hdr->len, &crc);
    hdr->crc = crc;
    lf_debug_packet((struct _fmr_packet *)packet);

    e = device->write(device, packet, sizeof(_packet));
    lf_assert(e, E_ENDPOINT, "Failed to send message to device '%s'.", device->name);

    e = device->read(device, &result, sizeof(struct _fmr_result));
    lf_assert(e, E_ENDPOINT, "Failed to receive message from the device '%s'.", device->name);

    lf_debug_result(&result);
    lf_assert(result.error == E_OK, result.error, "An error occurred on the device '%s':", device->name);

    *retval = result.value;

    return lf_success;
fail:
    return lf_error;
}

/*
 Used when host code knows the function index, not the pointer.
 */
int lf_invoke_by_index(struct _lf_device *device, const char *module, uint8_t index, lf_type ret, lf_return_t *retval, struct _lf_ll *args) {
    struct _fmr_packet _packet;
    struct _fmr_call_packet *packet = (struct _fmr_call_packet *)&_packet;
    struct _fmr_header *hdr = &packet->hdr;
    struct _fmr_call *call = &packet->call;
    struct _lf_module *m;
    struct _fmr_result result;
    lf_crc_t crc;
    int e;

    lf_assert(device, E_NULL, "invalid device");
    lf_assert(module, E_NULL, "invalid module name");

    memset(&_packet, 0, sizeof(_packet));

    m = dyld_module(device, module);
    lf_assert(m, E_MODULE, "No counterpart found for module '%s'.", module);

    // Skip client-side table check for remote devices
    // lf_assert(index < m->length, E_FMR, "Invalid function index %d for module '%s' (max %zu)", index, module, m->length);

    hdr->magic = FMR_MAGIC_NUMBER;
    hdr->len = sizeof(*hdr) + sizeof(*call);
    hdr->type = fmr_rpc_class;

    call->module = m->idx;
    call->function = index;
    call->ret = ret;
    call->argt = 0;
    call->argc = args ? lf_ll_count(args) : 0;

    if (args) {
        struct _lf_ll *node = args;
        uint8_t *argv = call->argv;
        uint8_t i = 0;
        while (node && i < call->argc) {
            struct _lf_arg *arg = node->item;
            lf_type type = arg->type;
            lf_arg value = arg->value;
            uint8_t size = lf_sizeof(type);
            memcpy(argv, &value, size);
            argv += size;
            hdr->len += size;
            call->argt |= (type << (i * 2));
            node = node->next;
            i++;
        }
    }

    lf_crc(packet, hdr->len, &crc);
    hdr->crc = crc;

    lf_debug_packet(&_packet);

    e = device->write(device, packet, sizeof(_packet));
    lf_assert(e == lf_success, E_ENDPOINT, "Failed to send message to device '%s'.", device->name);

    e = device->read(device, &result, sizeof(struct _fmr_result));
    lf_assert(e == lf_success, E_ENDPOINT, "Failed to receive message from the device '%s'.", device->name);

    lf_debug_result(&result);
    lf_assert(result.error == E_OK, result.error, "An error occurred on the device '%s'.", device->name);

    if (retval) *retval = result.value;

    return lf_success;

fail:
    return lf_error;
}
