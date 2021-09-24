CC=gcc
CFLAGS=-g -Wno-stringop-overflow -O0

all: fawncdn

fawncdn: fawncdn.c
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm fawncdn
