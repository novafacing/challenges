CC=g++
CFLAGS=-fexceptions
WFLAGS=-Wall -Wextra

all: allstar

allstar: allstar.cpp
	$(CC) $(CFLAGS) -o $@ $< $(WFLAGS)
	strip $@ --strip-all 
