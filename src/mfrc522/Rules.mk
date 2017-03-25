export CFLAGS_MFRC522 = -I$(ROOT_PATH)/src/
export LDFLAGS_MFRC522 = $(LDFLAGS_COMMON) $(LDFLAGS_RPI) $(LIB_PATH)/libmfrc522.a -lm
export DEPS_MFRC522 = $(DEPS_COMMON) $(DEPS_RPI) $(LIB_PATH)/libmfrc522.a

.PHONY: mfrc522 mfrc522_clean

mfrc522: common rpi

mfrc522:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/mfrc522 -f Build.mk

mfrc522_clean:
	@echo $@
	@$(MAKE) -C $(ROOT_PATH)/src/mfrc522 -f Build.mk clean

all: mfrc522

clean: mfrc522_clean
