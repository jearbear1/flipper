#include "libflipper.h"

// #if defined(ATMEGAU2) && defined(__AVR__)

#include <atmegau2.h>

#define SPI_DATA_MODE_0 0x00
#define SPI_DATA_MODE_1 0x04
#define SPI_DATA_MODE_2 0x08
#define SPI_DATA_MODE_3 0x0C

LF_FUNC int spi_configure() {
    /* Configure MOSI and SCK as inputs. */
    SPI_DDR |= (1 << MOSI) | (1 << SCK);
    /* Put the SPI bus into MODE3. */
    SPCR |= SPI_DATA_MODE_3;
    /* Configure the SPI clock to be 1/2 of the system clock. This is the fastest SCK we can specify.  */
    SPCR |= (1 << SPI2X);
    return lf_success;
}

LF_FUNC void spi_enable(void) {
    SPI_DDR |= (1 << MOSI) | (1 << SCK);
    SPCR |= (1 << MSTR) | (1 << SPE);
}

LF_FUNC void spi_disable(void) {
    SPCR &= ~((1 << MSTR) | (1 << SPE));
    SPI_DDR &= ~((1 << MOSI) | (1 << SCK));
}

LF_FUNC uint8_t spi_ready(void) {
    /* The ready state of the SPI can be determined by reading its interrupt flag. */
    return (SPSR) & (1 << SPIF);
}

LF_FUNC void spi_put(uint8_t byte) {
    /* Write the byte to the data register. */
    SPDR = byte;
    /* Wait until the byte has been sent. */
    while (!((SPSR) & (1 << SPIF)))
        ;
}

LF_FUNC uint8_t spi_get(void) {
    /* Send a byte to begin transmission. */
    spi_put(0x00);
    /* Read the data received. */
    return SPDR;
}

LF_FUNC int spi_write(void *src, uint32_t length) {
    while (length--) spi_put(*(uint8_t *)(src++));
    return lf_success;
}

LF_FUNC int spi_read(void *dst, uint32_t length) {
    while (length--) *(uint8_t *)(dst++) = spi_get();
    return lf_success;
}


