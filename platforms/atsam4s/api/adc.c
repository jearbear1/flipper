#include "libflipper.h"
#include "sam4s16b.h"
#include <sysclk.h> 
#include <pmc.h> 
#include <adc.h> 

int adc_configure(void);
int adc_enable_channels(int);
int adc_disable_channels(int);
int get_adc_channel_status(int);
int get_adc_channel_value(int);
int set_adc_resolution(int);
int get_latest_adc_value(void);
int start_adc(void);
int adc_cleanup(void);

LF_FUNC int adc_configure(void) {

    // 12-bit Analog-to-Digital Converter
    // Enable the Peripheral clock 
    // ADC Peripheral ID_ADC = 29 
    pmc_enable_periph_clk(ID_ADC);

    // Reset the ADC 
    adc_reset(ADC);

    return lf_success; 
 
}

// Enable ADC Channels

LF_FUNC int adc_enable_channels(int i) {

    if (i == 0) {
        // Enaable Analog to Digital Conversion channel 0
        adc_enable_channel(ADC, ADC_CHANNEL_0);
        return lf_success; 
    } else if (i == 1) { 
        // Enaable Analog to Digital Conversion channel 1
        adc_enable_channel(ADC, ADC_CHANNEL_1);
        return lf_success; 
    } else if (i == 2) { 
        // Enaable Analog to Digital Conversion channel 2
        adc_enable_channel(ADC, ADC_CHANNEL_2);
        return lf_success; 
    } else if (i == 3) {
        // Enaable Analog to Digital Conversion channel 3
        adc_enable_channel(ADC, ADC_CHANNEL_3);
        return lf_success;
    } else if (i == 4) {
        // Enaable Analog to Digital Conversion channel 4
        adc_enable_channel(ADC, ADC_CHANNEL_4);
        return lf_success; 
    } else if (i == 5) {
        // Enaable Analog to Digital Conversion channel 5
        adc_enable_channel(ADC, ADC_CHANNEL_5);
        return lf_success; 
    } else if (i == 6) {
        // Enaable Analog to Digital Conversion channel 6
        adc_enable_channel(ADC, ADC_CHANNEL_6);
        return lf_success; 
    } else {
        return lf_error;
    }
}

// Start ADC Conversion 
LF_FUNC int start_adc(void) {
    
    // Start Analog to Digital Conversion 
    adc_start(ADC);
    return lf_success; 
}

// Disable ADC Channels
LF_FUNC int adc_disable_channels(int i) {

    if (i == 0) {
        // Disable Analog to Digital Conversion channel 0
        adc_disable_channel(ADC, ADC_CHANNEL_0);
        return lf_success; 

    } else if (i == 1) { 
        // Disable Analog to Digital Conversion channel 1
        adc_disable_channel(ADC, ADC_CHANNEL_1);
        return lf_success; 

    } else if (i == 2) { 
        // Disable Analog to Digital Conversion channel 2
        adc_disable_channel(ADC, ADC_CHANNEL_2);
        return lf_success; 

    } else if (i == 3) {
        // Disable Analog to Digital Conversion channel 3
        adc_disable_channel(ADC, ADC_CHANNEL_3);
        return lf_success;

    } else if (i == 4) {
        // Disable Analog to Digital Conversion channel 4
        adc_disable_channel(ADC, ADC_CHANNEL_4);
        return lf_success; 

    } else if (i == 5) {
        // Disable Analog to Digital Conversion channel 5
        adc_disable_channel(ADC, ADC_CHANNEL_5);
        return lf_success; 

    } else if (i == 6) {
        // Disable Analog to Digital Conversion channel 6
        adc_disable_channel(ADC, ADC_CHANNEL_6);
        return lf_success; 
    } else {
        return lf_error;
    }
}

// Get the status of a specific Channel in the Analog to Digital Converter

LF_FUNC int get_adc_channel_status(int i) {

    if (i == 0) {
        // Get Status of ADC Channel 0
        adc_get_channel_status(ADC, ADC_CHANNEL_0);
        return lf_success; 

    } else if (i == 1) { 
        // Get Status of ADC Channel 1
        adc_get_channel_status(ADC, ADC_CHANNEL_1);
        return lf_success; 

    } else if (i == 2) { 
        // Get Status of ADC Channel 2
        adc_get_channel_status(ADC, ADC_CHANNEL_2);
        return lf_success; 

    } else if (i == 3) {
        // Get Status of ADC Channel 3 
        adc_get_channel_status(ADC, ADC_CHANNEL_3);
        return lf_success;

    } else if (i == 4) {
        // Get Status of ADC Channel 4
        adc_get_channel_status(ADC, ADC_CHANNEL_4);
        return lf_success; 

    } else if (i == 5) {
        // Get Status of ADC Channel 5
        adc_get_channel_status(ADC, ADC_CHANNEL_5);
        return lf_success; 

    } else if (i == 6) {
        // Get Status of ADC Channel 6
        adc_get_channel_status(ADC, ADC_CHANNEL_6);
        return lf_success; 
    } else {
        return lf_error;
    }
}

// Read the ADC result data of the specified channel

LF_FUNC int get_adc_channel_value(int i) {

    if (i == 0) {
        // Get Value of ADC Channel 0
        adc_get_channel_value(ADC, ADC_CHANNEL_0);
        return lf_success; 

    } else if (i == 1) { 
        // Get Value of ADC Channel 1
        adc_get_channel_value(ADC, ADC_CHANNEL_1);
        return lf_success; 

    } else if (i == 2) { 
        // Get Value of ADC Channel 2
        adc_get_channel_value(ADC, ADC_CHANNEL_2);
        return lf_success; 

    } else if (i == 3) {
        // Get Value of ADC Channel 3 
        adc_get_channel_value(ADC, ADC_CHANNEL_3);
        return lf_success;

    } else if (i == 4) {
        // Get Value of ADC Channel 4
        adc_get_channel_value(ADC, ADC_CHANNEL_4);
        return lf_success; 

    } else if (i == 5) {
        // Get Value of ADC Channel 5
        adc_get_channel_value(ADC, ADC_CHANNEL_5);
        return lf_success; 

    } else if (i == 6) {
        // Get Value of ADC Channel 6
        adc_get_channel_value(ADC, ADC_CHANNEL_6);
        return lf_success; 
    } else {
        return lf_error;
    }
}

// Set the ADC resolution 
LF_FUNC int set_adc_resolution(int i) {

    if (i == 10) {
        // ADC 10 Bit Resolution
        adc_set_resolution(ADC, ADC_10_BITS);
        return lf_success; 

    } else if (i == 12) {
        // ADC 12 Bit Resolution
        adc_set_resolution(ADC, ADC_12_BITS);
        return lf_success; 
    } else {
        return lf_error;
    }
    
}

LF_FUNC int get_latest_adc_value(void) {
   // Read the last ADC result data.
   adc_get_latest_value(ADC);
   return lf_success; 
}

LF_FUNC int adc_cleanup(void) {
    // Disable all ADC Channels
    adc_disable_all_channel(ADC);
    return lf_success;
}