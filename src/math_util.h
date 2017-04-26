/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include <math.h>

#include "main.h"
#include "stdlib.h"

#undef SQUARED
#define SQUARED(x) ((x) * (x))

#undef DISTANCE
#define DISTANCE(xa,ya,xb,yb) \
    sqrt((double)(SQUARED((xb)-(xa))+SQUARED((yb)-(ya))))

#undef DISTANCE3f
#define DISTANCE3f(xa,ya,za,xb,yb,zb) \
    sqrt((double)(SQUARED((xb)-(xa))+\
                 SQUARED((yb)-(ya))+\
                 SQUARED((zb)-(za))))

static inline fpoint fadd (const fpoint a, const fpoint b)
{
    fpoint r = {a.x + b.x, a.y + b.y};
    return (r);
}

static inline fpoint3d fadd3d (const fpoint3d a, const fpoint3d b)
{
    fpoint3d r = {a.x + b.x, a.y + b.y, a.z + b.z};

    return (r);
}

static inline fpoint fsub (const fpoint a, const fpoint b)
{
    fpoint r = {a.x - b.x, a.y - b.y};
    return (r);
}

static inline fpoint3d fsub3d (const fpoint3d a, const fpoint3d b)
{
    fpoint3d r = {a.x - b.x, a.y - b.y, a.z - b.z};

    return (r);
}

static inline fpoint fmul (double m, const fpoint b)
{
    fpoint r = {m * b.x, m * b.y};

    return (r);
}

static inline fpoint3d fmul3d (double m, const fpoint3d b)
{
    fpoint3d r = {m * b.x, m * b.y, m * b.z};

    return (r);
}

static inline fpoint fdiv (double m, const fpoint b)
{
    fpoint r = {b.x / m, b.y / m};

    return (r);
}

static inline fpoint3d fdiv3d (const double m, const fpoint3d b)
{
    fpoint3d r = {b.x / m, b.y / m, b.z / m};

    return (r);
}

static inline int fcmp (const fpoint3d a, const fpoint3d b)
{
    return ((a.x == b.x) && (a.y == b.y));
}

static inline int fcmp3d (const fpoint3d a, const fpoint3d b)
{
    return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

static inline int scmp3d (const spoint3d a, const spoint3d b)
{
    return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

static inline double fdot (const fpoint a, const fpoint b)
{
    return (a.x * b.x + a.y * b.y);
}

static inline double fdot3d (const fpoint3d a, const fpoint3d b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

static inline double fcross (const fpoint a, const fpoint b)
{
    double c = a.x*b.y - a.y*b.x;

    return (c);
}

static inline fpoint3d fpoint3d_cross (const fpoint3d a, const fpoint3d b)
{
    fpoint3d c = {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};

    return (c);
}

static inline fpoint fnormal (const fpoint v)
{
    fpoint n = v;

    double length = sqrt((v.x * v.x) + (v.y * v.y));

    n.x = v.x / length;
    n.y = v.y / length;

    return (n);
}

static inline fpoint3d fnormal3d (fpoint3d v)
{
    fpoint3d n = v;

    double length = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

    n.x = v.x / length;
    n.y = v.y / length;
    n.z = v.z / length;

    return (n);
}

static inline double flength (const fpoint a)
{
    return ((double)sqrt(a.x*a.x + a.y*a.y));
}

static inline double fdist (const fpoint a, const fpoint b)
{
    return (DISTANCE(a.x, a.y, b.x, b.y));
}

static inline double fdist3d (const fpoint3d a, const fpoint3d b)
{
    return (DISTANCE3f(a.x, a.y, a.z, b.x, b.y, b.z));
}

static inline fpoint funit (const fpoint p)
{
    const double length = sqrt(p.x*p.x + p.y*p.y);
    fpoint o;

    o.x = p.x / length;
    o.y = p.y / length;

    return (o);
}

static inline void unit (fpoint *p)
{
    const double length = sqrt(p->x*p->x + p->y*p->y);

    p->x = p->x / length;
    p->y = p->y / length;
}

static inline void unit3d (fpoint3d *p)
{
    const double length = sqrt(p->x*p->x + p->y*p->y + p->z*p->z);

    p->x = p->x / length;
    p->y = p->y / length;
    p->z = p->z / length;
}

uint32_t nextpoweroftwo(uint32_t val);

#define PI      3.14159265358979323846f /* pi */
#define RAD_45  0.78539816339744830962f /* pi/4 */
#define RAD_90  1.57079632679489661923f /* pi/2 */
#define RAD_180 3.14159265358979323846f /* pi */
#define RAD_360 (2.0f * 3.14159265358979323846f) /* 2pi */
#define RAD_270 (RAD_180 + RAD_90)

static inline double angle2radians (const double angle)
{
    return (angle * (RAD_360 / 360.0));
}

static inline double radians2angle (const double rad)
{
    return ((360.0 * rad) / RAD_360);
}

void math_init(void);

/*
 * Fast cached sin/cos routines.
 */
#define RAD_MAX 0xFFFF
#define RAD_STEP (RAD_360 / (double)RAD_MAX)

static inline double fsin (double rad)
{
#ifdef ENABLE_MATH_ESTIMATES
    const uint16_t index = (uint16_t)(rad / RAD_STEP);
    extern double FSIN[RAD_MAX];

    return (FSIN[index & (RAD_MAX-1)]);
#else
    return (sin(rad));
#endif
}

static inline double fasin (double rad)
{
#ifdef ENABLE_MATH_ESTIMATES
    const uint16_t index = (uint16_t)(rad / RAD_STEP);
    extern double FASIN[RAD_MAX];

    return (FASIN[index & (RAD_MAX-1)]);
#else
    return (asin(rad));
#endif
}

static inline double fcos (double rad)
{
#ifdef ENABLE_MATH_ESTIMATES
    const uint16_t index = (uint16_t)(rad / RAD_STEP);
    extern double FCOS[RAD_MAX];

    return (FCOS[index & (RAD_MAX-1)]);
#else
    return (cos(rad));
#endif
}

static inline float atan2_approximation1(float y, float x)
{
#ifdef ENABLE_MATH_ESTIMATES
    //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
    //Volkan SALMA

    const float ONEQTR_PI = M_PI / 4.0;
    const float THRQTR_PI = 3.0 * M_PI / 4.0;
    float r, angle;
    float abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition
    if ( x < 0.0f ) {
        r = (x + abs_y) / (abs_y - x);
        angle = THRQTR_PI;
    } else {
        r = (x - abs_y) / (x + abs_y);
        angle = ONEQTR_PI;
    }
    angle += (0.1963f * r * r - 0.9817f) * r;
    if ( y < 0.0f )
        return( -angle );     // negate if in quad III or IV
    else
        return( angle );
#else
    return (atan2(y, x));
#endif
}

static inline double anglerot (fpoint p)
{
    double theta = atan2_approximation1(p.y, p.x); // range (-PI, PI]
    if (theta < 0) {
        theta = RAD_360 + theta; // range [0, 360)
    }
    return theta;
}

static inline
fpoint fpoint_rotate (fpoint p, double theta)
{
    fpoint r;

    r.x = p.x * cos(theta) - p.y * sin(theta);
    r.y = p.x * sin(theta) + p.y * cos(theta);

    return (r);
}

/*
 * Yields an angle between 0 and 180 deg radians
 */
double fpoint_angle(const fpoint A, const fpoint B);

double fpoint_angle_clockwise(const fpoint A, const fpoint B);

/*
 * true if perpendicular line from point is in line segment.
 */
uint8_t
fpoint_dist_line(fpoint P0, fpoint L0, fpoint L1, double *dist,
                 fpoint *intersect);

/*
 * true if perpendicular line from point is in line segment.
 */
int
fpoint_dist_line2(fpoint P0, fpoint L0, fpoint L1, double *dist,
                 fpoint *intersect);

/*
 * true if lines intersect
 */
uint8_t get_line_intersection(fpoint p0,
                              fpoint p1,
                              fpoint p2,
                              fpoint p3,
                              fpoint *intersect);
uint8_t get_line_known_intersection(fpoint p0,
                              fpoint p1,
                              fpoint p2,
                              fpoint p3,
                              fpoint *intersect);

double gauss(const double m, const double s);

double fpoint_project_onto_line(fpoint P0, fpoint L0, fpoint L1);

double gaussrand(double mean, double stddev);

/** You know that lines a and b have an intersection and now you
    want to get it!
*/
static inline void getIntersection (fpoint a0,
                                    fpoint a1,
                                    fpoint b0,
                                    fpoint b1,
                                    fpoint *i)
{
    /* the intersection [(x1,y1), (x2, y2)]
       it might be a line or a single point. If it is a line,
       then x1 = x2 and y1 = y2.  */
    double x1, y1;
    // double x2;
    // double y2;

    if (a0.x == a1.x) {
        // Case (A)
        // As a is a perfect vertical line, it cannot be represented
        // nicely in a mathematical way. But we directly know that
        //
        x1 = a0.x;
//        x2 = x1;
        if (b0.x == b1.x) {
            // Case (AA): all x are the same!
            // Normalize
            if (a0.y > a1.y) {
                swap(a0, a1);
            }
            if (b0.y > b1.y) {
                swap(b0, b1);
            }
            if (a0.y > b0.y) {
                swap(a0, b0);
                swap(a1, b1);
            }

            // Now we know that the y-value of a["first"] is the
            // lowest of all 4 y values
            // this means, we are either in case (AAA):
            //   a: x--------------x
            //   b:    x---------------x
            // or in case (AAB)
            //   a: x--------------x
            //   b:    x-------x
            // in both cases:
            // get the relavant y intervall
            y1 = b0.y;
//            y2 = min(a1.y, b1.y);
        } else {
            // Case (AB)
            // we can mathematically represent line b as
            //     y = m*x + t <=> t = y - m*x
            // m = (y1-y2)/(x1-x2)
            double m, t;
            m = (b0.y - b1.y)/
                (b0.x - b1.x);
            t = b0.y - m*b0.x;
            y1 = m*x1 + t;
//            y2 = y1;
        }
    } else if (b0.x == b1.x) {
        // Case (B)
        // essentially the same as Case (AB), but with
        // a and b switched
        x1 = b0.x;
//        x2 = x1;

        swap(a0, b0);
        swap(a1, b1);

        double m, t;
        m = (b0.y - b1.y)/
            (b0.x - b1.x);
        t = b0.y - m*b0.x;
        y1 = m*x1 + t;
//        y2 = y1;
    } else {
        // Case (C)
        // Both lines can be represented mathematically
        double ma, mb, ta, tb;
        ma = (a0.y - a1.y)/
             (a0.x - a1.x);
        mb = (b0.y - b1.y)/
             (b0.x - b1.x);
        ta = a0.y - ma*a0.x;
        tb = b0.y - mb*b0.x;
        if (ma == mb) {
            // Case (CA)
            // both lines are in parallel. As we know that they
            // intersect, the intersection could be a line
            // when we rotated this, it would be the same situation
            // as in case (AA)

            // Normalize
            if (a0.x > a1.x) {
                swap(a0, a1);
            }
            if (b0.x > b1.x) {
                swap(b0, b1);
            }
            if (a0.x > b0.x) {
                swap(a0, b0);
                swap(a1, b1);
            }

            // get the relavant x intervall
            x1 = b0.x;
//            x2 = min(a1.x, b1.x);
            y1 = ma*x1+ta;
//            y2 = ma*x2+ta;
        } else {
            // Case (CB): only a point as intersection:
            // y = ma*x+ta
            // y = mb*x+tb
            // ma*x + ta = mb*x + tb
            // (ma-mb)*x = tb - ta
            // x = (tb - ta)/(ma-mb)
            x1 = (tb-ta)/(ma-mb);
            y1 = ma*x1+ta;
//            x2 = x1;
//            y2 = y1;
        }
    }

    i->x = x1;
    i->y = y1;

    /*
     * If a line
     *
    i->x = x2;
    i->y = y2;
     */
}
