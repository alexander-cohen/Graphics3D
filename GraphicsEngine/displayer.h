#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/extensions/Xdbe.h>
#include "color_util.h"

void paint_arr (int *arr);

const int 
WIDTH = 512, 
HEIGHT = 512,
BORDER_SIZE = 3;

