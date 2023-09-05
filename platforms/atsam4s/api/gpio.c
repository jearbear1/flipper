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

LF_FUNC int gpio_configure(void) {
    return lf_success;
}

LF_FUNC void gpio_enable(uint32_t enable, uint32_t disable) {
}

LF_FUNC void gpio_write(uint32_t set, uint32_t clear) {
}

LF_FUNC uint32_t gpio_read(uint32_t mask) {
    return 0;
}