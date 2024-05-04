CC=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -O3

SRC_DIR=.
COMMON_DIR=common
BUILD_DIR=build
BOTS_DIR=../bots

add_build_path=$(addprefix $(BUILD_DIR)/, $(1))

COMMON_SRCS=$(shell find $(COMMON_DIR) -name "*.cpp")
COMMON_OBJS=$(patsubst $(COMMON_DIR)/%.cpp, $(COMMON_DIR)/%.o, $(COMMON_SRCS))

MINMAX_OBJS=minmax.o $(COMMON_OBJS)
MINMAX_OBJS := $(call add_build_path, $(MINMAX_OBJS))

ALL_BOTS=minmax

all: $(ALL_BOTS)

minmax: $(MINMAX_OBJS)
	$(CC) $(MINMAX_OBJS) -o $(BOTS_DIR)/minmax

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(shell mkdir -p $(dir $@))
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf build
