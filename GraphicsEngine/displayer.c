#include "displayer.h"

const int 
BORDER_COL = BLACK,
BG_COL = WHITE;

int col_arr[WIDTH][HEIGHT];

int main(void) {
    Display *d;
    Window w;
    XEvent e;
    GC gc;
    char *msg = "Hello, World!";
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
    int i = 0;
    while(1) {
        printf("Frame %d\n", i);
        while(XPending(d))
            XNextEvent(d, &e);
        if (e.type == KeyPress)
            break;
        else if(e.type == Expose) {}
        else {
            printf("this event not handled currently\n");
        }
        XSetForeground(d, gc, col);
        XFillRectangle(d, w, gc, 0, 0, 512, 512);
        col += colorInc;
        if(col < minCol || col > maxCol) {
            col -= 2 * colorInc;
            colorInc = -colorInc;
        }
        nanosleep(&slptime, NULL);
        i++;
    }

    XCloseDisplay(d);
    return 0;
}
