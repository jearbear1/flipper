#include "libflipper.h"
#include "atsam4s.h"
#include "sam4s16b.h"
#include "pio/pio_sam4s16b.h"
#include <pio.h>
#include <sysclk.h> 
#include <pmc.h> 
#include "gpio.h"
#include "ioport.h"


int gpioConfigure(void);
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


void gpio_configure(uint32_t pin, const uint32_t flags, uint32_t dir) {
    ioport_set_pin_mode(pin, flags);
    ioport_set_pin_dir(pin, dir);
    ioport_enable_pin(pin);
}

void gpio_enable(Pio *pio, uint32_t pin_mask) {
    // 1. Enable the peripheral clock for the PIO controller
    if (pio == PIOA)
        pmc_enable_periph_clk(ID_PIOA);
    else if (pio == PIOB)
        pmc_enable_periph_clk(ID_PIOB);
    else if (pio == PIOC)
        pmc_enable_periph_clk(ID_PIOC);

    // 2. Disable PIO's peripheral control on that pin
    pio->PIO_PDR &= ~pin_mask;

    // 3. Enable PIO control (we want to manually control this pin)
    pio->PIO_PER |= pin_mask;

    // 4. Set it as output
    pio->PIO_OER |= pin_mask;

    // 5. Optionally set to default low (clear output data)
    pio->PIO_CODR = pin_mask;

    // 6. Enable pull-up if needed
    // pio->PIO_PUER |= pin_mask; 
}

void gpio_write(uint32_t set, uint32_t clear) {
}

uint32_t gpio_read(uint32_t mask) {
    return 0;
}
