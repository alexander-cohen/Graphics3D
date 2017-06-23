/* 
 * Xdbe example
 * Vincent Cruz 
 * 
 * To compile this : 
 * gcc -Wall dbe-test.c -I/usr/include/X11/ -lX11 -lXext -lm -o dbetest
 * 
 * You should see a black square turning around the center of the window.
 * Close the window to quit.
 * Use it at your own risk.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/Xdbe.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

Display *display;
Atom  wmProtocols[3];
Atom  wmDeleteWindow;
Atom  wmTakeFocus;

/* Event handling */
Bool process() {
  XEvent  event;
  Bool    run  = True;
  
  if(!XPending(display)) return True;
  
  XNextEvent(display, &event);

  switch(event.type) {
    /* Well this is just a test program, so a lot of event aren't processed */
    case ButtonPress:
    case ButtonRelease:
    case KeyPress:
    case KeyRelease:
    case KeymapNotify:
    case EnterNotify:
    case LeaveNotify:
    case ReparentNotify:
    break;

    case FocusIn:
    case FocusOut:
      switch(event.xfocus.mode) {
        case NotifyNormal:
        case NotifyGrab:
        case NotifyUngrab:
        break;
      }
    break;

    case VisibilityNotify:
    case ConfigureNotify:
    break;

    /* The window received a destroy notification */
    case DestroyNotify:
      run  = False;
    break;
    
    case ClientMessage:
      if (event.xclient.message_type == wmProtocols[0]) {
        /* The user may have pushed the close button. Or more generally */
        /* the WM wants the window to be closed.            */
        if(((Atom)event.xclient.data.l[0]) == wmDeleteWindow) {
          run  = False;
        }
        else if(event.xclient.data.l[0] == wmTakeFocus) {
          /* The WM wants the window to gain focus. */
          XSetInputFocus(display, event.xclient.window, RevertToParent, CurrentTime);
        }
      }
    break;

    default:
    break;
  }

  return run;
}

/* The almighty MAIN */
int main() {
  int           screen;
  Window          root;
  Window          win;
  XSetWindowAttributes  attributes;
  unsigned int      width, height;

  int             major, minor;   /* DBE version */
  XdbeBackBuffer        backBuffer;   /* Back buffer */
  XdbeBackBufferAttributes  *backAttr;    /* Back buffer attributes */
  XdbeSwapInfo        swapInfo;     /* Swap info */
  
  GC            gc;
  XGCValues       gcv;

  struct timeval      start, end;
  long int        elapsed;
  long int        interval;

  int           x, y;
  int           r;
  float         theta, dTheta;

    /* Open display */
  display = XOpenDisplay(NULL);

    /* We'll be using default screen */
  screen = DefaultScreen(display);

    /* The window width and height will be the quarter of the display dimension */
  width  = XDisplayWidth(display,screen) / 4;
  height = XDisplayHeight(display,screen) / 4; 

    /* List of events we want to handle */
  attributes.event_mask = 
    KeyPressMask         | KeyReleaseMask      | KeymapStateMask        | ButtonPressMask          |
    ButtonReleaseMask    | EnterWindowMask     | LeaveWindowMask        | ExposureMask             |
    VisibilityChangeMask | StructureNotifyMask | SubstructureNotifyMask | SubstructureRedirectMask |
    FocusChangeMask;
    
  /* Various window attributes */
  attributes.backing_store   = Always;
  attributes.save_under    = True;
  attributes.override_redirect = False;
  attributes.background_pixel  = WhitePixel(display, screen);

    /* Get defaul root window */
  root = DefaultRootWindow(display);

    /* Create our window */
  win = XCreateWindow( display, root, 0, 0, width, height, 0,
               CopyFromParent, InputOutput, CopyFromParent,
               CWBackingStore|CWOverrideRedirect|CWEventMask|CWSaveUnder|CWBackPixel,
               &attributes );

  /* Set WM protocols to handle (delete window and focus acquisition */
  wmProtocols[0]  =                  XInternAtom(display, "WM_PROTOCOLS", True);
  wmProtocols[1]  = wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
  wmProtocols[2]  = wmTakeFocus    = XInternAtom(display, "WM_TAKE_FOCUS", False);

  XSetWMProtocols(display, win, wmProtocols, 3);

  /* Create gc */
  gcv.foreground = WhitePixel(display, screen);
  gcv.background = BlackPixel(display, screen);
  
  gc = XCreateGC(display, win, GCForeground | GCBackground, &gcv);
  
  /* Get DBE version */
  if(!XdbeQueryExtension(display, &major, &minor)) {
    fprintf(stderr, "Error : unable to fetch Xdbe Version.\n");
    goto end;
  }
  
  printf("Xdbe version %d.%d\n",major,minor); 
      
  /* Allocate back buffer */
  /* From XdbeSwapBuffers man page :                        */
  /*    XdbeUndefined                             */
  /*      The contents of the new back buffer become undefined.  This  may  */
  /*      be  the most efficient action since it allows the implementation  */
  /*      to discard the contents of the buffer if it needs to.       */
  /* ... so we'll have to do the cleanup by hand. And as we didn't specify any  */
  /* background that' ok :)                           */
  backBuffer = XdbeAllocateBackBufferName(display, win, XdbeUndefined);
  
  /* Get back buffer attributes (used for swapping) */
  backAttr = XdbeGetBackBufferAttributes(display, backBuffer);
    swapInfo.swap_window = backAttr->window;
    swapInfo.swap_action = XdbeUndefined;
         
  XFree(backAttr);
         
  /* Map and raise window */
  XMapWindow(display, win);
  XRaiseWindow(display, win);

  /* Some initialization for animation */
  r      = (width > height) ? (height / 4) : (width / 4);
  theta  = 0.0;
  dTheta = M_PI / 1000000.0; /* A loop in 2s */
  
  /* Interval between each frame */
  interval = 1000000 / 50; /* 50 Hz */

  /* Evil loop (will exit when the user close the window) */
  gettimeofday(&start, NULL);
  while(process()) {  
    /* Brutal synchro */
    gettimeofday(&end, NULL);
    elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + end.tv_usec - start.tv_usec;
    if(elapsed >= interval) {
      gettimeofday(&start, NULL);
      
      theta += elapsed * dTheta;
      
      x = (width  / 2) + (r * cos(theta));
      y = (height / 2) + (r * sin(theta));
      
      /* Clear */
      XSetForeground(display, gc, WhitePixel(display, screen));
      XFillRectangle(display, backBuffer, gc, 0, 0, width, height);
      
      /* Draw something */
      XSetForeground(display, gc, BlackPixel(display, screen));
      XFillRectangle(display, backBuffer, gc, x, y, 32, 32);
      
      /* Swap buffers */
      XdbeSwapBuffers(display, &swapInfo, 1);
    }
    /* Sleep a little bit in order to avoid eating all the cpu. */
    usleep(10);
  }

  /* Deallocate back buffer */
  if(!XdbeDeallocateBackBufferName(display, backBuffer)) {
    fprintf(stderr,"Error : unable to deallocate back buffer.\n");
  }
  
end:
  /* Free gc */
  XFreeGC(display, gc);
  /* Destroy window */
  XDestroyWindow(display, win);
  /* Close display */
  XCloseDisplay(display);

  return 0;
}