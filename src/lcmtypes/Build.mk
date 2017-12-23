LCMTYPES_DIR   := $(realpath $(ROOT_PATH)/lcmtypes)

# Just the list of type names
LCMTYPES       := $(notdir $(shell ls $(LCMTYPES_DIR)/*.lcm))

# The list of .c and .h files that we will generate
LCMTYPES_C     := $(LCMTYPES:%.lcm=%.c)
LCMTYPES_H     := $(LCMTYPES:%.lcm=%.h)
LCMTYPES_O     := $(LCMTYPES:%.lcm=%.o)

%.c %.h : $(LCMTYPES_DIR)/%.lcm
	@lcm-gen -c $< --cinclude lcmtypes/

CFLAGS := $(CFLAGS_STD) $(CFLAGS_LCMTYPES)
LDFLAGS := $(LDFLAGS_STD) $(LDFLAGS_LCMTYPES)

include $(BUILD_COMMON)

all: $(LCMTYPES_C) $(LCMTYPES_H)
	@/bin/true

$(LIB_PATH)/librobotxlcmtypes.a: $(LCMTYPES_H) $(LCMTYPES_O)
	@$(AR) rc $@ $(LCMTYPES_O)

clean:
	@rm -rf *.o *.c *.h $(LIB_PATH)/librobotxlcmtypes.a
