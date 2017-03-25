.PHONY: gpio_test gpio_test_clean

gpio_test: common rpi

gpio_test:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/gpio_test -f Build.mk

gpio_test_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/gpio_test -f Build.mk clean

all: gpio_test

clean: gpio_test_clean
