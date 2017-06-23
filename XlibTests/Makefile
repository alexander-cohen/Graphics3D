CC=gcc
CFLAGS=-I.

all: recopy.c
	$(CC) recopy.c -I. -o recopy_buffer -lX11 -lXext -lm

run: all
	./recopy_buffer
