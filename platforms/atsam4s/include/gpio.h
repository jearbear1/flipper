#ifndef __gpio_h__
#define __gpio_h__

#include <stdint.h>
#include "asf.h"  

// ASF GPIO configuration
void gpio_configure(uint32_t pin, const uint32_t flags, uint32_t direction);

// Additional Utility Functions
void gpio_enable(Pio *pio, uint32_t pin_mask);
int gpioSetOutput(uint32_t pin);
int gpioSetInput(uint32_t pin);
void gpio_write(uint32_t set_mask, uint32_t clear_mask);
uint32_t gpio_read(uint32_t mask);

// Interrupt Configuration
int configureInterruptRiseEdge(uint32_t pin);
int configureInterruptFallEdge(uint32_t pin);
int getInterruptMask(const Pio *pio);
int getInterruptStatus(const Pio *pio);
int disableInterrupt(uint32_t pin);

// Misc Hardware Helpers
void getOutputDataStatus(const Pio *pio, const uint32_t mask);
void setSchmittTrigger(Pio *pio, const uint32_t mask);
void getSchmittTrigger(const Pio *pio);
int captureSetMode(int mode);
void captureEnable(Pio *pio);
void captureDisable(Pio *pio);

#endif
