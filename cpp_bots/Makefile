CC=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -O3

SRC_DIR=.
BUILD_DIR=build
BOTS_DIR=../bots

full_obj_name=$(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(1)))

ALL_BOTS=minmax

MINMAX_OBJS=minmax common/bot
MINMAX_OBJS := $(call full_obj_name, $(MINMAX_OBJS))

all: $(ALL_BOTS)

minmax: $(MINMAX_OBJS)
	$(CC) $(MINMAX_OBJS) -o $(BOTS_DIR)/minmax

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf build
