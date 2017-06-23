#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include "color_util.h"

void paint_arr (int *arr);

const int 
WIDTH = 512, 
HEIGHT = 512,
BORDER_SIZE = 3;

int run();
int rgbToHex(int, int, int);
clearWindow(Display *d, Window w);
writeWindow(Display *d, Window w, int screen, int *pixels, int width, int height);
