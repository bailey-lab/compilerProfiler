CXX = /usr/bin/clang++
CXXOUTNAME = macClangProfilerNoOpt
CXXFLAGS = -std=c++11 -Wall 
CXXFLAGS += 
CXXOPT += -DNDEBUG 
ifneq ($(UNAME_S),Darwin)
	CXXOPT += -march=native -mtune=native 
endif
LD_FLAGS += 
#debug
CXXDEBUG = -g -gstabs+ 
