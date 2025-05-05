#include "uart0.h"
#include "sam4s.h"
#include "sysclk.h"
#include "pio.h"
#include "pio_sam4s16b.h"
#include "uart.h"
#include "pmc.h"



#define UART0_BAUD_DEFAULT 115200

/** UART0 hardware pointer */
#define UART0_HW UART0

/** UART0 default settings */
#define UART0_ID ID_UART0
#define UART0_PINS (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)

/** UART0 PIO instance */
#define UART0_PIO PIOA

int uart0_configure(void) {
    /* Enable peripheral clock for UART0 */
    pmc_enable_periph_clk(UART0_ID);

    /* Configure PIO for UART0 RXD and TXD */
    pio_configure(UART0_PIO, PIO_PERIPH_A, UART0_PINS, PIO_DEFAULT);

    /* Initialize UART with default settings */
    const sam_uart_opt_t uart_settings = {
        .ul_mck = sysclk_get_cpu_hz(),
        .ul_baudrate = UART0_BAUD_DEFAULT,
        .ul_mode = UART_MR_PAR_NO
    };

    if (uart_init(UART0_HW, &uart_settings) != 0) {
        return -1;
    }

    uart_enable_tx(UART0_HW);
    uart_enable_rx(UART0_HW);

    return 0;
}

int uart0_setbaud(uint32_t baud) {
    const sam_uart_opt_t uart_settings = {
        .ul_mck = sysclk_get_cpu_hz(),
        .ul_baudrate = baud,
        .ul_mode = UART_MR_PAR_NO
    };

    return uart_init(UART0_HW, &uart_settings);
}

int uart0_reset(void) {
    uart_reset_status(UART0_HW);
    uart_reset_rx(UART0_HW);
    uart_reset_tx(UART0_HW);
    return 0;
}

int uart0_ready(void) {
    return uart_is_tx_ready(UART0_HW);
}

void uart0_enable(void) {
    uart_enable(UART0_HW);
}

void uart0_disable(void) {
    uart_disable(UART0_HW);
}

void uart0_put(uint8_t byte) {
    while (!uart_is_tx_ready(UART0_HW));
    uart_write(UART0_HW, byte);
}

uint8_t uart0_get(void) {
    uint8_t byte;
    while (!uart_is_rx_ready(UART0_HW));
    uart_read(UART0_HW, &byte);
    return byte;
}

int uart0_write(void *src, uint32_t length) {
    uint8_t *data = (uint8_t *)src;
    for (uint32_t i = 0; i < length; ++i) {
        uart0_put(data[i]);
    }
    return 0;
}

int uart0_read(void *dst, uint32_t length) {
    uint8_t *data = (uint8_t *)dst;
    for (uint32_t i = 0; i < length; ++i) {
        data[i] = uart0_get();
    }
    return 0;
}
