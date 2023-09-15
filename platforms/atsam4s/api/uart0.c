#include "libflipper.h"
#include <uart.h>

const uint8_t uc_data;
uint8_t *puc_data;

int uart0_configure(void);
int uart0_write(const uint8_t uc_data);
int uart0_read(uint8_t *puc_data);


LF_FUNC int uart0_configure(void) {

    return lf_success;
}

LF_FUNC int uart0_setbaud(uint32_t baud) {
    return lf_success;
}

LF_FUNC int uart0_reset(void) {
    return lf_success;
}

LF_FUNC int uart0_ready(void) {
    return 0;
}

LF_FUNC void uart0_enable(void) {
}

LF_FUNC void uart0_disable(void) {
}

LF_FUNC void uart0_put(uint8_t byte) {
}

LF_FUNC uint8_t uart0_get(void) {
    return 0;
}

LF_FUNC int uart0_write(const uint8_t uc_data) {
    return lf_success;
}

LF_FUNC int uart0_read(uint8_t *puc_data) {
    return lf_success;
}
