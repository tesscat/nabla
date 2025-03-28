PROJECT := nabla
BASE_DIR := $(shell pwd)
# TODO: redo link-specific flags
# ARCH := x86_64
BUILD_DIR := build
OUT_DIR := out
SRC_DIR := src
HEADERS_DIR := $(SRC_DIR)/headers
COMPONENTS := loader core tui
MODULES := core tui

INC_FLAGS := -I$(HEADERS_DIR) -Ivendor/utfcpp/source -Ivendor/tomlplusplus/include 

LINK := clang++
CPPC := clang++

CPPC_DEFAULT_FLAGS := $(INC_FLAGS) -g -std=c++23 -Wall -Wextra -fPIC
LINK_DEFAULT_FLAGS := -std=c++23 -Wall -Wextra

clean:
	rm -rf $(OUT_DIR) $(BUILD_DIR)

# Default rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CPPC) $(CPPC_DEFAULT_FLAGS) $^ -c -o $@

COMPONENT_MKCONFIGS := $(foreach component, $(COMPONENTS), $(SRC_DIR)/$(component)/$(component).mk)

include $(COMPONENT_MKCONFIGS)


