CC = gcc
CXX = g++
CXXOUTNAME = gxxProfiler
CXXFLAGS = -std=c++11 -Wall
CXXOPT += -O2 -DOPTLEVEL=\"O2\" -funroll-loops -DUNROLL_LOOPS -DNDEBUG  
ifneq ($(UNAME_S),Darwin)
	CXXOPT += -march=native -mtune=native 
endif
LD_FLAGS += -L /Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/ -Wl,-rpath,/Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/
#debug
CXXDEBUG = -g -gstabs+ 
INSTALL_DIR=tempInstallTest

USE_CPPITERTOOLS = 1
USE_CPPPROGUTILS = 1

