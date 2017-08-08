CC=gcc -fgnu89-inline
INC=-I.
CARGS=-g

all: clean graphics2do linalgo thiso pipelineo datastructureso pongo objectso prepipelineo transformo
	$(CC) *.o ${INC} -o display_xwindow -lX11 -lXext -lm ${CARGS}

run: all
	./display_xwindow

thiso:
	$(CC) -c *.c ${INC} ${CARGS}

pipelineo:
	$(CC) -c pipeline/*.c ${INC} ${CARGS}

datastructureso:
	$(CC) -c datastructures/*.c ${INC} ${CARGS}

graphics2do:
	$(CC) -c graphics2d/*.c ${INC} ${CARGS}

linalgo:
	$(CC) -c linalg/*.c ${INC} ${CARGS}

pongo:
	$(CC) -c pong/*.c ${INC} ${CARGS}

objectso:
	$(CC) -c objects/*.c ${INC} ${CARGS}

prepipelineo:
	$(CC) -c prepipeline/*.c ${INC} ${CARGS}

transformo:
	$(CC) -c transform/*.c ${INC} ${CARGS}

clean:
	-rm *.o
	-rm display_xwindow
