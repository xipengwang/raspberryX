.PHONY: car_control car_control_clean

car_control: common rpi lcmtypes

car_control:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/car-control -f Build.mk

car_control_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/car-control -f Build.mk clean

all: car_control

clean: car_control_clean
