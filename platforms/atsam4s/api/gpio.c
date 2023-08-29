#include "libflipper.h"
#include "atsam4s.h"
#include "sam4s16b.h"
#include "pio/pio_sam4s16b.h"
#include <pio.h>
#include <sysclk.h> 
#include <pmc.h> 

int gpioConfigure(void);
void gpio_enable(uint32_t enable, uint32_t disable);
int gpioSetOutput(uint32_t x);
int gpioSetInput(uint32_t x);
void gpio_write(uint32_t set, uint32_t clear);
uint32_t gpio_read(uint32_t mask);


LF_FUNC int gpioConfigure(void) {
    // Parallel I/O Controller A (PIOA) ID = (11)
    // Parallel I/O Controller B (PIOA) ID = (12)
    sysclk_enable_peripheral_clock(ID_PIOA);
    sysclk_enable_peripheral_clock(ID_PIOB);
    // Clear GPIO signals
    // Clear IO_1
    pio_clear(PIOA, PIO_PA22);
    // Clear IO_2
    pio_clear(PIOA, PIO_PA23);
    // Clear IO_3
    pio_clear(PIOA, PIO_PA2);
    // Clear IO_4
    pio_clear(PIOA, PIO_PA30);
    // Clear IO_5
    pio_clear(PIOA, PIO_PA29);
    // Clear IO_6
    pio_clear(PIOA, PIO_PA28);
    // Clear IO_7
    pio_clear(PIOA, PIO_PA27);
    // Clear IO_8
    pio_clear(PIOA, PIO_PA7);
    // Clear IO_9
    pio_clear(PIOA, PIO_PA8);
    // Clear IO_10
    pio_clear(PIOA, PIO_PA26);
    // Clear IO_11
    pio_clear(PIOA, PIO_PA25);
    // Clear IO_12
    pio_clear(PIOA, PIO_PA24);
    // Clear IO_14
    pio_clear(PIOA, PIO_PA16);
    // Clear IO_15
    pio_clear(PIOB, PIO_PB7);
    // Clear IO_16
    pio_clear(PIOB, PIO_PB6);
    // clear IO_A1
     pio_clear(PIOA, PIO_PA20X1_AD3);
    // clear IO_A2
     pio_clear(PIOA, PIO_PA19X1_AD2);
    // clear IO_A3
     pio_clear(PIOA, PIO_PA18X1_AD1);
    // clear IO_A4
     pio_clear(PIOA, PIO_PA17X1_AD0);
    // clear IO_A5
     pio_clear(PIOB, PIO_PB3X1_AD7);
    // clear IO_A6
     pio_clear(PIOB, PIO_PB2X1_AD6);
    // clear IO_A7
     pio_clear(PIOB, PIO_PB1X1_AD5);
    // clear IO_A8
     pio_clear(PIOB, PIO_PB0X1_AD4);
    return lf_success;
}

LF_FUNC void gpio_enable(uint32_t enable, uint32_t disable) {
}

LF_FUNC int gpioSetOutput(uint32_t x) {
  if (x == (IO_1) || (IO_2) || (IO_3) || (IO_4) || (IO_5) || (IO_6) || (IO_7) || (IO_8) 
        || (IO_9) || (IO_10) || (IO_11) || (IO_12) || (IO_14) || (IO_15) || (IO_16))
    {
        // pin level default is low, open drain is disabled, pull-up enabled.
        pio_set_output(PIOA, x, LOW, DISABLE, ENABLE);
        // Enable PIO output write for synchronous data output.
        pio_enable_output_write(PIOA, x);   
        return lf_success;  
    }  else {
        return lf_error;
    }
}

LF_FUNC int gpioSetInput(uint32_t x) {
  if (x == (IO_1) || (IO_2) || (IO_3) || (IO_4) || (IO_5) || (IO_6) || (IO_7) || (IO_8) 
        || (IO_9) || (IO_10) || (IO_11) || (IO_12) || (IO_14) || (IO_15) || (IO_16)
        || (IO_A1) || (IO_A2) || (IO_A3) || (IO_A4) || (IO_A5) || (IO_A6) || (IO_A7) || (IO_A8)) 
    {
        // pins are set to pio_pullup by defualt.
        pio_set_input(PIOA, x, PIO_PULLUP);
        return lf_success;  
    }  else {
        return lf_error;
    }
}

LF_FUNC void gpio_write(uint32_t set, uint32_t clear) {
    // set & clear are ul_mask (Bitmask of one or more pin(s) to configure)
    set = clear;
    pio_set(PIOA, set);  
}

LF_FUNC uint32_t gpio_read(uint32_t mask) {
    pio_get(PIOA, PIO_TYPE_PIO_INPUT, mask);
    pio_get(PIOB, PIO_TYPE_PIO_INPUT, mask);
    return 0;
}



/*
void pio_set_schmitt_trigger(Pio *p_pio, const uint32_t ul_mask);
uint32_t pio_get_schmitt_trigger(const Pio *p_pio);

void pio_configure_interrupt(Pio *p_pio, const uint32_t ul_mask,
		const uint32_t ul_attr);
void pio_enable_interrupt(Pio *p_pio, const uint32_t ul_mask);
void pio_disable_interrupt(Pio *p_pio, const uint32_t ul_mask);
uint32_t pio_get_interrupt_status(const Pio *p_pio);
uint32_t pio_get_interrupt_mask(const Pio *p_pio);

void pio_capture_set_mode(Pio *p_pio, uint32_t ul_mode);
void pio_capture_enable(Pio *p_pio);
void pio_capture_disable(Pio *p_pio);

pio_get_output_data_status

AX pin are the pins which are used for only reading analog input
*/