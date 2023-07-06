#include <flipper.h>
#include "libflipper.h"
#include "sam4s16b.h"
#include <sysclk.h>
#include <conf_clock.h>
#include <pmc.h>
#include <dacc.h>


int main(int argc, char *argv[]) {
    
    //enable peripheral clock for DACC
    pmc_enable_periph_clk(ID_DACC);

    //begin DACC configuration by resetting the DACC hardware
    dacc_reset(DACC);
        
    //write one 16-bit value at a time, not two 16-bit values in one 32-bit word
    dacc_set_transfer_mode(DACC, HALFWORD_MODE);
            
    //refer to the article for details
    dacc_set_timing(DACC, MAXSPEED_MODE_DISABLED, STARTUP_TIME_1920_TICKS);
        
    //select channel 0
    dacc_set_channel_selection(DACC, DACC_CHANNEL0);
        
    /*
    Choose the TIO output from timer/counter channel 1 as the trigger.
    Note that "channel 1" in this case refers only to channel 1 of timer/counter
    module 0, not channel 1 of timer/counter module 1.

    only TIOA will trigger the DAC, not TIOB.
     */
    dacc_set_trigger(DACC, TC_CHANNEL1_TIO);
        
    //enable DACC channel 0
    dacc_enable_channel(DACC, DACC_CHANNEL0);

    n = 0;
    
    while (1)
    {
        //check the TXRDY flag
        if ( (dacc_get_interrupt_status(DACC) & TXRDY) == false) {
            continue; }
            
        //write the conversion value
        dacc_write_conversion_data(DACC, n);
            
        if (Increase_or_Decrease == INCREASE)
        {
            if (n == 4095) {
                Increase_or_Decrease = DECREASE; }
            else {
                n++; }
        }
            
        else
        {
            if (n == 0) {
                Increase_or_Decrease = INCREASE; }
            else {
                n--; }
        }
    }
    return lf_success;

}
