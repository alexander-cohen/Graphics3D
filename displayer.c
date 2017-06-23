#include "displayer.h"

const int 
BORDER_COL = BLACK,
BG_COL = WHITE;

int col_arr[WIDTH][HEIGHT];

main() {
    setbuf(stdout, NULL);
    srand(time(NULL));
    run();
}

int run() {
    Display *d;
    Window w;
    XEvent e;
    GC gc;
    char msg[] = "Hello, World!";
    int s;

    d = XOpenDisplay(NULL);
    if (d == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    s = DefaultScreen(d);
    gc = DefaultGC(d, s);
    w = XCreateSimpleWindow(
            d, RootWindow(d, s),
            100, 100,
            WIDTH, HEIGHT,
            BORDER_SIZE,
            BORDER_COL, BG_COL);  // happy christmas

    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);
    int colorInc = rgbToHex(1, -1, -1);
    int col = rgbToHex(127, 127, 127);
    int minCol = rgbToHex(1, 1, 1); // boundaries 1 off to prevent overflow errors with dumb gradient
    int maxCol = rgbToHex(254, 254, 254);
    struct timespec slptime = {0, 20000000}; // 50 fps
    int frameNum = 0;
    // make a bunch of windows
    int *screens = malloc(sizeof(int) * 100 * 512 * 512);
    int i, j, k;
    for(i = 0; i < 100*512*512; i++) {
        screens[i] = rand();
    }
    printf("randomizing done");
    while(XPending(d)) {
        XNextEvent(d, &e);
        if(e.type == Expose)
            break;
        else {

        }
    }
    printf("Got Expose event, moving on");
    while(1) {
        printf("Frame %d\n", frameNum);
        while(XPending(d)) {
            XNextEvent(d, &e);
            if (e.type == KeyPress)
                break;
            else if(e.type == Expose) {}
            else {
                printf("this event not handled currently\n");
            }
        }
        
        XSetForeground(d, gc, col);
        XFillRectangle(d, w, gc, 0, 0, 512, 512);
        writeWindow(d, w, s, screens + frameNum * 512 * 512, 512, 512);
        col += colorInc;
        if(col < minCol || col > maxCol) {
            col -= 2 * colorInc;
            colorInc = -colorInc;
        }
        nanosleep(&slptime, NULL);
        frameNum++;
    }

    XCloseDisplay(d);
    free(pixels);
    return 0;
}

int rgbToHex(int r, int g, int b) {
    return (r << 16) + (g << 8) + b;
}

clearWindow(Display *d, Window w) {
    XClearWindow(d, w);
}

writeWindow(Display *d, Window w, int screen, int *pixels, int width, int height) {
    GC gc = DefaultGC(d, screen);
    int x, y;
    for(y = 0; y < height; y++) {
        for(x = 0; x < width; x++) {
            XSetForeground(d, gc, pixels[y*width+x]);
            XDrawPoint(d, w, gc, x, y);
        }
    }
}