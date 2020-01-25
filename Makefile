# export PATH := /Users/dennisren/arm-none-eabi/bin:$(PATH)
PYTHONPATH=/Applications/MBEDCLI.app/Contents/Resources/miniconda/bin/python
MBEDCLI=/Applications/MBEDCLI.app/Contents/Resources/miniconda/bin/mbed
MBEDOS=mbed-os
MBEDARGS=compile --toolchain GCC_ARM
BUILDPATH=$(CURDIR)/BUILD
DISTPATH=$(CURDIR)/DIST
PROFILEPATH=$(CURDIR)/profiles
DOCPATH=$(CURDIR)/docs/manual
.PHONY: dev deploy
all: dev deploy

dev:
	@echo "making for dev"
	$(PYTHONPATH) $(MBEDCLI) $(MBEDARGS) --target NUCLEO_L432KC --profile dev.json
	mkdir -p $(DISTPATH)
	mkdir -p $(DISTPATH)/DEV/
	cp $(BUILDPATH)/NUCLEO_L432KC/GCC_ARM-DEV/MAFW_Sun.bin $(DISTPATH)/DEV/
	cp $(BUILDPATH)/NUCLEO_L432KC/GCC_ARM-DEV/MAFW_Sun.elf $(DISTPATH)/DEV/

deploy:
	-cp $(DISTPATH)/DEV/MAFW_Sun.bin /Volumes/NODE_L432KC
	# cp $(DISTPATH)/DEV/MAFW_Sun.bin /Volumes/FRDM-K66FJ
clean:
	rm -rf $(BUILDPATH)
