CC=gcc
CFLAGS=-I.

all: displayer.c
	$(CC) -lX11 displayer.c -I. -o display_xwindow