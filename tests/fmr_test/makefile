##
## Makefile for fmrTest (Flipper Carbon Edition)
##

MODULE = fmrTest

# Include core Flipper build system
include flipper.mk

# Toolchain and API generation
COMPILER_PREFIX = arm-none-eabi-
GEN = api.mk

#################################################
# Embedded Target: fmrTest
INC_DIRS = include
SRC_DIRS = src
CFLAGS   = -I/usr/local/include
LDFLAGS  = -Wl,-T /usr/local/share/flipper/ram.ld \
           -Wl,-R /usr/local/share/flipper/platforms/atsam4s.elf

$(eval $(call ADD_TARGET,fmrTest))

# Generate .hex from .elf
hex: $(BUILD)/fmrTest/fmrTest.so
	$(COMPILER_PREFIX)objcopy -O ihex $(BUILD)/fmrTest/fmrTest.elf $(BUILD)/fmrTest/fmrTest.hex

# Flash via OpenOCD over SWD (for Flipper Carbon ATSAM4S)
flash: $(BUILD)/fmrTest/fmrTest.elf
	openocd -f interface/stlink.cfg -f target/at91sam4s.cfg \
		-c "program $< verify reset exit"

#################################################
# Host build: fmrTest_host
INC_DIRS = include
SRC_DIRS = host $(BUILD)/fmrTest/gen/api
CFLAGS   = -I/usr/local/include -I$(BUILD)/fmrTest/gen/api
LDFLAGS  = -L/usr/local/lib -lflipper

$(eval $(call ADD_TARGET,fmrTest_host))

#################################################
# FVM-specific variant: fmrTest_fvm
INC_DIRS = include
SRC_DIRS = src $(BUILD)/fmrTest/gen/api
CFLAGS   = -I/usr/local/include -I$(BUILD)/fmrTest/gen/api
LDFLAGS  = -L/usr/local/lib -lflipper

$(eval $(call ADD_TARGET,fmrTest_fvm))

#################################################
# Build everything + hex
all:: \
  $(BUILD)/fmrTest/fmrTest.so \
  $(BUILD)/fmrTest_host/fmrTest_host \
  $(BUILD)/fmrTest_fmrTest_fvm.so \
  hex
