# Compiler (adjust if needed)
CC = gcc-12

# Module name
MODULE ?= dacTest

# Paths (relative to Makefile location, assuming fmr_test/ subdirectory)
FLIPPER_ROOT = $(realpath $(CURDIR)/../../)
BUILD_DIR = $(FLIPPER_ROOT)/.build
TARGET = $(BUILD_DIR)/$(MODULE).so
CALL_FVM = host/call_fvm
CLIENT_SRC = host/main.c

# Compilation flags for the client
CFLAGS = \
  -Wall \
  -I$(FLIPPER_ROOT)/lib \
  -I$(BUILD_DIR)/include \
  -I$(FLIPPER_ROOT)/platforms/atsam4s/asf/include \
  -I$(FLIPPER_ROOT)/platforms/posix/include

# Linker flags (shared across module and client)
LDFLAGS = \
  -L$(BUILD_DIR)/libflipper \
  -Wl,-rpath,$(BUILD_DIR)/libflipper \
  -lflipper

# Default target
all: call_fvm

# Build the client
call_fvm: $(CALL_FVM)

$(CALL_FVM): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -D FLIPPER_PLATFORM_POSIX $(CLIENT_SRC) -o $@ $(LDFLAGS)

# Alternate manual client build target
build_client:
	$(CC) -o call_fvm host/main.c $(FLIPPER_ROOT)/utils/fvm/src/api/fvm_dac_module.c \
	  -I$(FLIPPER_ROOT)/lib \
	  -I$(BUILD_DIR)/include \
	  -I$(FLIPPER_ROOT)/platforms/atsam4s/asf/include \
	  -I$(FLIPPER_ROOT)/platforms/posix/include \
	  -D FLIPPER_PLATFORM_POSIX \
	  -L$(BUILD_DIR)/libflipper \
	  -Wl,-rpath,$(BUILD_DIR)/libflipper \
	  -lflipper

# Create build directory
$(BUILD_DIR):
	@mkdir -p $@ || { echo "Error: Failed to create build directory $(@)"; exit 1; }

# Clean
clean:
	@rm -f host/call_fvm

.PHONY: all module call_fvm build_client clean
