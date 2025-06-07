# --- UTILITIES --- #

CC = /usr/bin/clang
AS = /usr/bin/clang
LD = /usr/bin/clang
OBJCOPY =
OBJDUMP =

# -------- fdfu -------- #
DEPENDENCIES = libflipper
INC_DIRS = $(BUILD)/include
SRC_DIRS = utils/fdfu/src
LDFLAGS  = -L$(BUILD)/libflipper -lflipper

$(eval $(call ADD_TARGET,fdfu))

# -------- fdebug -------- #
DEPENDENCIES = libflipper
INC_DIRS = $(BUILD)/include lib platforms/atmegau2/include
SRC_DIRS = utils/fdebug/src lib
CFLAGS   = $(shell pkg-config --cflags libusb-1.0) $(addprefix -I,$(INC_DIRS))
LDFLAGS  = -L$(BUILD)/libflipper -lflipper $(shell pkg-config --libs libusb-1.0)

$(eval $(call ADD_TARGET,fdebug))

# -------- fload -------- #
DEPENDENCIES = libflipper
INC_DIRS = $(BUILD)/include
SRC_DIRS = utils/fload/src
LDFLAGS  = -L$(BUILD)/libflipper -lflipper

$(eval $(call ADD_TARGET,fload))

# -------- fvm -------- #
DEPENDENCIES = libflipper
INC_DIRS = $(BUILD)/include \
           lib \
           platforms/atsam4s \
           platforms/atsam4s/include \
           platforms/posix/include \
           utils/fvm/src/api
SRC_DIRS = utils/fvm/src utils/fvm/src/api lib
CFLAGS  += $(addprefix -I,$(INC_DIRS))
LDFLAGS = -L$(BUILD)/libflipper -lflipper -ldl -Wl,-rpath,@executable_path/../libflipper

$(eval $(call ADD_TARGET,fvm))

# --- UTILS --- #

.PHONY: utils install-utils uninstall-utils clean

utils: $(BUILD)/fload/fload $(BUILD)/fvm/fvm

all:: utils

install-utils: utils | $(BUILD)/utils/fdwarf/.dir
	# $(_v)cp -r $(BUILD)/fdfu/fdfu $(PREFIX)/bin
	# $(_v)cp -r $(BUILD)/fdebug/fdebug $(PREFIX)/bin
	$(_v)cp -r $(BUILD)/fvm/fvm $(PREFIX)/bin
	$(_v)cp -r $(BUILD)/fload/fload $(PREFIX)/bin
	$(_v)cp utils/fdwarf/fdwarf.py $(PREFIX)/bin/fdwarf
	$(_v)chmod +x $(PREFIX)/bin/fdwarf

install:: install-utils

uninstall-utils:
	# $(_v)rm $(PREFIX)/bin/fdfu
	# $(_v)rm $(PREFIX)/bin/fdebug
	$(_v)rm $(PREFIX)/bin/fload
	$(_v)rm $(PREFIX)/bin/ftest
	$(_v)rm $(PREFIX)/bin/fvm

uninstall:: uninstall-utils
