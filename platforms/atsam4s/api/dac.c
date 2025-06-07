#include "dac.h"
#include "dyld.h"
#include "atsam4s.h"
#include "sam4s16b.h"
#include <sysclk.h>
#include <conf_clock.h>
#include <pmc.h>
#include <component/component_pmc.h>
#include <genclk.h>
#include <osc.h>
#include <dacc.h>

/*
 The clock configuration (found in “conf_clock.h”) sets
 a master clock (MCK) frequency of 80 MHz using PLLA (240 MHz) with prescaler 3.
 The frequency of the peripheral clock is equal to that of MCK,
 and the frequency of the clock that drives the DAC circuitry (the “DAC clock”)
 is the peripheral-clock frequency divided by two.
 Thus, the configuration gives fDAC_clock = 40 MHz.
 The maximum DAC-clock frequency is 50 MHz.

The SAM4S DAC has a 12-bit resolution (DACC_RESOLUTION = 12),
so data written to the DAC should be in the range 0–4095
 
Configure the DAC with a 40 MHz clock (PLLA = 240 MHz, MCK = 80 MHz).
*/

LF_FUNC int dac_configure(void) {
    // Configure PLLA: (12 MHz * 20) / 1 = 240 MHz (matches conf_clock.h)
    pmc_enable_pllack(20, 32, 1);
    
    // Wait for PLLA to lock
    while (!pmc_is_locked_pllack());

    // Switch MCK to PLLA with prescaler 3 (240 MHz / 3 = 80 MHz)
    pmc_switch_mck_to_pllack(SYSCLK_PRES_3);
    // Direct Register Check the bit and timeout if not set to avoid hanging and infinite looping
    uint32_t ul_timeout = PMC_TIMEOUT;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY) && --ul_timeout) {
        if (ul_timeout == 0) {
            return lf_error; // Timeout
        }
    }
    
    // Enable the DAC peripheral clock (ID_DACC = 30)
    pmc_enable_periph_clk(ID_DACC);

    // Reset the DAC
    dacc_reset(DACC);

    // Enable flexible channel selection
    dacc_enable_flexible_selection(DACC);

    // Set timing: normal mode, 1920 cycles startup time
    dacc_set_timing(DACC, DACC_MR_MAXS_NORMAL, DACC_MR_STARTUP_1920);

    // Set transfer mode: half-word
    dacc_set_transfer_mode(DACC, DACC_MR_WORD_HALF);

    // Configure trigger: use software trigger
    dacc_set_trigger(DACC, 0);
    
    // Configure trigger: use TC0
    /*
     pmc_enable_periph_clk(ID_TC0);
     tc_init(TC0, 0, TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_WAVE);
     tc_write_rc(TC0, 0, (80000000 / 2) / desired_frequency); // MCK=80MHz
     tc_start(TC0, 0);
     dacc_set_trigger(DACC, 1); // TC0 trigger
     */

    // Enable TXRDY interrupt
    dacc_enable_interrupt(DACC, DACC_IER_TXRDY);

    // Enable NVIC for DACC interrupt
    NVIC_EnableIRQ(DACC_IRQn);

    return lf_success;
}

/*
  Interrupt handler for DAC TXRDY
*/
void DACC_Handler(void) {
    uint32_t status = dacc_get_interrupt_status(DACC);
    if (status & DACC_ISR_TXRDY) {
        // Handle TXRDY interrupt (e.g., write next data)
        // Example: dacc_write_conversion_data(DACC, next_data);
    }
}

/*
  Enable DAC channel (0 or 1)
*/
LF_FUNC int dac_enable_channels(int i) {
    if (i == 0) {
        dacc_enable_channel(DACC, DACC_MR_USER_SEL_CHANNEL0);
        return lf_success;
    } else if (i == 1) {
        dacc_enable_channel(DACC, DACC_MR_USER_SEL_CHANNEL1);
        return lf_success;
    }
    return lf_error;
}

/*
  Select DAC channel (0 or 1)
*/
LF_FUNC int dac_set_channels(int i) {
    if (i == 0) {
        dacc_set_channel_selection(DACC, DACC_MR_USER_SEL_CHANNEL0);
        return lf_success;
    } else if (i == 1) {
        dacc_set_channel_selection(DACC, DACC_MR_USER_SEL_CHANNEL1);
        return lf_success;
    }
    return lf_error;
}

/*
  Write 12-bit data to DAC (0-4095)
*/
int dac_write(uint32_t data) {
    // Ensure data is within 12-bit range
    if (data > DACC_MAX_DATA) {
        return lf_error;
    }
    dacc_write_conversion_data(DACC, data);
    return lf_success;
}


/*
  Cleanup DAC configuration
*/
LF_FUNC int dac_cleanup(void) {
    // Disable interrupt
    dacc_disable_interrupt(DACC, DACC_IER_TXRDY);

    // Disable NVIC for DACC
    NVIC_DisableIRQ(DACC_IRQn);

    // Disable trigger
    dacc_disable_trigger(DACC);

    // Disable channels
    dacc_disable_channel(DACC, DACC_MR_USER_SEL_CHANNEL0);
    dacc_disable_channel(DACC, DACC_MR_USER_SEL_CHANNEL1);

    // Disable DAC peripheral clock
    pmc_disable_periph_clk(ID_DACC);

    return lf_success;
}

/*
  Initialize DAC module for dynamic registration
*/
LF_FUNC int dac_init_module(void) {
    uint16_t idx;

    struct _lf_device *device = dyld_lf_get_selected();
    if (device == NULL) return lf_error;
    if (lf_dyld_dyn_sym_lookup(device, "dac_module", &idx) != lf_success) return lf_error;

    struct _lf_module *m = lf_module_create("dac_module", idx);
    if (!m) return lf_error;

    m->table = dac_table;
    m->length = 5; // Set table size
    return dyld_register(lf_get_selected(), m);
}
