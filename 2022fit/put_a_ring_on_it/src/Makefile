CC=gcc
CFLAGS=-Wall -Wpedantic -Werror -Wno-deprecated-declarations
LDFLAGS=-Wl,-z,now -Wl,-z,relro


all: put_a_ring_on_it 
debug: put_a_ring_on_it_dbg

put_a_ring_on_it: put_a_ring_on_it.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

put_a_ring_on_it_dbg: put_a_ring_on_it.c hexdump.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm put_a_ring_on_it put_a_ring_on_it_dbg
