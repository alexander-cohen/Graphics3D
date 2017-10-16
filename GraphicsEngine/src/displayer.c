#include "displayer.h"
#define BORDER_COL (BLACK)
#define BG_COL (WHITE)

Display *dis;
Window win;
XEvent evt;
GC gc;

int scr;
int *max_iarr = NULL;
XImage *img = NULL;

g2d_context *my_g2d_context;

int *col_arr; // [HEIGHT * WIDTH] __attribute__((aligned(32)));

/*int run_avx_test() 
{
    g2d_set_col (BLACK);
    g2d_fill_triangle_boundingbox_avx (1, 1, 100, 100, 400, 1);
    put_frame();
}
*/


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
    return 0;
}

int open_window() {
    col_arr = malloc(WIDTH * HEIGHT * sizeof(int));
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

        //printf ("\ntime to create %d triangles within box of %d:\n", ntrials, tri_box_size);
        gettimeofday(&end, NULL);
        double nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        printf("%0.2f,", nsecs * 1000);
        //printf ("%0.2f,", nsecs / ntrials * 1000000000);
        
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

            g2d_set_col (color);
            g2d_fill_triangle_boundingbox (x1, y1, x2, y2, x3, y3);

            // g2d_set_col (0xFF0000);
            // g2d_fill_ellipse (x1, y1, 5, 5);

            // g2d_set_col (0x00FF00);
            // g2d_fill_ellipse (x2, y2, 5, 5);

            // g2d_set_col (0x0000FF);
            // g2d_fill_ellipse (x3, y3, 5, 5);
        }

        gettimeofday(&end, NULL);
        nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        printf("%0.2f\n", nsecs * 1000);
        //printf ("%0.2f,", nsecs / ntrials * 1000000000);

        put_frame();

        
        ntrials = ntrials * tri_box_size / (tri_box_size + 5);
        tri_box_size += 5;
        if (tri_box_size > 250)
        {
            break;
        }

        frameNum++;
        //printf("Average of %0.2f FPS\n", ((double)frameNum/ (double)nsecs));
    }
    gettimeofday(&end, NULL);
    float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf("\n10000 loops: %f ms\n", nsecs * 1000);
    printf("Average of %0.2f FPS\n", (10000.0 / (double)nsecs));

    //XCloseDisplay(dis);
    
    return 0;
}

int run_test() {
    //struct timespec slptime = {0, 20000000}; // 50 fps
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
    //char pixBuf[WIDTH * HEIGHT * 3 + 1];
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

int run_tri_test() {

    //struct timespec slptime = {0, 20000000}; // 50 fps
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
        //g2d_fill_triangle_boundingbox(0,0,100,0,0,100);
        // other way flat top+flat left
        //g2d_fill_triangle_boundingbox(100,200,0,200,0,300);
        //flat bot+flat right
        //g2d_fill_triangle_boundingbox(200,0,200,100,100,100);
        //orhter way flat bot + flat right
        //g2d_fill_triangle_boundingbox(200,200,100,300,200,300);
        //downright-slanting top, downright-slanting left
        //g2d_fill_triangle_boundingbox(200,0,300,10,210,100);
        //downleft-slanting top, downleft-slanting right
        //g2d_fill_triangle_boundingbox(300,10,400,0,390,100);
        //all near vertical, diff sides
        //g2d_fill_triangle_boundingbox(450,0,460,90,440,40);
        //all near horizontal, diff sides
        //g2d_fill_triangle_boundingbox(0,150,100,140,40,160);
        //all near vertical, same side
        //g2d_fill_triangle_boundingbox(150,100,140,200,135,140);
        //all near horizontal, same side
        //g2d_fill_triangle_boundingbox(200,150,300,140,240,135);
        //all near horizontal, same side, flipped
        //g2d_fill_triangle_boundingbox(300,150,400,160,340,165);
        g2d_fill_triangle_boundingbox(0,0,511,100,200,511);
        // int 
        // x1 = 30, y1 = 30,
        // x2 = 200, y2 = 40,
        // x3 = 100, y3 = 200;

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
        printf("Average of %d FPS\n", (int)(frameNum / nsecs));
    }
    gettimeofday(&end, NULL);
    float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf("10000 loops: %f ms\n", nsecs * 1000);
    printf("Average of %d FPS", (int)(10000 / nsecs));

    //XCloseDisplay(dis);
    
    return 0;
}

/* broke all this trash

int run_render_test() {
    
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
    arrayvec *tris = av_create (2, sizeof (triangle)),
        *materials = av_create (2, sizeof (materials));
    triangle *t1 = malloc(sizeof(triangle)),
        *t2 = malloc(sizeof(triangle));
    material *m1 = malloc(sizeof(material)),
        *m2 = malloc(sizeof(material));

    t1->hasn = true;
    t1->hast = true;
    t1->p1 = (Vec3) {0, 0, 300};
    t1->p2 = (Vec3) {500, 200, 100};
    t1->p3 = (Vec3) {300, 500, 100};
    t1->n1 = (Vec3) {0, 0, 0};
    t1->n2 = (Vec3) {0, 0, 0};
    t1->n3 = (Vec3) {0, 0, 0};
    t1->t1 = (Vec2) {0, 0};
    t1->t2 = (Vec2) {0, 0};
    t1->t3 = (Vec2) {0, 0};
    t1->mat = 0;
    
    t2->hasn = true;
    t2->hast = true;
    t2->p1 = (Vec3) {500, 200, 300};
    t2->p2 = (Vec3) {0, 500, 0};
    t2->p3 = (Vec3) {300, 0, 100};
    t2->n1 = (Vec3) {0, 0, 0};
    t2->n2 = (Vec3) {0, 0, 0};
    t2->n3 = (Vec3) {0, 0, 0};
    t2->t1 = (Vec2) {0, 0};
    t2->t2 = (Vec2) {0, 0};
    t2->t3 = (Vec2) {0, 0};
    t2->mat = 1;

    m1->color = CYAN;
    m2->color = RED;

    av_append(tris, t1, true);
    av_append(tris, t2, true);
    av_append(materials, m1, false);
    av_append(materials, m2, false);

    t1 = tris->data;
    t2 = t1 + sizeof(triangle);
    while(frameNum < 10000) {
        //g2d_fill_bg (CYAN);

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

        col_arr = render(tris, materials);

        img = XCreateImage(dis, CopyFromParent, 24, ZPixmap, 0, (char *)col_arr, WIDTH, HEIGHT, 32, 0);
        XPutImage(dis, win, gc, img, 0, 0, 0, 0, WIDTH, HEIGHT);
        t1->p1.z += 1;
        t1->p2.z -= 1;
        t1->p3.z -= 1;
        //t1->p1.x -= 1;
        //t1->p1.y -= 1;
        //t2->p2.y -= 1;
        //t2->p2.x += 1;
        //t2->p3.x += 1;
        //t2->p3.y += 1;
        t2->p1.z += 1;
        t2->p2.z -= 1;
        t2->p3.z += 1;
        //t2->p2.y -= 1;
        //t2->p1.x -= 1;

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

int run_tetra() {

    struct timespec slptime = {0, 20000000}; // 50 fps
    int frameNum = 0;
    // make a bunch of windows

    printf("Frame generation complete\n");
    while (XPending(dis)) {
        XNextEvent(dis, &evt);
        if (evt.type == Expose)
            break;
        else {

        }
    }
    printf("Got Expose event, moving on\n");
    struct timeval begin, end;
    XImage *img = NULL;
    gettimeofday(&begin, NULL);
    Vec3 p1 = {400, 400, 400};
    Vec3 p2 = {400, 100, 100};
    Vec3 p3 = {100, 400, 100};
    Vec3 p4 = {100, 100, 400};

    arrayvec *tris = av_create(2, sizeof(triangle)),
            *materials = av_create(2, sizeof(materials));
    triangle *t1 = malloc(sizeof(triangle)),
            *t2 = malloc(sizeof(triangle)),
            *t3 = malloc(sizeof(triangle)),
            *t4 = malloc(sizeof(triangle));
    material *m1 = malloc(sizeof(material)),
            *m2 = malloc(sizeof(material));

    t1->hasn = true;
    t1->hast = true;
    t1->p1 = p1;
    t1->p2 = p2;
    t1->p3 = p3;
    t1->n1 = (Vec3) {0, 0, 0};
    t1->n2 = (Vec3) {0, 0, 0};
    t1->n3 = (Vec3) {0, 0, 0};
    t1->t1 = (Vec2) {0, 0};
    t1->t2 = (Vec2) {0, 0};
    t1->t3 = (Vec2) {0, 0};
    t1->mat = 0;

    *t4 = *t3 = *t2 = *t1;

    t2->p1 = p1;
    t2->p2 = p4;
    t2->p3 = p2;

    t3->p1 = p2;
    t3->p2 = p4;
    t3->p3 = p3;

    t4->p1 = p1;
    t4->p2 = p3;
    t4->p3 = p4;

    m1->color = CYAN;
    m2->color = RED;

    av_append(tris, t1, true);
    av_append(tris, t2, true);
    av_append(tris, t3, true);
    av_append(tris, t4, true);
    av_append(materials, m1, false);
    av_append(materials, m2, false);

    t1 = tris->data;
    t2 = t1 + sizeof(triangle);
    t3 = t2 + sizeof(triangle);
    t4 = t3 + sizeof(triangle);
    while (frameNum < 10000) {
        //g2d_fill_bg (CYAN);

        printf("Frame %d\n", frameNum);
        while (XPending(dis)) {
            XNextEvent(dis, &evt);
            if (evt.type == KeyPress)
                break;
            else if (evt.type == Expose) {}
            else {
                printf("this event not handled currently\n");
            }
        }

        col_arr = render(tris, materials);

        img = XCreateImage(dis, CopyFromParent, 24, ZPixmap, 0, (char *) col_arr, WIDTH, HEIGHT, 32, 0);
        XPutImage(dis, win, gc, img, 0, 0, 0, 0, WIDTH, HEIGHT);

        printf("t1 p1 x: %f\n", t1->p1.x);

        nanosleep(&slptime, NULL);
        frameNum++;
        gettimeofday(&end, NULL);
        float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec) / 1000000.0);
        printf("Average of %d FPS\n", (int) (frameNum / nsecs));
    }
    gettimeofday(&end, NULL);
    float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec) / 1000000.0);
    printf("10000 loops: %f ms\n", nsecs * 1000);
    printf("Average of %d FPS", (int) (10000 / nsecs));

    //XCloseDisplay(dis);

    return 0;
} */

double dipow(double base, unsigned int exp) {
    double res = 1;
    while(exp) {
        if(exp & 1)
            res *= base;
        exp>>=1;
        base *= base;
    }
    return res;
} 

int phong_shader_x(Vec3 pos, Vec3 n, Vec2 t, environment env, material mat) { // example shader, copied from my old python implementation
    Vec3 v = env.view;
    arrayvec *lights = env.lights;
    Vec3 PV = vec3sub(v, pos);
    double _pvn = vec3norm(PV);
    vec3idiv(PV, _pvn);
    Vec3 col = {0,0,0};
    int i;
    for(i = 0; i < lights->used_len; i++) {
        light l = av_get_value(lights, i, light);
        Vec3 PL = vec3sub(l.pos, pos);
        double _pln = vec3norm(PL);
        vec3idiv(PL, _pln);
        double Lmn = vec3dot(PL, n);
        Vec3 Rm = vec3sub(vec3mul(n, 2*Lmn), PL);
        double diffuse = Lmn < 0 ? 0 : Lmn;
        double RmV = vec3dot(Rm, PV);
        double spectral = RmV < 0 ? 0 : dipow(RmV, mat.shininess);
        Vec3 light_add = vec3add(vec3add(vec3elmul(mat.Ka, l.Ia), vec3elmul(mat.Kd, vec3mul(l.Id, diffuse))), vec3elmul(mat.Ks, vec3mul(l.Is, spectral)));
        vec3iadd(col, light_add);
    }
    col = vec3cap(col, 0, 1);
    return vec3ToHex(col);
}

// alex shader, better than all your shader's by a lot
int alex_shader (Vec3 pos, Vec3 n, Vec2 t, environment env, material mat) { 
    //Vec3 col = vec3add (vec3mul (n, 0.5), ((Vec3){0.5, 0.5, 0.5}));
    // Vec3 point = (Vec3){env.time * 1, env.time * 1, env.time * 1};
    // Vec3 dist = vec3sub (pos, point);
    // double abs_dist = vec3norm (dist) * 0.001;
    // Vec3 col = vec3div (dist, vec3norm (dist));
    // Vec3 adder = (Vec3) {abs_dist, abs_dist, abs_dist};
    Vec3 col = vec3mul (vec3mul (vec3add (mat.Ka, n), 0.25), env.time * 0.01);

    //col = vec3mul (vec3add (col, vec3mul (n, 0.5)), 0.5);

    col = vec3cap (col, 0, 1);
    return vec3ToHex(col);
}

int pp_sobelfilter_shader(int *inarr, int width, int height, int i, int j) {
    int iT = i == 0 ? 1 : i-1;
    int iB = i == height-1 ? height-2 : i+1;
    int jL = j = 0 ? 1 : j-1;
    int jR = j == width-1 ? width-2 : j+1;
    int idx;
    int energy = 0;
    //printf("postprocessing i=%d, j=%d\n", i, j);
    for(idx = 0; idx < 17; idx += 8) {
        int x_energy, y_energy;
        // X convolve
        x_energy = (inarr[iT*width+jR] >> idx) % 256;
        x_energy += (2*inarr[i*width+jR] >> idx) % 256;
        x_energy += (inarr[iB*width+jR] >> idx) % 256;
        x_energy -= (inarr[iT*width+jL] >> idx) % 256;
        x_energy -= (2*inarr[i*width+jL] >> idx) % 256;
        x_energy -= (inarr[iB*width+jL] >> idx) % 256;
        // Y convolve
        y_energy = (inarr[iB*width+jL] >> idx) % 256;
        y_energy += (2*inarr[iB*width+j] >> idx) % 256;
        y_energy += (inarr[iB*width+jR] >> idx) % 256;
        y_energy -= (inarr[iT*width+jL] >> idx) % 256;
        y_energy -= (2*inarr[iT*width+j] >> idx) % 256;
        y_energy -= (inarr[iT*width+jR] >> idx) % 256;

        energy += y_energy*y_energy+x_energy*x_energy;
    }   
    return energy;
}

const int thresh = 300; // under threshhold -> black, over threshhold -> scale from 55-255
//thresh of 0 means all pixels brightened, thresh of 51000 means no pixels brightened
// every 200 thresh = 1 rgb increment
int *pp_grayscale_energy(int *inarr, int width, int height) {
    int *outarr = malloc(sizeof(int)*width*height);
    int max_inarr = 0;
    int *inarr_end = inarr+width*height;
    int *iptr = inarr;
    unsigned int col;
    int idx;
    for(; iptr < inarr_end; iptr++) {
        max_inarr = max(max_inarr, *iptr);
    }
    for(idx = 0; idx < width*height; idx++) {
        col = (inarr[idx]*255*200/(max_inarr));
        col = col > thresh ? (col / 255 + 55) : 0; // scale to range 55-255 unless close to 0 for visibility
        outarr[idx] = rgbToHex(col, col, col);
    }
    return outarr;
}

int pp_blur9x9(int *inarr, int width, int height, int i, int j) { // 81 ops per pixel
    int iC, jC, c = 0, r = 0, g = 0, b = 0;
    for(iC = i - 4; iC < i + 5; iC++) {
        if(iC < 0)
            continue;
        if(iC >= height)
            break;
        for(jC = j - 4; jC < j + 5; jC++) {
            if(jC < 0)
                continue;
            if(jC >= width)
                break;
            r += (inarr[iC * width + jC] >> 16) % 256;
            g += (inarr[iC * width + jC] >> 8) % 256;
            b += (inarr[iC * width + jC]) % 256;
            c++;
        }
    }
    if(c == 0) return 0;
    return rgbToHex(r/c, g/c, b/c);
}

int pp_blur3x3(int *inarr, int width, int height, int i, int j) { // 9 ops per pixel
    int iC, jC, c = 0, r = 0, g = 0, b = 0;
    for(iC = i - 1; iC < i + 2; iC++) {
        if(iC < 0)
            continue;
        if(iC >= height)
            break;
        for(jC = j - 1; jC < j + 2; jC++) {
            if(jC < 0)
                continue;
            if(jC >= width)
                break;
            r += (inarr[iC * width + jC] >> 16) % 256;
            g += (inarr[iC * width + jC] >> 8) % 256;
            b += (inarr[iC * width + jC]) % 256;
            c++;
        }
    }
    if(c == 0) return 0;
    return rgbToHex(r/c, g/c, b/c);
}

#define iabs(x) ((x) < 0 ? -(x) : (x))
int *pp_blur9x9_all(int *inarr, int width, int height) {
    short *outarr = calloc(6, width*height);
    int *retarr = malloc(sizeof(int)*width*height);
    int i, i3, j, iC, jC, idx;
    for(iC = -4; iC < 5; iC++) {
        for(jC = -4; jC < 5; jC++) {
            //printf("iter %d, %d\n", iC, jC);
            idx = 0;
            for(i = 0; i < height; i++) {
                for(j = 0; j < width; j++, idx+=3) {
                    outarr[idx] += inarr[iabs(i + iC)*width + iabs(j + jC)] % 256; // B
                    outarr[idx+1] += (inarr[iabs(i + iC)*width + iabs(j + jC)] >> 8) % 256; // G
                    outarr[idx+2] += (inarr[iabs(i + iC)*width + iabs(j + jC)] >> 16) % 256; // R
                }
            }
        }
    }
    for(i3 = 0, i = 0; i < width*height; i++, i3+=3) {
        retarr[i] = (outarr[i3]/81) + ((outarr[i3 + 1]/81) << 8) + ((outarr[i3 + 2]/81) << 16);
    }
    return retarr;
}
int *pp_blur3x3_all(int *inarr, int width, int height) {
    short *outarr = calloc(6, width*height);
    int *retarr = malloc(sizeof(int)*width*height);
    int i, i3, j, iC, jC, idx;
    for(iC = -1; iC < 2; iC++) {
        for(jC = -1; jC < 2; jC++) {
            //printf("iter %d, %d\n", iC, jC);
            idx = 0;
            for(i = 0; i < height; i++) {
                for(j = 0; j < width; j++, idx+=3) {
                    outarr[idx] += inarr[iabs(i + iC)*width + iabs(j + jC)] % 256; // B
                    outarr[idx+1] += (inarr[iabs(i + iC)*width + iabs(j + jC)] >> 8) % 256; // G
                    outarr[idx+2] += (inarr[iabs(i + iC)*width + iabs(j + jC)] >> 16) % 256; // R
                }
            }
        }
    }
    for(i3 = 0, i = 0; i < width*height; i++, i3+=3) {
        retarr[i] = (outarr[i3]/9) + ((outarr[i3 + 1]/9) << 8) + ((outarr[i3 + 2]/9) << 16);
    }
    return retarr;
}
#undef iabs

arrayvec *geom_extrudefaces(triangle tri) {
    arrayvec *ret = av_create(2, sizeof(triangle));
    //av_append(ret, &tri, false);
    Vec3 sn = vec3cross(vec3sub(tri.p1, tri.p2), vec3sub(tri.p2, tri.p3));
    Vec3 extrude = vec3div(sn, vec3norm(sn) / 40);
    vec3iadd(tri.p1, extrude);
    vec3iadd(tri.p2, extrude);
    vec3iadd(tri.p3, extrude);
    av_append(ret, &tri, false);
    Vec3 tmp = tri.p1;
    tri.p1 = tri.p2;
    tri.p2 = tmp;
    tmp = tri.n1;
    tri.n1 = tri.n2;
    tri.n2 = tmp;
    Vec2 tmp2 = tri.t1;
    tri.t1 = tri.t2;
    tri.t2 = tmp2;
    Vec3 z = {0,0,0};
    tri.n1 = vec3sub(z, tri.n1);
    tri.n2 = vec3sub(z, tri.n2);
    tri.n3 = vec3sub(z, tri.n3);
    av_append(ret, &tri, false);
    return ret;
}

int run_sphere() {
    bool vertex_shade = true; // ENABLE THIS TO EXPAND MIND
    int steps = 10; // more = higher poly count for sphere
    
    struct timespec slptime = {0, 0}; // x ms * (ns/ms)
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
    arrayvec *materials = av_create(2, sizeof(material));
    arrayvec *lights = av_create(2, sizeof(light));
    material *m1 = malloc(sizeof(material)),
            *m2 = malloc(sizeof(material));
    // m1 is a dull red material, m2 is a shiny light blue material
    m1->Ka = (Vec3){0.3, 0.1, 0.1}; // color when no light is shined
    m1->Kd = (Vec3){0.8, 0.2, 0.1}; // diffuse color, i.e. color imbued on scattered light
    m1->Ks = (Vec3){0.2, 0.03, 0.02}; // spectral color, i.e. color imbued on light shined directly into the eye
    m1->shininess = 5; // what it says. 0 shouldn't be used. 1-5 = very dull, 5-10 = some sheen, 10-20 = pretty shiny, 20+ = very shiny
    m2->Ka = (Vec3){0.05, 0.1, 0.25};
    m2->Kd = (Vec3){0.1, 0.3, 0.8};
    m2->Ks = (Vec3){0.1, 0.3, 1.0};
    m2->shininess = 15;
    av_append(materials, m1, false);
    av_append(materials, m2, false);
    light *l1 = malloc(sizeof(light)),
            *l2 = malloc(sizeof(light));
    // l1 is a bright green light at the top right of the image but out of the screen,
    // l2 is a bright pink light at middle of the left side of the image, also out of the screen
    l1->pos = (Vec3){500, 0, 200};
    l1->Ia = (Vec3){0, 0.15, 0}; // color this light casts onto every surface equally
    l1->Id = (Vec3){0.1, 0.7, 0.25}; // color this light casts that is spread diffusely
    l1->Is = (Vec3){0.1, 1.0, 0.25}; // color this light casts that is spectrally reflecteed
    l2->pos = (Vec3){0, 250, 300};
    l2->Ia = (Vec3){0.1, 0, 0.05};
    l2->Id = (Vec3){0.7, 0.1, 0.45};
    l2->Is = (Vec3){1.0, 0.15, 0.7};
    av_append(lights, l1, false);
    av_append(lights, l2, false);

    //printf("materials[0].color: %d", av_get_type(materials, 0, material)->color);

    // Vec3 p1 = {400, 400, 400};
    // Vec3 p2 = {400, 100, 100};
    // Vec3 p3 = {100, 400, 100};
    // Vec3 p4 = {100, 100, 400};
    Vec3 zero3 = {0,0,0};
    Vec2 zero2 = {0,0};
    int zero1 = 0;
    int one = 1;

    // arrayvec *tris = av_create(0, sizeof(triangle));

    arrayvec *pts = torusPoints(250, 250, 0, 150, 50, steps, steps);
    arrayvec *tri_idxs = torusTris(steps, steps);
    arrayvec *norms = av_create(pts->used_len, sizeof(Vec3));
    arrayvec *tcs = av_create(pts->used_len, sizeof(Vec2));
    arrayvec *mats = av_create(pts->used_len, sizeof(int));
    av_fill(norms, &zero3, pts->used_len);
    av_fill(tcs, &zero2, pts->used_len);
    av_fill(mats, &one, tri_idxs->used_len / 3);
    fix_overlap(pts, tri_idxs);
    if(vertex_shade)
        gen_vertex_normals(pts, tri_idxs, norms, tcs);
    //exit(0);
    arrayvec *tris = VTNT_to_AV(pts, tri_idxs, norms, tcs, mats);
    if(!vertex_shade)
        gen_surface_normals(tris);


    arrayvec *spts = spherePoints(250, 250, 0, 75, steps);
    arrayvec *stri_idxs = sphereTris(steps);
    arrayvec *snorms = av_create(spts->used_len, sizeof(Vec3));
    arrayvec *stcs = av_create(spts->used_len, sizeof(Vec2));
    arrayvec *smats = av_create(spts->used_len, sizeof(int));
    av_fill(snorms, &zero3, spts->used_len);
    av_fill(stcs, &zero2, spts->used_len);
    av_fill(smats, &zero1, stri_idxs->used_len / 3);
    fix_overlap(spts, stri_idxs);
    if(vertex_shade) {
        gen_vertex_normals(spts, stri_idxs, snorms, stcs);
        //check_orient(spts, snorms);
        //check_orient_sns(spts, stri_idxs);
        //exit(0);
    }
    //exit(0);
    arrayvec *stris = VTNT_to_AV(spts, stri_idxs, snorms, stcs, smats);
    if(!vertex_shade)
        gen_surface_normals(stris);

    arrayvec *boxpts = boxPoints(10, 10, -10, 100, 60, 80);
    arrayvec *boxtri_idxs = boxTris();
    arrayvec *boxnorms = av_create(boxpts->used_len, sizeof(Vec3));
    arrayvec *boxtcs = av_create(boxpts->used_len, sizeof(Vec2));
    arrayvec *boxmats = av_create(boxpts->used_len, sizeof(int));
    av_fill(boxnorms, &zero3, boxpts->used_len);
    av_fill(boxtcs, &zero2, boxpts->used_len);
    av_fill(boxmats, &zero1, boxtri_idxs->used_len / 3);
    arrayvec *boxtris = VTNT_to_AV(boxpts, boxtri_idxs, boxnorms, boxtcs, boxmats);
    gen_surface_normals(boxtris);
    apply_flat_tcs(boxtris, boxFlatTCs());

    arrayvec **vtnt = obj_get_lists_vtnt("mario/mario.obj");
    //exit(1);
    arrayvec *teavxs = vtnt[0],
        *teatri_idxs = vtnt[1],
        *teanorms = vtnt[2],
        *teatcs = vtnt[3],
        *teamats = av_create(teatri_idxs->used_len / 3, sizeof(int));
    printf("vxs 1 . x: %f\n", av_get_type(teavxs, 1, Vec3)->x);
    //exit(1);
    av_fill(teamats, &one, teatri_idxs->used_len / 3);
    printf("%d %d %d %d %d\n", teavxs->used_len, teatri_idxs->used_len, teanorms->used_len, teatcs->used_len, teamats->used_len);
    int *iter = malloc(sizeof(int));
    *iter = 0;
    Vec3 *dat;
    int *triidx;
    while((triidx = av_next(teatri_idxs, iter)) != NULL) {
        printf("tri %d: %d\n", *iter, *triidx);
    }
    //exit(1);
    *iter = 0;
    while((dat = av_next(teanorms, iter)) != NULL) {
        printf("teanorms %d: (%f, %f, %f)\n", *iter, dat->x, dat->y, dat->z);
    }
    //exit(1);

    //fix_overlap(teavxs, teatri_idxs);
    if(vertex_shade) {
        //av_fill(teanorms, &zero3, teavxs->used_len);
        //av_fill(teatcs, &zero2, teavxs->used_len);
        //gen_vertex_normals(teavxs, teatri_idxs, teanorms, teatcs);
    }
    arrayvec *teatris = VTNT_to_AV(teavxs, teatri_idxs, teanorms, teatcs, teamats);
    triangle *tri;
    *iter = 0;
    while((tri = av_next(teatris, iter)) != NULL) {
        printf("teatris %d: (%f, %f, %f)\n", *iter, tri->p1.x, tri->p1.y, tri->p1.z);
    }
    //exit(1);
    matrix *fw_tea, *bw_tea;
    fw_tea = scale(1, 1, 1);
    matmul_ip(rotate(2, M_PI), fw_tea);
    matmul_ip(translate(250, 300, 0), fw_tea);
    bw_tea = ident();
    gluInvertMatrix(fw_tea->data, bw_tea->data);
    itranspose(bw_tea);
    teatris = apply_transform(fw_tea, bw_tea, teatris);
    printf("len teatris = %d\n", teatris->used_len);
    printf("x is %f\n", (av_get_type(teatris, 50, triangle)->p1.x));
    //exit(1);
    free(fw_tea->data);
    free(bw_tea->data);
    free(fw_tea);
    free(bw_tea);

    //av_concat(tris, boxtris);
    //av_concat(tris, stris);
    av_concat(tris, teatris);
    //check_orients_tris(tris);
    printf("sns gend\n");
    double angle = 0;
    shaderlist shaders;
    shaders.render_shaders = av_create(20, sizeof(render_shader));
    render_shader extrude;
    extrude.type = GEOMETRY;
    extrude.func = geom_extrudefaces;
    //av_append(shaders.render_shaders, &extrude, false);
    shaders.fragment_shader = phong_shader_x;
    arrayvec *pp_multiblur_edges = av_create(20, sizeof(postprocess_shader));
    postprocess_shader blur;
    blur.type = SINGLE;
    blur.func = pp_blur3x3;
    postprocess_shader sobelenergy;
    sobelenergy.type = SINGLE;
    sobelenergy.func = pp_sobelfilter_shader;
    postprocess_shader scalegray;
    scalegray.type = ALL;
    scalegray.func = pp_grayscale_energy;
    //av_append(pp_multiblur_edges, &blur, false);
    //v_append(pp_multiblur_edges, &blur, false);
    //av_append(pp_multiblur_edges, &blur, false);
    //av_append(pp_multiblur_edges, &sobelenergy, false);
    //av_append(pp_multiblur_edges, &scalegray, false);
    
    //av_append_literal(shaders.postprocess_shaders, pp_sobelfilter_shader, void *);
    //av_append_literal(shaders.postprocess_shaders, pp_grayscale_energy, void *);
    environment env;
    env.lights = lights;
    env.view = (Vec3){250, 250, 1e30};
    //exit(1);
    while(frameNum < 1000) {
        env.time = frameNum;
        //g2d_fill_bg (CYAN);

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
        matrix *fw_t, *bw_t;
        fw_t = translate(-250, -250, 0);
        matmul_ip(rotate(0, angle), fw_t);
        matmul_ip(rotate(1, -3./8), fw_t);
        matmul_ip(translate(250, 250, 0), fw_t);        
        print_matrix(fw_t);
        printf("\nnorm: %f\n", matrix_normsquared(fw_t));
        
        bw_t = ident();
        gluInvertMatrix(fw_t->data, bw_t->data);
        itranspose(bw_t);
        // bw_t = translate_inv_t(250, 250, 0);
        // //matmul_ip(rotate_inv_t(1, 3./8), bw_t);
        // matmul_ip(rotate_inv_t(0, angle), bw_t);
        // matmul_ip(translate_inv_t(-250, -250, 0), bw_t);
        //check_orients_tris(apply_transform(fw_t, bw_t, tris));
        arrayvec *trans_tris = apply_transform(fw_t, bw_t, tris);
        free(fw_t->data);
        free(bw_t->data);
        free(fw_t);
        free(bw_t);
        // test matrices
        //transpose(bw_t, fw_t);
        //printf("A*AT:\n");
        //matmul_ip(bw_t, fw_t);
        //print_matrix(fw_t);
        //printf("%d\n",trans_tris->used_len);
        //triangle tri = av_get_value(trans_tris, 78, triangle);

        //printf("Triangle #78 @ (%f, %f, %f)<%f, %f, %f>, (%f, %f, %f)<%f, %f, %f>, (%f, %f, %f)<%f, %f, %f>\n", tri.p1.x, tri.p1.y, tri.p1.z, tri.n1.x, tri.n1.y, tri.n1.z, tri.p2.x, tri.p2.y, tri.p2.z, tri.n2.x, tri.n2.y, tri.n2.z, tri.p3.x, tri.p3.y, tri.p3.z, tri.n3.x, tri.n3.y, tri.n3.z);
        //tri.mat=1;
        //av_set(trans_tris, &tri, 78, false);
        shaders.postprocess_shaders = pp_multiblur_edges;
        col_arr = render(trans_tris, materials, env, shaders);
        av_clear(trans_tris);
        free(trans_tris);
        // apply_transform_inplace(trans_matrix, trans_matrix_inv_t, tris);
        
        img = XCreateImage(dis, CopyFromParent, 24, ZPixmap, 0, (char *)col_arr, WIDTH, HEIGHT, 32, 0);
        XPutImage(dis, win, gc, img, 0, 0, 0, 0, WIDTH, HEIGHT);
        free(img);
        free(col_arr);
        nanosleep(&slptime, NULL);
        frameNum++;
        gettimeofday(&end, NULL);
        float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        printf("Average of %d FPS\n", (int)(frameNum / nsecs));
        angle += 0.04764523423421;
    }
    gettimeofday(&end, NULL);
    float nsecs = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf("10000 loops: %f ms\n", nsecs * 1000);
    printf("Average of %d FPS", (int)(10000 / nsecs));

    //XCloseDisplay(dis);

    return 0;
}

int main() {
    setbuf(stdout, NULL);
    srand(time(NULL));

    open_window();
    //run_line();
    //run_pong();
    //run_triangle();
    //run_tri_test ();
    run_sphere();
}