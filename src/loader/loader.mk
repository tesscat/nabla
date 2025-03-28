LOADER_NAME := loader
LOADER_BASE_DIR := $(SRC_DIR)/$(LOADER_NAME)
LOADER_SRC_DIR := $(LOADER_BASE_DIR)/src
LOADER_HEADERS_DIR := $(LOADER_BASE_DIR)/headers

LOADER_CPP_SRC := $(shell find $(LOADER_SRC_DIR) -name "*.cpp")
LOADER_CPP_OBJ := $(shell echo " $(LOADER_CPP_SRC:.cpp=.o)" | sed "s| $(SRC_DIR)| $(BUILD_DIR)|g")

LOADER_INCLUDE_FLAGS := -Ivendor/tomlplusplus/include -I$(LOADER_HEADERS_DIR)

.PHONY: all run dbg

$(BUILD_DIR)/$(LOADER_NAME)/%.o: $(SRC_DIR)/$(LOADER_NAME)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CPPC) $(CPPC_DEFAULT_FLAGS) $(LOADER_INCLUDE_FLAGS) $^ -c -o $@

$(OUT_DIR)/nabla: $(LOADER_CPP_OBJ)
	mkdir -p $(shell dirname $@)
	$(LINK) $(LINK_DEFAULT_FLAGS) -rdynamic $^ -o $@

all: $(OUT_DIR)/nabla $(foreach module, $(MODULES), $(OUT_DIR)/modules/$(module).so)

run: all
	NABLA_loader__search_path="['./$(OUT_DIR)/modules']" NABLA_loader__modules="['core', 'tui']" $(OUT_DIR)/nabla

dbg: all 
	NABLA_loader__search_path="['./$(OUT_DIR)/modules']" NABLA_loader__modules="['core', 'tui']" lldb $(OUT_DIR)/nabla

