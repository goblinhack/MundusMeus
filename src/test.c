/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "glapi.h"

#define VOX_RES    6

#define CUBE_W      20
#define CUBE_H      20
#define CUBE_Z      10

#define VOX_W       ((CUBE_W + 1) * VOX_RES)
#define VOX_H       ((CUBE_H + 1) * VOX_RES)
#define VOX_Z       ((CUBE_Z + 1) * VOX_RES)

typedef struct {
    float r, g, b, a;
    fpoint3d p;
} vertice_t;

typedef struct {
    vertice_t *v[3];
} triangle_t;

typedef struct {
    triangle_t t[VOX_RES * VOX_RES * 3 * 2];
    unsigned short triangle_count;
} cube_t;

static vertice_t vertices[VOX_W][VOX_H][VOX_Z];
static cube_t cubes[CUBE_W][CUBE_H][CUBE_Z];

void test(void);

static short gl_need_degen_triangle;

#define GL_DEGEN_TRIANGLE_NEEDED()                              \
    gl_need_degen_triangle = true;

#define GL_DEGEN_TRIANGLE_IF_NEEDED()                           \
                                                                \
if (unlikely(gl_need_degen_triangle))                           \
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

static void 
cube_vertice (GLfloat **P,
              short Xvox, short Yvox, short Zvox,
              short Xv, short Yv, short Zv,
              short dx, short dy, short dz)
{                                                               
    GLfloat *p = *P;

    gl_push_texcoord(p, 0, 0);                                  
    gl_push_vertex_3d(p, Xv + dx, Yv + dy, Zv + dz);        

    vertice_t *v = &vertices[Xvox + dx][Yvox + dy][Zvox + dz];    

    gl_push_rgba(p, v->r, v->g, v->b, v->a);                
    *P = p;
}

static void cube (GLfloat **P, short Xcube, short Ycube, short Zcube)
{
    GLfloat *p = *P;
    short x, y, z;

    short Xc = Xcube - (CUBE_W - 1) / 2;
    short Yc = Ycube - (CUBE_H - 1) / 2;
    short Zc = Zcube - (CUBE_Z - 1) / 2;

    /*
     * Top part of voxel cube.
     */
    for (y = 0; y < VOX_RES; y++) {
        z = 0;

        for (x = 0; x < VOX_RES; x++) {

            float Xv = (Xc * VOX_RES) + x;
            float Yv = (Yc * VOX_RES) + y;
            float Zv = (Zc * VOX_RES) + z;
            short Xvox = (Xcube * VOX_RES) + x;
            short Yvox = (Ycube * VOX_RES) + y;
            short Zvox = (Zcube * VOX_RES) + z;

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         0, 0, VOX_RES);

            GL_DEGEN_TRIANGLE_IF_NEEDED()

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         0, 1, VOX_RES);

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         1, 0, VOX_RES);

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         1, 1, VOX_RES);
        }

        GL_DEGEN_TRIANGLE_NEEDED()
    }

    /*
     * Right face of voxel cube.
     */
    for (y = 0; y < VOX_RES; y++) {
        x = VOX_RES;

        for (z = 0; z < VOX_RES; z++) {

            float Xv = (Xc * VOX_RES) + x;
            float Yv = (Yc * VOX_RES) + y;
            float Zv = (Zc * VOX_RES) + z;
            short Xvox = (Xcube * VOX_RES) + x;
            short Yvox = (Ycube * VOX_RES) + y;
            short Zvox = (Zcube * VOX_RES) + z;

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         0, 0, 0);

            GL_DEGEN_TRIANGLE_IF_NEEDED()

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         0, 1, 0);

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         0, 0, 1);

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         0, 1, 1);
        }

        GL_DEGEN_TRIANGLE_NEEDED()
    }

    /*
     * Left face of voxel cube.
     */
    for (x = 0; x < VOX_RES; x++) {
        y = VOX_RES;

        for (z = 0; z < VOX_RES; z++) {

            float Xv = (Xc * VOX_RES) + x;
            float Yv = (Yc * VOX_RES) + y;
            float Zv = (Zc * VOX_RES) + z;
            short Xvox = (Xcube * VOX_RES) + x;
            short Yvox = (Ycube * VOX_RES) + y;
            short Zvox = (Zcube * VOX_RES) + z;

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         0, 0, 0);

            GL_DEGEN_TRIANGLE_IF_NEEDED()

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         1, 0, 0);

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         0, 0, 1);

            cube_vertice(&p,
                         Xvox, Yvox, Zvox,
                         Xv, Yv, Zv,
                         1, 0, 1);
        }

        GL_DEGEN_TRIANGLE_NEEDED()
    }

    *P = p;
}

static void 
triangle_populate (short Xcube, short Ycube, short Zcube,
                   short Xvox, short Yvox, short Zvox,
                   short dx1, short dy1, short dz1,
                   short dx2, short dy2, short dz2,
                   short dx3, short dy3, short dz3)
{                                                                   
    cube_t *c = &cubes[Xcube][Ycube][Zcube];                        
                                                                    
    if (c->triangle_count >= ARRAY_SIZE(c->t)) {                    
        DIE("overflow");                                            
    }                                                               
                                                                    
    triangle_t *t = &c->t[c->triangle_count++];                     
                                                                    
    vertice_t *v1 = &vertices[Xvox + dx1][Yvox + dy1][Zvox + dz1];  
    vertice_t *v2 = &vertices[Xvox + dx2][Yvox + dy2][Zvox + dz2];  
    vertice_t *v3 = &vertices[Xvox + dx3][Yvox + dy3][Zvox + dz3];  
                                                                    
    t->v[0] = v1;                                                   
    t->v[1] = v2;                                                   
    t->v[2] = v3;                                                   
                                                                    
    v1->p.x = Xvox + dx1;                                           
    v1->p.y = Yvox + dy1;                                           
    v1->p.z = Zvox + dz1;                                           
    v2->p.x = Xvox + dx2;                                           
    v2->p.y = Yvox + dy2;                                           
    v2->p.z = Zvox + dz2;                                           
    v3->p.x = Xvox + dx3;                                           
    v3->p.y = Yvox + dy3;                                           
    v3->p.z = Zvox + dz3;                                           
}

static void 
cube_populate (short Xcube, short Ycube, short Zcube)
{
    short x, y, z;

    /*
     * Top part of voxel cube.
     */
    for (y = 0; y < VOX_RES; y++) {
        z = 0;

        for (x = 0; x < VOX_RES; x++) {

            short Xvox = (Xcube * VOX_RES) + x;
            short Yvox = (Ycube * VOX_RES) + y;
            short Zvox = (Zcube * VOX_RES) + z;

            triangle_populate(Xcube, Ycube, Zcube,
                         Xvox, Yvox, Zvox,
                         0, 0, VOX_RES, 
                         0, 1, VOX_RES, 
                         1, 0, VOX_RES);

            triangle_populate(Xcube, Ycube, Zcube,
                         Xvox, Yvox, Zvox,
                         1, 0, VOX_RES, 
                         0, 1, VOX_RES, 
                         1, 1, VOX_RES);
        }
    }

    /*
     * Right face of voxel cube.
     */
    for (y = 0; y < VOX_RES; y++) {
        x = VOX_RES;

        for (z = 0; z < VOX_RES; z++) {

            short Xvox = (Xcube * VOX_RES) + x;
            short Yvox = (Ycube * VOX_RES) + y;
            short Zvox = (Zcube * VOX_RES) + z;

            triangle_populate(Xcube, Ycube, Zcube,
                         Xvox, Yvox, Zvox,
                         0, 0, 0, 
                         0, 1, 0, 
                         0, 0, 1);

            triangle_populate(Xcube, Ycube, Zcube,
                         Xvox, Yvox, Zvox,
                         0, 0, 1, 
                         0, 1, 0, 
                         0, 1, 1);
        }
    }

    /*
     * Left face of voxel cube.
     */
    for (x = 0; x < VOX_RES; x++) {
        y = VOX_RES;

        for (z = 0; z < VOX_RES; z++) {

            short Xvox = (Xcube * VOX_RES) + x;
            short Yvox = (Ycube * VOX_RES) + y;
            short Zvox = (Zcube * VOX_RES) + z;

            triangle_populate(Xcube, Ycube, Zcube,
                         Xvox, Yvox, Zvox,
                         0, 0, 0, 
                         1, 0, 0, 
                         0, 0, 1);

            triangle_populate(Xcube, Ycube, Zcube,
                         Xvox, Yvox, Zvox,
                         0, 0, 1, 
                         1, 0, 0, 
                         1, 0, 1);
        }
    }
}

void test (void)
{
    glBindTexture(GL_TEXTURE_2D, 0);

    blit_init();

    GLfloat *p = bufp;

    short x;
    short y;
    short z;

    mysrand(10);
    static short done;

    if (!done) {
        done = true;

        for (x = 0; x < CUBE_W; x++) {
            for (y = 0; y < CUBE_H; y++) {
                for (z = 0; z < CUBE_Z; z++) {
                    cube_populate(x, y, z);
                }
            }
        }

        for (x = 0; x < VOX_W; x++) {
            for (y = 0; y < VOX_H; y++) {
                for (z = 0; z < VOX_Z; z++) {
                    vertice_t *v;

                    v = &vertices[x][y][z];

                    v->r = 0.0 + x * 0.02;
                    v->g = 0.0 + y * 0.02;
                    v->b = 0.0 + z * 0.02;
                    v->a = 1.0;

                    if (z >= CUBE_Z * VOX_RES) {
                        v->r = 1.0;
                        v->g = 0.0;
                        v->b = 0.0;
                        v->a = 1.0;
                    }
                }
            }
        }
    }
    for (x = 0; x < CUBE_W; x++) {
        for (y = 0; y < CUBE_H; y++) {
            if ((x == 0) || (y == 0)) {
                for (z = 0; z < CUBE_Z; z++) {
                    cube(&p, x, y, z);
                }
            } else if ((x == CUBE_W / 2) || (y == CUBE_H / 2)) {
                for (z = 0; z < CUBE_Z / 2; z++) {
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
