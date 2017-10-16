CC=gcc -g -std=gnu99 -fgnu89-inline	
INCLUDE_FLAGS=-Isrc
LIBRARY_FLAGS=-lX11 -lXext -lm

all: clean display_xwindow

display_xwindow: 
	$(CC) $(INCLUDE_FLAGS) -c src/*.c src/*/*.c
	$(CC) -o display_xwindow *.o $(LIBRARY_FLAGS)

run: clean display_xwindow
	./display_xwindow

clean:
	-rm -f *.o
	-rm -f display_xwindow
