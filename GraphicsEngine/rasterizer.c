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
    rac->mat_buffer = malloc(sizeof(int) * width * height);
    int i1, i3;
    triangle tri;
    for(i1 = 0, i3 = 0; i1 < rc->mlist->len; i1++, i3+=3) {
        tri.p1 = rc->vlist->data[i3];
        tri.p2 = rc->vlist->data[i3 + 1];
        tri.p3 = rc->vlist->data[i3 + 2];
        tri.n1 = rc->nlist->data[i3];
        tri.n2 = rc->nlist->data[i3 + 1];
        tri.n3 = rc->nlist->data[i3 + 2];
        tri.t1 = rc->tlist->data[i3];
        tri.t2 = rc->tlist->data[i3 + 1];
        tri.t3 = rc->tlist->data[i3 + 2];
        tri.mat = rc->mlist->data[i1];
        raster_tri(rac, tri);
    }
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

void raster_tri(raster_context *rac, triangle tri) {
    double idt = idet(tri.p1, tri.p2, tri.p3);
    double x1 = tri.p1.x, y1 = tri.p1.y, z1 = tri.p1.z,
        x2 = tri.p2.x, y2 = tri.p2.y, z2 = tri.p2.z,
        x3 = tri.p3.x, y3 = tri.p3.y, z3 = tri.p3.z;
    //y's are negated bc the axis is inverted
    double
            dx12 = (x2 - x1), dy12 = (y2 - y1),
            dx23 = (x3 - x2), dy23 = (y3 - y2),
            dx31 = (x1 - x3), dy31 = (y1 - y3);

    //change the order so its counter clockwise
    if (cross2d (dx12, dy12, dx23, dy23) < 0)
    {
        return g2d_fill_triangle_boundingbox (x1, y1, x3, y3, x2, y2);
    }

    //printf ("points: %d, %d; %d, %d; %d %d; %d\n", x1, y1, x2, y2, x3, y3, cross2d (dx12, dy12, dx23, dy23));
    //printf ("difs: %d, %d; %d, %d; %d %d\n", dx12, dy12, dx23, dy23, dx31, dy31);


    short min_x = inwidth (rac, min3 (x1, x2, x3));
    short min_y = inheight (rac, min3 (y1, y2, y3));

    short max_x = 1 + inwidth (rac, max3 (x1, x2, x3));
    short max_y = 1 + inheight (rac, max3 (y1, y2, y3));
    Vec3 baryTL, baryTR, baryBL;
    barycentric(&baryTL, (Vec2){min_x, min_y}, tri.p1, tri.p2, tri.p3, idt);
    barycentric(&baryTR, (Vec2){max_x, min_y}, tri.p1, tri.p2, tri.p3, idt);
    barycentric(&baryBL, (Vec2){min_x, max_y}, tri.p1, tri.p2, tri.p3, idt);

    double zTL = z1*baryTL.x + z2*baryTL.y + z3*baryTL.z;
    double zTR = z1*baryTR.x + z2*baryTR.y + z3*baryTR.z;
    double zBL = z1*baryBL.x + z2*baryBL.y + z3*baryBL.z;

    printf("tl: %f tr: %f bl: %f\n", zTL, zTR, zBL);
    double z_inc = (zTR - zTL) / (max_x - min_x);
    double z_row_inc = (zBL - zTL) / (max_y - min_y);
    double z_row = zTL;
    //printsf ("min: %d, %d\n", min_x, min_y);
    double w1_row = orient2d (x2, y2, x3, y3, min_x, min_y);
    double w2_row = orient2d (x3, y3, x1, y1, min_x, min_y);
    double w3_row = orient2d (x1, y1, x2, y2, min_x, min_y);

    double w1, w2, w3, z;
    int xnext = min_x;
    int idx;
    for (short y = min_y; y <= max_y; y++)
    {
        w1 = w1_row;
        w2 = w2_row;
        w3 = w3_row;
        z = z_row;
        //printf ("%d, %d\n", xnext, y);

        bool found = false;

        short x = xnext;
        for (; w1_row >= 0 && w2_row >= 0 && w3_row >= 0 && xnext >= min_x; xnext--)
        {
            //printf ("x: %d, y: %d\n", xnext, y);
            //*g2d_buffer_get (y, xnext) = graphics_context -> color;
            //PUT PIXEL AT Y, XNEXT
            idx = y * rac->width + xnext;
            if(z > rac->z_buffer[idx]) {
                rac->z_buffer[idx] = z;
                rac->mat_buffer[idx] = tri.mat;
            }
            //printf ("(after) x: %d, y: %d\n", xnext, y);
            w1_row += dy23;
            w2_row += dy31;
            w3_row += dy12;
            z_row += z_row_inc;
        }

        for (; x <= max_x; x++)
        {
            //printf ("x2: %d, y2: %d\n", x, y);

            if (w1 >= 0 && w2 >= 0 && w3 >= 0)
            {
                //*g2d_buffer_get (y, x) = graphics_context -> color;
                //PUT PIXEL AT Y, X
                idx = y * rac->width + x;
                if(z > rac->z_buffer[idx]) {
                    rac->z_buffer[idx] = z;
                    rac->mat_buffer[idx] = tri.mat;
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
        }

        w1_row += dx23;
        w2_row += dx31;
        w3_row += dx12;
        z_row += z_row_inc;
    }
}

//main() {
//    render_context *rc = input_assembler((triangle[]){{1, 1, (Vec3){0,0,0},(Vec3){200,100,100},(Vec3){300,400,200},(Vec3){300,400,200},(Vec3){300,400,200},(Vec3){300,400,200},
//                                                              (Vec2){0,0},(Vec2){0,0},(Vec2){0,0},1}}, 1, new_matarrayvec());
//    rasterizer(rc, 512, 512);
//}