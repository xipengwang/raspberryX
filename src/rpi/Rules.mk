export CFLAGS_RPI = -I$(ROOT_PATH)/src/
export LDFLAGS_RPI = $(LDFLAGS_COMMON) $(LIB_PATH)/librpi.a -lm
export DEPS_RPI = $(DEPS_COMMON) $(LIB_PATH)/librpi.a

.PHONY: rpi rpi_clean

rpi: common

rpi:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/rpi -f Build.mk

rpi_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/rpi -f Build.mk clean

all: rpi

clean: rpi_clean
