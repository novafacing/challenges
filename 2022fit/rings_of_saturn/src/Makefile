CC=gcc
CFLAGS=-Wall -Wpedantic -Werror -Wno-deprecated-declarations -no-pie
LDFLAGS=


all: rings_of_saturn 
debug: rings_of_saturn_dbg

rings_of_saturn: rings_of_saturn.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

rings_of_saturn_dbg: rings_of_saturn.c hexdump.c
	$(CC) $(CFLAGS) $(LDFLAGS) -DDEBUG=1 -o $@ $^

clean:
	rm rings_of_saturn rings_of_saturn_dbg
