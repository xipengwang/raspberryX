.PHONY: matrix_test matrix_test_clean

matrix_test: common rpi

matrix_test:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/matrix_test -f Build.mk

matrix_test_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/examples/matrix_test -f Build.mk clean

all: matrix_test

clean: matrix_test_clean
