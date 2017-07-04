#include "clipper.h"
void clip(render_context *rc, int width, int height) {
}

//    int i, i1, i3;
//    bool c1, c2, c3;
//    Vec3 *p1, *p2, *p3, r1, r2, nr1, nr2;
//    Vec2 tr1, tr2;
//    Vec3 oob = {-1, -1, 0};
//    for(i1 = 0, i3 = 0; i1 < rc->mlist->len; i1++, i3++) {
//
//        c1 = false; c2 = false; c3 = false;
//        p1 = &rc->vlist->data[i3];
//        p2 = &rc->vlist->data[i3+1];
//        p3 = &rc->vlist->data[i3+2];
//
//        bool c1High = p1->y < 0;
//        bool c1Low = p1->y > height;
//        bool c1Left = p1->x < 0;
//        bool c1Right = p1->x > width;
//        bool p1Out = c1High || c1Low || c1Left || c1Right;
//
//        bool c2High = p2->y < 0;
//        bool c2Low = p2->y > height;
//        bool c2Left = p2->x < 0;
//        bool c2Right = p2->x > width;
//        bool p2Out = c2High || c2Low || c2Left || c2Right;
//
//        bool c3High = p3->y < 0;
//        bool c3Low = p3->y > height;
//        bool c3Left = p3->x < 0;
//        bool c3Right = p3->x > width;
//        bool p3Out = c3High || c3Low || c3Left || c3Right;
//
//        /* new tris and ordering
//         * tris are in order p1,p2,p3
//         * let's say p1 is clipped into points r1, r2
//         * new tris should be r1,p2,p3 (ordering works because r1 near p1),
//         * r1, p3, r2 (ordering works again?) */
//        else if(c1) {
//            r1 = clip1(*p1, *p2, width, height, c1High, c1Low, c1Left, c1Right);
//            r2 = clip1(*p1, *p3, width, height, c1High, c1Low, c1Left, c1Right);
//            // interp normals and shit
//            double t1 = (p1.x - r1.x)/(p1.x - p2.x);
//            nr1 = vec3lerp(rc->nlist->data[i3], rc->nlist->data[i3+1], t1);
//            tr1 = vec2lerp(rc->tlist->data[i3], rc->tlist->data[i3+1], t1);
//            double t2 = (p1.x - r2.x)/(p1.x - p3.x);
//            nr2 = vec3lerp(rc->nlist->data[i3], rc->nlist->data[i3+2], t2);
//            tr2 = vec2lerp(rc->tlist->data[i3], rc->tlist->data[i3+2], t2);
//            *p1 = r1;
//            &rc->nlist->data[i3] = nr1;
//            &rc->tlist->data[i3] = tr1;
//            &rc->p
//        }
//    }
//}
//
//void clip_line(Vec3 *p1, Vec3 *p2, int width, int height) {
//    bool c1High = p1->y < 0;
//    bool c1Low = p1->y > height;
//    bool c1Left = p1->x < 0;
//    bool c1Right = p1->x > width;
//    bool p1Out = c1High || c1Low || c1Left || c1Right;
//
//    bool c2High = p2->y < 0;
//    bool c2Low = p2->y > height;
//    bool c2Left = p2->x < 0;
//    bool c2Right = p2->x > width;
//    bool p2Out = c2High || c2Low || c2Left || c2Right;
//
//    if(p1Out) clip1(p1, *p2, width, height, c1High, c1Low, c1Left, c1Right);
//    if(p2Out) clip1(p2, *p1, width, height, c2High, c2Low, c2Left, c2Right);
//}
//
//void clip1(Vec3 c, Vec3 o, int width, int height, bool U, bool D, bool L, bool R) {
//    // 2 point form: (y-y1)(x2-x1) = (x-x1)(y2-y1)
//    if(U) { // clip against y=0
//        c = {-c.y*(o.x-c.x)/(o.y-c.y)+c.x, 0};
//    }
//    if(L) { // clip against x=0
//        c = {0, -c.x*(o.y-c.y)/(o.x-c.x)+c.y};
//    }
//    if(D) { // clip against y=H
//        c = {(height - c.y)*(o.x-c.x)/(o.y-c.y)+c.x, height};
//    }
//    if(R) { // clip against x=W
//        c = {width, (width-c.x)*(o.y-c.y)/(o.x-c.x)+c.y};
//    }
//    return c;
//}