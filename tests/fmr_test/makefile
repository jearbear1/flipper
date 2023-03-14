##
## Makefile for fmrTest
##

# This sets the name of the Flipper module being built
MODULE = fmrTest

# This next line *MUST* come after MODULE has been set
include flipper.mk

COMPILER_PREFIX   = arm-none-eabi-

GEN = api.mk
INC_DIRS = include
SRC_DIRS = src
CFLAGS = -I/usr/local/include
LDFLAGS = -Wl,-T /usr/local/share/flipper/ram.ld \
           -Wl,-R /usr/local/share/flipper/platforms/atsam4s.elf \

$(eval $(call ADD_TARGET,fmrTest))

INC_DIRS = include
SRC_DIRS = host $(BUILD)/fmrTest/gen/api
CFLAGS = -I/usr/local/include -I$(BUILD)/fmrTest/gen/api
LDFLAGS = -L/usr/local/lib -lflipper

$(eval $(call ADD_TARGET,fmrTest_host))

INC_DIRS = include
SRC_DIRS = src $(BUILD)/fmrTest/gen/api
CFLAGS = -I/usr/local/include -I$(BUILD)/fmrTest/gen/api
LDFLAGS = -L/usr/local/lib -lflipper

$(eval $(call ADD_TARGET,fmrTest_fvm))

all:: $(BUILD)/fmrTest/fmrTest.so $(BUILD)/fmrTest_host/fmrTest_host $(BUILD)/fmrTest_fvm/fmrTest_fvm.so
