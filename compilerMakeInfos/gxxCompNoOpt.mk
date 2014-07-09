CXX = g++
CXXOUTNAME = gxxProfilerNoOpt
CXXFLAGS = -std=c++11 -Wall
CXXOPT += -DNDEBUG 
ifneq ($(UNAME_S),Darwin)
	CXXOPT += -march=native -mtune=native 
endif
LD_FLAGS += -L /Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/ -Wl,-rpath,/Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/
#debug
CXXDEBUG = -g -gstabs+ 
