export CFLAGS_COMMON = -I$(ROOT_PATH)/src/
export LDFLAGS_COMMON = $(LIB_PATH)/libcommon.a -lm
export DEPS_COMMON = $(LIB_PATH)/libcommon.a

.PHONY: common common_clean

common:

common:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/common -f Build.mk

common_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/common -f Build.mk clean

all: common

clean: common_clean
