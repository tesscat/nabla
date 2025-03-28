TUI_NAME := tui
TUI_SRC_DIR := $(SRC_DIR)/$(TUI_NAME)/src

TUI_CPP_SRC := $(shell find $(TUI_SRC_DIR) -name "*.cpp")
TUI_CPP_OBJ := $(shell echo " $(TUI_CPP_SRC:.cpp=.o)" | sed "s| $(SRC_DIR)| $(BUILD_DIR)|g")

.PHONY: all run

$(OUT_DIR)/modules/$(TUI_NAME).so: $(TUI_CPP_OBJ)
	mkdir -p $(shell dirname $@)
	$(LINK) $(LINK_DEFAULT_FLAGS) -shared $^ -o $@
