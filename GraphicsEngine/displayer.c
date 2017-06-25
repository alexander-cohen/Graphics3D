#include "displayer.h"

#define BORDER_COL (BLACK)
#define BG_COL (WHITE)

Display *dis;
Window win;
XEvent evt;
GC gc;

int scr;
XImage *img = NULL;

g2d_context *my_g2d_context;


int col_arr[HEIGHT * WIDTH];

int main() {
    setbuf(stdout, NULL);
    srand(time(NULL));

    open_window();
    //run_line();
    //run_pong();
    //run_triangle();
    run_avx_test ();
}

int run_avx_test() 
{
    g2d_set_col (BLACK);
    g2d_fill_triangle_boundingbox_avx (1, 1, 100, 100, 400, 1);
    put_frame();
}

int run_pong ()
{
    struct timeval begin, end;
    
    int frameNum = 0;

    pong_init(dis);
    int target_fps = 20;
    int nanos_per_frame = NANO_PER_SECOND / target_fps;

    while (true)
    {
        gettimeofday(&begin, NULL);
        pong_run_frame();
        frameNum++;

        gettimeofday(&end, NULL);
        float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);

        //printf("FPS: %d, %0.2f\n", (int)(nsecs * 1000), 1.0 / nsecs);
        ui64 nanos_to_sleep = nanos_per_frame - (nsecs * NANO_PER_SECOND);
        nanosleep ((const struct timespec[]){{0, nanos_to_sleep}}, NULL);

        gettimeofday(&end, NULL);
        nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        printf("FPS: %lld, %d, %0.2f\n", nanos_to_sleep, (int)(nsecs * 1000), 1.0 / nsecs);
    }
}

int put_frame ()
{
    img = XCreateImage(dis, CopyFromParent, 24, ZPixmap, 0, (char *)col_arr, WIDTH, HEIGHT, 32, 0);
    XPutImage(dis, win, gc, img, 0, 0, 0, 0, WIDTH, HEIGHT);
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

    my_g2d_context = g2d_create_graphics_context (col_arr, WIDTH, HEIGHT);
    g2d_set_context (my_g2d_context);

    while(XPending(dis)) {
        XNextEvent(dis, &evt);
        if(evt.type == Expose)
            break;
        else {

        }
    }
    printf("Got Expose event, moving on\n");


    XSelectInput(dis, win, StructureNotifyMask | SubstructureRedirectMask | ResizeRedirectMask | KeyPressMask | KeyReleaseMask);
    XGrabKeyboard(dis, win, False, GrabModeAsync, GrabModeAsync, CurrentTime);


    return 0;
}


int run_triangle ()
{
    struct timeval begin, end;
    
    int frameNum = 0;
    
    bool wait_for_key = false;
    int tri_box_size = 5;
    int ntrials = 1000000;

    while(frameNum < 10000) {
        g2d_fill_bg (CYAN);

        //printf("Frame %d\n", frameNum);
        bool keyfound = false;
        while (!keyfound && wait_for_key)
        {
            while(XPending(dis)) {
                XNextEvent(dis, &evt);
                if (evt.type == KeyPress)
                {
                    //printf ("Key pressed: %d, %d\n", evt.xkey.state, evt.xkey.keycode);
                    keyfound = true;
                }
                else if (evt.type == KeyRelease)
                {
                    //printf ("Key released: %d, %d\n", evt.xkey.state, evt.xkey.keycode);
                }
            }
        }
        
        gettimeofday(&begin, NULL);
        g2d_set_col (YELLOW);

        for (int i = 0; i < ntrials; i++)
        {   
            int midx = rand() % 512;
            int midy = rand() % 512;

            int color = rand() % (0xFFFFFF);


            int x1 = midx + (rand() % tri_box_size) - tri_box_size / 2;
            int y1 = midy + (rand() % tri_box_size) - tri_box_size / 2;

            int x2 = midx + (rand() % tri_box_size) - tri_box_size / 2;
            int y2 = midy + (rand() % tri_box_size) - tri_box_size / 2;

            int x3 = midx + (rand() % tri_box_size) - tri_box_size / 2;
            int y3 = midy + (rand() % tri_box_size) - tri_box_size / 2;

            
            // x1 = 266;
            // y1 = 292; 
            // x2 = 328;
            // y2 = 219; 
            // x3 = 286; 
            // y3 = 232;


            g2d_set_col (color);
            g2d_fill_triangle (x1, y1, x2, y2, x3, y3);

            // g2d_set_col (0xFF0000);
            // g2d_fill_ellipse (x1, y1, 5, 5);

            // g2d_set_col (0x00FF00);
            // g2d_fill_ellipse (x2, y2, 5, 5);

            // g2d_set_col (0x0000FF);
            // g2d_fill_ellipse (x3, y3, 5, 5);


            // printf ("created triangle #%d: (%d, %d); (%d, %d); (%d, %d)\n", 
            //     i, x1, y1, x2, y2, x3, y3);
        }

        printf ("\ntime to create %d triangles within box of %d:\n", ntrials, tri_box_size);
        gettimeofday(&end, NULL);
        double nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        printf("total time for edge scan: %0.2f ms\n", nsecs * 1000);
        printf ("avg triangle for time edge scan: %0.2f ns\n", nsecs / ntrials * 1000000000);
        
        gettimeofday(&begin, NULL);
        g2d_set_col (YELLOW);
        for (int i = 0; i < ntrials; i++)
        {
            int midx = rand() % 512;
            int midy = rand() % 512;

            int color = rand() % (0x00FFFF);


            int x1 = midx + (rand() % tri_box_size) - tri_box_size / 2;
            int y1 = midy + (rand() % tri_box_size) - tri_box_size / 2;

            int x2 = midx + (rand() % tri_box_size) - tri_box_size / 2;
            int y2 = midy + (rand() % tri_box_size) - tri_box_size / 2;

            int x3 = midx + (rand() % tri_box_size) - tri_box_size / 2;
            int y3 = midy + (rand() % tri_box_size) - tri_box_size / 2;

            g2d_set_col (color);
            g2d_fill_triangle_boundingbox (x1, y1, x2, y2, x3, y3);
        }

        gettimeofday(&end, NULL);
        nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        printf("total time for bounding box: %0.2f ms\n", nsecs * 1000);
        printf ("avg triangle for time bounding box: %0.2f ns\n", nsecs / ntrials * 1000000000);

        put_frame();

        
        ntrials = ntrials * tri_box_size / (tri_box_size + 5);
        tri_box_size += 5;
        if (tri_box_size > 500)
        {
            break;
        }

        frameNum++;
        
        
        //printf("Average of %0.2f FPS\n", ((double)frameNum/ (double)nsecs));
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