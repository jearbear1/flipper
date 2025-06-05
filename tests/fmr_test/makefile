# Compiler (adjust if needed)
CC = gcc-12

# Module name (e.g., make MODULE=fmrTest)
MODULE ?= fmrTest

# Paths (relative to Makefile location, assuming fmr_test/ subdirectory)
FLIPPER_ROOT = $(realpath $(CURDIR)/../../)
BUILD_DIR = $(FLIPPER_ROOT)/.build
SRC = src/fmrTest.c
TARGET = $(BUILD_DIR)/$(MODULE).so
CALL_FVM = src/call_fvm
CLIENT_SRC = host/main.c

# Compilation flags for the module
MODULE_CFLAGS = \
  -fPIC \
  -Wall \
  -DLF_MODULE \
  -rdynamic \
  -I$(FLIPPER_ROOT)/lib \
  -I$(BUILD_DIR)/include \
  -Iinclude \
  -I$(FLIPPER_ROOT)/platforms/atsam4s/asf/include \
  -I$(FLIPPER_ROOT)/platforms/posix/include

# Compilation flags for the client
CLIENT_CFLAGS = \
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

# Default target (optional, for convenience)
all: module call_fvm

# Build the module
module: $(TARGET)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CC) $(MODULE_CFLAGS) $(SRC) -o $@ -shared $(LDFLAGS)

# Build the client
call_fvm: $(CALL_FVM)

$(CALL_FVM): $(CLIENT_SRC) | $(BUILD_DIR)
	$(CC) $(CLIENT_CFLAGS) -D FLIPPER_PLATFORM_POSIX $(CLIENT_SRC) -o $@ $(LDFLAGS)

# Create build directory with error checking
$(BUILD_DIR):
	@mkdir -p $@ || { echo "Error: Failed to create build directory $(@)"; exit 1; }

# Clean generated files specific to fmr_test
clean:
	@find src -type f ! -name '*.c' -exec rm -f {} +

.PHONY: all module call_fvm clean
