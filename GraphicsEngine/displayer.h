#ifndef DISPLAYER_H
#define DISPLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/extensions/Xdbe.h>

#include "helper.h"
#include "graphics2d/graphics2d.h"
#include "pong.h"
#include "renderer.h"
#include "allav.h"

int open_window();
int run_line();
int run_test();
int run_pong();
int put_frame();
int run_triangle();
int run_avx_test();

#define WIDTH (512) 
#define HEIGHT (512)
#define BORDER_SIZE (3)

#endif