CXX = clang++

SRC_DIR := src
OBJ_DIR := build

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

EXA := $(wildcard examples/*/*.cpp)

CPPFLAGS := -Iinclude -std=c++11 -Isrc
CFLAGS   := -Wall# some warnings about bad code

all: library

$(OBJ): $(OBJ_DIR)/%.o :$(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $< $(CPPFLAGS) -fPIC

library: $(OBJ)
	$(CXX) $(CFLAGS) -fPIC -shared -o libaxis_control.so $(OBJ)

install: library
	cp libaxis_control.so /usr/lib
	chmod 0755 /usr/lib/libaxis_control.so
	mkdir -p /usr/local/include/axis_control/
	cp include/* /usr/local/include/axis_control/

examples: library #	: $(EXA)
	$(CXX) $(CPPFLAGS) -g -o a.out examples/test_axis/test_axis.cpp -L. -laxis_control

.PHONY: list
list:
	@LC_ALL=C $(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$'