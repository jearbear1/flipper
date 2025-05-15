# Adjust this if you installed a different version of GCC via Homebrew
CC = gcc-12

MODULE = fvm_test
SRC = src/main.c
BUILD_DIR = ../../.build
TARGET = $(BUILD_DIR)/$(MODULE).so

CFLAGS = \
  -fPIC \
  -Wall \
  -DLF_MODULE \
  -rdynamic \
  -I../../lib \
  -I../../.build/include \
  -I../../platforms/atsam4s/asf/include \
  -I../../platforms/posix/include

LDFLAGS = \
  -shared \
  -L../../.build/libflipper \
  -lflipper

all: $(TARGET)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	find src -type f ! -name '*.c' -exec rm -f {} +
