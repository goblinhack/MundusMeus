/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "glapi.h"

#define CUBE_RES 3

void test(void);

static void cube (GLfloat **P)
{
    int x, y, z;
    GLfloat r, b, g;
    GLfloat a = 1.0;
    GLfloat *p = *P;

    for (y = 0; y < CUBE_RES; y++) {
        z = 0;
        x = 0;

        if (y) {
            gl_push_texcoord(p, 0, 0);
            gl_push_vertex_3d(p, x, y, z);
            gl_push_rgba(p, r, g, b, a);
        }

        for (x = 0; x < CUBE_RES; x++) {
            if ((x + y + z) & 1) {
                r = 0.1; g = 0.2; b = 0.3;
            } else {
                r = 0.1; g = 0.5; b = 0.1;
            }

            gl_push_texcoord(p, 0, 0);
            gl_push_vertex_3d(p, x, y, z);
            gl_push_rgba(p, r, g, b, a);

            gl_push_texcoord(p, 1, 1);
            gl_push_vertex_3d(p, x, y + 1, z);
            gl_push_rgba(p, r, g, b, a);

            gl_push_texcoord(p, 1, 1);
            gl_push_vertex_3d(p, x + 1, y, z);
            gl_push_rgba(p, r, g, b, a);

            gl_push_texcoord(p, 1, 1);
            gl_push_vertex_3d(p, x + 1, y + 1, z);
            gl_push_rgba(p, r, g, b, a);
        }

        x--;
        gl_push_texcoord(p, 1, 1);
        gl_push_vertex_3d(p, x + 1, y + 1, z);
        gl_push_rgba(p, r, g, b, a);
    }

    for (y = 0; y < CUBE_RES; y++) {
        z = 0;
        x = CUBE_RES;

        gl_push_texcoord(p, 0, 0);
        gl_push_vertex_3d(p, x, y, z);
        gl_push_rgba(p, r, g, b, a);

        for (z = 0; z > -CUBE_RES; z--) {
            if ((x + y + z) & 1) {
                r = 0.7; g = 0.8; b = 0.9;
            } else {
                r = 0.1; g = 0.2; b = 0.8;
            }

            gl_push_texcoord(p, 0, 0);
            gl_push_vertex_3d(p, x, y, z);
            gl_push_rgba(p, r, g, b, a);

            gl_push_texcoord(p, 1, 1);
            gl_push_vertex_3d(p, x, y + 1, z);
            gl_push_rgba(p, r, g, b, a);

            gl_push_texcoord(p, 1, 1);
            gl_push_vertex_3d(p, x, y, z - 1);
            gl_push_rgba(p, r, g, b, a);

            gl_push_texcoord(p, 1, 1);
            gl_push_vertex_3d(p, x, y + 1, z - 1);
            gl_push_rgba(p, r, g, b, a);
        }

        z++;
        gl_push_texcoord(p, 1, 1);
        gl_push_vertex_3d(p, x, y + 1, z - 1);
        gl_push_rgba(p, r, g, b, a);
    }

    for (x = 0; x < CUBE_RES; x++) {
        z = 0;
        y = CUBE_RES;

        gl_push_texcoord(p, 0, 0);
        gl_push_vertex_3d(p, x, y, z);
        gl_push_rgba(p, r, g, b, a);

        for (z = 0; z > -CUBE_RES; z--) {
            if ((x + y + z) & 1) {
                r = 1.0; g = 0.2; b = 0.3;
            } else {
                r = 0.3; g = 1.0; b = 1.0;
            }

            gl_push_texcoord(p, 0, 0);
            gl_push_vertex_3d(p, x, y, z);
            gl_push_rgba(p, r, g, b, a);

            gl_push_texcoord(p, 1, 1);
            gl_push_vertex_3d(p, x + 1, y, z);
            gl_push_rgba(p, r, g, b, a);

            gl_push_texcoord(p, 1, 1);
            gl_push_vertex_3d(p, x, y, z - 1);
            gl_push_rgba(p, r, g, b, a);

            gl_push_texcoord(p, 1, 1);
            gl_push_vertex_3d(p, x + 1, y, z - 1);
            gl_push_rgba(p, r, g, b, a);
        }

        z++;
        gl_push_texcoord(p, 1, 1);
        gl_push_vertex_3d(p, x + 1, y, z - 1);
        gl_push_rgba(p, r, g, b, a);
    }

    *P = p;
}

void test (void)
{
    glBindTexture(GL_TEXTURE_2D, 0);

    blit_init();

    GLfloat *p = bufp;

    int n;
    for (n = 0; n < 13 * 13 * 13; n++) {
    cube(&p);
    }

    bufp = p;
    blit_flush_3d();
}
