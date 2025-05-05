#include "ioport.h"
#include "sam4s16b.h" 

// Encoding helpers
#define PIN_PORT_MASK  0xFF00
#define PIN_NUM_MASK   0x00FF
#define PIN_PORT_SHIFT 8

static inline Pio* get_pio_from_pin(uint32_t pin) {
    uint8_t port_index = (pin & PIN_PORT_MASK) >> PIN_PORT_SHIFT;

    switch (port_index) {
        case IOPORT_PORT_A: return PIOA;
        case IOPORT_PORT_B: return PIOB;
        case IOPORT_PORT_C: return PIOC;
        default: return NULL; // Invalid port
    }
}

static inline uint32_t get_pin_mask(uint32_t pin) {
    return 1 << (pin & PIN_NUM_MASK);
}

void ioport_set_pin_mode(uint32_t pin, uint32_t flags) {
    // This stub ignores flags, but you could handle pull-ups, debounce, etc. here.
    (void)pin;
    (void)flags;
}

void ioport_set_pin_dir(uint32_t pin, enum ioport_direction dir) {
    Pio *port = get_pio_from_pin(pin);
    if (!port) return;

    uint32_t mask = get_pin_mask(pin);
    port->PIO_PER = mask; // Enable PIO control

    if (dir == IOPORT_DIR_OUTPUT) {
        port->PIO_OER = mask; // Output Enable
    } else {
        port->PIO_ODR = mask; // Output Disable (input)
    }
}

void ioport_enable_pin(uint32_t pin) {
    Pio *port = get_pio_from_pin(pin);
    if (!port) return;

    uint32_t mask = get_pin_mask(pin);
    port->PIO_PER = mask; // Enable control of the pin via PIO
}
