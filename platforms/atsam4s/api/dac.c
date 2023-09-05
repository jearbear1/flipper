#include "libflipper.h"
#include "atsam4s.h"
#include "sam4s16b.h"
#include <sysclk.h> 
#include <conf_clock.h>
#include <pmc.h> 
#include <dacc.h> 


int dac_configure(void);
int dac_setup(void);
int dac_enbale_channels(int);
int dac_set_channels(int);
int dac_write(void);
int dac_cleanup(void);
int dac_trigger0(void);

// Communicate at 1000000 (1 megabaud)
int dacBauderate = PLATFORM_BAUDRATE; 
// F_CPU = 96000000
int dacFrequency = F_CPU;
int dacClock = BOARD_MCKR;

/*
The default clock configuration (found in “conf_clock.h”) gives 
a master clock (MCK) frequency of 120 MHz.
The frequency of the peripheral clock is equal to that of MCK
and the frequency of the clock that drives the DAC circuitry ( the “DAC clock”)
is the peripheral-clock frequency divided by two. 
Thus, the default configuration leaves us with fDAC_clock = 60 MHz.
The maximum DAC-clock frequency is 50 MHz
*/


LF_FUNC int dac_configure(void) {
  // ===== PLL1 (B) Options   (Fpll = (Fclk * PLL_mul) / PLL_div)
  // (Fpll = (120Mhz * 16) / 2) = 960Mhz
  // Set fDAC_clock = 480 MHz ?

  // 12 bit DAC

  // Enable the Peripheral clock 
  // DAC Peripheral ID = 30 
  pmc_enable_periph_clk(ID_DACC);
   
  // Reset the DAC 
  dacc_reset(DACC);

  return lf_success;
  
}

LF_FUNC int dac_setup(void) {

  // Enable the DAC 
  dacc_enable_flexible_selection(DACC);

  // Set the timing for the DAC
  dacc_set_timing(DACC, DACC_MR_MAXS_NORMAL, DACC_MR_STARTUP_1920);

  // Set DAC Transfer Mode
  dacc_set_transfer_mode(DACC, DACC_MR_WORD_HALF);

  // Configure the DAC Trigger
  dacc_set_trigger(DACC, DACC_MR_TRGEN);

  // Enable Interrupt 
  dacc_enable_interrupt(DACC, DACC_IER_TXRDY);

 return lf_success; 
 
}

LF_FUNC int dac_enbale_channels(int i) {

    if (i == 0) {
        // Enaable Digital to Analog Conversion channel 0
        dacc_enable_channel(DACC, DACC_MR_USER_SEL_CHANNEL0); 
    return lf_success; 
  } else if (i == 1) { 
        // Enaable Digital to Analog Conversion channel 1
        dacc_enable_channel(DACC, DACC_MR_USER_SEL_CHANNEL1); 
    return lf_success;
  } else {
    return lf_error;
  }

}

// Set Digital to Analog Conversion channels

LF_FUNC int dac_set_channels(int i) {
  if (i == 0) {
    // set Digital to Analog Conversion channel to channel 0
    dacc_set_channel_selection(DACC, DACC_MR_USER_SEL_CHANNEL0);
  return lf_success; 
  } else if (i == 1) { 
    // set Digital to Analog Conversion channel to channel 1
    dacc_set_channel_selection(DACC, DACC_MR_USER_SEL_CHANNEL1); 
  return lf_success;
  } else {
  return lf_error;
  }

}

LF_FUNC int dac_write(void) {

    // Write Data to DAC
    // DACC_CDR_DATA_Msk is the data to convert
    dacc_write_conversion_data(DACC, DACC_CDR_DATA_Msk);
   
   return lf_success; 
}

LF_FUNC int dac_cleanup(void) {

    // Disable Interrupt 
    dacc_disable_interrupt(DACC, DACC_IER_TXRDY);

    // Disable the DAC Trigger
    dacc_disable_trigger(DACC);

    // Disable the DAC
    dacc_disable_channel(DACC, DACC_MR_USER_SEL_CHANNEL0);
    dacc_disable_channel(DACC, DACC_MR_USER_SEL_CHANNEL1);

    return lf_success;
}