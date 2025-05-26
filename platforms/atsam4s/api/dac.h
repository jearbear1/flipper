#ifndef __DAC_H__
#define __DAC_H__

#include "libflipper.h"

// Declare the function table defined in dac_table.c
extern lf_function dac_table[];

// Declare init function so module_init.c can call it
int dac_init_module(void);

// Declare the core DAC control functions
int dac_configure(void);
int dac_enable_channels(int);
int dac_set_channels(int);
int dac_write(void);
int dac_trigger0(void);
int dac_cleanup(void);


// Declare _dac_module:
extern struct _lf_module _dac_module;

#endif
