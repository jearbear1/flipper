#include "libflipper.h"
#include "sam4s16b.h"
#include <sysclk.h> 
#include <pmc.h> 
#include <uart.h> 

int uart0_configure(void);
int uart0_reset(void);
int uart0_ready(void);
int uart0_enable(void); 
int uart0_disable(void);
uint8_t uart0_get(void); 
int uart0_write(void);
int uart0_read(void);
int uart0_get_uart_interrupt_mask(void);
const uint8_t uc_data;
uint8_t *puc_data;
int changeBaudRate(int);
int changeMode(int);

/*
    
The baud rate clock is the peripheral clock divided by 16 times the clock divisor (CD)
value written in the Baud Rate Generator register (UART_BRGR). 
If UART_BRGR is set to 0, the baud rate clock is disabled and the UART remains inactive. 
The maximum allowable baud rate is peripheral clock divided by 16. 
The minimum allowable baud rate is peripheral clock divided by (16 x 65536).

The Automatic echo mode allows a bit-by-bit retransmission. 
When a bit is received on the URXD line, it is sent to the UTXD line.
The transmitter operates normally, but has no effect on the UTXD line.

The Local loopback mode allows the transmitted characters to be received. 
UTXD and URXD pins are not used and the output of the transmitter is internally connected to the input of the receiver. 
The URXD pin level has no effect and the UTXD line is held high, as in idle state.

The Remote loopback mode directly connects the URXD pin to the UTXD line. 
The transmitter and the receiver are disabled and have no effect. 
This mode allows a bit-by-bit retransmission.

0: Normal mode
1: Automatic echo
2: Local loopback
3: Remote loopback
*/

sam_uart_opt_t uart0_opt;

LF_FUNC int uart0_configure(void) {

    // Enable the Peripheral clock 
    // UART0 Peripheral ID = 8 
    pmc_enable_periph_clk(ID_UART0);

    // configure uart options struct
    // 120 MHz Master Clock, 9600 Baudrate, Normal Mode
    uart0_opt.ul_mck = 120;
    uart0_opt.ul_baudrate = 9600;
    uart0_opt.ul_mode = 0;

    uart_init(UART0, &uart0_opt);
    // UART0_IRQn = 8
    uart_enable_interrupt(UART0, UART0_IRQn);

    return lf_success;
}

LF_FUNC int uart0_enable(void) {
    // Enable UART
    uart_enable(UART0);
    return lf_success;
}

LF_FUNC int uart0_disable(void) {

    // Disable UART receiver.
    uart_disable_rx(UART0);
    // Disable UART transmitter.
    uart_disable_tx(UART0);
    // Disable UART Interrupt UART0_IRQn = 8
    uart_disable_interrupt(UART0, UART0_IRQn);
    // Disable UART Peripheral
    uart_disable(UART0);
   
     return lf_success;
}

LF_FUNC int uart0_reset(void) {
    // reset transmitter receiver and uart status
    uart_reset_status(UART0);
    uart_reset(UART0);
    return lf_success;
}

LF_FUNC int uart0_ready(void) {
    uart_is_rx_ready(UART0);
    uart_is_tx_ready(UART0);
    return lf_success;
}

LF_FUNC uint8_t uart0_get(void) {
    // get UART status
    uart_get_status(UART0);
    return lf_success;
}

LF_FUNC int uart0_write(void) {
    uart_write(UART0, uc_data);
    return lf_success;
}

LF_FUNC int uart0_read(void) {
    uart_read(UART0, puc_data);
    return lf_success;
}


LF_FUNC int uart0_get_uart_interrupt_mask(void) { 
    uart_get_interrupt_mask(UART0);
    return lf_success;
}

LF_FUNC int changeBaudRate(int x) {
    /*
    if:
        (x > peripheral clock divided by (16 x 65536)
        &&
        (x < peripheral clock divided by 16 )
        then 
        uart0_opt.ul_baudrate = x;
        return lf_success;
    */
    return lf_success;
}

LF_FUNC int changeMode(int x) {

    if (x == 0) {
        uart0_opt.ul_mode = 0;
        return lf_success;
    } else if (x == 1) {
        uart0_opt.ul_mode = 1;
        return lf_success;
    } else if (x == 2) {
        uart0_opt.ul_mode = 2;
        return lf_success;
    } else if (x == 3) {
        uart0_opt.ul_mode = 3;
        return lf_success;
    } else {
        return lf_error; 
    }

    return lf_success;
}

// DO NOT DELETE THIS FUNCTION
LF_FUNC void uart0_put(uint8_t byte) {
}

