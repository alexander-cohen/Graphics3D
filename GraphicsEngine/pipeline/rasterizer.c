//
// Created by gmarks on 6/26/17.
//

#include "rasterizer.h"
#include "linalg/vector.h"

raster_context *rasterizer(render_context *rc, int width, int height) {
    raster_context *rac = malloc(sizeof(raster_context));
    rac->materials = rc->materials;
    rac->width = width;
    rac->height = height;
    rac->color_buffer = malloc(sizeof(int) * width * height);
    rac->z_buffer = malloc(sizeof(double) * width * height);
    rac->n_buffer = malloc(sizeof(Vec3) * width * height);
    if(rac->z_buffer == NULL) {
        printf("WARNING!!! ZBUF NULL!!!!\n");
    }
    rac->mat_buffer = malloc(sizeof(int) * width * height);
    int i;
    for(i = 0; i < width*height; i++) {
        rac->z_buffer[i] = -INFINITY;
        rac->color_buffer[i] = (1 << 24) - 1;
        rac->mat_buffer[i] = -1;
    }

    int i1, i3;
    triangle tri;
    printf("num tris: %d\n", rc->mlist->used_len);
    for(i1 = 0, i3 = 0; i1 < rc->mlist->used_len; i1++, i3+=3) {
        tri.p1 = av_get_value(rc->vlist, i3, Vec3);
        tri.p2 = av_get_value(rc->vlist, i3 + 1, Vec3);
        tri.p3 = av_get_value(rc->vlist, i3 + 2, Vec3);
        tri.n1 = av_get_value(rc->nlist, i3, Vec3);
        tri.n2 = av_get_value(rc->nlist, i3 + 1, Vec3);
        tri.n3 = av_get_value(rc->nlist, i3 + 2, Vec3);
        printf("surface norm for tri %d: (%f, %f, %f)\n", i1, tri.n1.x, tri.n1.y, tri.n1.z);
        tri.t1 = av_get_value(rc->tlist, i3, Vec2);
        tri.t2 = av_get_value(rc->tlist, i3 + 1, Vec2);
        tri.t3 = av_get_value(rc->tlist, i3 + 2, Vec2);
        tri.mat = av_get_value(rc->mlist, i1, int);

        // av_pop_value(rc->vlist, &tri.p1, Vec3);
        // av_pop_value(rc->vlist, &tri.p2, Vec3);
        // av_pop_value(rc->vlist, &tri.p3, Vec3);
        // av_pop_value(rc->nlist, &tri.n1, Vec3);
        // av_pop_value(rc->nlist, &tri.n2, Vec3);
        // av_pop_value(rc->nlist, &tri.n3, Vec3);
        // av_pop_value(rc->tlist, &tri.t1, Vec2);
        // av_pop_value(rc->tlist, &tri.t2, Vec2);
        // av_pop_value(rc->tlist, &tri.t3, Vec2);
        // av_pop_value(rc->mlist, &tri.mat, int);

        raster_tri(rac, tri);
    }
    return rac;
}

double idet(Vec3 p1, Vec3 p2, Vec3 p3) {
    return 1 / ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
}

void barycentric(Vec3 *bary, Vec2 c, Vec3 p1, Vec3 p2, Vec3 p3, double idt) {
    bary->x = idt * ((p2.y - p3.y)*(c.x - p3.x) + (p3.x - p2.x)*(c.y - p3.y));
    bary->y = idt * ((p3.y - p1.y)*(c.x - p3.x) + (p1.x - p3.x)*(c.y - p3.y));
    bary->z = 1 - bary->x - bary->y;
}


static inline int inwidth (raster_context *rac, int x)
{
    return (int) min (max (x, 0), (rac -> width) - 1);
}

static inline int inheight (raster_context *rac, int y)
{
    return (int) min (max (y, 0), (rac -> height) - 1);
}

static double orient2d (double x1, double y1, double x2, double y2, double x3, double y3)
{
    return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

static double cross2d (double dx1, double dy1, double dx2, double dy2)
{
    return dx1 * dy2 - dx2 * dy1;
}

static triangle rot_tri(triangle tri) {
    Vec3 p1, n1;
    Vec2 t1;
    p1 = tri.p1;
    n1 = tri.n1;
    t1 = tri.t1;
    tri.p1 = tri.p2;
    tri.n1 = tri.n2;
    tri.t1 = tri.t2;
    tri.p2 = p1;
    tri.n2 = n1;
    tri.t2 = t1;
    return tri;
}

void raster_tri(raster_context *rac, triangle tri) {
    double idt = idet(tri.p1, tri.p2, tri.p3);
    double x1 = tri.p1.x, y1 = tri.p1.y, z1 = tri.p1.z,
        x2 = tri.p2.x, y2 = tri.p2.y, z2 = tri.p2.z,
        x3 = tri.p3.x, y3 = tri.p3.y, z3 = tri.p3.z;
    printf("coords: (%f,%f,%f),(%f,%f,%f),(%f,%f,%f)\n", x1, y1, z1, x2, y2, z2, x3, y3, z3);
    //y's are negated bc the axis is inverted
    

    double
            dx12 = (x2 - x1), dy12 = (y2 - y1),
            dx23 = (x3 - x2), dy23 = (y3 - y2),
            dx31 = (x1 - x3), dy31 = (y1 - y3);

    if (cross2d (dx12, dy12, dx23, dy23) < 0) {
        printf("triangle culled because it is a backface.\n");
        //return raster_tri(rac, rot_tri(tri));
    }
   
    //printf ("points: %d, %d; %d, %d; %d %d; %d\n", x1, y1, x2, y2, x3, y3, cross2d (dx12, dy12, dx23, dy23));
    //printf ("difs: %d, %d; %d, %d; %d %d\n", dx12, dy12, dx23, dy23, dx31, dy31);


    short min_x = max(0, min3 (x1, x2, x3));
    short min_y = max(0, min3 (y1, y2, y3));

    short max_x = min(rac->width-1, max3 (x1, x2, x3));
    short max_y = min(rac->height-1, max3 (y1, y2, y3));
    //printf("bbox: (%d,%d),(%d,%d)\n", min_x, min_y, max_x, max_y);
    Vec3 baryTL, baryTR, baryBL;
    barycentric(&baryTL, (Vec2){min_x, min_y}, tri.p1, tri.p2, tri.p3, idt);
    barycentric(&baryTR, (Vec2){max_x, min_y}, tri.p1, tri.p2, tri.p3, idt);
    barycentric(&baryBL, (Vec2){min_x, max_y}, tri.p1, tri.p2, tri.p3, idt);

    double zTL = z1*baryTL.x + z2*baryTL.y + z3*baryTL.z;
    double zTR = z1*baryTR.x + z2*baryTR.y + z3*baryTR.z;
    double zBL = z1*baryBL.x + z2*baryBL.y + z3*baryBL.z;
    Vec3 nTL = vec3iadd(vec3iadd(vec3imul(tri.n1, baryTL.x), vec3imul(tri.n2, baryTL.y)), vec3imul(tri.n3, baryTL.z));
    Vec3 nTR = vec3iadd(vec3iadd(vec3imul(tri.n1, baryTR.x), vec3imul(tri.n2, baryTR.y)), vec3imul(tri.n3, baryTR.z));
    Vec3 nBL = vec3iadd(vec3iadd(vec3imul(tri.n1, baryBL.x), vec3imul(tri.n2, baryBL.y)), vec3imul(tri.n3, baryBL.z));

    //printf("tl: %f tr: %f bl: %f\n", zTL, zTR, zBL);
    double z_inc = (zTR - zTL) / (max_x - min_x);
    double z_row_inc = (zBL - zTL) / (max_y - min_y);
    double z_row = zTL;
    Vec3 n_inc = vec3idiv(vec3isub(nTR, nTL), max_x - min_x);
    Vec3 n_row_inc = vec3idiv(vec3isub(nBL, nTL), max_y - min_y);
    Vec3 n_row = nTL;
    //printsf ("min: %d, %d\n", min_x, min_y);
    double w1_row = orient2d (x2, y2, x3, y3, min_x, min_y);
    double w2_row = orient2d (x3, y3, x1, y1, min_x, min_y);
    double w3_row = orient2d (x1, y1, x2, y2, min_x, min_y);
    //printf("%f %f %f\n", w1_row, w2_row, w3_row);
    double w1, w2, w3, z;
    Vec3 norm;
    int xnext = min_x;
    int idx;
    printf("(%d,%d),(%d,%d)\n", min_x, min_y, max_x, max_y);
    for (short y = min_y; y <= max_y; y++)
    {
        w1 = w1_row;
        w2 = w2_row;
        w3 = w3_row;
        z = z_row;
        norm = n_row;
        //printf ("%d, %d\n", xnext, y);

        bool found = false;

        short x = max(xnext, min_x);
        for (; w1_row >= 0 && w2_row >= 0 && w3_row >= 0 && xnext >= min_x; xnext--)
        {
            //printf ("x: %d, y: %d\n", xnext, y);
            //*g2d_buffer_get (y, xnext) = graphics_context -> color;
            //PUT PIXEL AT Y, XNEXT
            idx = y * rac->width + xnext;
            if(z > rac->z_buffer[idx] && norm.z >= 0) {
                //printf("passed z-buffer test\n");
                rac->n_buffer[idx] = norm;
                rac->z_buffer[idx] = z;
                rac->mat_buffer[idx] = tri.mat;
            }
            else {
                //printf("failed z-buffer test\n");
            }
            //printf ("(after) x: %d, y: %d\n", xnext, y);
            w1_row += dy23;
            w2_row += dy31;
            w3_row += dy12;
            z_row += z_row_inc;
            n_row = vec3iadd(n_row, n_row_inc);
        }
        for (; x <= max_x; x++)
        {

            if (w1 >= 0 && w2 >= 0 && w3 >= 0)
            {
                //printf ("x2: %d, y2: %d\n", xnext, y);
                //*g2d_buffer_get (y, x) = graphics_context -> color;
                //PUT PIXEL AT Y, X
                idx = y * rac->width + x;
                if(z > rac->z_buffer[idx] && norm.z >= 0) {
                    //printf("passed z-buffer test\n");
                    rac->n_buffer[idx] = norm;
                    rac->z_buffer[idx] = z;
                    rac->mat_buffer[idx] = tri.mat;
                }
                else {
                    //printf("failed z-buffer test\n");
                }
                found = true;
            }

                //once there are no more pixels, skip to the next scanline
            else if (found)
            {
                break;
            }

            w1 -= dy23;
            w2 -= dy31;
            w3 -= dy12;
            z += z_inc;
            norm = vec3iadd(norm, n_inc);
        }

        w1_row += dx23;
        w2_row += dx31;
        w3_row += dx12;
        z_row += z_row_inc;
        n_row = vec3iadd(n_row, n_row_inc);
    }
}

//main() {
//    render_context *rc = input_assembler((triangle[]){{1, 1, (Vec3){0,0,0},(Vec3){200,100,100},(Vec3){300,400,200},(Vec3){300,400,200},(Vec3){300,400,200},(Vec3){300,400,200},
//                                                              (Vec2){0,0},(Vec2){0,0},(Vec2){0,0},1}}, 1, new_matarrayvec());
//    rasterizer(rc, 512, 512);
//}