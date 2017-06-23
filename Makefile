CC=gcc -std=gnu99
CFLAGS=-I.

all: displayer.c
	$(CC) displayer.c -I. -o display_xwindow -lX11

run: all
	./display_xwindow
