#define __private_include__
#include <nvm.h>
#include <flipper/carbon/platforms/atmega16u2.h>
#include <flipper/carbon/modules/spi.h>

int nvm_configure(void) {
	/* Enable the internal pull up resistor for the CS pin. */
	set_bit_in_port(FLASH_CS_PIN, FLASH_CS_PORT);
	/* Configure the external flash memory chip's CS pin as an input. */
	clear_bit_in_port(FLASH_CS_PIN, FLASH_CS_DDR);
	/* Configure the external flash memory chip's reset pin as an output. */
	set_bit_in_port(FLASH_RESET_PIN, FLASH_RESET_DDR);
	/* Pull the reset pin high. (This is its default, inactive state.) */
	set_bit_in_port(FLASH_RESET_PIN, FLASH_RESET_PORT);
	/* Configure the external flash memory chip's WP pin as an output. */
	set_bit_in_port(FLASH_WP_PIN, FLASH_WP_DDR);
	/* Pull the WP pin high. (This is its default, inactive state.) */
	set_bit_in_port(FLASH_WP_PIN, FLASH_WP_PORT);
	return lf_success;
}

void nvm_enable(void) {
	spi_enable();
	/* Configure the external flash memory chip's CS pin as an output. */
	set_bit_in_port(FLASH_CS_PIN, FLASH_CS_DDR);
	/* Pull the CS pin low to enable the device. */
	clear_bit_in_port(FLASH_CS_PIN, FLASH_CS_PORT);
}

void nvm_disable(void) {
	/* Pull the CS pin high to disable the device. */
	set_bit_in_port(FLASH_CS_PIN, FLASH_CS_PORT);
	/* Configure the external flash memory chip's CS pin as an input. */
	clear_bit_in_port(FLASH_CS_PIN, FLASH_CS_DDR);
	spi_disable();
}

void nvm_reset(void) {
	spi_enable();
	/* Configure the external flash memory chip's CS pin as an output. */
	set_bit_in_port(FLASH_CS_PIN, FLASH_CS_DDR);
	/* Pull the CS pin high to enable the device. */
	set_bit_in_port(FLASH_CS_PIN, FLASH_CS_PORT);
	/* Pull the CS pin low. This issues a command decoder reset and puts the device into an idle state. */
	clear_bit_in_port(FLASH_CS_PIN, FLASH_CS_PORT);
}

/* The symbols 'nvm_write', 'nvm_put', 'nvm_read', and 'nvm_get' are declared in 'osmium/src/nvm/nvm.c'. */

/* The symbols 'nvm_alloc' and 'nvm_free' are declared in alloc.c. */

void nvm_format(void) {

}
