EDITOR_NAME := editor
EDITOR_SRC_DIR := $(SRC_DIR)/$(EDITOR_NAME)/src

EDITOR_CPP_SRC := $(shell find $(EDITOR_SRC_DIR) -name "*.cpp")
EDITOR_CPP_OBJ := $(shell echo " $(EDITOR_CPP_SRC:.cpp=.o)" | sed "s| $(SRC_DIR)| $(BUILD_DIR)|g")

.PHONY: all run

$(OUT_DIR)/modules/$(EDITOR_NAME).so: $(EDITOR_CPP_OBJ)
	mkdir -p $(shell dirname $@)
	$(LINK) $(LINK_DEFAULT_FLAGS) -shared $^ -o $@
