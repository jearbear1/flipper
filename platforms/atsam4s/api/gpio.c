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
int configureInterruptRiseEdge(uint32_t x);
int configureInterruptFallEdge(uint32_t x);
int getInterruptMask(const Pio *p_pio);
int getInterruptStatus(const Pio *p_pio);
int disableInterrupt(uint32_t x);
void getOutputDataStatus(const Pio *p_pio, const uint32_t ul_mask);
void setSchmittTrigger(Pio *p_pio, const uint32_t ul_mask);
void getSchmittTrigger(const Pio *p_pio);
int captureSetMode(int x);
void captureEnable(Pio *p_pio);
void captureDisable(Pio *p_pio);


LF_FUNC int gpioConfigure(void) {
    // Parallel I/O Controller A (PIOA) ID = (11)
    // Parallel I/O Controller B (PIOA) ID = (12)
    sysclk_enable_peripheral_clock(ID_PIOA);
    sysclk_enable_peripheral_clock(ID_PIOB);
    // Clear GPIO signals
    pio_clear(PIOA, IO_1);
    pio_clear(PIOA, IO_2);
    pio_clear(PIOA, IO_3);
    pio_clear(PIOA, IO_4);
    pio_clear(PIOA, IO_5);
    pio_clear(PIOA, IO_6);
    pio_clear(PIOA, IO_7);
    pio_clear(PIOA, IO_8);
    pio_clear(PIOA, IO_9);
    pio_clear(PIOA, IO_10);
    pio_clear(PIOA, IO_11);
    pio_clear(PIOA, IO_12);
    pio_clear(PIOA, IO_14);
    pio_clear(PIOB, IO_15);
    pio_clear(PIOB, IO_16);
    pio_clear(PIOA, IO_A1);
    pio_clear(PIOA, IO_A2);
    pio_clear(PIOA, IO_A3);
    pio_clear(PIOA, IO_A4);
    pio_clear(PIOB, IO_A5);
    pio_clear(PIOB, IO_A6);
    pio_clear(PIOB, IO_A7);
    pio_clear(PIOB, IO_A8);
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
        || (IO_9) || (IO_10) || (IO_11) || (IO_12) || (IO_14) || (IO_A1) || (IO_A2)
        || (IO_A3) || (IO_A4)) 
    {
        // pins are set to pio_pullup by defualt.
        pio_set_input(PIOA, x, PIO_PULLUP);
        return lf_success;  

    } else  if (x == (IO_15) || (IO_16) || (IO_A5) || (IO_A6) || (IO_A7) || (IO_A8)) {

        pio_set_input(PIOB, x, PIO_PULLUP);
        return lf_success;
    } else {
        return lf_error;
    }
}

LF_FUNC void gpio_write(uint32_t set, uint32_t clear) {
    // set & clear are ul_mask (Bitmask of one or more pin(s) to configure)
    set = clear;
    pio_set(PIOA, set);  
}

LF_FUNC uint32_t gpio_read(uint32_t mask) {
    if (mask == (IO_1) || (IO_2) || (IO_3) || (IO_4) || (IO_5) || (IO_6) || (IO_7) || (IO_8) 
        || (IO_9) || (IO_10) || (IO_11) || (IO_12) || (IO_14) || (IO_A1) || (IO_A2)
        || (IO_A3) || (IO_A4)) 
    {
        pio_get(PIOA, PIO_TYPE_PIO_INPUT, mask);
        return lf_success;  

    } else  if (mask == (IO_15) || (IO_16) || (IO_A5) || (IO_A6) || (IO_A7) || (IO_A8)) {

        pio_get(PIOB, PIO_TYPE_PIO_INPUT, mask);
        return lf_success;
    } else {
        return lf_error;
    }
 
}

LF_FUNC int configureInterruptRiseEdge(uint32_t x) {

    if (x == (IO_1) || (IO_2) || (IO_3) || (IO_4) || (IO_5) || (IO_6) || (IO_7) || (IO_8) 
        || (IO_9) || (IO_10) || (IO_11) || (IO_12) || (IO_14)) 
    {
        pio_configure_interrupt(PIOA, x, PIO_IT_RISE_EDGE);
        pio_enable_interrupt(PIOA, x);
        return lf_success;  
    }  else  if (x == (IO_15) || (IO_16)) {
        pio_configure_interrupt(PIOB, x, PIO_IT_RISE_EDGE);
        pio_enable_interrupt(PIOB, x);
        return lf_success;  
    } else {
        return lf_error;
    }
}

LF_FUNC int configureInterruptFallEdge(uint32_t x) {

    if (x == (IO_1) || (IO_2) || (IO_3) || (IO_4) || (IO_5) || (IO_6) || (IO_7) || (IO_8) 
        || (IO_9) || (IO_10) || (IO_11) || (IO_12) || (IO_14)) 
    {
        pio_configure_interrupt(PIOA, x, PIO_IT_FALL_EDGE);
        pio_enable_interrupt(PIOA, x);
        return lf_success;  
    }  else  if (x == (IO_15) || (IO_16)) {
        pio_configure_interrupt(PIOB, x, PIO_IT_FALL_EDGE);
        pio_enable_interrupt(PIOB, x);
        return lf_success;  
    } else {
        return lf_error;
    }
}

LF_FUNC int getInterruptMask(const Pio *p_pio) {
    pio_get_interrupt_mask(p_pio);
    return lf_success;
}

LF_FUNC int getInterruptStatus(const Pio *p_pio) {
    pio_get_interrupt_status(p_pio);
    return lf_success;
}

LF_FUNC int disableInterrupt(uint32_t x) {

    if (x == (IO_1) || (IO_2) || (IO_3) || (IO_4) || (IO_5) || (IO_6) || (IO_7) || (IO_8) 
        || (IO_9) || (IO_10) || (IO_11) || (IO_12) || (IO_14)) 
    {
        pio_disable_interrupt(PIOA, x);
        return lf_success;  
    }  else  if (x == (IO_15) || (IO_16)) {
        pio_disable_interrupt(PIOB, x);
        return lf_success;  
    } else {
        return lf_error;
    }
}

LF_FUNC void getOutputDataStatus(const Pio *p_pio, const uint32_t ul_mask) {
    pio_get_output_data_status(p_pio, ul_mask);		
}
 
LF_FUNC void setSchmittTrigger(Pio *p_pio, const uint32_t ul_mask) {
    pio_set_schmitt_trigger(p_pio, ul_mask);
}

LF_FUNC void getSchmittTrigger(const Pio *p_pio) {
    pio_get_schmitt_trigger(p_pio);
}

LF_FUNC int captureSetMode(int x) {
    if (x == 1) {
        // Always Sampling 
        pio_capture_set_mode(PIOA, PIO_PCMR_ALWYS);
        return lf_success; 
    } else if(x == 2) {
        // Half Sampling 
        pio_capture_set_mode(PIOA, PIO_PCMR_HALFS);
        return lf_success; 
    } else if ( x == 3) {
        // First Sample
        pio_capture_set_mode(PIOA, PIO_PCMR_FRSTS);
        return lf_success; 
    } else {
        return lf_error;
    }
}

LF_FUNC void captureEnable(Pio *p_pio) {
    pio_capture_enable(p_pio);
}

LF_FUNC void captureDisable(Pio *p_pio) {
    pio_capture_disable(p_pio);
}