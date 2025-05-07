#include <flipper/flipper.h>

int dac_configure(void) {

  // ===== PLL1 (B) Options   (Fpll = (Fclk * PLL_mul) / PLL_div)
  // (Fpll = (120Mhz * 16) / 2) = 960Mhz
  // Set fDAC_clock = 480 MHz ?

  // 12 bit DAC

  // Enable the Peripheral clock 
  // DAC Peripheral ID = 30 
  pmc_enable_periph_clk(ID_DACC);
   
  // Reset the DAC 
  dacc_reset(DACC);

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

  printf("Configured the dac.\n");
  return lf_success;

}
