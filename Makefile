ROOT = $(realpath ./)
EXT_PATH=$(realpath external)
USE_CPPITERTOOLS = 1
#USE_R = 0
#USE_BOOST = 0
#USE_ZI_LIB = 1
#USE_BAMTOOLS = 0
#HATHAWAY = 1

include $(ROOT)/makefile-common.mk

UNAME_S := $(shell uname -s)
# from http://stackoverflow.com/a/8654800
HEADERS = $(call rwildcard, src/, *.h) \
	$(call rwildcard, src/, *.hpp)



GXXCPP = g++-4.8
OBJ_DIR_GXX = buildGxx
GXXOBJ = $(addprefix $(OBJ_DIR_GXX)/, $(patsubst %.cpp, %.o, $(call rwildcard, src/, *.cpp)))
BINGXX = bin/gxxProfiler
#GXXCXXFLAGS = -std=c++11 -fopenmp -Wall
GXXCXXFLAGS = -std=c++11 -Wall
GXXCXXOPT += -O2 -DOPTLEV2 -funroll-loops -DUNROLL_LOOPS -DNDEBUG 
ifeq ($(UNAME_S),Darwin)
	ifdef HATHAWAY
		GXXCXXFLAGS += -I /Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/ -I /Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/x86_64-apple-darwin13.2.0/ 
	endif
else
    GXXCXXOPT += -march=native -mtune=native 
endif

GXXCOMMON = $(GXXCXXFLAGS) $(GXXCXXOPT) $(COMLIBS)


#CLANGCPP = /usr/bin/clang++
CLANGCPP = clang++
OBJ_DIR_CLANG = buildClang
CLANGOBJ = $(addprefix $(OBJ_DIR_CLANG)/, $(patsubst %.cpp, %.o, $(call rwildcard, src/, *.cpp)))
BINCLANG = bin/clangProfiler
#CLANGCXXFLAGS = -std=c++11 -fopenmp -Wall
CLANGCXXFLAGS = -std=c++11 -Wall -stdlib=libstdc++
CLANGCXXOPT = -O2 -DOPTLEV2 -funroll-loops -DUNROLL_LOOPS -DNDEBUG
ifeq ($(UNAME_S),Darwin)
	ifdef HATHAWAY
		CLANGCXXFLAGS += -I /Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/ -I /Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/x86_64-apple-darwin13.2.0/ 
	endif
else
   	CLANGCXXOPT += -O2 -march=native -mtune=native
endif
CLANGCOMMON = $(CLANGCXXFLAGS) $(CLANGCXXOPT) $(COMLIBS)

ifdef HATHAWAY
	LD_FLAGS += -L /Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/ -Wl,-rpath,/Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/
endif	

############ main
.PHONY: all
all: $(OBJ_DIR_GXX) $(OBJ_DIR_CLANG) $(BINGXX) $(BINCLANG)
	scripts/fixDyLinking_mac.sh bin external

$(OBJ_DIR_GXX):
	mkdir -p $(OBJ_DIR_GXX)
	mkdir -p bin

$(OBJ_DIR_GXX)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR_GXX)/$(shell dirname $<)
	$(GXXCPP) $(GXXCOMMON) -c $< -o $@

$(OBJ_DIR_GXX)/%.d: %.cpp
	$(SHELL) -ec '$(GXXCPP) -M  $< | sed '\"s/$*.o/& $@/g'\" > $@'
	

$(OBJ_DIR_CLANG):
	mkdir -p $(OBJ_DIR_CLANG)
	mkdir -p bin

$(OBJ_DIR_CLANG)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR_CLANG)/$(shell dirname $<)
	$(CLANGCPP) $(CLANGCOMMON) -c $< -o $@

$(OBJ_DIR_CLANG)/%.d: %.cpp
	$(SHELL) -ec '$(CLANGCPP) -M  $< | sed '\"s/$*.o/& $@/g'\" > $@'

$(BINGXX): $(GXXOBJ) buildGxx/src/main.o
	$(GXXCPP) -o $@ $^ $(LD_FLAGS)

$(BINCLANG): $(CLANGOBJ) buildClang/src/main.o
	$(CLANGCPP) $(CLANGCOMMON) -o $@ $^ $(LD_FLAGS)
############ clean
.PHONY: clean
clean:
	@rm -f $(BINGXX) $(BINCLANG)
	@rm -rf $(OBJ_DIR_GXX) $(OBJ_DIR_CLANG) 
	