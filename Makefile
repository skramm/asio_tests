# makefile for platinum_IO
# author: S. Kramm, 2017

COLOR_1=-e "\e[1;33m"
COLOR_2=-e "\e[1;34m"
COLOR_3=-e "\e[1;35m"
COLOR_OFF="\e[0m"

#--------------------------------
# general compiler flags
CFLAGS = -std=c++11 -Wall -O2 -Iinclude
#LDFLAGS += -L/usr/lib/x86_64-linux-gnu/ -lboost_system -lboost_thread -pthread -lboost_iostreams -lboost_serialization
LDFLAGS += -L/usr/lib/x86_64-linux-gnu/ -lboost_system -lboost_thread -pthread -lboost_serialization

# needed, so object files that are inner part of successive pattern rules don't get erased at the end of build
#.PRECIOUS: obj/demo/%.o obj/lib/release/%.o obj/lib/debug/%.o
.SECONDARY:

# disable implicit rules
.SUFFIXES:

# list of targets that are NOT files
.PHONY: all clean cleanall r1.dot

SHELL=/bin/bash

BIN_DIR=bin
SRC_DIR=src
OBJ_DIR=obj


HEADER_FILES = $(wildcard $(SRC_DIR)/*.h*)

#DOT_FILES = $(wildcard *.dot)
#SVG_FILES = $(patsubst %.dot,%.svg,$(DOT_FILES))

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
EXEC_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%,$(SRC_FILES))

# default target
all: $(EXEC_FILES)
	@echo "- Done target $@"

#dot: $(SVG_FILES)
#	@echo "- Done target $@"

# dot, circo, ...
#GRAPHIZ_APP = dot

#%.svg: %.dot
#	$(GRAPHIZ_APP) -Tsvg -Grankdir=LR -Nfontsize=24 $< >$@
#	$(GRAPHIZ_APP) -Tsvg $< >$@


#r2: r2.svg
#	@echo "- Done target $@"


show:
	@echo HEADER_FILES=$(HEADER_FILES)
	@echo SRC_FILES=$(SRC_FILES)
	@echo OBJ_FILES=$(OBJ_FILES)
	@echo EXEC_FILES=$(EXEC_FILES)
	@echo DOT_FILES=$(DOT_FILES)
	@echo SVG_FILES=$(SVG_FILES)


clean:
	-rm $(OBJ_DIR)/*

cleanall: clean
	-rm $(BIN_DIR)/*


# generic compile rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES)
	@echo $(COLOR_2) " - Compiling app file $<." $(COLOR_OFF)
	$(CXX) -o $@ -c $< $(CFLAGS)

# linking
$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	@echo $(COLOR_3) " - Link demo $@." $(COLOR_OFF)
	$(CXX) -o $@ -s $(subst $(BIN_DIR)/,$(OBJ_DIR)/,$@).o  $(LDFLAGS)


