SRCS = $(shell ls *.c)
OBJS = $(SRCS:%.c=%.o)
TARGET = $(BIN_PATH)/gpio-test

CFLAGS := $(CFLAGS_STD) $(CFLAGS_COMMON) $(CFLAGS_RPI)
LDFLAGS := $(LDFLAGS_STD) $(LDFLAGS_COMMON) $(LDFLAGS_RPI)
DEPS := $(DEPS_STD) $(DEPS_COMMON) $(DEPS_RPI)

include $(BUILD_COMMON)


all: $(TARGET)
	@/bin/true

$(TARGET): $(OBJS) $(DEPS)
	@$(LD) -o $@ $^ $(LDFLAGS)

clean:
	@rm -rf *.o $(TARGET)
