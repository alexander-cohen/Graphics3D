#include "displayer.h"

#define BORDER_COL (BLACK)
#define BG_COL (WHITE)

Display *dis;
Window win;
XEvent evt;
GC gc;

int scr;


int col_arr[HEIGHT * WIDTH];

int main() {
    setbuf(stdout, NULL);
    srand(time(NULL));
    open_window();
    run_line();
}

int open_window() {
    dis = XOpenDisplay(NULL);
    if (dis == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
    scr = DefaultScreen(dis);
    win = XCreateSimpleWindow(
            dis, RootWindow(dis, scr),
            100, 100,
            WIDTH, HEIGHT,
            BORDER_SIZE,
            BORDER_COL, BG_COL);  // happy christmas

    gc = XCreateGC(dis, win, 0, NULL);
    XSelectInput(dis, win, ExposureMask | KeyPressMask);
    XMapWindow(dis, win);
    XRaiseWindow(dis, win);

    g2d_set_context (g2d_create_graphics_context (col_arr, WIDTH, HEIGHT));

    return 0;
}


int run_line ()
{
    struct timespec slptime = {0, 20000000}; // 50 fps
    int frameNum = 0;
    // make a bunch of windows

    printf("Frame generation complete\n");
    while(XPending(dis)) {
        XNextEvent(dis, &evt);
        if(evt.type == Expose)
            break;
        else {

        }
    }
    printf("Got Expose event, moving on\n");
    struct timeval begin, end;
    XImage *img = NULL;
    gettimeofday(&begin, NULL);
    

    while(frameNum < 10000) {
        g2d_fill_bg (CYAN);

        printf("Frame %d\n", frameNum);
        while(XPending(dis)) {
            XNextEvent(dis, &evt);
            if (evt.type == KeyPress)
                break;
            else if(evt.type == Expose) {}
            else {
                printf("this event not handled currently\n");
            }
        }


        
        g2d_set_thickness (2);
        g2d_set_col (RED);
        //g2d_draw_line (40, 40, 100, 100);

        g2d_set_col (YELLOW);
        //g2d_fill_ellipse (256, 256, 100, 25);
        // flat top+flat left
        //g2d_fill_triangle(0,0,100,0,0,100);
        // other way flat top+flat left
        
        for (int i = 0; i < 1000; i++)
        {
            g2d_fill_triangle_boundingbox(100,200,0,200,0,300);
            // //flat bot+flat right
            g2d_fill_triangle_boundingbox(200,0,200,100,100,100);

            //orhter way flat bot + flat right
            g2d_fill_triangle_boundingbox(200,200,100,300,200,300);
            //downright-slanting top, downright-slanting left
            g2d_fill_triangle_boundingbox(200,0,300,10,210,100);
            //downleft-slanting top, downleft-slanting right
            g2d_fill_triangle_boundingbox(300,10,400,0,390,100);
            //all near vertical, diff sides
            g2d_fill_triangle_boundingbox(450,0,460,90,440,40);
            //all near horizontal, diff sides
            g2d_fill_triangle_boundingbox(0,150,100,140,40,160);
            //all near vertical, same side
            g2d_fill_triangle_boundingbox(150,100,140,200,135,140);
            //all near horizontal, same side
            g2d_fill_triangle_boundingbox(200,150,300,140,240,135);
            //all near horizontal, same side, flipped
            g2d_fill_triangle_boundingbox(300,150,400,160,340,165);
        }
        

        int 
        x1 = 30, y1 = 30,
        x2 = 200, y2 = 30,
        x3 = 3, y3 = 200;

        //g2d_fill_triangle (x1, y1, x2, y2, x3, y3);

        g2d_set_col (RED);

        g2d_set_thickness (1);
        //g2d_draw_line (x1, y1, x2, y2);
        //g2d_draw_line (x1, y1, x3, y3);
        //g2d_draw_line (x2, y2, x3, y3);


        // for (int r = 0; r < HEIGHT; r++)
        // {
        //     for (int c = 0; c < WIDTH; c++)
        //     {
        //         printf ("%d", col_arr[r * WIDTH + c] < WHITE);
        //     }
        //     printf ("\n");
        // }

        img = XCreateImage(dis, CopyFromParent, 24, ZPixmap, 0, (char *)col_arr, WIDTH, HEIGHT, 32, 0);
        XPutImage(dis, win, gc, img, 0, 0, 0, 0, WIDTH, HEIGHT);

        frameNum++;
        gettimeofday(&end, NULL);
        float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        printf("Average of %0.2f FPS\n", ((double)frameNum/ (double)nsecs));
    }
    gettimeofday(&end, NULL);
    float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf("10000 loops: %f ms\n", nsecs * 1000);
    printf("Average of %0.2f FPS\n", (10000.0 / (double)nsecs));

    //XCloseDisplay(dis);
    
    return 0;
}

int run_test() {
    struct timespec slptime = {0, 20000000}; // 50 fps
    int frameNum = 0;
    // make a bunch of windows
    int *screens = malloc(sizeof(int) * 100 * WIDTH * HEIGHT);
    int i, j, k;
    for(i = 0; i < 100; i++) {
        for(j = 0; j < HEIGHT; j++) {
            for(k = 0; k < WIDTH; k++) {
                screens[i*HEIGHT*WIDTH+j*WIDTH+k] = rand() % (1 << 24);
            }
        }
    }
    printf("Frame generation complete\n");
    while(XPending(dis)) {
        XNextEvent(dis, &evt);
        if(evt.type == Expose)
            break;
        else {

        }
    }
    printf("Got Expose event, moving on\n");
    char pixBuf[WIDTH * HEIGHT * 3 + 1];
    struct timeval begin, end;
    XImage *img = NULL;
    gettimeofday(&begin, NULL);
    while(frameNum < 10000) {
        printf("Frame %d\n", frameNum);
        while(XPending(dis)) {
            XNextEvent(dis, &evt);
            if (evt.type == KeyPress)
                break;
            else if(evt.type == Expose) {}
            else {
                printf("this event not handled currently\n");
            }
        }
        
        int idx = frameNum % 199 - 99;
        idx = idx < 0 ? -idx : idx;
        int *screen = screens + idx * WIDTH * HEIGHT;
        img = XCreateImage(dis, CopyFromParent, 24, ZPixmap, 0, (char *)screen, WIDTH, HEIGHT, 32, 0);
        XPutImage(dis, win, gc, img, 0, 0, 0, 0, WIDTH, HEIGHT);

        frameNum++;
        gettimeofday(&end, NULL);
        float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        printf("Average of %d FPS\n", (int)(frameNum / nsecs));
    }
    gettimeofday(&end, NULL);
    float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf("10000 loops: %f ms\n", nsecs * 1000);
    printf("Average of %d FPS", (int)(10000 / nsecs));

    XCloseDisplay(dis);
    free(screens);
    return 0;
}