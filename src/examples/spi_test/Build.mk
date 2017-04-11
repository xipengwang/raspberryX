CFLAGS := $(CFLAGS_STD) $(CFLAGS_COMMON) $(CFLAGS_RPI)
LDFLAGS := $(LDFLAGS_STD) $(LDFLAGS_COMMON) $(LDFLAGS_RPI)
DEPS := $(DEPS_STD) $(DEPS_COMMON) $(DEPS_RPI)

include $(BUILD_COMMON)


all: $(BIN_PATH)/mfrc522-test $(BIN_PATH)/spi-test
	@/bin/true


$(BIN_PATH)/mfrc522-test: mfrc522_test.o $(DEPS) $(DEPS_MFRC522)
	@$(LD) -o $@ $^ $(LDFLAGS) $(LDFLAGS_MFRC522)

$(BIN_PATH)/spi-test: spi_test.o $(DEPS)
	@$(LD) -o $@ $^ $(LDFLAGS)

clean:
	@rm -rf *.o $(BIN_PATH)/mfrc522-test $(BIN_PATH)/spi-test
