CORE_NAME := core
CORE_SRC_DIR := $(SRC_DIR)/$(CORE_NAME)/src

CORE_CPP_SRC := $(shell find $(CORE_SRC_DIR) -name "*.cpp")
CORE_CPP_OBJ := $(shell echo " $(CORE_CPP_SRC:.cpp=.o)" | sed "s| $(SRC_DIR)| $(BUILD_DIR)|g")

.PHONY: all run

$(OUT_DIR)/modules/$(CORE_NAME).so: $(CORE_CPP_OBJ)
	mkdir -p $(shell dirname $@)
	$(LINK) $(LINK_DEFAULT_FLAGS) -shared $^ -o $@
