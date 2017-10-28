CC=gcc -g -std=gnu99 -fgnu89-inline	
INCLUDE_FLAGS=-Isrc
LIBRARY_FLAGS=-lX11 -lXext -lm -lprofiler

all: clean speed display_xwindow

debug:
	$(CC) -Og $(INCLUDE_FLAGS) -c src/*.c src/*/*.c
	$(CC) -Og -o display_xwindow *.o $(LIBRARY_FLAGS)

speed:
	$(CC) -O3 $(INCLUDE_FLAGS) -c src/*.c src/*/*.c
	$(CC) -O3 -o display_xwindow *.o $(LIBRARY_FLAGS)

display_xwindow: 
	$(CC) $(INCLUDE_FLAGS) -c src/*.c src/*/*.c
	$(CC) -o display_xwindow *.o $(LIBRARY_FLAGS)

run: clean speed display_xwindow
	./display_xwindow

rundebug: clean debug display_xwindow
	./display_xwindow

clean:
	-rm -f *.o
	-rm -f display_xwindow
