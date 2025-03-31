MAIN_NAME := main
MAIN_SRC_DIR := $(SRC_DIR)/$(MAIN_NAME)/src

MAIN_CPP_SRC := $(shell find $(MAIN_SRC_DIR) -name "*.cpp")
MAIN_CPP_OBJ := $(shell echo " $(MAIN_CPP_SRC:.cpp=.o)" | sed "s| $(SRC_DIR)| $(BUILD_DIR)|g")

.PHONY: all run

$(OUT_DIR)/modules/$(MAIN_NAME).so: $(MAIN_CPP_OBJ)
	mkdir -p $(shell dirname $@)
	$(LINK) $(LINK_DEFAULT_FLAGS) -shared $^ -o $@
