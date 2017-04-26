/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "glapi.h"
#include "geo.h"
#include "math_util.h"
#include "tile.h"
#include <string.h>
#include <SDL.h>

/*
 * How many voxels per cube face edge
 */
#define VOX_RES             1

/*
 * Max size of the renderable level
 */
#define CUBE_W              21
#define CUBE_H              21
#define CUBE_Z              10

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

#define MAX_VERTICES        65535 // (MAX_CUBES * VERTICES_PER_CUBE)
#define MAX_TRIANGLES       65535 // (MAX_CUBES * TRIANGLES_PER_CUBE)

#define MAX_LIGHT_BLOCKERS  192

struct triangle_t_;
struct cube_t_;

typedef struct {
    uint16_t light_blockers[MAX_LIGHT_BLOCKERS];
    spoint3d p;
    float distance;
    uint8_t r, g, b, a;
    uint16_t nlight_blockers;
    uint8_t shadow:7;
    uint8_t solid:1;
} vertice_t;

typedef struct triangle_t_ {
    uint16_t vpis[3];
    float distance;
    uint8_t solid:1;
} triangle_t;

typedef struct cube_t_ {
    uint16_t tpis[TRIANGLES_PER_CUBE];
    uint8_t solid:1;
} cube_t;

typedef struct {
    /*
     * Sorted vertices by distance from light
     */
    vertice_t vertices[MAX_VERTICES];
    uint16_t vi_sorted[MAX_VERTICES];
    uint16_t vertice_count;

    /*
     * Sorted triangles by distance from light
     */
    triangle_t triangles[MAX_TRIANGLES];
    uint16_t ti_sorted[MAX_TRIANGLES];
    uint16_t triangle_count;

    /*
     * We waste space here for ease of use with voxels that are not on the 
     * cube faces.
     */
    uint16_t vpis[VOX_W][VOX_H][VOX_Z];

    cube_t cubes[CUBE_W][CUBE_H][CUBE_Z];
} iso_t;

static int map[MAP_WIDTH][MAP_HEIGHT][CUBE_Z];

static iso_t iso;

void test(void);

/*
 * Compare two triangles
 */
static int
triangle_cmp (vertice_t *A, vertice_t *B, vertice_t *C,
              vertice_t *a, vertice_t *b, vertice_t *c)
{
    if (scmp3d(A->p, a->p) && scmp3d(B->p, b->p) && scmp3d(C->p, a->p)) { return (1); }
    if (scmp3d(A->p, a->p) && scmp3d(B->p, b->p) && scmp3d(C->p, c->p)) { return (1); }
    if (scmp3d(A->p, b->p) && scmp3d(B->p, c->p) && scmp3d(C->p, a->p)) { return (1); }
    if (scmp3d(A->p, b->p) && scmp3d(B->p, c->p) && scmp3d(C->p, b->p)) { return (1); }
    if (scmp3d(A->p, c->p) && scmp3d(B->p, a->p) && scmp3d(C->p, b->p)) { return (1); }
    if (scmp3d(A->p, c->p) && scmp3d(B->p, a->p) && scmp3d(C->p, c->p)) { return (1); }

    return (0);
}

/*
 * Add a triangle to the list; only if it is unique.
 */
static int
triangle_add (vertice_t * a, vertice_t * b, vertice_t * c)
{
    triangle_t *t;
    uint32_t i;
    static const int hash_size = MAX_TRIANGLES;
    static uint8_t hash[hash_size];

    uint32_t hi;
    uint32_t shift = 131;

    hi = a->p.x * CUBE_W * VOX_RES;
    hi *= shift;
    hi += a->p.y * CUBE_H * VOX_RES;
    hi *= shift;
    hi += a->p.z * CUBE_Z * VOX_RES;
    hi *= shift;
    hi += b->p.x * CUBE_W * VOX_RES;
    hi *= shift;
    hi += b->p.y * CUBE_H * VOX_RES;
    hi *= shift;
    hi += b->p.z * CUBE_Z * VOX_RES;
    hi *= shift;
    hi += c->p.x * CUBE_W * VOX_RES;
    hi *= shift;
    hi += c->p.y * CUBE_H * VOX_RES;
    hi *= shift;
    hi += c->p.z * CUBE_Z * VOX_RES;
    hi *= shift;
    hi %= hash_size;

    if (!hash[hi]) {
        hash[hi] = 1;
    } else {
        for (i = 0; i < iso.triangle_count; i++) {
            triangle_t *o = &iso.triangles[i];

            vertice_t *A = &iso.vertices[o->vpis[0]];
            vertice_t *B = &iso.vertices[o->vpis[1]];
            vertice_t *C = &iso.vertices[o->vpis[2]];

            if (triangle_cmp(a, b, c, A, B, C)) {
                return (i);
            }
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
vertice_add (spoint3d p)
{
    vertice_t *v;
    uint32_t i;

    i = iso.vpis[(int)p.x][(int)p.y][(int)p.z];
    if (i) {
        vertice_t *o = &iso.vertices[i];
        return (o);
    }

#if 0
    for (i = 0; i < iso.vertice_count; i++) {
        vertice_t *o = &iso.vertices[i];

        if (fcmp3d(p, o->p)) {
            return (o);
        }
    }
#endif

    if (iso.vertice_count >= MAX_VERTICES) {
        DIE("could not add vertice");
    }

    v = &iso.vertices[iso.vertice_count];

    memset(v, 0, sizeof(*v));
    v->p = p;

    if ((p.x < 0) || (p.x > VOX_W) ||
        (p.y < 0) || (p.y > VOX_H) ||
        (p.z < 0) || (p.z > VOX_Z)) {
        DIE("overflow on vertice add at %d,%d,%d", p.x, p.y, p.z);
    }

    iso.vpis[(int)p.x][(int)p.y][(int)p.z] = iso.vertice_count++;

    return (v);
}

/*
 * Update the distance to the light for all triangles
 */
static void
triangle_set_distances (spoint3d light)
{
    uint32_t i;

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
vertices_set_distances (spoint3d light)
{
    uint32_t i;

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
    uint32_t i;

    for (i = 0; i < iso.vertice_count; i++) {
        iso.vi_sorted[i] = i;
    }

    do {
        sorting = false;

        uint32_t i;

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
    uint32_t i;

    for (i = 0; i < iso.triangle_count; i++) {
        iso.ti_sorted[i] = i;
    }

    do {
        sorting = false;

        uint32_t i;

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
					 float tx, float ty,
                     int Xvox, int Yvox, int Zvox,
                     int Xv, int Yv, int Zv,
                     int dx, int dy, int dz)
{
    GLfloat *p = *P;

    static tilep tile;
    if (!tile) {
        tile = tile_find("iso_rock");
	}

	float tx1 = tile->x1;
	float ty1 = tile->y1;
	float tw = tile->x2 - tile->x1;
	float th = tile->y2 - tile->y1;

    buf_tex = tile->gl_surface_binding;

    gl_push_texcoord(p, tx1 + tw * tx, ty1 + th * ty);

    gl_push_vertex_3d(p, Xv + dx, Yv + dy, Zv + dz);

    vertice_t *v = &iso.vertices[iso.vpis[Xvox + dx][Yvox + dy][Zvox + dz]];
    if (!v) {
        DIE("no vertice at %d,%d,%d", Xvox + dx, Yvox + dy, Zvox + dz);
    }

    gl_push_rgba(p, 
                 ((double)v->r) / 255.0, 
                 ((double)v->g) / 255.0, 
                 ((double)v->b) / 255.0, 
                 ((double)v->a) / 255.0);
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

#if 0
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
#endif

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
								1.00, 0.75,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 0);

            cube_degen_triangle(&p, tri_degen_needed);

            cube_render_vertice(&p,
							    0.50, 1.00,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 1, 0);

            cube_render_vertice(&p,
							    1.00, 0.25,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 1);

            cube_render_vertice(&p,
								0.50, 0.50,
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
								0.00, 0.75,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 0);

            cube_degen_triangle(&p, tri_degen_needed);

            cube_render_vertice(&p,
							    0.50, 1.00,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                1, 0, 0);

            cube_render_vertice(&p,
							    0.00, 0.25,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, 1);

            cube_render_vertice(&p,
								0.50, 0.50,
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
								0.5, 0.0,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 0, VOX_RES);

            cube_degen_triangle(&p, tri_degen_needed);

            cube_render_vertice(&p,
								0.0, 0.25,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                0, 1, VOX_RES);

            cube_render_vertice(&p,
                                1.0, 0.25,
                                Xvox, Yvox, Zvox,
                                Xv, Yv, Zv,
                                1, 0, VOX_RES);

            cube_render_vertice(&p,
								0.5, 0.5,
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
triangle_populate (int *triangle_count,
                   int Xcube, int Ycube, int Zcube,
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

    if (*triangle_count >= TRIANGLES_PER_CUBE) {
        DIE("overflow on triangle populate");
    }

    cube->tpis[*triangle_count] = triangle_add(A, B, C);

    (*triangle_count)++;
}

static void 
cube_populate (int Xcube, int Ycube, int Zcube)
{
    int x, y, z;
    int triangle_count = 0;

    /*
     * Back left
     */
    for (y = 0; y < VOX_RES; y++) {
        x = 0;

        for (z = 0; z < VOX_RES; z++) {

            int Xvox = (Xcube * VOX_RES) + x;
            int Yvox = (Ycube * VOX_RES) + y;
            int Zvox = (Zcube * VOX_RES) + z;

            triangle_populate(&triangle_count,
                              Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 0, 0,
                              0, 1, 0,
                              0, 0, 1);

            triangle_populate(&triangle_count,
                              Xcube, Ycube, Zcube,
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

            triangle_populate(&triangle_count,
                              Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 0, 0,
                              1, 0, 0,
                              0, 0, 1);

            triangle_populate(&triangle_count,
                              Xcube, Ycube, Zcube,
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

            triangle_populate(&triangle_count,
                              Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 0, 0,
                              0, 1, 0,
                              0, 0, 1);

            triangle_populate(&triangle_count,
                              Xcube, Ycube, Zcube,
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

            triangle_populate(&triangle_count,
                              Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 0, 0,
                              1, 0, 0,
                              0, 0, 1);

            triangle_populate(&triangle_count,
                              Xcube, Ycube, Zcube,
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

            triangle_populate(&triangle_count,
                              Xcube, Ycube, Zcube,
                              Xvox, Yvox, Zvox,
                              0, 0, VOX_RES,
                              0, 1, VOX_RES,
                              1, 0, VOX_RES);

            triangle_populate(&triangle_count,
                              Xcube, Ycube, Zcube,
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
vertices_walk_light_blockers (spoint3d light)
{
    int x, y, z;
    uint32_t i, j, k;

    for (i = 0; i < iso.vertice_count; i++) {
        vertice_t *v = &iso.vertices[iso.vi_sorted[i]];

        v->shadow = 0;
    }

    for (x = 0; x < CUBE_W; x++) {
        for (y = 0; y < CUBE_H; y++) {
            for (z = 0; z < CUBE_Z; z++) {

                cube_t *cube = &iso.cubes[x][y][z];
                if (!cube->solid) {
                    continue;
                }

                for (i = 0; i < TRIANGLES_PER_CUBE; i++) {
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
                                v->shadow++;
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
			double s = v->shadow;
            v->r = 100.0 / s;
            v->g = 100.0 / s;
            v->b = 100.0 / s;
            v->a = 255;
            continue;
        }

        v->r = 255; // v->p.x * 50;
        v->g = 255; // v->p.y * 40;
        v->b = 255; // v->p.z * 30;
        v->a = 255;
    }
}

static void 
cubes_set_solid (cube_t *cube, int solid)
{
    uint32_t i, j;

	cube->solid = solid ? 1 : 0;

    for (i = 0; i < TRIANGLES_PER_CUBE; i++) {
        triangle_t *t = &iso.triangles[cube->tpis[i]];

		t->solid = solid ? 1 : 0;

        for (j = 0; j < 3; j++) {
            vertice_t *v = &iso.vertices[t->vpis[j]];
			v->solid = solid ? 1 : 0;
        }
    }
}

static void 
cubes_render (spoint3d light)
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

static void vertices_find_light_blockers (spoint3d light)
{
    uint32_t light_blockers_max = 0;
    uint32_t i, j;

    line L;

    L.P0.x = light.x;
    L.P0.y = light.y;
    L.P0.z = light.z;

    for (i = 0; i < iso.vertice_count; i++) {
        vertice_t *v = &iso.vertices[iso.vi_sorted[i]];

        v->nlight_blockers = 0;

        for (j = 0; j < iso.triangle_count; j++) {
            triangle_t *t = &iso.triangles[iso.ti_sorted[j]];

            if (t->distance > v->distance) {
                break;
            }

            triangle T;

            spoint3d a = iso.vertices[t->vpis[0]].p;
            spoint3d b = iso.vertices[t->vpis[1]].p;
            spoint3d c = iso.vertices[t->vpis[2]].p;

            T.V0.x = a.x;
            T.V0.y = a.y;
            T.V0.z = a.z;

            T.V1.x = b.x;
            T.V1.y = b.y;
            T.V1.z = b.z;

            T.V2.x = c.x;
            T.V2.y = c.y;
            T.V2.z = c.z;

            if (scmp3d(v->p, a) ||
                scmp3d(v->p, b) ||
                scmp3d(v->p, c)) {
                continue;
            }

            L.P1.x = v->p.x;
            L.P1.y = v->p.y;
            L.P1.z = v->p.z;

            fpoint3d intersection;

            if (triangle_line_intersect(L, T, &intersection) == 1) {
                if (v->nlight_blockers >= MAX_LIGHT_BLOCKERS) {
                    DIE("too many light blockers for (%d, %d, %d)",
                        v->p.x, v->p.y, v->p.z);
                }

                v->light_blockers[v->nlight_blockers++] = t - iso.triangles;

                light_blockers_max = max(v->nlight_blockers, light_blockers_max);
            }
        }
    }
printf("iso %lu\n", sizeof(iso));
printf("light_blockers %u\n", light_blockers_max);
printf("triangle_count %u\n", iso.triangle_count);
printf("vertice_count %u\n", iso.vertice_count);
}

static void 
cubes_init (spoint3d light)
{
    int x;
    int y;
    int z;

    mysrand(10);

    for (z = 0; z < CUBE_Z; z++) {
        for (x = 0; x < CUBE_W; x++) {
            for (y = 0; y < CUBE_H; y++) {
                cube_populate(x, y, z);
            }
        }
    }

    triangle_set_distances(light);
    vertices_set_distances(light);
    vertices_sort();
    triangles_sort();
    vertices_find_light_blockers(light);
}

static void 
map_init (void)
{
    int x;
    int y;
    int z;

    for (x = 0; x < MAP_WIDTH; x++) {
        for (y = 0; y < MAP_HEIGHT; y++) {
			int r = myrand() % 100;
			int R = 0;
			if (r < 5) {
				R = myrand() % 5;
			}

            for (z = 0; z < CUBE_Z; z++) {

                uint8_t solid = false;

				if (z < R) {
                    solid = true;
				}

                if (x == 0) {
                    solid = true;
                }

                if ((x == CUBE_W / 2) && (y == CUBE_H / 2)) {
                    solid = true;
                }

                if ((x == CUBE_W / 4) && (y == CUBE_H / 3)) {
                    solid = true;
                }

                if ((x == CUBE_W / 3) && (y == CUBE_H / 4)) {
                    solid = true;
                }

                if (z <= 2) {
                    solid = true;
                }

                map[x][y][z] = solid;
            }
        }
    }
}

static void 
map_update (int ox, int oy, int oz)
{
    int x;
    int y;
    int z;

    for (x = 0; x < CUBE_W; x++) {
        for (y = 0; y < CUBE_H; y++) {
            for (z = 0; z < CUBE_Z; z++) {

                cube_t *cube = &iso.cubes[x][y][z];

                cubes_set_solid(cube, 0);
            }
        }
    }

    for (x = 0; x < CUBE_W; x++) {
        for (y = 0; y < CUBE_H; y++) {
            for (z = 0; z < CUBE_Z; z++) {

                int X = x + ox;
                int Y = y + oy;
                int Z = z + oz;

                if (X < 0) continue;
                if (Y < 0) continue;
                if (Z < 0) continue;
                if (X >= MAP_WIDTH) continue;
                if (Y >= MAP_HEIGHT) continue;
                if (Z >= CUBE_Z) continue;

				if (map[X][Y][Z]) {
					cube_t *cube = &iso.cubes[x][y][z];

					cubes_set_solid(cube, map[X][Y][Z] ? 1 : 0);
				}

            }
        }
    }
}

static int ox, oy, oz;

void test (void)
{
    static int done;

    spoint3d light = {VOX_W / 2, VOX_H / 2 , VOX_Z / 2};

    if (!done) {
        done = true;
        cubes_init(light);
        map_init();
    }

    map_update(ox, oy, oz);
    
    vertices_walk_light_blockers(light);

    blit_init();
    cubes_render(light);
    blit_flush_3d();

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_LEFT]) {
		ox--;
	}

	if (state[SDL_SCANCODE_RIGHT]) {
		ox++;
	}

	if (state[SDL_SCANCODE_UP]) {
		oy--;
	}

	if (state[SDL_SCANCODE_DOWN]) {
		oy++;
	}
}
