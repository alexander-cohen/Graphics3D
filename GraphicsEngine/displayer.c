#include "displayer.h"
#include "image.h"

const int 
BORDER_COL = BLACK,
BG_COL = WHITE;


main() {
    setbuf(stdout, NULL);
    srand(time(NULL));
    run();
}

int run() {
    Display *dis;
    Window win;
    XEvent evt;
    GC gc;
    XGCValues gcv;
    XdbeBackBuffer backBuffer;   /* Back buffer */
    XdbeBackBufferAttributes *backAttr;    /* Back buffer attributes */
    XdbeSwapInfo swapInfo;     /* Swap info */
    char msg[] = "Hello, World!";
    int scr;
    int col_arr[WIDTH][HEIGHT];

    dis = XOpenDisplay(NULL);
    if (dis == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    scr = DefaultScreen(dis);
    gc = DefaultGC(dis, scr);
    win = XCreateSimpleWindow(
            dis, RootWindow(dis, scr),
            100, 100,
            WIDTH, HEIGHT,
            BORDER_SIZE,
            BORDER_COL, BG_COL);  // happy christmas

    /* Allocate back buffer */
    /* From XdbeSwapBuffers man page :                        */
    /*    XdbeUndefined                             */
    /*      The contents of the new back buffer become undefined.  This  may  */
    /*      be  the most efficient action since it allows the implementation  */
    /*      to discard the contents of the buffer if it needs to.       */
    /* ... so we'll have to do the cleanup by hand. And as we didn't specify any  */
    /* background that' ok :)                           */
    backBuffer = XdbeAllocateBackBufferName(dis, win, XdbeUndefined);

    /* Get back buffer attributes (used for swapping) */
    backAttr = XdbeGetBackBufferAttributes(dis, backBuffer);
    swapInfo.swap_window = backAttr->window;
    swapInfo.swap_action = XdbeUndefined;
         
    XFree(backAttr);
    
    XSelectInput(dis, win, ExposureMask | KeyPressMask);
    XMapWindow(dis, win);
    XRaiseWindow(dis, win);
    int colorInc = rgbToHex(1, -1, -1);
    int col = rgbToHex(127, 127, 127);
    int minCol = rgbToHex(1, 1, 1); // boundaries 1 off to prevent overflow errors with dumb gradient
    int maxCol = rgbToHex(254, 254, 254);
    struct timespec slptime = {0, 20000000}; // 50 fps
    int frameNum = 0;
    // make a bunch of windows
    int *screens = malloc(sizeof(int) * 100 * WIDTH * HEIGHT);
    int i, j, k;
    for(i = 0; i < 100*WIDTH*HEIGHT; i++) {
        screens[i] = rand();
    }
    printf("randomizing done");
    while(XPending(dis)) {
        XNextEvent(dis, &evt);
        if(evt.type == Expose)
            break;
        else {

        }
    }
    printf("Got Expose event, moving on");
    char pixBuf[WIDTH * HEIGHT * 3 + 1];
    struct timeval begin, end, bW, eW;
    while(1) {
        gettimeofday(&begin, NULL);
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
        
        XSetForeground(dis, gc, col);
        //XFillRectangle(dis, backBuffer, gc, 0, 0, WIDTH, HEIGHT);
        //clock_t t = clock();
        int *screen = screens + frameNum * WIDTH * HEIGHT;
        gettimeofday(&bW, NULL);
        formattedRGB(pixBuf, screen, WIDTH, HEIGHT);
        gettimeofday(&eW, NULL);
        printf("writePpm: %f ms\n", (eW.tv_sec - bW.tv_sec) * 1000 + ((eW.tv_usec - bW.tv_usec)/1000.0));
        XImage *img = XCreateImage(dis, DirectColor, 24, ZPixmap, 1, screen, WIDTH, HEIGHT, 8, 4);
        //XImage *img = XCreateImage(dis, TrueColor, 24, ZPixmap, 0, pixBuf, WIDTH, HEIGHT, 32, 0);

        if(img == NULL) {
            printf("img is null\n");
        }
        else {
            XPutImage(dis, win, gc, img, 1, 1, 1, 1, 100, 100);
            XDestroyImage(img);
        }

        //XCreateImage(dis, DirectColor, 24, ZPixmap, 0, pixBuf, WIDTH, HEIGHT, 0, 0);

        //writeWindow(dis, backBuffer, scr, screens + frameNum * WIDTH * HEIGHT);
        //XdbeSwapBuffers(dis, &swapInfo, 1);
        col += colorInc;
        if(col < minCol || col > maxCol) {
            col -= 2 * colorInc;
            colorInc = -colorInc;
        }
        gettimeofday(&end, NULL);
        printf("Display loop (excl. nanosleep): %f ms\n", (end.tv_sec - begin.tv_sec) * 1000 + ((end.tv_usec - begin.tv_usec)/1000.0));
        nanosleep(&slptime, NULL);
        frameNum++;
    }

    XCloseDisplay(dis);
    free(screens);
    return 0;
}


clearWindow(Display *d, Window w) {
    XClearWindow(d, w);
}

writeWindow(Display *d, Window w, int screen, int *pixels) {
    int *og = pixels;
    GC gc = DefaultGC(d, screen);
    int x, y;
    for(y = 0; y < HEIGHT; y++) {
        for(x = 0; x < WIDTH; x++, pixels++) {
            //printf("%d\n", (*pixels)/1000000000*1000000000);
            XSetForeground(d, gc, (*pixels));
            XDrawPoint(d, w, gc, x, y);

        }
    }
}
