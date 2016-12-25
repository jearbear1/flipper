/* libusb.c - USB endpoint wrapper using libusb. */

#define __private_include__
#include <flipper/flipper.h>
#include <platform/posix/libusb.h>
#include <libusb.h>
#include <flipper/error.h>

struct _lf_endpoint lf_usb_ep = {
	lf_usb_configure,
	lf_usb_ready,
	lf_usb_put,
	lf_usb_get,
	lf_usb_push,
	lf_usb_pull,
	lf_usb_destroy
};

struct _lf_usb_record {
	struct libusb_device_handle *handle;
	struct libusb_context *context;
};

int lf_usb_configure(struct _lf_endpoint *endpoint) {
	if (!endpoint) {
		error_raise(E_NULL, error_message("No endpoint record provided for libusb configuration. Reattach your device and try again."));
		return lf_error;
	}
	/* Allocate memory for the USB record if it has not yet been allocated. */
	if (!(endpoint -> record)) {
		endpoint -> record = malloc(sizeof(struct _lf_usb_record));
	}
	struct _lf_usb_record *record = endpoint -> record;
	/* Initialize the libusb context associated with this endpoint. */
	int _e = libusb_init(&(record -> context));
	if (_e < 0) {
		error_raise(E_LIBUSB, error_message("Failed to initialize libusb. Reboot and try again."));
		return lf_error;
	}
	/* Attach a physical device to this endpoint. */
	record -> handle = libusb_open_device_with_vid_pid(record -> context, USB_VENDOR_ID, USB_PRODUCT_ID);
	if (!(record -> handle)) {
		error_raise(E_NO_DEVICE, error_message("Could not find any devices connected via USB. Ensure that a device is connected."));
		return lf_error;
	}
	/* Claim the interface used to send and receive message runtime packets. */
	_e = libusb_claim_interface(record -> handle, 0);
	if (_e < 0) {
		error_raise(E_LIBUSB, error_message("Failed to claim interface on attached device. Please quit any other programs using your device."));
		return lf_error;
	}
	libusb_set_debug(record -> context, 3);
	/* Reset the device's USB controller. */
	libusb_reset_device(record -> handle);
	return lf_success;
}

uint8_t lf_usb_ready(void) {

	return 0;
}

void lf_usb_put(uint8_t byte) {

}

uint8_t lf_usb_get(void) {

	return 0;
}

int lf_usb_push(void *source, lf_size_t length) {
	struct _lf_usb_record *record = lf_device() -> endpoint -> record;
	int _length;
	int _e;
#ifndef __ALL_BULK__
	if (length <= INTERRUPT_OUT_SIZE) {
		_e = libusb_interrupt_transfer(record -> handle, INTERRUPT_OUT_ENDPOINT, source, length, &_length, 0);
	} else {
#endif
		_e = libusb_bulk_transfer(record -> handle, BULK_OUT_ENDPOINT, source, length, &_length, 0);
#ifndef __ALL_BULK__
	}
#endif
	if (_e < 0 || _length != length) {
		error_raise(E_COMMUNICATION, error_message("Failed to transmit complete USB packet."));
		return lf_error;
	}
	return lf_success;
}

int lf_usb_pull(void *destination, lf_size_t length) {
	struct _lf_usb_record *record = lf_device() -> endpoint -> record;
	int _length;
	int _e;
#ifndef __ALL_BULK__
	if (length <= INTERRUPT_IN_SIZE) {
		_e = libusb_interrupt_transfer(record -> handle, INTERRUPT_IN_ENDPOINT, destination, length, &_length, 0);
	} else {
#endif
		_e = libusb_bulk_transfer(record -> handle, BULK_IN_ENDPOINT, destination, length, &_length, 0);
#ifndef __ALL_BULK__
	}
#endif
	if (_e < 0 || _length != length) {
		error_raise(E_COMMUNICATION, error_message("Failed to receive complete USB packet."));
		return lf_error;
	}
	return lf_success;
}

int lf_usb_destroy(struct _lf_endpoint *endpoint) {
	struct _lf_usb_record *record = lf_device() -> endpoint -> record;
	if (!record) {
		error_raise(E_ENDPOINT, error_message("No libusb record associated with the USB endpoint."));
		return lf_error;
	}
	/* Close the device handle. */
	libusb_close(record -> handle);
	/* Destroy the libusb context. */
	libusb_exit(record -> context);
	return lf_success;
}
