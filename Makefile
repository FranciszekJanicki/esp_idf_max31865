include make/espidf.mk
include make/linters.mk
include make/proto.mk

.PHONY: setup_usb
setup_usb:
	$(SUDO) cp 99-usb-serial.rules /etc/udev/rules.d/
	$(SUDO) service udev restart
	$(SUDO) chmod a+rw $(PORT)

.PHONY: setup
setup: setup_linters setup_proto setup_usb

.PHONY: all
all: 
	clear && make build && make erase_flash_monitor