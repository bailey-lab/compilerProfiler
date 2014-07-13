ROOT = $(realpath ./)
EXT_PATH=$(realpath external)
LIB_DIR=$(realpath lib)

USE_CPPITERTOOLS = 1
HATHAWAY = 1

include $(ROOT)/makefile-common.mk
include $(COMPFILE)

UNAME_S := $(shell uname -s)
# from http://stackoverflow.com/a/8654800
HEADERS = $(call rwildcard, src/, *.h) \
	$(call rwildcard, src/, *.hpp)

OBJ_DIR = $(addsuffix Build, $(CXXOUTNAME))
OBJ_DIRSO = $(addsuffix BuildSo, $(CXXOUTNAME))
OBJ = $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp, %.o, $(call rwildcard, src/, *.cpp)))
OBJNOMAIN = $(filter-out $(addsuffix /src/main.o, $(OBJ_DIR)), $(OBJ))

OBJSO = $(addprefix $(OBJ_DIRSO)/, $(patsubst %.cpp, %.o, $(call rwildcard, src/, *.cpp)))
OBJNOMAINSO = $(filter-out $(addsuffix /src/main.o, $(OBJ_DIRSO)), $(OBJSO))

BIN = $(addsuffix $(CXXOUTNAME), bin/)
DYLIB = $(addprefix $(addsuffix $(CXXOUTNAME), $(LIB_DIR)/lib), .dylib)
SOLIB = $(addprefix $(addsuffix $(CXXOUTNAME), $(LIB_DIR)/lib), .so)
COMMON = $(CXXFLAGS) $(CXXOPT) $(COMLIBS)

############ main
.PHONY: all
all: $(OBJ_DIR) $(BIN)
	scripts/fixDyLinking_mac.sh bin external

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p bin
	mkdir -p lib

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

############ shared library
.PHONY: sharedLibrary
sharedLibrary: $(OBJ_DIRSO) $(SOLIB)

$(OBJ_DIRSO):
	mkdir -p $(OBJ_DIRSO)
	mkdir -p lib

$(OBJ_DIRSO)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIRSO)/$(shell dirname $<)
	$(CXX) $(COMMON) -fpic -c $< -o $@

$(OBJ_DIRSO)/%.d: %.cpp
	$(SHELL) -ec '$(CPP) -M  $< | sed '\"s/$*.o/& $@/g'\" > $@'

$(SOLIB): $(OBJNOMAINSO)
	$(CXX) $(COMMON) -shared -o $@ $^ $(LD_FLAGS) 
	
############ dylibLibrary
.PHONY: dylibLibrary
dylibLibrary: $(OBJ_DIR) $(DYLIB)
	
$(DYLIB): $(OBJNOMAIN)
	$(CXX) $(COMMON) -dynamiclib -o $@ $^ $(LD_FLAGS) 
	
	