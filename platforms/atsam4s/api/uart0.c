#include "libflipper.h"
#include "sam4s16b.h"
#include <sysclk.h> 
#include <pmc.h> 
#include <uart.h> 


int uart0_configure(void);
int uart0_setbaud(uint32_t baud);
int uart0_reset(void);
int uart0_ready(void);
int uart0_enable(void); 
int uart0_disable(void);
void uart0_put(uint8_t byte);
uint8_t uart0_get(void); 
int uart0_write(void *src, uint32_t length);
int uart0_read(void *dst, uint32_t length);

/*
int uart_configure(int);
int reset_uart(int);
int reset_uart_rx(int);
int reset_uart_tx(int);
int uart_cleanup(int);
int get_uart_status(int);
int is_uart_rx_ready(int);
int is_uart_tx_ready(int);
int uart_read_data(int, int);
int uart_write_data(int, int);


LF_FUNC int uart_configure(int i) {

    } else if (i == 1) {
        // Enable the Peripheral clock 
        // UART1 Peripheral ID = 9 
        pmc_enable_periph_clk(ID_UART1);
        // Enable UART receiver and transmitter.
        uart_enable(UART1);
        // UART1_IRQn = 9
        uart_enable_interrupt(UART1, UART1_IRQn);

        // Enable UART receiver.
        uart_enable_rx(UART1);
        // Enable UART transmitter.
        uart_enable_tx(UART1);
        return lf_success;
        
    } else {
        return lf_error;
    }

}

LF_FUNC int reset_uart(int i) {

   if (i == 1) {
        uart_reset(UART1);
        uart_reset_status(UART1);
         return lf_success;

    } else {
        return lf_error;
    }
}

LF_FUNC int reset_uart_rx(int i) {

    if (i == 1) {
        uart_reset_rx(UART1);
        return lf_success; 
    } else {
        return lf_error;
    }

}
LF_FUNC int reset_uart_tx(int i) {
     if (i == 1) {
        reset_uart_tx(UART1);
        return lf_success; 
    } else {
        return lf_error;
    }

}


LF_FUNC int uart_cleanup(int i) {
     (i == 1) {
        // Enable UART receiver and transmitter.
        uart_disable(UART1);
        // UART1_IRQn = 9
        uart_disable_interrupt(UART1, UART1_IRQn);

        // Enable UART receiver.
        uart_disable_rx(UART1);
        // Enable UART transmitter.
        uart_disable_tx(UART1);
        return lf_success;
            
    } else {
        return lf_error;
    }

}

LF_FUNC get_uart_status(int i) {
    if (i == 1) {
        uart_get_status(UART1);
        return lf_success;
    } else {
        return lf_error;
    }
}

LF_FUNC int is_uart_rx_ready(int i) {
    if (i == 1) {
        uart_is_rx_ready(UART1);
        return lf_success;
    } else {
        return lf_error;
    }
}
LF_FUNC int is_uart_tx_ready(int i) {
     if (i == 1) {
         uart_is_tx_ready(UART1);
        return lf_success;
    } else {
        return lf_error;
    }
}

LF_FUNC uart_read_data(int i, int puc_data) {
     if (i == 1) {
        if (uart_is_rx_ready(UART1) == 1) {
        uart_read(UART0, puc_data);
        }
     } else {
        return lf_error;
     }
}


LF_FUNC uart_write_data(int i, int uc_data) {
    if (i == 1) {
        if (uart_is_tx_empty(UART1) == 1) {
            uart_write(UART0, uc_data);
        }
     } else {
        return lf_error;
     }
}

*/



LF_FUNC int uart0_configure(void) {

    // Enable the Peripheral clock 
    // UART0 Peripheral ID = 8 
    pmc_enable_periph_clk(ID_UART0);

    // UART0_IRQn = 8
    uart_enable_interrupt(UART0, UART0_IRQn);

    // Enable UART receiver.
    uart_enable_rx(UART0);
    // Enable UART transmitter.
    uart_enable_tx(UART0);

    return lf_success;
}

/*

LF_FUNC int uart0_setbaud(uint32_t baud) {
    
The baud rate clock is the peripheral clock divided by 16 times the clock divisor (CD)
value written in the Baud Rate Generator register (UART_BRGR). 
If UART_BRGR is set to 0, the baud rate clock is disabled and the UART remains inactive. 
The maximum allowable baud rate is peripheral clock divided by 16. 
The minimum allowable baud rate is peripheral clock divided by (16 x 65536).

    return lf_success;
}

*/

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

LF_FUNC void uart0_put(uint8_t byte) {

}

LF_FUNC uint8_t uart0_get(void) {
    // get UART status
    uart_get_status(UART0);
    return lf_success;
}

LF_FUNC int uart0_write(void *src, uint32_t length) {
    if (uart_is_tx_empty(UART0) == 1) {
        uart_write(UART0, length);
    }
    return lf_success;
}

LF_FUNC int uart0_read(void *dst, uint32_t length) {
    uart_read(UART0, length);
    return lf_success;
}

/*
uart_get_interrupt_mask
uart_set_clock_divisor 
*/