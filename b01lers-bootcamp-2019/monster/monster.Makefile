CC=gcc
CFLAGS=
WFLAGS=-Wall -Wextra

all: monster 

monster: monster.c
	$(CC) $(CFLAGS) $< -o $@ $(WFLAGS)
	rm $<
