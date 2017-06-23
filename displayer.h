#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

int run();
int rgbToHex(int, int, int);
clearWindow(Display *d, Window w);
writeWindow(Display *d, Window w, int screen, int *pixels, int width, int height);