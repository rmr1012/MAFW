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
	$(PYTHONPATH) $(MBEDCLI) $(MBEDARGS) --target NUCLEO_F303ZE --profile develop.json
	mkdir -p $(DISTPATH)
	mkdir -p $(DISTPATH)/DEV/
	cp $(BUILDPATH)/NUCLEO_F303ZE/GCC_ARM-DEVELOP/MAFW.bin $(DISTPATH)/DEV/
	cp $(BUILDPATH)/NUCLEO_F303ZE/GCC_ARM-DEVELOP/MAFW.elf $(DISTPATH)/DEV/
df:
	@echo "making for dev"
	$(PYTHONPATH) $(MBEDCLI) $(MBEDARGS) --target K66F --profile profile.json
	mkdir -p $(DISTPATH)
	mkdir -p $(DISTPATH)/DEV/
	cp $(BUILDPATH)/K66F/GCC_ARM-PROFILE/MAFW.bin $(DISTPATH)/DEV/

deploy:
	-cp $(DISTPATH)/DEV/MAFW.bin /Volumes/NODE_F303ZE
	# cp $(DISTPATH)/DEV/MAFW.bin /Volumes/FRDM-K66FJ
clean:
	rm -rf $(BUILDPATH)