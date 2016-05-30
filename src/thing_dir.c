/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "thing.h"
#include "math_util.h"

void thing_set_dir_none (levelp level, thingp t)
{
    verify(t);

    if (thing_is_animated_no_dir(t)) {
        return;
    }

    if (t->dir != THING_DIR_NONE) {
        t->dir = THING_DIR_NONE;
        thing_set_weapon_placement(level, t);
    }
}

uint8_t thing_is_dir_none (thingp t)
{
    verify(t);

    return (t->dir == THING_DIR_NONE);
}

void thing_set_dir_down (levelp level, thingp t)
{
    verify(t);

    if (thing_is_submerged(level, t)) {
        return;
    }

    if (thing_is_animated_no_dir(t)) {
        return;
    }

    if (t->dir != THING_DIR_DOWN) {
        t->dir = THING_DIR_DOWN;
        thing_set_weapon_placement(level, t);
    }
}

uint8_t thing_is_dir_down (thingp t)
{
    verify(t);

    return (t->dir == THING_DIR_DOWN);
}

void thing_set_dir_up (levelp level, thingp t)
{
    verify(t);

    if (thing_is_submerged(level, t)) {
        return;
    }

    if (thing_is_animated_no_dir(t)) {
        return;
    }

    if (t->dir != THING_DIR_UP) {
        t->dir = THING_DIR_UP;
        thing_set_weapon_placement(level, t);
    }
}

uint8_t thing_is_dir_up (thingp t)
{
    verify(t);

    return (t->dir == THING_DIR_UP);
}

void thing_set_dir_left (levelp level, thingp t)
{
    verify(t);

    if (thing_is_animated_no_dir(t)) {
        return;
    }

    if (t->dir != THING_DIR_LEFT) {
        t->dir = THING_DIR_LEFT;
        thing_set_weapon_placement(level, t);
    }
}

uint8_t thing_is_dir_left (thingp t)
{
    verify(t);

    return (t->dir == THING_DIR_LEFT);
}

void thing_set_dir_right (levelp level, thingp t)
{
    verify(t);

    if (thing_is_animated_no_dir(t)) {
        return;
    }

    if (t->dir != THING_DIR_RIGHT) {
        t->dir = THING_DIR_RIGHT;
        thing_set_weapon_placement(level, t);
    }
}

uint8_t thing_is_dir_right (thingp t)
{
    verify(t);

    return (t->dir == THING_DIR_RIGHT);
}

void thing_set_dir_tl (levelp level, thingp t)
{
    verify(t);

    if (thing_is_animated_no_dir(t)) {
        return;
    }

    if (t->dir != THING_DIR_TL) {
        t->dir = THING_DIR_TL;
        thing_set_weapon_placement(level, t);
    }
}

uint8_t thing_is_dir_tl (thingp t)
{
    verify(t);

    return (t->dir == THING_DIR_TL);
}

void thing_set_dir_bl (levelp level, thingp t)
{
    verify(t);

    if (thing_is_animated_no_dir(t)) {
        return;
    }

    if (t->dir != THING_DIR_BL) {
        t->dir = THING_DIR_BL;
        thing_set_weapon_placement(level, t);
    }
}

uint8_t thing_is_dir_bl (thingp t)
{
    verify(t);

    return (t->dir == THING_DIR_BL);
}

void thing_set_dir_tr (levelp level, thingp t)
{
    verify(t);

    if (thing_is_animated_no_dir(t)) {
        return;
    }

    if (t->dir != THING_DIR_TR) {
        t->dir = THING_DIR_TR;
        thing_set_weapon_placement(level, t);
    }
}

uint8_t thing_is_dir_tr (thingp t)
{
    verify(t);

    return (t->dir == THING_DIR_TR);
}

void thing_set_dir_br (levelp level, thingp t)
{
    verify(t);

    if (thing_is_animated_no_dir(t)) {
        return;
    }

    if (t->dir != THING_DIR_BR) {
        t->dir = THING_DIR_BR;
        thing_set_weapon_placement(level, t);
    }
}

uint8_t thing_is_dir_br (thingp t)
{
    verify(t);

    return (t->dir == THING_DIR_BR);
}

void thing_dir (thingp t, double *dx, double *dy)
{
    *dx = 0;
    *dy = 0;

    if (thing_is_dir_down(t)) {
        *dy = 1.0;
    }

    if (thing_is_dir_up(t)) {
        *dy = -1.0;
    }

    if (thing_is_dir_right(t)) {
        *dx = 1.0;
    }

    if (thing_is_dir_left(t)) {
        *dx = -1.0;
    }

    if (thing_is_dir_tl(t)) {
        *dx = -1.0;
        *dy = -1.0;
    }

    if (thing_is_dir_tr(t)) {
        *dx = 1.0;
        *dy = -1.0;
    }

    if (thing_is_dir_bl(t)) {
        *dx = -1.0;
        *dy = 1.0;
    }

    if (thing_is_dir_br(t)) {
        *dx = 1.0;
        *dy = 1.0;
    }
}

int thing_angle_to_dir (double dx, double dy)
{
    fpoint r;
    r.x = dx;
    r.y = -dy;

    int segment = ((int)(round(radians2angle(anglerot(r)) / 45.0))) % 8;

    switch (segment) {
        case 2:
            return (THING_DIR_UP);
        case 1:
            return (THING_DIR_TR);
        case 0:
            return (THING_DIR_RIGHT);
        case 7:
            return (THING_DIR_BR);
        case 6:
            return (THING_DIR_DOWN);
        case 5:
            return (THING_DIR_BL);
        case 4:
            return (THING_DIR_LEFT);
        case 3:
            return (THING_DIR_TL);
    }

    DIE("thing_angle_to_dir bug");
}
