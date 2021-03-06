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
#include "pong/pong.h"
#include "pipeline/renderer.h"
#include "datastructures/arrayvec.h"
#include "objects/shapes.h"
#include "prepipeline/vnt.h"
#include "linalg/matrix.h"
#include "transform/transform.h"
#include "pipeline/fragment_shader.h"
#include "colors.h"
#include "objects/wavefront.h"

int open_window();
int run_line();
int run_test();
int run_pong();
int put_frame();
int run_triangle();
int run_avx_test();
int pp_sobelfilter_shader(int *, int, int, int, int);

#define WIDTH (512) 
#define HEIGHT (512)
#define BORDER_SIZE (3)

#endif