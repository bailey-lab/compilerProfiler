ROOT = $(realpath ../)
EXT_PATH = $(realpath ../external)
USE_CPPITERTOOLS = 1

include $(ROOT)/makefile-common.mk
include $(COMPFILE)

SRC += -I../src/



OBJ_DIR = $(addsuffix Build, $(CXXOUTNAME))
TESTSRC = $(realpath ./src)
SRCSRC = $(realpath ../src)
OBJ = $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp, %.o, $(call rwildcard, $(TESTSRC), *.cpp))) \
	$(addprefix $(OBJ_DIR)/, $(patsubst %.cpp, %.o, $(call rwildcard, $(SRCSRC)/compro, *.cpp))) 
HEADERS = $(call rwildcard, src/, *.h) \
	$(call rwildcard, src/, *.hpp)

BIN = $(addsuffix $(addsuffix Tester, $(CXXOUTNAME)), bin/)
COMMON = $(CXXFLAGS) $(CXXOPT) $(COMLIBS)

############ main
.PHONY: all
all: $(OBJ_DIR) $(BIN)
	../scripts/fixDyLinking_mac.sh bin ../external

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p bin

$(OBJ_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR)/$(shell dirname $<)
	$(CXX) $(COMMON) -c $< -o $@

$(OBJ_DIR)/%.d: %.cpp
	$(SHELL) -ec '$(CPP) -M  $< | sed '\"s/$*.o/& $@/g'\" > $@'

$(BIN): $(OBJ)
	$(CXX) $(COMMON) -o $@ $^ $(LD_FLAGS) 

############ clean
.PHONY: clean
clean:
	@rm -f $(BIN)
	@rm -rf $(OBJ_DIR)
