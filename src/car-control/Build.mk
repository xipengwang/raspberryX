SRCS = $(shell ls *.c)
OBJS = $(SRCS:%.c=%.o)
TARGET = $(BIN_PATH)/car-control

CFLAGS := $(CFLAGS_STD) $(CFLAGS_COMMON) $(CFLAGS_RPI) $(CFLAGS_LCMTYPES)
LDFLAGS := $(LDFLAGS_STD) $(LDFLAGS_COMMON) $(LDFLAGS_RPI) $(LDFLAGS_LCMTYPES)
DEPS := $(DEPS_STD) $(DEPS_COMMON) $(DEPS_RPI)

include $(BUILD_COMMON)


all: $(TARGET)
	@/usr/bin/true

$(TARGET): $(OBJS) $(DEPS)
	@$(LD) -o $@ $^ $(LDFLAGS)

clean:
	@rm -rf *.o $(TARGET)
