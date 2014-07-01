UNAME_S := $(shell uname -s)
CPP = g++-4.8
CXXFLAGS = -std=c++11 -fopenmp -Wall
ifeq ($(UNAME_S),Darwin)
	CPP = clang++
	CXXFLAGS = -std=c++11 -Wall    
endif



LOCAL_PATH = $(EXT_PATH)/local

ifeq ($(UNAME_S),Darwin)
	CXXOPT = -O2 -funroll-loops -DNDEBUG 
    #CXXOPT = -O2 -funroll-loops -DNDEBUG -ffast-math
    #CXXOPT = -O3 -funroll-loops -DNDEBUG  
else
    #CXXOPT = -O2 -march=native -mtune=native -funroll-loops -DNDEBUG --fast-math
    CXXOPT = -O2 -march=native -mtune=native -funroll-loops -DNDEBUG 
endif
LD_FLAGS = 
#defaults for most progjects
LOCALTOOLS = -I$(LOCAL_PATH)
EXTTOOLS = -I$(EXT_PATH)
SRC = -I./src/
COMLIBS = $(LOCALTOOLS) $(EXTTOOLS) $(SRC)

#debug
CXXDEBUG = -g -gstabs+
#boost
ifdef USE_BOOST
	CXXOPT += -DBOOST_UBLAS_NDEBUG
	COMLIBS += -isystem$(LOCAL_PATH)/boost/include
	LD_FLAGS +=  -Wl,-rpath,$(LOCAL_PATH)/boost/lib \
			-L$(LOCAL_PATH)/boost/lib  \
			-lpthread -lboost_program_options -lboost_system -lboost_thread \
			-lboost_filesystem -lboost_iostreams -lboost_regex -lboost_serialization
endif

#cppcms
ifdef USE_CPPCMS
	COMLIBS += -isystem$(LOCAL_PATH)/cppcms/include
	LD_FLAGS += -Wl,-rpath,$(LOCAL_PATH)/cppcms/lib \
			-L$(LOCAL_PATH)/cppcms/lib  \
			-lcppcms -lbooster
endif

#armadillo
ifdef USE_ARMADILLO
	COMLIBS += -isystem$(LOCAL_PATH)/armadillo/include
	LD_FLAGS += -Wl,-rpath,$(LOCAL_PATH)/armadillo/lib \
			-L$(LOCAL_PATH)/armadillo/lib  \
			-larmadillo
endif

#shark
#SHARK = -isystem$(LOCAL_PATH)/shark/include
#LD_FLAGS += -Wl,-rpath,$(LOCAL_PATH)/shark/lib \
#	-L$(LOCAL_PATH)/shark/lib  \
#	-lshark

#ZI_LIB
ifdef USE_ZI_LIB
	COMLIBS += -I$(LOCAL_PATH)/zi_lib
	#CXXFLAGS += -DZI_USE_OPENMP
	ifeq ($(UNAME_S),Darwin)

	else
		CXXFLAGS += -DZI_USE_OPENMP
    	LD_FLAGS += -lrt
	endif
endif


#bamtools
ifdef USE_BAMTOOLS
	COMLIBS += -I$(LOCAL_PATH)/bamtools/include/bamtools
	LD_FLAGS += -Wl,-rpath,$(LOCAL_PATH)/bamtools/lib/bamtools \
			-L$(LOCAL_PATH)/bamtools/lib/bamtools\
			-lbamtools
endif
#c url library 
ifdef USE_CURL
	LD_FLAGS += -lcurl
endif
#ml_pack
ifdef USE_MLPACK

	ifeq ($(UNAME_S),Darwin)
    	LD_FLAGS += -llapack  -lcblas # non-threaded
	else
   		LD_FLAGS += -llapack -lf77blas -lcblas -latlas # non-threaded
	endif
endif

#qt5
ifdef USE_QT5

	ifeq ($(UNAME_S),Darwin)
		LD_FLAGS += -Wl,-rpath,/usr/local/opt/qt5/lib \
	 				-L/usr/local/opt/qt5/lib \
	 				-lQt5UiTools
    	COMLIBS += -I/usr/local/opt/qt5/include
	endif
endif

ifdef USE_R
	include $(ROOT)/r-makefile-common.mk
endif

ifeq ($(UNAME_S),Darwin)
    #for dylib path fixing in macs, this gets rid of the name_size limit
    LD_FLAGS += -headerpad_max_install_names
endif

COMMON_OPT = $(CXXFLAGS) $(CXXOPT) $(COMLIBS)
COMMON_DEBUG = $(CXXFLAGS) $(CXXDEBUG) $(COMLIBS)

# from http://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
