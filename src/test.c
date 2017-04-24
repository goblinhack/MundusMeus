/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "glapi.h"
#include "geo.h"
#include "math_util.h"
#include <string.h>

/*
 * How many voxels per cube face edge
 */
#define VOX_RES             3

/*
 * Max size of the renderable level
 */
#define CUBE_W              7
#define CUBE_H              7
#define CUBE_Z              3

/*
 * How many voxels overall, including some overflow for the last voxel
 * on the edge of the last cube
 */
#define VOX_W               ((CUBE_W + 1) * VOX_RES)
#define VOX_H               ((CUBE_H + 1) * VOX_RES)
#define VOX_Z               ((CUBE_Z + 1) * VOX_RES)

/*
 * How many vertices. It is a bit more complex as corners share vertices.
 * I take the easy path and use the higher value.
 */
#define MAX_CUBES           ((CUBE_W + 1) * (CUBE_H + 1) * (CUBE_Z + 1))

/*
 * Four sides and top.
 */
#define VERTICES_PER_CUBE   (VOX_RES * VOX_RES * 5)
#define TRIANGLES_PER_CUBE  (VERTICES_PER_CUBE * 2)

#define MAX_VERTICES        (MAX_CUBES * VERTICES_PER_CUBE)
#define MAX_TRIANGLES       (MAX_CUBES * TRIANGLES_PER_CUBE)

#define MAX_LIGHT_BLOCKERS 100

struct triangle_t_;
struct cube_t_;

typedef struct {
    int light_blockers[MAX_LIGHT_BLOCKERS];
    int nlight_blockers;
    float r, g, b, a;
    fpoint3d p;
    float distance;
    uint8_t shadow:1;
    uint8_t solid:1;
} vertice_t;

typedef struct triangle_t_ {
    int vpis[3];
    float distance;
    uint8_t solid;
} triangle_t;

typedef struct cube_t_ {
    fpoint3d p;
    int tpis[TRIANGLES_PER_CUBE];
    int triangle_count;
    uint8_t solid;
} cube_t;

typedef struct {
    /*
     * Sorted vertices by distance from light
     */
    vertice_t vertices[MAX_VERTICES];
    int vi_sorted[MAX_VERTICES];
    int vertice_count;

    /*
     * Sorted triangles by distance from light
     */
    triangle_t triangles[MAX_TRIANGLES];
    int ti_sorted[MAX_TRIANGLES];
    int triangle_count;

    /*
     * We waste space here for ease of use with voxels that are not on the 
     * cube faces.
     */
    int vpis[VOX_W][VOX_H][VOX_Z];

    cube_t cubes[CUBE_W][CUBE_H][CUBE_Z];
} iso_t;

static iso_t iso;

void test(void);

/*
 * Compare two triangles
 */
static int
triangle_cmp (vertice_t *A, vertice_t *B, vertice_t *C,
              vertice_t *a, vertice_t *b, vertice_t *c)
{
    if (fcmp3d(A->p, a->p) && fcmp3d(B->p, b->p) && fcmp3d(C->p, a->p)) { return (1); }
    if (fcmp3d(A->p, a->p) && fcmp3d(B->p, b->p) && fcmp3d(C->p, c->p)) { return (1); }
    if (fcmp3d(A->p, b->p) && fcmp3d(B->p, c->p) && fcmp3d(C->p, a->p)) { return (1); }
    if (fcmp3d(A->p, b->p) && fcmp3d(B->p, c->p) && fcmp3d(C->p, b->p)) { return (1); }
    if (fcmp3d(A->p, c->p) && fcmp3d(B->p, a->p) && fcmp3d(C->p, b->p)) { return (1); }
    if (fcmp3d(A->p, c->p) && fcmp3d(B->p, a->p) && fcmp3d(C->p, c->p)) { return (1); }

    return (0);
}

/*
 * Add a triangle to the list; only if it is unique.
 */
static int
triangle_add (vertice_t * a, vertice_t * b, vertice_t * c)
{
    triangle_t *t;
    int i;

    for (i = 0; i < iso.triangle_count; i++) {
        triangle_t *o = &iso.triangles[i];

        vertice_t *A = &iso.vertices[o->vpis[0]];
        vertice_t *B = &iso.vertices[o->vpis[1]];
        vertice_t *C = &iso.vertices[o->vpis[2]];

        if (triangle_cmp(a, b, c, A, B, C)) {
            return (i);
        }
    }

    if (iso.triangle_count >= MAX_TRIANGLES) {
        DIE("could not add triangle");
    }

    t = &iso.triangles[iso.triangle_count];
    t->vpis[0] = a - iso.vertices;
    t->vpis[1] = b - iso.vertices;
    t->vpis[2] = c - iso.vertices;
    iso.triangle_count++;

    return (t - iso.triangles);
}

/*
 * Add p vertice to the list; only if it is unique.
 */
static vertice_t *
vertice_add (fpoint3d p)
{
    vertice_t *v;
    int i;

    for (i = 0; i < iso.vertice_count; i++) {
        vertice_t *o = &iso.vertices[i];

        if (fcmp3d(p, o->p)) {
            return (o);
        }
    }

    if (iso.vertice_count >= MAX_VERTICES) {
        DIE("could not add vertice");
    }

    v = &iso.vertices[iso.vertice_count];

    memset(v, 0, sizeof(*v));
    v->p = p;

    if ((p.x < 0) || (p.x > VOX_W) ||
        (p.y < 0) || (p.y > VOX_H) ||
        (p.z < 0) || (p.z > VOX_Z)) {
        DIE("overflow on vertice add at %f,%f,%f", p.x, p.y, p.z);
    }

    iso.vpis[(int)p.x][(int)p.y][(int)p.z] = iso.vertice_count++;

    return (v);
}

/*
 * Update the distance to the light for all triangles
 */
static void
triangle_set_distances (fpoint3d light)
{
    int i;

    for (i = 0; i < iso.triangle_count; i++) {
        triangle_t *t = &iso.triangles[i];

        vertice_t *A = &iso.vertices[t->vpis[0]];
        vertice_t *B = &iso.vertices[t->vpis[1]];
        vertice_t *C = &iso.vertices[t->vpis[2]];

        t->distance = DISTANCE3f(light.x, light.y, light.z, 
                                 A->p.x, A->p.y, A->p.z);
        t->distance = min(t->distance,
                          DISTANCE3f(light.x, light.y, light.z, 
                                     B->p.x, B->p.y, B->p.z));
        t->distance = min(t->distance,
                          DISTANCE3f(light.x, light.y, light.z, 
                                     C->p.x, C->p.y, C->p.z));
    }
}

/*
 * Update the distance to the light for all vertices
 */
static void
vertices_set_distances (fpoint3d light)
{
    int i;

    for (i = 0; i < iso.vertice_count; i++) {
        vertice_t *v = &iso.vertices[i];

        v->distance = DISTANCE3f(light.x, light.y, light.z, 
                                 v->p.x, v->p.y, v->p.z);
    }
}

/*
 * bubble sort of vertices by distance.
 */
static void
vertices_sort (void)
{
    int sorting;
    int i;

    for (i = 0; i < iso.vertice_count; i++) {
        iso.vi_sorted[i] = i;
    }

    do {
        sorting = false;

        int i;

        for (i = 0; i < iso.vertice_count-1; i++) {
            vertice_t *a = &iso.vertices[iso.vi_sorted[i]];
            vertice_t *b = &iso.vertices[iso.vi_sorted[i+1]];

            if (a->distance > b->distance) {
                swap(iso.vi_sorted[i], iso.vi_sorted[i+1]);
                sorting = true;
            }
        }
    } while (sorting);
}

/*
 * bubble sort of vertices by distance.
 */
static void
triangles_sort (void)
{
    int sorting;
    int i;

    for (i = 0; i < iso.triangle_count; i++) {
        iso.ti_sorted[i] = i;
    }

    do {
        sorting = false;

        int i;

        for (i = 0; i < iso.triangle_count-1; i++) {
            triangle_t *a = &iso.triangles[iso.ti_sorted[i]];
            triangle_t *b = &iso.triangles[iso.ti_sorted[i+1]];

            if (a->distance > b->distance) {
                swap(iso.ti_sorted[i], iso.ti_sorted[i+1]);
                sorting = true;
            }
        }
    } while (sorting);
}

/*
 * Create a pair of degenerate triangles (if needed) from the previous and 
 * next vertice. The 'if needed' part is fed to us when we know there is a 
 * line break.
 */
static void 
cube_degen_triangle (GLfloat **P,
                     uint8_t *tri_degen_needed)
{
    if (likely(!*tri_degen_needed)) {
        return;
    }

    GLfloat *p = *P;

    *tri_degen_needed = false;                             
                                                                
    memcpy(p + NUMBER_FLOATS_PER_VERTICE_3D,                    
           p - NUMBER_FLOATS_PER_VERTICE_3D,                    
           sizeof(float) * NUMBER_FLOATS_PER_VERTICE_3D);       

    memcpy(p,                                                   
           p - NUMBER_FLOATS_PER_VERTICE_3D,                    
           sizeof(float) * NUMBER_FLOATS_PER_VERTICE_3D);       
                                                                
    memcpy(p - NUMBER_FLOATS_PER_VERTICE_3D,                    
           p - NUMBER_FLOATS_PER_VERTICE_3D * 2,                
           sizeof(float) * NUMBER_FLOATS_PER_VERTICE_3D);       
                                                                
    p += NUMBER_FLOATS_PER_VERTICE_3D;                          
    p += NUMBER_FLOATS_PER_VERTICE_3D;                          

    *P = p;
}

static void
cube_render_vertice (GLfloat **P,
                     int Xvox, int Yvox, int Zvox,
                     int Xv, int Yv, int Zv,
                     int dx, int dy, int dz)
{
    GLfloat *p = *P;

    gl_push_texcoord(p, 0, 0);
    gl_push_vertex_3d(p, Xv + dx, Yv + dy, Zv + dz);

    vertice_t *v = &iso.vertices[iso.vpis[Xvox + dx][Yvox + dy][Zvox + dz]];
    if (!v) {
        DIE("no vertice at %d,%d,%d", Xvox + dx, Yvox + dy, Zvox + dz);
    }

    gl_push_rgba(p, v->r, v->g, v->b, v->a);

    *P = p;
}

static void 
cube_render (GLfloat **P, 
             uint8_t *tri_degen_needed,
             int Xcube, int Ycube, int Zcube)
{
    GLfloat *p = *P;
    int x, y, z;

    int Xc = Xcube - (CUBE_W - 1) / 2;
    int Yc = Ycube - (CUBE_H - 1) / 2;
    int Zc = Zcube - (CUBE_Z - 1) / 2;

    /*
     * Back left
     */
    for (y = 0; y < VOX_RES; y++) {
        x = 0;

        for (z = 0; z < VOX_RES; z++) {

            float Xv = (Xc * VOX_RES) + x;
            float Yv = (Yc * VOX_RES) + y;
            float Zv = (Zc * VOX_RES) + z;
            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 0);

            cube_degen_triangle(&p, tri_degen_needed);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 1, 0);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 1);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 1, 1);
        }

        *tri_degen_needed = true;
    }

    /*
     * Back right
     */
    for (x = 0; x < VOX_RES; x++) {
        y = 0;

        for (z = 0; z < VOX_RES; z++) {

            float Xv = (Xc * VOX_RES) + x;
            float Yv = (Yc * VOX_RES) + y;
            float Zv = (Zc * VOX_RES) + z;
            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 0);

            cube_degen_triangle(&p, tri_degen_needed);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                1, 0, 0);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 1);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                1, 0, 1);
        }

        *tri_degen_needed = true;
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
            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 0);

            cube_degen_triangle(&p, tri_degen_needed);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 1, 0);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 1);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 1, 1);
        }

        *tri_degen_needed = true;
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
            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 0);

            cube_degen_triangle(&p, tri_degen_needed);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                1, 0, 0);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 1);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                1, 0, 1);
        }

        *tri_degen_needed = true;
    }

    /*
     * Top part of voxel cube.
     */
    for (y = 0; y < VOX_RES; y++) {
        z = 0;

        for (x = 0; x < VOX_RES; x++) {

            float Xv = (Xc * VOX_RES) + x;
            float Yv = (Yc * VOX_RES) + y;
            float Zv = (Zc * VOX_RES) + z;
            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, VOX_RES);

            cube_degen_triangle(&p, tri_degen_needed);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 1, VOX_RES);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                1, 0, VOX_RES);

            cube_render_vertice(&p,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                1, 1, VOX_RES);
        }

        *tri_degen_needed = true;
    }

    *P = p;
}

/*
 * Add a triangle and all its verties uniquely.
 */
static void
triangle_populate (int Xcube, int Ycube, int Zcube,
                   int Xvox, int Yvox, int Zvox,
                   int dx1, int dy1, int dz1,
                   int dx2, int dy2, int dz2,
                   int dx3, int dy3, int dz3)
{
    if ((Xcube < 0) || (Xcube >= CUBE_W) ||
        (Ycube < 0) || (Ycube >= CUBE_H) ||
        (Zcube < 0) || (Zcube >= CUBE_Z)) {
        DIE("cube overflow %d,%d,%d", Xcube, Ycube, Zcube);
    }

    cube_t *cube = &iso.cubes[Xcube][Ycube][Zcube];

    cube->p.x = Xcube;
    cube->p.y = Ycube;
    cube->p.z = Zcube;

    vertice_t v1;
    vertice_t v2;
    vertice_t v3;

    memset(&v1, 0, sizeof(v1));
    memset(&v2, 0, sizeof(v2));
    memset(&v3, 0, sizeof(v3));

    v1.p.x = Xvox + dx1;
    v1.p.y = Yvox + dy1;
    v1.p.z = Zvox + dz1;

    v2.p.x = Xvox + dx2;
    v2.p.y = Yvox + dy2;
    v2.p.z = Zvox + dz2;

    v3.p.x = Xvox + dx3;
    v3.p.y = Yvox + dy3;
    v3.p.z = Zvox + dz3;

    vertice_t *A;
    vertice_t *B;
    vertice_t *C;

    A = vertice_add(v1.p);
    B = vertice_add(v2.p);
    C = vertice_add(v3.p);

    if (cube->triangle_count >= TRIANGLES_PER_CUBE) {
        DIE("overflow on triangle populate");
    }

    cube->tpis[cube->triangle_count++] = triangle_add(A, B, C);
}

static void 
cube_populate (int Xcube, int Ycube, int Zcube)
{
    int x, y, z;

    /*
     * Back left
     */
    for (y = 0; y < VOX_RES; y++) {
        x = 0;

        for (z = 0; z < VOX_RES; z++) {

            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            triangle_populate(Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 0, 0,
                              0, 1, 0,
                              0, 0, 1);

            triangle_populate(Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 1, 0,
                              0, 0, 1,
                              0, 1, 1);
        }
    }

    /*
     * Back right
     */
    for (x = 0; x < VOX_RES; x++) {
        y = 0;

        for (z = 0; z < VOX_RES; z++) {

            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            triangle_populate(Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 0, 0,
                              1, 0, 0,
                              0, 0, 1);

            triangle_populate(Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              1, 0, 0,
                              0, 0, 1,
                              1, 0, 1);
        }
    }

    /*
     * Right face of voxel cube.
     */
    for (y = 0; y < VOX_RES; y++) {
        x = VOX_RES;

        for (z = 0; z < VOX_RES; z++) {

            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            triangle_populate(Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 0, 0,
                              0, 1, 0,
                              0, 0, 1);

            triangle_populate(Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 1, 0,
                              0, 0, 1,
                              0, 1, 1);
        }
    }

    /*
     * Left face of voxel cube.
     */
    for (x = 0; x < VOX_RES; x++) {
        y = VOX_RES;

        for (z = 0; z < VOX_RES; z++) {

            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            triangle_populate(Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 0, 0,
                              1, 0, 0,
                              0, 0, 1);

            triangle_populate(Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              1, 0, 0,
                              0, 0, 1,
                              1, 0, 1);
        }
    }

    /*
     * Top part of voxel cube.
     */
    for (y = 0; y < VOX_RES; y++) {
        z = 0;

        for (x = 0; x < VOX_RES; x++) {

            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            triangle_populate(Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 0, VOX_RES,
                              0, 1, VOX_RES,
                              1, 0, VOX_RES);

            triangle_populate(Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 1, VOX_RES,
                              1, 0, VOX_RES,
                              1, 1, VOX_RES);
        }
    }
}

/*
 * For each vertice, walk all triangles blocking it heading to the light 
 * source.
 */
static void 
vertices_walk_light_blockers (fpoint3d light)
{
    int x, y, z, i, j, k;

    for (x = 0; x < CUBE_W; x++) {
        for (y = 0; y < CUBE_H; y++) {
            for (z = 0; z < CUBE_Z; z++) {

                cube_t *cube = &iso.cubes[x][y][z];
                if (!cube->solid) {
                    continue;
                }

                for (i = 0; i < cube->triangle_count; i++) {
                    triangle_t *t = &iso.triangles[cube->tpis[i]];

                    for (j = 0; j < 3; j++) {
                        vertice_t *v = &iso.vertices[t->vpis[j]];

                        if (!v->solid) {
                            continue;
                        }

                        for (k = 0; k < v->nlight_blockers; k++) {
                            triangle_t *t;

                            t = &iso.triangles[v->light_blockers[k]];

                            if (t->solid) {
                                v->shadow = 1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < iso.vertice_count; i++) {
        vertice_t *v = &iso.vertices[iso.vi_sorted[i]];

        if (v->shadow) {
            v->r = 0.1;
            v->g = 0.1;
            v->b = 0.1;
            v->a = 1.0;
            continue;
        }

        v->r = 1.0;
        v->g = 1.0;
        v->b = 1.0;
        v->a = 1.0;
    }
}

static void 
cubes_set_solid (cube_t *cube, int solid)
{
    int i, j;

    if (solid) {
        cube->solid = 1;
    }

    for (i = 0; i < cube->triangle_count; i++) {
        triangle_t *t = &iso.triangles[cube->tpis[i]];

        if (solid) {
            t->solid = 1;
        }

        for (j = 0; j < 3; j++) {
            vertice_t *v = &iso.vertices[t->vpis[j]];
            if (solid) {
                v->solid = 1;
            }
        }
    }
}

static void 
cubes_render (fpoint3d light)
{
    uint8_t tri_degen_needed = false;
    GLfloat *p = bufp;
    int x;
    int y;
    int z;
    for (x = 0; x < CUBE_W; x++) {
        for (y = 0; y < CUBE_H; y++) {
            for (z = 0; z < CUBE_Z; z++) {
                cube_t *cube = &iso.cubes[x][y][z];

                if (cube->solid) {
                    cube_render(&p, &tri_degen_needed, x, y, z);
                }
            }
        }
    }

    bufp = p;
}

static void vertices_find_light_blockers (fpoint3d light)
{
    int light_blockers_max = 0;
    int i, j;

    line L;
    L.P0 = light;

    for (i = 0; i < iso.vertice_count; i++) {
        vertice_t *v = &iso.vertices[iso.vi_sorted[i]];

        v->nlight_blockers = 0;

        for (j = 0; j < iso.triangle_count; j++) {
            triangle_t *t = &iso.triangles[iso.ti_sorted[j]];

            if (t->distance > v->distance) {
                break;
            }

            triangle T;

            T.V0 = iso.vertices[t->vpis[0]].p;
            T.V1 = iso.vertices[t->vpis[1]].p;
            T.V2 = iso.vertices[t->vpis[2]].p;

            if (fcmp3d(v->p, T.V0) ||
                fcmp3d(v->p, T.V1) ||
                fcmp3d(v->p, T.V2)) {
                continue;
            }

            L.P1 = v->p;

            fpoint3d intersection;

            if (triangle_line_intersect(L, T, &intersection) == 1) {
                if (v->nlight_blockers >= MAX_LIGHT_BLOCKERS) {
                    DIE("too many light blockers for (%f, %f, %f)",
                        v->p.x, v->p.y, v->p.z);
                }

                v->light_blockers[v->nlight_blockers++] = t - iso.triangles;

                light_blockers_max = max(v->nlight_blockers, light_blockers_max);
            }
        }
    }
printf("light_blockers %u\n", light_blockers_max);
}

static void 
cubes_init (fpoint3d light)
{
    int x;
    int y;
    int z;

    mysrand(10);

    for (x = 0; x < CUBE_W; x++) {
        for (y = 0; y < CUBE_H; y++) {
            for (z = 0; z < CUBE_Z; z++) {
                cube_populate(x, y, z);
            }
        }
    }

    triangle_set_distances(light);
    vertices_set_distances(light);
    vertices_sort();
    triangles_sort();
    vertices_find_light_blockers(light);

    for (x = 0; x < CUBE_W; x++) {
        for (y = 0; y < CUBE_H; y++) {
            for (z = 0; z < CUBE_Z; z++) {

                cube_t *cube = &iso.cubes[x][y][z];

                uint8_t solid = false;

                if ((x == CUBE_W / 2) && (y == CUBE_H / 2)) {
                    solid = true;
                }

                if ((x == CUBE_W / 4) && (y == CUBE_H / 3)) {
                    solid = true;
                }

                if (z == 0) {
                    solid = true;
                }

                cubes_set_solid(cube, solid);
            }
        }
    }
}

void test (void)
{
    static int done;

    fpoint3d light = {VOX_W  - 2, 0 , VOX_Z / 2};

    if (!done) {
printf("%lu\n", sizeof(iso) / (1024 * 1024));
printf(".%lu\n", sizeof(iso.cubes));
printf(".%lu\n", sizeof(iso.triangles));
printf(".%lu\n", sizeof(iso.vertices));
        done = true;
        cubes_init(light);
    }

    vertices_walk_light_blockers(light);

    glBindTexture(GL_TEXTURE_2D, 0);

    int x = 1;
    if (x) {
    blit_init();
    cubes_render(light);
    blit_flush_3d();
    }

//        SDL_Delay(200);
}
