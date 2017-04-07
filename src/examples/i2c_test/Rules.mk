.PHONY: i2c_test i2c_test_clean

i2c_test: common rpi

i2c_test:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/i2c_test -f Build.mk

i2c_test_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/i2c_test -f Build.mk clean

all: i2c_test

clean: i2c_test_clean
