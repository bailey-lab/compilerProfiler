ROOT = $(realpath ./)
EXT_PATH=$(realpath external)
USE_CPPITERTOOLS = 1
#USE_R = 0
#USE_BOOST = 0
USE_ZI_LIB = 1
#USE_BAMTOOLS = 0
#HATHAWAY = 1

include $(ROOT)/makefile-common.mk

UNAME_S := $(shell uname -s)

GXXCPP = g++-4.8
CLANGCPP = clang++

OBJ_DIR_GXX = buildGxx
OBJ_DIR_CLANG = buildClang

# from http://stackoverflow.com/a/8654800
GXXOBJ = $(addprefix $(OBJ_DIR_GXX)/, $(patsubst %.cpp, %.o, $(call rwildcard, src/, *.cpp)))
CLANGOBJ = $(addprefix $(OBJ_DIR_CLANG)/, $(patsubst %.cpp, %.o, $(call rwildcard, src/, *.cpp)))
HEADERS = $(call rwildcard, src/, *.h) \
	$(call rwildcard, src/, *.hpp)


BINGXX = bin/gxxProfiler
BINCLANG = bin/clangProfiler

#GXXCXXFLAGS = -std=c++11 -fopenmp -Wall
GXXCXXFLAGS = -std=c++11 -Wall
ifeq ($(UNAME_S),Darwin)
	#temp for now 
	ifdef HATHAWAY
		GXXCXXFLAGS += -I /Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/ -I /Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/x86_64-apple-darwin13.2.0/ 
	endif
	GXXCXXOPT = -O2 -funroll-loops -DNDEBUG 
else
    GXXCXXOPT = -O2 -march=native -mtune=native -funroll-loops -DNDEBUG 
endif
#CLANGCXXFLAGS = -std=c++11 -fopenmp -Wall
CLANGCXXFLAGS = -std=c++11 -Wall
ifeq ($(UNAME_S),Darwin)
	CLANGCXXFLAGS += -stdlib=libstdc++
	#temp for now
	ifdef HATHAWAY
		CLANGCXXFLAGS += -I /Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/ -I /Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/x86_64-apple-darwin13.2.0/ 
	endif
	
	CLANGCXXOPT = -O2 -funroll-loops -DNDEBUG 
else
	CLANGCXXFLAGS += -stdlib=libstdc++
   	CLANGCXXOPT = -O2 -march=native -mtune=native -funroll-loops -DNDEBUG 
endif

GXXCOMMON = $(GXXCXXFLAGS) $(GXXCXXOPT) $(COMLIBS)
CLANGCOMMON = $(CLANGCXXFLAGS) $(CLANGCXXOPT) $(COMLIBS)

ifdef HATHAWAY
	LD_FLAGS += -L /Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/ -Wl,-rpath,/Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/
endif	


############ main
.PHONY: all
all: $(OBJ_DIR_GXX) $(OBJ_DIR_CLANG) $(BINGXX) $(BINCLANG)
	scripts/fixDyLinking_mac.sh bin external

############ main
.PHONY: dev
dev: $(OBJ_DIR_GXX) $(OBJ_DIR_CLANG) $(BINGXX) $(BINCLANG)
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
	