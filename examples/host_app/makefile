# Adjust this if you installed a different version of GCC via Homebrew
CC = gcc-12

# Dynamically chosen module (e.g. make MODULE=fvm_test)
MODULE ?= module

# Paths
SRC = src/main.c
BUILD_DIR = ../../.build
TARGET = $(BUILD_DIR)/$(MODULE).so
CALL_FVM = src/call_fvm
CLIENT_SRC = src/client_calls_fvm.c

# Compilation flags
CFLAGS = \
  -fPIC \
  -Wall \
  -DLF_MODULE \
  -rdynamic \
  -I../../lib \
  -I../../.build/include \
  -I../../platforms/atsam4s/asf/include \
  -I../../platforms/posix/include

# Linker flags
LDFLAGS = \
  -shared \
  -L../../.build/libflipper \
  -lflipper

# Target to build just the module
module: $(TARGET)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

# Target to build just the client
call_fvm: $(CALL_FVM)

$(CALL_FVM): $(CLIENT_SRC)
	$(CC) -I../../platforms/posix/include \
	      -I../../lib \
	      -I../../.build/include \
	      -L../../.build/libflipper \
	      -lflipper \
	      $(CLIENT_SRC) -o $(CALL_FVM)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean generated files (but not .c)
clean:
	find src -type f ! -name '*.c' -exec rm -f {} +
