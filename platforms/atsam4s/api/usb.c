#include "libflipper.h"
#include "atsam4s.h"
#include "sam4s16b.h"
#include <pmc.h> 
#include "pio/pio_sam4s16b.h"

#include <pio.h>
#include <sysclk.h> 


int UDPClock = (F_CPU / 2); // 48Mhz
int masterClock = F_CPU; // 96Mhz
int udpInterrupt = UDP_IRQn;

// To configure DDP or DDM as PIOs, the user needs to configure
// the system I/O configuration register (CCFG_SYSIO) in the MATRIX.


// USB signals 
// USB D+ line
// IO_DP 
// USB D- line
// IO_DM 


LF_FUNC int usb_configure(void) {

// Thus, the USB device receives two clocks from the Power Management Controller (PMC):
// the master clock, MCK, used to drive the peripheral user interface, 
// and the UDPCK, used to interface with the bus USB signals (recovered 12 MHz domain).
    pmc_enable_periph_clk(ID_UDP);
    pmc_enable_udpck();

    return lf_success;
}
