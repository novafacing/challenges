CC=gcc
CFLAGS=-s -O0

all: artform

artform: artform.c
	$(CC) -o $@ $< $(CFLAGS)