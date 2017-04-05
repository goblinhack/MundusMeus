/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "glapi.h"

#define CUBE_RES    5

#define ISO_W       20
#define ISO_H       20
#define ISO_Z       10

#define VOX_W       ((ISO_W + 1) * CUBE_RES)
#define VOX_H       ((ISO_H + 1) * CUBE_RES)
#define VOX_Z       ((ISO_Z + 1) * CUBE_RES)

typedef struct {
    float r;
    float g;
    float b;
    float a;
} vertice_t;

static vertice_t vertices[VOX_W][VOX_H][VOX_Z];

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

#define GL_CUBE_VERTICE(x, y, z, dx, dy, dz)                    \
{                                                               \
    gl_push_texcoord(p, 0, 0);                                  \
    {                                                           \
        float X = (cx * CUBE_RES) + x;                          \
        float Y = (cy * CUBE_RES) + y;                          \
        float Z = (cz * CUBE_RES) + z;               \
        gl_push_vertex_3d(p, X + dx, Y + dy, Z + dz);           \
    }                                                           \
    {                                                           \
        int X = (ox * CUBE_RES) + x;                            \
        int Y = (oy * CUBE_RES) + y;                            \
        int Z = (oz * CUBE_RES) + z;                 \
        vertice_t *v = &vertices[X + dx][Y + dy][Z + dz];       \
        gl_push_rgba(p, v->r, v->g, v->b, v->a);                \
    }                                                           \
}

static void cube (GLfloat **P, int ox, int oy, int oz)
{
    GLfloat *p = *P;
    int x, y, z;

    int cx = ox - (ISO_W - 1) / 2;
    int cy = oy - (ISO_H - 1) / 2;
    int cz = oz - (ISO_Z - 1) / 2;

    /*
     * Top part of voxel cube.
     */
    for (y = 0; y < CUBE_RES; y++) {
        z = 0;

        for (x = 0; x < CUBE_RES; x++) {
            GL_CUBE_VERTICE(x, y, z, 0, 0, CUBE_RES)
            GL_DEGEN_TRIANGLE_IF_NEEDED()
            GL_CUBE_VERTICE(x, y, z, 0, 1, CUBE_RES)
            GL_CUBE_VERTICE(x, y, z, 1, 0, CUBE_RES)
            GL_CUBE_VERTICE(x, y, z, 1, 1, CUBE_RES)
        }

        GL_DEGEN_TRIANGLE_NEEDED()
    }

    /*
     * Right face of voxel cube.
     */
    for (y = 0; y < CUBE_RES; y++) {
        x = CUBE_RES;

        for (z = 0; z < CUBE_RES; z++) {
            GL_CUBE_VERTICE(x, y, z, 0, 0, 0)
            GL_DEGEN_TRIANGLE_IF_NEEDED()
            GL_CUBE_VERTICE(x, y, z, 0, 1, 0)
            GL_CUBE_VERTICE(x, y, z, 0, 0, 1)
            GL_CUBE_VERTICE(x, y, z, 0, 1, 1)
        }

        GL_DEGEN_TRIANGLE_NEEDED()
    }

    /*
     * Left face of voxel cube.
     */
    for (x = 0; x < CUBE_RES; x++) {
        y = CUBE_RES;

        for (z = 0; z < CUBE_RES; z++) {
            GL_CUBE_VERTICE(x, y, z, 0, 0, 0)
            GL_DEGEN_TRIANGLE_IF_NEEDED()
            GL_CUBE_VERTICE(x, y, z, 1, 0, 0)
            GL_CUBE_VERTICE(x, y, z, 0, 0, 1)
            GL_CUBE_VERTICE(x, y, z, 1, 0, 1)
        }

        GL_DEGEN_TRIANGLE_NEEDED()
    }

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

    mysrand(10);
    static int done;

    if (!done) {
        done = true;

        for (x = 0; x < VOX_W; x++) {
            for (y = 0; y < VOX_H; y++) {
                for (z = 0; z < VOX_Z; z++) {
                    vertice_t *v;

                    v = &vertices[x][y][z];

                    v->r = 0.0 + x * 0.02;
                    v->g = 0.0 + y * 0.02;
                    v->b = 0.0 + z * 0.02;
                    v->a = 1.0;

                    if (z >= ISO_Z * CUBE_RES) {
                        v->r = 1.0;
                        v->g = 0.0;
                        v->b = 0.0;
                        v->a = 1.0;
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
            } else if ((x == ISO_W / 2) || (y == ISO_H / 2)) {
                for (z = 0; z < ISO_Z / 2; z++) {
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
