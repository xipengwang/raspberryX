export CFLAGS_LCMTYPES  = $(CFLAGS_LCM)
export LDFLAGS_LCMTYPES = $(LIB_PATH)/librobotxlcmtypes.a $(LDFLAGS_LCM)
export DEPS_LCMTYPES = $(LIB_PATH)/librobotxlcmtypes.a $(DEPS_LCM)

.PHONY: lcmtypes lcmtypes_clean

lcmtypes:

lcmtypes:
	@echo $(CFLAGS_LCM)
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/lcmtypes -f Build.mk

lcmtypes_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/lcmtypes -f Build.mk clean

all: lcmtypes

clean: lcmtypes_clean
