include make/espidf.mk
include make/linters.mk
include make/proto.mk

.PHONY: setup_usb
setup_usb:
	$(SUDO) cp 99-usb-serial.rules /etc/udev/rules.d/
	$(SUDO) service udev restart
	$(SUDO) chmod a+rw $(PORT)

.PHONY: add_utility
add_utility:
	git submodule add -f https://github.com/franciszekjanicki/utility.git ${UTILITY_DIR}

.PHONY: remove_utility
remove_utility:
	git submodule deinit -f ${UTILITY_DIR}
	git rm -rf ${UTILITY_DIR}
	rm -rf ${UTILITY_DIR}
	rm -rf .git/modules/components/utility

.PHONY: add_esp32_utility
add_esp32_utility:
	git submodule add -f https://github.com/franciszekjanicki/esp32_utility.git ${ESP32_UTILITY_DIR}

.PHONY: remove_esp32_utility
remove_esp32_utility:
	git submodule deinit -f ${ESP32_UTILITY_DIR}
	git rm -rf ${ESP32_UTILITY_DIR}
	rm -rf ${ESP32_UTILITY_DIR}
	rm -rf .git/modules/components/esp32_utility

.PHONY: setup
setup: setup_linters setup_proto setup_usb

.PHONY: all
all: 
	clear && make build && make erase_flash_monitor