CC=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -O3

SRC_DIR=.
COMMON_DIR=common
BUILD_DIR=build
BOTS_DIR=../bots

add_build_path=$(addprefix $(BUILD_DIR)/, $(1))

# TODO: add separate targets from COMMON (engine, bot, eval, ...)
COMMON_SRCS=$(shell find $(COMMON_DIR) -name "*.cpp")
COMMON_OBJS=$(patsubst %.cpp, %.o, $(COMMON_SRCS))

MINMAX_OBJS=minmax.o $(COMMON_OBJS)
MINMAX_OBJS := $(call add_build_path, $(MINMAX_OBJS))

MCTS_SRCS=$(shell find mcts -name "*.cpp")
MCTS_OBJS=$(patsubst %.cpp, %.o, $(MCTS_SRCS)) $(COMMON_OBJS)
MCTS_OBJS := $(call add_build_path, $(MCTS_OBJS))

ALL_BOTS=minmax mcts

all: $(ALL_BOTS)

minmax: $(MINMAX_OBJS)
	$(CC) $(MINMAX_OBJS) -o $(BOTS_DIR)/minmax

mcts: $(MCTS_OBJS)
	$(CC) $(MCTS_OBJS) -o $(BOTS_DIR)/mcts

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(shell mkdir -p $(dir $@))
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf build
