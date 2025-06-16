include make/common.mk

.PHONY: setup-linters
setup-linters:
	$(SUDO) apt-get update
	$(SUDO) apt-get -y install clang-format

.PHONY: clang-format
clang-format:
	for ext in h c cpp hpp; do \
		find $(MAIN_DIR) -iname "*.$$ext" -print0 | xargs -0 -r clang-format -i; \
		find $(COMPONENT_DIR) -iname "*.$$ext" -not -path "*/putrl_proto/*" -print0 | xargs -0 -r clang-format -i; \
	done

.PHONY: clang-format-dry
clang-format-dry:
	for ext in h c cpp hpp; do \
		find $(MAIN_DIR) -iname "*.$$ext" -print0 | xargs -0 -r clang-format --dry-run -Werror -i; \
		find $(COMPONENT_DIR) -iname "*.$$ext" -not -path "*/putrl_proto/*" -print0 | xargs -0 -r clang-format --dry-run -Werror -i; \
	done

.PHONY: clang_tidy
clang_tidy:
	for ext in h c cpp hpp; do \
		find $(APP_DIR) -iname "*.$$ext" -print0 | xargs -0 -r clang-tidy -fix; \
	done

.PHONY: lint
lint: clang-format-dry clang-tidy
