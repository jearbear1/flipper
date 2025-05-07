#ifndef __DAC_H__
#define __DAC_H__

#include "libflipper.h"

LF_FUNC int dac_configure(void);
LF_FUNC int dac_enbale_channels(int);
LF_FUNC int dac_set_channels(int);
LF_FUNC int dac_write(void);
LF_FUNC int dac_trigger0(void);
LF_FUNC int dac_cleanup(void);


// Declare _dac_module:
extern struct _lf_module _dac_module;

#endif
