PYTHONPATH=/Applications/MBEDCLI.app/Contents/Resources/miniconda/bin/python
MBEDCLI=/Applications/MBEDCLI.app/Contents/Resources/miniconda/bin/mbed
MBEDOS=mbed-os
MBEDARGS=compile --toolchain GCC_ARM
BUILDPATH=$(CURDIR)/BUILD
DISTPATH=$(CURDIR)/DIST
PROFILEPATH=$(CURDIR)/profiles
DOCPATH=$(CURDIR)/docs/manual

all: Dev

dev:
	@echo "making for dev"
	$(PYTHONPATH) $(MBEDCLI) $(MBEDARGS) --target NUCLEO_F303ZE --profile profile.json
	mkdir -p $(DISTPATH)
	mkdir -p $(DISTPATH)/DEV/
	cp $(BUILDPATH)/NUCLEO_F303ZE/GCC_ARM-PROFILE/MAFW.bin $(DISTPATH)/DEV/
df:
	@echo "making for dev"
	$(PYTHONPATH) $(MBEDCLI) $(MBEDARGS) --target K66F --profile profile.json
	mkdir -p $(DISTPATH)
	mkdir -p $(DISTPATH)/DEV/
	cp $(BUILDPATH)/K66F/GCC_ARM-PROFILE/MAFW.bin $(DISTPATH)/DEV/

deploy:
	-cp $(DISTPATH)/DEV/MAFW.bin /Volumes/NODE_F303ZE
	cp $(DISTPATH)/DEV/MAFW.bin /Volumes/FRDM-K66FJ
clean:
	rm -rf $(BUILDPATH)
