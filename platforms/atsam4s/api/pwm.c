#include "libflipper.h"
#include <pwm.h>
#include <sysclk.h> 
#include "atsam4s.h"
#include "sam4s16b.h"

pwm_channel_t pwm_channel_instance;
pwm_clock_t clock_setting;

LF_FUNC int pwm_configure(void) {

    pmc_enable_periph_clk(ID_PWM);
    clock_setting.ul_clka = 1000 * 100;
    clock_setting.ul_clkb = 0;
    clock_setting.ul_mck = 48000000;
    pwm_channel_enable(PWM, PWM_CHANNEL_0);
    pwm_init(PWM, &clock_setting);
    pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
    pwm_channel_instance.ul_period = 100;
    pwm_channel_instance.ul_duty = 50;
    pwm_channel_instance.channel = PWM_CHANNEL_0;
    pwm_channel_init(PWM, &pwm_channel_instance);
    return lf_success;
}
