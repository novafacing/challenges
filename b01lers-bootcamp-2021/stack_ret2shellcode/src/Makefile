CC=gcc
CXX=
CFLAGS=-fno-stack-protector -z execstack -fPIE -fPIC -ggdb
CXXFLAGS=
LDFLAGS=-Wl,-z,now -Wl,-z,relro

all: stack_ret2shellcode

stack_ret2shellcode: stack_ret2shellcode.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
