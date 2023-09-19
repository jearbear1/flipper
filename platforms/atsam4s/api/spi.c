#include "libflipper.h"
#include <spi.h>

int spi_configure(Spi *p_spi);
int setSpiMode(Spi *p_spi, int x);
void enableSpi(Spi *p_spi); 
void disableSpi(Spi *p_spi);
uint8_t spi_ready(void) ;
void spi_end(void); 
void spiPut(Spi *p_spi, uint16_t data); 
int spiGet(Spi *p_spi); 
void spi_isr(void);


LF_FUNC int spi_configure(Spi *p_spi) {
    spi_enable_clock(p_spi);
    spi_reset(p_spi);
    spi_disable_mode_fault_detect(p_spi);
    spi_disable_loopback(p_spi);
    spi_set_fixed_peripheral_select(p_spi);
    spi_disable_peripheral_select_decode(p_spi);
    return lf_success;
}

LF_FUNC void enableSpi(Spi *p_spi) {
    spi_enable(p_spi);
}

LF_FUNC void disableSpi(Spi *p_spi) {
    spi_disable(p_spi);
}

LF_FUNC int setSpiMode(Spi *p_spi, int x) {
    if (x == 1) {
        spi_set_master_mode(p_spi);
        return lf_success;
    } else if (x == 2) {
        spi_set_slave_mode(p_spi);
        return lf_success;
    } else {
        return lf_error;
    }
}


LF_FUNC uint8_t spi_ready(void) {
    return 0;
}

LF_FUNC void spi_end(void) {
}

LF_FUNC void spiPut(Spi *p_spi, uint16_t data) {
}

LF_FUNC int spiGet(Spi *p_spi) {
    spi_get_mode(p_spi);
    return 0;
}

/*

LF_FUNC int spi_write(void *src, uint32_t length) {
    return lf_success;
}

LF_FUNC int spi_read(void *dst, uint32_t length) {
    return lf_success;
}

*/

/* interrupt handler for spi */
LF_FUNC void spi_isr(void) {
}
