CC=gcc
CFLAGS=-fPIE -fstack-protector-all -D_FORTIFY_SOURCE=2 -ggdb

all: stack_variable_overflow

stack_variable_overflow: stack_variable_overflow.c
	$(CC) $(CFLAGS) -o $@ $^
