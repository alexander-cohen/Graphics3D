CC=gcc -g
CFLAGS=-I.

all: displayer.c
	$(CC) displayer.c -I. -o display_xwindow -lX11 -lXext

run: all
	./display_xwindow
