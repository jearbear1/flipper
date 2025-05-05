#ifndef IOPORT_H
#define IOPORT_H

#include <stdint.h>
#include <sam4s16b.h>
#include <stddef.h>  
/**
 * \defgroup ioport_group IOPORT API for ATSAM
 *
 * This provides GPIO pin configuration and control in a
 * standardized manner for ATSAM4S and related devices.
 *
 * Port pin control code should produce both compact and fast execution times
 * when used with constant values.
 *
 * \section dependencies Dependencies
 * - SAM4S CMSIS headers (for base register definitions)
 * - ioport services for high-level pin access (optional)

 * brief IOPORT pin directions 
 **/

/** \brief IOPORT pin directions */
enum ioport_direction {
    IOPORT_DIR_INPUT,   /*!< Pin as input */
    IOPORT_DIR_OUTPUT,  /*!< Pin as output */
};

/** \brief IOPORT logic levels */
enum ioport_value {
    IOPORT_PIN_LEVEL_LOW,   /*!< Logic low */
    IOPORT_PIN_LEVEL_HIGH,  /*!< Logic high */
};

/** \brief IOPORT edge and level sensing modes for interrupt config */
enum ioport_sense {
    IOPORT_SENSE_BOTHEDGES,   /*!< Sense both edges */
    IOPORT_SENSE_FALLING,     /*!< Sense falling edge */
    IOPORT_SENSE_RISING,      /*!< Sense rising edge */
    IOPORT_SENSE_LEVEL_LOW,   /*!< Sense low level */
    IOPORT_SENSE_LEVEL_HIGH,  /*!< Sense high level */
};

/* Port encoding macros */
#define IOPORT_PORT_A 0
#define IOPORT_PORT_B 1
#define IOPORT_PORT_C 2

#define IOPORT_CREATE_PIN(port, pin) (((port) << 8) | (pin))

/* Helpful named pin macros */
#define PIN_PA0  IOPORT_CREATE_PIN(IOPORT_PORT_A, 0)
#define PIN_PA1  IOPORT_CREATE_PIN(IOPORT_PORT_A, 1)
#define PIN_PB0  IOPORT_CREATE_PIN(IOPORT_PORT_B, 0)
#define PIN_PB1  IOPORT_CREATE_PIN(IOPORT_PORT_B, 1)
#define PIN_PC0  IOPORT_CREATE_PIN(IOPORT_PORT_C, 0)
#define PIN_PC1  IOPORT_CREATE_PIN(IOPORT_PORT_C, 1)
/* Add more as needed */

/* Function prototypes */
void ioport_set_pin_mode(uint32_t pin, uint32_t flags);
void ioport_set_pin_dir(uint32_t pin, enum ioport_direction dir);
void ioport_enable_pin(uint32_t pin);

#endif /* IOPORT_H */
