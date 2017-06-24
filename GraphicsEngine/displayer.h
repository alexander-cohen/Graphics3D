#ifndef _DISPLAYER_H
#define _DISPLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/extensions/Xdbe.h>
#include "helper.h"
#include "graphics2d.h"
#include "vector.h"

int open_window();
int run_line ();
int run_test();

const int 
WIDTH = 512, 
HEIGHT = 512,
BORDER_SIZE = 3;

#endif