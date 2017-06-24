#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int writePpm(char *outfile, char *outbuf, int *pixels, int width, int height) {
	if(width > 99999 || height > 99999) {
		printf("Width or height too great in writePpm\n");
		1/0; // x d
	}
	char header[20];
	snprintf(header, 20, "P6 %d %d 255\n", width, height);
	int fd = open(outfile, O_WRONLY | O_CREAT, 0644);
	if(write(fd, header, 20) == -1)
		perror("In writePpm header write");
	int i;
	formattedRGB(outbuf, pixels, width, height);
	if(write(fd, outbuf, width * height * 3 + 1) == -1) {
		perror("In writePpm looped write");
	}
	close(fd);
}

char *formattedRGB(char *dest, int *pixels, int width, int height) {
	int i;
	char *orig = dest;
	char *cpix = (char *) pixels;
	for(i = 0; i < width * height; i++, pixels += 4, dest += 3) {
		dest[0] = cpix[0];
		dest[1] = cpix[1];
		dest[2] = cpix[2];
	}
	dest[0] = 0;
	return orig;
}