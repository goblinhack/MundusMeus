/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "glapi.h"

#define CUBE_RES    5

#define ISO_W       21
#define ISO_H       21
#define ISO_Z       10

typedef struct {
    colorf c[CUBE_RES][CUBE_RES][CUBE_RES];
} cube_t;

cube_t cubes[ISO_W][ISO_H][ISO_Z];

void test(void);

static int gl_need_degen_triangle;

#define GL_DEGEN_TRIANGLE_NEEDED()                              \
    gl_need_degen_triangle = true;

#define GL_DEGEN_TRIANGLE_IF_NEEDED()                           \
{                                                               \
    gl_need_degen_triangle = false;                             \
                                                                \
    memcpy(p + NUMBER_FLOATS_PER_VERTICE_3D,                    \
           p - NUMBER_FLOATS_PER_VERTICE_3D,                    \
           sizeof(float) * NUMBER_FLOATS_PER_VERTICE_3D);       \
    memcpy(p,                                                   \
           p - NUMBER_FLOATS_PER_VERTICE_3D,                    \
           sizeof(float) * NUMBER_FLOATS_PER_VERTICE_3D);       \
                                                                \
    memcpy(p - NUMBER_FLOATS_PER_VERTICE_3D,                    \
           p - NUMBER_FLOATS_PER_VERTICE_3D * 2,                \
           sizeof(float) * NUMBER_FLOATS_PER_VERTICE_3D);       \
                                                                \
    p += NUMBER_FLOATS_PER_VERTICE_3D;                          \
    p += NUMBER_FLOATS_PER_VERTICE_3D;                          \
}

#define GL_CUBE_VERTICE(X, Y, Z, x, y, z, dx, dy, dz)                    \
{                                                               \
    c = &cubes[ox + dx][oy + dy][oz + dz];                      \
    gl_push_texcoord(p, 0, 0);                                  \
    gl_push_vertex_3d(p, X + dx, Y + dy, Z + dz);               \
    gl_push_rgba(p, 0.1 * x, 0.1 * y, 0.1 * y, 1.0); \
}

#if 0
    gl_push_rgba(p, c->c[x][y][z].r, c->c[x][y][z].g, c->c[x][y][z].b, c->c[x][y][z].a);            
#endif

static void cube (GLfloat **P, int ox, int oy, int oz)
{
    GLfloat *p = *P;
    int x, y, z;
    int X, Y, Z;
    cube_t *c;

    ox -= (ISO_W - 1) / 2;
    oy -= (ISO_H - 1) / 2;
    oz -= (ISO_Z - 1) / 2;

#if 1
    /*
     * Top part of cibe.
     */
    for (y = 0; y < CUBE_RES; y++) {
        z = 0;

        for (x = 0; x < CUBE_RES; x++) {
            X = (ox * CUBE_RES) + x;
            Y = (oy * CUBE_RES) + y;
            Z = (oz * CUBE_RES) + z;

            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 0, 0, 0)
            GL_DEGEN_TRIANGLE_IF_NEEDED()
            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 0, 1, 0)
            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 1, 0, 0)
            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 1, 1, 0)
        }

        GL_DEGEN_TRIANGLE_NEEDED()
    }
#endif

#if 1
    /*
     * Right face of cibe.
     */
    for (y = 0; y < CUBE_RES; y++) {
        x = CUBE_RES;

        for (z = 0; z > -CUBE_RES; z--) {
            X = (ox * CUBE_RES) + x;
            Y = (oy * CUBE_RES) + y;
            Z = (oz * CUBE_RES) + z;

            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 0, 0, 0)
            GL_DEGEN_TRIANGLE_IF_NEEDED()
            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 0, 1, 0)
            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 0, 0, -1)
            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 0, 1, -1)
        }

        GL_DEGEN_TRIANGLE_NEEDED()
    }
#endif

#if 1
    /*
     * Left face of cibe.
     */
    for (x = 0; x < CUBE_RES; x++) {
        y = CUBE_RES;

        for (z = 0; z > -CUBE_RES; z--) {
            X = (ox * CUBE_RES) + x;
            Y = (oy * CUBE_RES) + y;
            Z = (oz * CUBE_RES) + z;

            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 0, 0, 0)
            GL_DEGEN_TRIANGLE_IF_NEEDED()
            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 1, 0, 0)
            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 0, 0, -1)
            GL_CUBE_VERTICE(X, Y, Z, x, y, z, 1, 0, -1)
        }

        GL_DEGEN_TRIANGLE_NEEDED()
    }
#endif

    *P = p;
}

void test (void)
{
    glBindTexture(GL_TEXTURE_2D, 0);

    blit_init();

    GLfloat *p = bufp;

    int x;
    int y;
    int z;
    int X;
    int Y;
    int Z;

    mysrand(10);

    for (X = 0; X < ISO_W; X++) {
        for (Y = 0; Y < ISO_H; Y++) {
            for (Z = 0; Z < ISO_Z; Z++) {
                for (x = 0; x < CUBE_RES; x++) {
                    for (y = 0; y < CUBE_RES; y++) {
                        for (z = 0; z < CUBE_RES; z++) {
                            cube_t *c;

                            c = &cubes[X][Y][Z];

                            c->c[x][y][z].r = 0.1;
                            c->c[x][y][z].g = 0.1;
                            c->c[x][y][z].b = 0.1;
                            c->c[x][y][z].a = 0.1;
                        }
                    }
                }
            }
        }
    }
    for (x = 0; x < ISO_W; x++) {
        for (y = 0; y < ISO_H; y++) {
            if ((x == 0) || (y == 0)) {
                for (z = 0; z < ISO_Z; z++) {
                    cube(&p, x, y, z);
                }
            } else {
                cube(&p, x, y, 0);
            }
        }
    }

    bufp = p;
    blit_flush_3d();
}
