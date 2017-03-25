.PHONY: uart_test uart_test_clean

uart_test: common rpi

uart_test:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/uart_test -f Build.mk

uart_test_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/uart_test -f Build.mk clean

all: uart_test

clean: uart_test_clean
