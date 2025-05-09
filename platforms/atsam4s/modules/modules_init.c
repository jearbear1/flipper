#include "libflipper.h"

// Forward declare all peripheral init functions

extern int dac_init_module(void);
/*
extern int adc_init_module(void);
extern int button_init_module(void);
extern int gpio_init_module(void);
extern int i2c_init_module(void);
extern int led_init_module(void);
extern int pwm_init_module(void);
extern int rtc_init_module(void);
extern int spi_init_module(void);
extern int swd_init_module(void);
extern int temp_init_module(void);
extern int timer_init_module(void);
extern int uart0_init_module(void);
extern int usart_init_module(void);
extern int usb_init_module(void);
extern int wdt_init_module(void);
 */

// Call this once in your platform bootup
void platform_init(void) {
    
    dac_init_module();
    /*
    adc_init_module();
    button_init_module();
    gpio_init_module();
    i2c_init_module();
    led_init_module();
    pwm_init_module();
    rtc_init_module();
    spi_init_module();
    swd_init_module();
    temp_init_module();
    timer_init_module();
    uart0_init_module();
    usart_init_module();
    usb_init_module();
    wdt_init_module();
    */
}
