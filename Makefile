.SUFFIXES:
.SUFFIXES:	.c .o .h

export ROOT_PATH    := $(CURDIR)
export BIN_PATH     := $(ROOT_PATH)/bin
export LIB_PATH     := $(ROOT_PATH)/lib
export BUILD_COMMON := $(ROOT_PATH)/Build.common

### Build flags for all targets
export CFLAGS_STD := -std=gnu99 -g -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE \
		-D_REENTRANT -Wall -Wno-unused-parameter -Wno-unused-variable \
		-Wno-format-zero-length -pthread -fPIC -I$(ROOT_PATH)/src/
		 #-Werror \
export LDFLAGS_STD :=
export DEPS_STD    :=

export CFLAGS_LCM  := `pkg-config --cflags lcm`
export LDFLAGS_LCM := `pkg-config --libs lcm`
export DEPS_LCM    :=

MAKE := $(MAKE) --no-print-directory

# more dependencies will be added to all and clean later
all:

clean: clean-targets

.PHONY: deploy
deploy : all
	$(ROOT_PATH)/scripts/push_to_robot.sh

clean-targets:
	@rm -f bin/* lib/*

# This begins the recursive decent crawl over all the Rules.mk files.
# Add additional roots here as necessary.
include $(ROOT_PATH)/Rules.mk
