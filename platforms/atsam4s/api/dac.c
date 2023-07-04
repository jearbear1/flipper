#include "libflipper.h"
#include <dacc.h> 
#include "sam4s16b.h"

LF_FUNC int dac_configure(void) {

    // 12 bit DAC
    // initialize system clock 
    //sysclk_init();
   

    // Enable the Peripheral clock 
    // DAC Peripheral ID = 30 
    //sysclk_enable_peripheral_clock(30);
    pmc_enable_periph_clk(DACC);

    // Reset the DAC 
    dacc_reset(DACC);

    // Enable the DAC 
    //dacc_enable(DACC);


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

LF_FUNC int dac_enbale_channel0(void) {

    // Enaable Digital to Analog Conversion channel 0
    dacc_enable_channel(DACC, DACC_MR_USER_SEL_CHANNEL0); 

    return lf_success; 
}

LF_FUNC int dac_set_channel0(void) {

    // set Digital to Analog Conversion channel to channel 0
    dacc_set_channel_selection(DACC, DACC_MR_USER_SEL_CHANNEL0); 

   return lf_success; 
}


LF_FUNC int dac_enbale_channel1(void) {

    // Enaable Digital to Analog Conversion channel 1
    dacc_enable_channel(DACC, DACC_MR_USER_SEL_CHANNEL1); 

    return lf_success; 
}

LF_FUNC int dac_set_channel1(void) {

    // set Digital to Analog Conversion channel to channel 1
    dacc_set_channel_selection(DACC, DACC_MR_USER_SEL_CHANNEL1); 

   return lf_success; 
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
    //dacc_disable(DACC);

    return lf_success;
}



