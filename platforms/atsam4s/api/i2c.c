#include "libflipper.h"
#include <twi.h>
#include "sam4s16b.h"
#include <sysclk.h> 
#include <pmc.h> 

void mastrClkConfig(int x);
void speedConfig(int x);
void chipConfig(int x);
void smbusConfig(int x);


twi_options_t twi_opt;
twi_packet_t packet;

LF_FUNC masterEnable(void) {
    twi_enable_master_mode(TWI0);
}

LF_FUNC void mastrClkConfig(int x) {
    /*
        // MCK for TWI.
        twi_opt.master_clk = x;
    */
}

LF_FUNC void speedConfig(int x) {
    //! The baud rate of the TWI bus.
    twi_opt.speed = x;
}

LF_FUNC void chipConfig(int x) { 
    // The desired address.
    twi_opt.chip = x;
}

LF_FUNC void smbusConfig(int x) { 
    // SMBUS mode (set 1 to use SMBUS quick command, otherwise don't).
    twi_opt.smbus = 1;
}

LF_FUNC int i2c_configure(void) {

    /*
    TWD and TWCK pins may be multiplexed with PIO lines.
    To enable the TWI, the user must program the PIO Controller 
    to dedicate TWD and TWCK as peripheral lines.
    */

   /*
    Two Wire Interface 0 (TWI0)
    ID_TWI0   (19)
    Two Wire Interface 1 (TWI1) 
    ID_TWI1   (20) 
    */

    twi_master_init(TWI0, &twi_opt);
    sysclk_enable_peripheral_clock(19);
    return lf_success;
}

// Starts a read session. 
LF_FUNC void i2c_start_read(uint8_t address, uint8_t length) {

    twi_master_read(TWI0, &packet);
}

// Contiunues reading once a read session has begun. 
LF_FUNC uint8_t i2c_read(void) {
    return 0;
}

// Starts a write session. 
LF_FUNC void i2c_start_write(uint8_t address, uint8_t length) {

    twi_master_write(TWI0, &packet);
}

// Continues writing once a write session has begun. 
LF_FUNC void i2c_write(uint8_t byte) {
}

// Stops the active session. 
LF_FUNC void i2c_stop(void) {
    twi_disable_master_mode(TWI0);
}

