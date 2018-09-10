#include <flipper.h>

static struct _lf_ll *lf_attached_devices;
static struct _lf_device *lf_current_device;

/* Setter for the lf_current_device global. */
static void lf_set_current_device(struct _lf_device *device) {
	lf_current_device = device;
}

/* Getter for the lf_current_device global. */
static struct _lf_device *lf_get_current_device(void) {
	return lf_current_device;
}

int lf_attach(struct _lf_device *device) {
	lf_assert(device, E_NULL, "Attempt to attach an invalid device.");

	lf_ll_append(&lf_attached_devices, device, lf_device_release);
	lf_select(device);

	return lf_success;
fail:
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

int lf_invoke(struct _lf_device *device, const char *module, lf_function function, lf_type ret, lf_return_t *retval, struct _lf_ll *args) {
	lf_assert(device, E_NULL, "invalid device");
	lf_assert(module, E_NULL, "invalid module");

	struct _fmr_packet _packet;
	struct _fmr_result result;
	int e;

	lf_debug("Invoking function on device '%s'.", device->name);

	memset(&_packet, 0, sizeof(struct _fmr_packet));
	_packet.header.magic = FMR_MAGIC_NUMBER;
	_packet.header.length = sizeof(struct _fmr_invocation_packet);

	struct _lf_module *m = dyld_module(device, module);
	lf_assert(m, E_MODULE, "No counterpart found for module '%s'.", module);

	struct _fmr_invocation_packet *packet = (struct _fmr_invocation_packet *)(&_packet);

	e = lf_create_call(m->idx, function, ret, args, &_packet.header, &packet->call);
	lf_assert(e , E_NULL, "Failed to generate a valid call to module '%s'.", module);

	_packet.header.checksum = lf_crc(&_packet, _packet.header.length);
	lf_debug_packet(&_packet, sizeof(struct _fmr_packet));

	e = device->write(device, &_packet, sizeof(struct _fmr_packet));
	lf_assert(e , E_ENDPOINT, "Failed to send message to device '%s'.", device->name);

	e = device->read(device, &result, sizeof(struct _fmr_result));
	lf_assert(e , E_ENDPOINT, "Failed to receive message from the device '%s'.", device->name);

	lf_debug_result(&result);
	lf_assert(result.error == E_OK, result.error, "An error occured on the device '%s':", device->name);

	*retval = result.value;

	return lf_success;
fail:
	return lf_error;
}

int lf_push(struct _lf_device *device, void *dst, void *src, size_t len) {
	lf_assert(device, E_NULL, "invalid device");
	lf_assert(dst, E_NULL, "NULL dst");
	lf_assert(src, E_NULL, "NULL src");
	lf_assert(len, E_NULL, "Zero length");

	struct _fmr_packet _packet;
	struct _fmr_result result;
	int e;

	lf_debug("Pushing to memory on device '%s'.", device->name);

	memset(&_packet, 0, sizeof(struct _fmr_packet));
	_packet.header.magic = FMR_MAGIC_NUMBER;
	_packet.header.length = sizeof(struct _fmr_push_pull_packet);
	_packet.header.type = fmr_push_class;

	struct _fmr_push_pull_packet *packet = (struct _fmr_push_pull_packet *)(&_packet);
	packet->len = len;
	packet->ptr = (uintptr_t)dst;
	_packet.header.checksum = lf_crc(&_packet, _packet.header.length);

	lf_debug_packet(&_packet, sizeof(struct _fmr_packet));

	e = device->write(device, &_packet, sizeof(struct _fmr_packet));
	lf_assert(e , E_ENDPOINT, "Failed to send message to device '%s'.", device->name);
	e = device->write(device, src, len);
	lf_assert(e , E_FMR, "Failed to push data to device '%s'.", device->name);

	e = device->read(device, &result, sizeof(struct _fmr_result));
	lf_assert(e , E_ENDPOINT, "Failed to receive message from the device '%s'.", device->name);

	lf_debug_result(&result);
	lf_assert(result.error == E_OK, result.error, "An error occured on the device '%s':", device->name);

	return lf_success;
fail:
	return lf_error;
}

int lf_pull(struct _lf_device *device, void *dst, void *src, size_t len) {
	lf_assert(device, E_NULL, "invalid device");
	lf_assert(dst, E_NULL, "NULL dst");
	lf_assert(src, E_NULL, "NULL src");
	lf_assert(len, E_NULL, "Zero length");

	struct _fmr_packet _packet;
	struct _fmr_result result;
	int e;

	lf_debug("Pulling from memory on device '%s'.", device->name);

	memset(&_packet, 0, sizeof(struct _fmr_packet));
	_packet.header.magic = FMR_MAGIC_NUMBER;
	_packet.header.length = sizeof(struct _fmr_push_pull_packet);
	_packet.header.type = fmr_pull_class;

	struct _fmr_push_pull_packet *packet = (struct _fmr_push_pull_packet *)(&_packet);
	packet->len = len;
	packet->ptr = (uintptr_t)src;
	_packet.header.checksum = lf_crc(&_packet, _packet.header.length);

	lf_debug_packet(&_packet, sizeof(struct _fmr_packet));

	e = device->write(device, &_packet, sizeof(struct _fmr_packet));
	lf_assert(e , E_ENDPOINT, "Failed to send message to device '%s'.", device->name);

	e = device->read(device, dst, len);
	lf_assert(e , E_FMR, "Failed to pull data from device '%s'.", device->name);

	e = device->read(device, &result, sizeof(struct _fmr_result));
	lf_assert(e , E_ENDPOINT, "Failed to receive message from the device '%s'.", device->name);

	lf_debug_result(&result);
	lf_assert(result.error == E_OK, result.error, "An error occured on the device '%s':", device->name);

	return lf_success;
fail:
	return lf_error;
}

int lf_dyld(struct _lf_device *device, const char *module, int *idx) {
	lf_assert(device, E_NULL, "invalid device");
	lf_assert(module, E_NULL, "invalid module");
	lf_assert(strlen(module) < 16, E_OVERFLOW, "Module name '%s' is invalid. Module names must be 16 characters or less.", module);

	struct _fmr_packet _packet;
	struct _fmr_result result;
	int e;

	lf_debug("Syncing with loader on device '%s'.", device->name);

	memset(&_packet, 0, sizeof(struct _fmr_packet));
	_packet.header.magic = FMR_MAGIC_NUMBER;
	_packet.header.length = sizeof(struct _fmr_dyld_packet);
	_packet.header.type = fmr_dyld_class;

	struct _fmr_dyld_packet *packet = (struct _fmr_dyld_packet *)(&_packet);
	strcpy(packet->module, module);
	_packet.header.checksum = lf_crc(&_packet, _packet.header.length);

	lf_debug_packet(&_packet, sizeof(struct _fmr_packet));

	e = device->write(device, &_packet, sizeof(struct _fmr_packet));
	lf_assert(e , E_ENDPOINT, "Failed to send message to device '%s'.", device->name);

	e = device->read(device, &result, sizeof(struct _fmr_result));
	lf_assert(e , E_ENDPOINT, "Failed to receive message from the device '%s'.", device->name);

	lf_debug_result(&result);
	lf_assert(result.error == E_OK, result.error, "An error occured on the device '%s':", device->name);

	*idx = result.value;

	return lf_success;
fail:
	return lf_error;
}

int lf_malloc(struct _lf_device *device, size_t size, void **ptr) {
	lf_assert(device, E_NULL, "invalid device");

	struct _fmr_packet _packet;
	struct _fmr_result result;
	int e;

	lf_debug("Allocating memory on device '%s'.", device->name);

	memset(&_packet, 0, sizeof(struct _fmr_packet));
	_packet.header.magic = FMR_MAGIC_NUMBER;
	_packet.header.length = sizeof(struct _fmr_dyld_packet);
	_packet.header.type = fmr_malloc_class;

	struct _fmr_memory_packet *packet = (struct _fmr_memory_packet *)(&_packet);
	packet->size = size;
	_packet.header.checksum = lf_crc(&_packet, _packet.header.length);
	lf_debug_packet(&_packet, sizeof(struct _fmr_packet));

	e = device->write(device, &_packet, sizeof(struct _fmr_packet));
	lf_assert(e , E_ENDPOINT, "Failed to send message to device '%s'.", device->name);

	e = device->read(device, &result, sizeof(struct _fmr_result));
	lf_assert(e , E_ENDPOINT, "Failed to receive message from the device '%s'.", device->name);

	lf_debug_result(&result);
	lf_assert(result.error == E_OK, result.error, "An error occured on the device '%s':", device->name);

	*ptr = (void *)(uintptr_t)result.value;

	return lf_success;
fail:
	return lf_error;
}

int lf_free(struct _lf_device *device, void *ptr) {
	lf_assert(device, E_NULL, "invalid device");

	struct _fmr_packet _packet;
	struct _fmr_result result;
	int e;

	lf_debug("Freeing memory on device '%s'.", device->name);

	memset(&_packet, 0, sizeof(struct _fmr_packet));
	_packet.header.magic = FMR_MAGIC_NUMBER;
	_packet.header.length = sizeof(struct _fmr_dyld_packet);
	_packet.header.type = fmr_free_class;

	struct _fmr_memory_packet *packet = (struct _fmr_memory_packet *)(&_packet);
	packet->ptr = (uintptr_t)ptr;
	_packet.header.checksum = lf_crc(&_packet, _packet.header.length);

	lf_debug_packet(&_packet, sizeof(struct _fmr_packet));

	e = device->write(device, &_packet, sizeof(struct _fmr_packet));
	lf_assert(e , E_ENDPOINT, "Failed to send message to device '%s'.", device->name);

	e = device->read(device, &result, sizeof(struct _fmr_result));
	lf_assert(e , E_ENDPOINT, "Failed to receive message from the device '%s'.", device->name);

	lf_debug_result(&result);
	lf_assert(result.error == E_OK, result.error, "An error occured on the device '%s':", device->name);

	return lf_success;
fail:
	return lf_error;
}
