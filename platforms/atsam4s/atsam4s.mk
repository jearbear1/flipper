# ARM target variables

CC = clang
AS = clang
AR = arm-none-eabi-ar
LD = arm-none-eabi-ld
OBJCOPY = llvm-objcopy
OBJDUMP = llvm-objdump

GEN = git.mk api.mk

# Directories that need to be included for this target.
INC_DIRS = os/include \
           platforms/atsam4s/include \
           platforms/atsam4s/asf/include \
           platforms/atsam4s/asf/include/pio \
           platforms/atsam4s/api \
           platforms/atsam4s/asf/src \
           lib


SRC_DIRS = platforms/atsam4s \
           platforms/atsam4s/api \
           platforms/atsam4s/api/modules \
           os/arch/armv7 \
           lib

CFLAGS = -mthumb \
         --target=thumbv7em-unknown-none-elf \
         -mfloat-abi=soft \
         -I/opt/homebrew/Cellar/arm-gcc-bin@12/12.2.Rel1/arm-none-eabi/include \
         -gdwarf-2 \
         -D__SAM4S16B__

LDFLAGS = -L/opt/homebrew/Cellar/arm-gcc-bin@12/12.2.Rel1/arm-none-eabi/lib/thumb/v7+fp/softfp \
         -lc \
         -lm \
         -L/opt/homebrew/Cellar/arm-gcc-bin@12/12.2.1/thumb/v7e-m+fp/softfp \
         -lgcc \
         -Tplatforms/atsam4s/config/sam4s16.ld \
         -no-enum-size-warning

# Ensure the generated ELF directory exists before linking
$(BUILD)/atsam4s/atsam4s.elf: | $(BUILD)/atsam4s/gen/.dir

$(BUILD)/atsam4s/gen/.dir:
	$(_v)mkdir -p $(BUILD)/atsam4s/gen

$(eval $(call ADD_TARGET,atsam4s))

.PHONY: atsam4s install-atsam4s

atsam4s: $(BUILD)/atsam4s/atsam4s.elf
	$(_v)mkdir -p $(PREFIX)/share/flipper
	$(_v)cp assets/ram.ld $(PREFIX)/share/flipper
	$(_v)cp $(BUILD)/atsam4s/atsam4s.elf $(PREFIX)/share/flipper
	$(_v)test -d $(BUILD)/atsam4s/gen/api && cp -n $(BUILD)/atsam4s/gen/api/*.h platforms/atsam4s/api/modules/ 2>/dev/null || true

all:: atsam4s

install-atsam4s: utils atsam4s $(BUILD)/atsam4s/atsam4s.bin
	$(_v)$(BUILD)/fdfu/fdfu $(BUILD)/atsam4s/atsam4s.bin
