CC=gcc
CFLAGS=-fexceptions
WFLAGS=-Wall -Wextra
OFLAGS=-O0
LFLAGS=-lgmp -lbsd

all: shrek

shrek: shrek.c
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $< $(WFLAGS) $(LFLAGS)
	strip $@ --strip-all 
