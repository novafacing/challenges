CC=gcc
CFLAGS=-O3 -fPIE -fstack-protector-all -D_FORTIFY_SOURCE=2 
LDFLAGS=-Wl,-z,now -Wl,-z,relro

all: guardian

guardian: guardian.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)