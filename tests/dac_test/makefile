##
## Makefile for dacTest
##

# This sets the name of the Flipper module being built
MODULE = dacTest

# This next line *MUST* come after MODULE has been set
include flipper.mk

COMPILER_PREFIX   = arm-none-eabi-

GEN = api.mk
INC_DIRS = include
SRC_DIRS = src
CFLAGS = -I/usr/local/include
LDFLAGS = -Wl,-T /usr/local/share/flipper/ram.ld \
           -Wl,-R /usr/local/share/flipper/platforms/atsam4s.elf \

$(eval $(call ADD_TARGET,dacTest))

INC_DIRS = include
SRC_DIRS = host $(BUILD)/dacTest/gen/api
CFLAGS = -I/usr/local/include -I$(BUILD)/dacTest/gen/api
LDFLAGS = -L/usr/local/lib -lflipper

$(eval $(call ADD_TARGET,dacTest_host))

INC_DIRS = include
SRC_DIRS = src $(BUILD)/dacTest/gen/api
CFLAGS = -I/usr/local/include -I$(BUILD)/dacTest/gen/api
LDFLAGS = -L/usr/local/lib -lflipper

$(eval $(call ADD_TARGET,dacTest_fvm))

all:: $(BUILD)/dacTest/dacTest.so $(BUILD)/dacTest_host/dacTest_host $(BUILD)/dacTest_fvm/dacTest_fvm.so
