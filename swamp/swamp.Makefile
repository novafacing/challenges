CC=gcc

all: swamp

swamp: swamp.c
	$(CC) -o $@ $<
