CC=gcc -w -g
INC=-I.
CARGS=

all: clean arrayveco graphics2do linalgo thiso
	$(CC) *.o ${INC} ${CARGS} -o display_xwindow -L/opt/X11/lib -lX11 -lXext

run: all
	./display_xwindow

thiso:
	$(CC) -c *.c ${INC} ${CARGS}

arrayveco:
	$(CC) -c arrayvec/*.c ${INC} ${CARGS}

graphics2do:
	$(CC) -c graphics2d/*.c ${INC} ${CARGS}

linalgo:
	$(CC) -c linalg/*.c ${INC} ${CARGS}

clean:
	-rm *.o
	-rm display_xwindow
