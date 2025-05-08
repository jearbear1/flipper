# Toolchain
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
LD = arm-none-eabi-gcc

# Project paths
BUILD = ../.build/qux
SRC_DIRS = src ../lib ../os/arch/armv7
INCLUDE_DIRS = \
    include \
    ../.build/include \
    ../.build/include/flipper \
    ../.build/include/flipper/platforms/atsam4s/atsam4s/h \
    /opt/homebrew/include \
    ../platforms/atsam4s/asf/include \
    ../platforms/atsam4s/asf/include/pio \
    ../platforms/atsam4s/asf/include/sam \
    ../os/include \
    ../os/arch/armv7 \
    ../lib

# Source files
C_SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
S_SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.S))

# Object files
C_OBJS = $(patsubst %.c,$(BUILD)/%.o,$(notdir $(C_SRCS)))
S_OBJS = $(patsubst %.S,$(BUILD)/%.o,$(notdir $(S_SRCS)))
OBJS = $(C_OBJS) $(S_OBJS)

# Flags
CFLAGS = -std=c99 -Wall -Wextra -Wno-unused-parameter -Wno-expansion-to-defined -Wno-implicit-function-declaration -DAPIGEN -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -gdwarf-2 $(addprefix -I,$(INCLUDE_DIRS))
ASFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -gdwarf-2 $(addprefix -I,$(INCLUDE_DIRS))

# Targets
TARGET = $(BUILD)/qux.elf

all: $(TARGET)

# Link everything
$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o $@

# Compile C files
$(BUILD)/%.o: $(SRC_DIRS)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble ASM files
$(BUILD)/%.o: $(SRC_DIRS)/%.S
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)

.PHONY: all clean
