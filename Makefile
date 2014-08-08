ROOT = $(realpath ./)
EXT_PATH=$(realpath external)
USE_CPPITERTOOLS = 1
USE_CPPPROGUTILS = 1

include $(ROOT)/makefile-common.mk

COMMON = $(COMMON_OPT)

.PHONY: all
all: main.cpp
	mkdir -p bin
	g++-4.8 -std=c++11  main.cpp $(COMMON) -o bin/fileModAffect $(LD_FLAGS)