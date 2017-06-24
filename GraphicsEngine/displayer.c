#include "displayer.h"
#include "image.h"

const int 
BORDER_COL = BLACK,
BG_COL = WHITE;

Display *dis;
Window win;
XEvent evt;
GC gc;

int scr;

buffer_info info;
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

    info.pixels = &col_arr[0];
    info.width = WIDTH;
    info.height = HEIGHT;

    return 0;
}

int run_line ()
{
    struct timespec slptime = {0, 20000000}; // 50 fps
    int frameNum = 0;
    // make a bunch of windows

    for (int i = 0; i < HEIGHT * WIDTH; i++)
    {
        col_arr[i] = WHITE;    
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
        
        Vec2 p1 = {.x = 2, .y = 5};
        Vec2 p2 = {.x = 353, .y = 107};
        draw_line (&info, p1, p2, BLACK, false);

        img = XCreateImage(dis, CopyFromParent, 24, ZPixmap, 0, &col_arr[0], WIDTH, HEIGHT, 32, 0);
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
        img = XCreateImage(dis, CopyFromParent, 24, ZPixmap, 0, screen, WIDTH, HEIGHT, 32, 0);
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