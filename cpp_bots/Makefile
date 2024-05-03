CC=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -O3

BOTS_DIR=../bots

ALL_BOTS=minmax
MINMAX_OBJS=minmax.o bot.o

all: $(ALL_BOTS)

minmax: $(MINMAX_OBJS)
	$(CC) $(MINMAX_OBJS) -o $(BOTS_DIR)/minmax

%.o: %.cpp %.hpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o
