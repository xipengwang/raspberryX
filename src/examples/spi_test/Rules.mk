.PHONY: spi_test spi_test_clean

spi_test: common rpi mfrc522

spi_test:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/spi_test -f Build.mk

spi_test_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/spi_test -f Build.mk clean

all: spi_test

clean: spi_test_clean
