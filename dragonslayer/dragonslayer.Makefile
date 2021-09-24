CC=gcc
CFLAGS=
WFLAGS=-Wall -Wextra

all: dragonslayer

dragonslayer: dragonslayer.c
	$(CC) $(CFLAGS) $< -o $@ $(WFLAGS)
	# rm $<
