SRCS = $(shell ls *.c)
OBJS = $(SRCS:%.c=%.o)
HEADERS =

CFLAGS := $(CFLAGS_STD) -I..
LDFLAGS := $(LDFLAGS_STD) $(LDFLGS_COMMON)
DEPS := $(DEPS_STD) $(DEPS_COMMON)
HDEPS := $(HEADERS)

include $(BUILD_COMMON)

all: $(LIB_PATH)/librpi.a
	@/bin/true

$(LIB_PATH)/librpi.a: $(OBJS) $(DEPS)
	@$(AR) rc $@ $^

clean:
	@rm -rf *.o $(LIB_PATH)/librpi.a
