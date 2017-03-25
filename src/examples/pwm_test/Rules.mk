.PHONY: pwm_test pwm_test_clean

pwm_test: common rpi

pwm_test:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/pwm_test -f Build.mk

pwm_test_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/pwm_test -f Build.mk clean

all: pwm_test

clean: pwm_test_clean
