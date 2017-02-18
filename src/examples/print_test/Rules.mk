.PHONY: print_test print_test_clean

print_test: common rpi

print_test:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/print_test -f Build.mk

print_test_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/print_test -f Build.mk clean

all: print_test

clean: print_test_clean
