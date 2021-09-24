CC=gcc
CPP=g++

all: shorty bosswave

shorty: shorty.c
	$(CC) -o $@ $< encode.c
	rm $<
	rm encode.c
	rm encode.h

bosswave: bosswave.cpp
	$(CPP) -o $@ $<
	rm $<
