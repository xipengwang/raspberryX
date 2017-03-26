SRCS = $(shell ls *.c)
OBJS = $(SRCS:%.c=%.o)
HEADERS =

CFLAGS := $(CFLAGS_STD) $(CFLAGS_COMMON) $(CFLAGS_RPI) -I..
LDFLAGS := $(LDFLAGS_STD) $(LDFLGS_COMMON) $(LDFLAGS_RPI)
DEPS := $(DEPS_STD) $(DEPS_COMMON) $(DEPS_RPI)
HDEPS := $(HEADERS)

include $(BUILD_COMMON)

all: $(LIB_PATH)/libmfrc522.a
	@/bin/true

$(LIB_PATH)/libmfrc522.a: $(OBJS) $(DEPS)
	@$(AR) rc $@ $^

clean:
	@rm -rf *.o $(LIB_PATH)/libmfrc522.a
