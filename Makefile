ROOT = $(realpath ./)
EXT_PATH=$(realpath external)

USE_CPPITERTOOLS = 1
HATHAWAY = 1

include $(ROOT)/makefile-common.mk
include $(COMPFILE)

UNAME_S := $(shell uname -s)
# from http://stackoverflow.com/a/8654800
HEADERS = $(call rwildcard, src/, *.h) \
	$(call rwildcard, src/, *.hpp)

OBJ_DIR = $(addsuffix Build, $(CXXOUTNAME))
OBJ = $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp, %.o, $(call rwildcard, src/, *.cpp)))
BIN = $(addsuffix $(CXXOUTNAME), bin/)
COMMON = $(CXXFLAGS) $(CXXOPT) $(COMLIBS)

############ main
.PHONY: all
all: $(OBJ_DIR) $(BIN)
	scripts/fixDyLinking_mac.sh bin external

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p bin

$(OBJ_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR)/$(shell dirname $<)
	$(CXX) $(COMMON) -c $< -o $@

$(OBJ_DIR)/%.d: %.cpp
	$(SHELL) -ec '$(CPP) -M  $< | sed '\"s/$*.o/& $@/g'\" > $@'

$(BIN): $(OBJ) $(OBJ_DIR)/src/main.o
	$(CXX) $(COMMON) -o $@ $^ $(LD_FLAGS) 

############ clean
.PHONY: clean
clean:
	@rm -f $(BIN)
	@rm -rf $(OBJ_DIR)
	