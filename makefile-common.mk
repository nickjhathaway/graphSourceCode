UNAME_S := $(shell uname -s)
CPP = g++-4.8
CXXFLAGS = -std=c++11 -fopenmp -Wall
LD_FLAGS = 
ifeq ($(UNAME_S),Darwin)
	#CPP = /Users/nickhathaway/source_codes/compilers/bin/clang++
	#CXXFLAGS = -std=c++11 -Wall -stdlib=libstdc++  -I/Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/ -I /Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/x86_64-apple-darwin13.2.0/
	#LD_FLAGS += -L /Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/ -Wl,-rpath,/Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/
	CPP = /usr/bin/clang++
	CXXFLAGS = -std=c++11 -Wall
endif

LOCAL_PATH = $(EXT_PATH)/local

ifeq ($(UNAME_S),Darwin)
	#CXXOPT = -g -Og
	#CXXOPT = -O2 -funroll-loops -DNDEBUG 
    #CXXOPT = -O2 -funroll-loops -DNDEBUG -ffast-math
    CXXOPT = -O2 -funroll-loops -DNDEBUG  
else
	#CXXOPT = -g -gstabs+ -Og
    #CXXOPT = -O2 -march=native -mtune=native -funroll-loops -DNDEBUG --fast-math
    CXXOPT = -O2 -march=native -mtune=native -funroll-loops -DNDEBUG 
endif

#defaults for most progjects
LOCALTOOLS = -I$(LOCAL_PATH)
EXTTOOLS = -I$(EXT_PATH)
SRC = -I./src/
COMLIBS = $(LOCALTOOLS) $(EXTTOOLS) $(SRC)

#debug
CXXDEBUG = -g -gstabs+
#boost
ifeq ($(USE_BOOST),1)
	CXXOPT += -DBOOST_UBLAS_NDEBUG
	COMLIBS += -isystem$(LOCAL_PATH)/boost/include
	LD_FLAGS +=  -Wl,-rpath,$(LOCAL_PATH)/boost/lib \
			-L$(LOCAL_PATH)/boost/lib  \
			-lpthread -lboost_program_options -lboost_system -lboost_thread \
			-lboost_filesystem -lboost_iostreams -lboost_regex -lboost_serialization
endif

#CPPPROGUTILS
ifeq ($(USE_CPPPROGUTILS),1)
	COMLIBS += -I$(LOCAL_PATH)/cppprogutils
endif

#cppcms
ifeq ($(USE_CPPCMS),1)
	COMLIBS += -isystem$(LOCAL_PATH)/cppcms/include
	LD_FLAGS += -Wl,-rpath,$(LOCAL_PATH)/cppcms/lib \
			-L$(LOCAL_PATH)/cppcms/lib  \
			-lcppcms -lbooster
endif

#armadillo
ifeq ($(USE_ARMADILLO),1)
	COMLIBS += -isystem$(LOCAL_PATH)/armadillo/include
	LD_FLAGS += -Wl,-rpath,$(LOCAL_PATH)/armadillo/lib \
			-L$(LOCAL_PATH)/armadillo/lib  \
			-larmadillo
endif

#shark
ifeq ($(USE_SHARK),1)
	COMLIBS += -isystem$(LOCAL_PATH)/shark/include
	LD_FLAGS += -Wl,-rpath,$(LOCAL_PATH)/shark/lib \
		-L$(LOCAL_PATH)/shark/lib  \
		-lshark
endif

#ZI_LIB
ifeq ($(USE_ZI_LIB),1)
	COMLIBS += -I$(LOCAL_PATH)/zi_lib
	#CXXFLAGS += -DZI_USE_OPENMP
	ifeq ($(UNAME_S),Darwin)

	else
		CXXFLAGS += -DZI_USE_OPENMP
    	LD_FLAGS += -lrt
	endif
endif


#bamtools
ifeq ($(USE_BAMTOOLS),1)
	COMLIBS += -I$(LOCAL_PATH)/bamtools/include/bamtools
	LD_FLAGS += -Wl,-rpath,$(LOCAL_PATH)/bamtools/lib/bamtools \
			-L$(LOCAL_PATH)/bamtools/lib/bamtools\
			-lbamtools
endif
#c url library 
ifeq ($(USE_CURL),1)
	LD_FLAGS += -lcurl
endif

#ml_pack
ifeq ($(USE_MLPACK),1)
	ifeq ($(UNAME_S),Darwin)
    	LD_FLAGS += -llapack  -lcblas # non-threaded
	else
   		LD_FLAGS += -llapack -lf77blas -lcblas -latlas # non-threaded
	endif
endif

#qt5
ifeq ($USE_QT5,1)
	ifeq ($(UNAME_S),Darwin)
		LD_FLAGS += -Wl,-rpath,/usr/local/opt/qt5/lib \
	 				-L/usr/local/opt/qt5/lib \
	 				-lQt5UiTools
    	COMLIBS += -I/usr/local/opt/qt5/include
	endif
endif

ifeq ($(USE_R),1)
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
