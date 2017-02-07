/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file.
 */

#pragma once

#include "tree.h"
#include "wid.h"
#include "thing_template.h"
#include <stdlib.h>
#include <math.h>
#include <Python.h>

uint8_t thing_init(void);
void thing_fini(void);
thingp thing_new(const char *name, long int thing_id, const char *tp_name);
void thing_destroyed_(thingp t, const char *reason);
void thing_set_tilename_(thingp t, const char *tilename);
void thing_set_tp_(thingp t, const char *tp);
void thing_set_depth_(thingp t, double);

thingp thing_find(const char *name);
void thing_move_(thingp t, double x, double y);
PyObject *thing_push_(thingp t, double x, double y);
void thing_pop_(thingp t);
void thing_sanity(thingp);
int thing_tick_all(levelp level);
void thing_animate_all(levelp level);
void thing_wake(thingp t);
#ifdef _MSC_VER
void thing_dead(thingp, thingp killer, const char *fmt, ...);
#else
void thing_dead(thingp, thingp killer,
                const char *fmt, ...) __attribute__ ((format (printf, 3, 4)));
#endif
void thing_set_wid(thingp, widp);
widp thing_wid(thingp);
const char *thing_logname(thingp);
uint8_t thing_z_depth(thingp);
uint8_t thing_z_order(thingp);
tree_rootp thing_tile_tiles(thingp);
void thing_animate(thingp);
typedef uint8_t (*thing_is_fn)(thingp t);

uint8_t thing_is_light_source(thingp t);
uint8_t thing_is_candle_light(thingp t);
uint8_t thing_is_explosion(thingp t);

void thing_set_dir_none(thingp t);
uint8_t thing_is_dir_none(thingp t);
void thing_set_dir_down(thingp t);
uint8_t thing_is_dir_down(thingp t);
void thing_set_dir_up(thingp t);
uint8_t thing_is_dir_up(thingp t);
void thing_set_dir_left(thingp t);
uint8_t thing_is_dir_left(thingp t);
void thing_set_dir_right(thingp t);
uint8_t thing_is_dir_right(thingp t);

void thing_set_dir_tl(thingp t);
uint8_t thing_is_dir_tl(thingp t);
void thing_set_dir_bl(thingp t);
uint8_t thing_is_dir_bl(thingp t);
void thing_set_dir_tr(thingp t);
uint8_t thing_is_dir_tr(thingp t);
void thing_set_dir_br(thingp t);
uint8_t thing_is_dir_br(thingp t);

void thing_set_opened_exit(thingp t, uint8_t val);
uint8_t thing_opened_exit(thingp t);
void thing_set_is_sleeping(thingp t, uint8_t val);
void thing_set_is_open(thingp t, uint8_t val);
void thing_set_is_dead(thingp t, uint8_t val);
const char *thing_name(thingp);
const char *thing_short_name(thingp);
const char *thing_tooltip(thingp);
uint8_t thing_z_depth(thingp);
uint8_t thing_z_order(thingp);
tree_rootp thing_tiles(thingp);
thing_tilep thing_current_tile(thingp t);

enum {
    THING_DIR_NONE,
    THING_DIR_DOWN,
    THING_DIR_UP,
    THING_DIR_LEFT,
    THING_DIR_RIGHT,
    THING_DIR_TL,
    THING_DIR_BL,
    THING_DIR_TR,
    THING_DIR_BR,
};

typedef struct thing_ {

    tree_key_string tree;

    /*
     * Allocated in python
     */
    long int thing_id;

    /*
     * Pointer to common settings for this thing.
     */
    tpp tp;

    /*
     * Widget for displaying thing.
     */
    widp wid;

    /*
     * Grid coordinates.
     */
    double x;
    double y;

    /*
     * Water depth
     */
    double depth;

    /*
     * Last anim frame position. To be able to detect moving things.
     */
    double anim_x;
    double anim_y;

    /*
     * For animation.
     */
    uint16_t current_tile;

    /*
     * Previous hop where we were. We use this to interpolate the real
     * position when moving.
     */
    double last_x;
    double last_y;

    /*
     * For moving
     */
    double dx;
    double dy;

    /*
     * e.g. IS_JOIN_BLOCK
     */
    uint8_t join_index;

    uint8_t dir;

    /*
     * Only used for display purposes.
     */
    int score;
    int gold;
    int hp;

    /*
     * How many rays of light are hitting this thing?
     */
    uint16_t lit;

    uint32_t timestamp_change_to_next_frame;

    /*
     * Debugging this thing?
     */
    uint32_t debug:1;

    /*
     * Really just for animation.
     */
    uint32_t is_dead:1;
    uint32_t is_sleeping:1;
    uint32_t is_moving:1;
    uint32_t is_open:1;

    /*
     * Used on the map for selecting items.
     */
    uint32_t is_focus:1;

    /*
     * Temporary level decorations that can be ignored.
     */
    uint32_t is_deco:1;
} thing;

#include "thing_template.h"

static inline int thing_is_dead (thingp t)
{
    return (t->is_dead);
}

static inline int thing_is_moving (thingp t)
{
    return (t->is_moving);
}

static inline int thing_is_open (thingp t)
{
    return (t->is_open);
}

static inline int thing_is_sleeping (thingp t)
{
    return (t->is_sleeping);
}

static inline tpp thing_tp (thingp t)
{
    verify(t);

    return (t->tp);
}

static inline uint8_t thing_is_monst (thingp t)
{
    verify(t);

    return (tp_is_monst(thing_tp(t)));
}

static inline uint8_t thing_is_door (thingp t)
{
    verify(t);

    return (tp_is_door(thing_tp(t)));
}

static inline uint8_t thing_is_wall (thingp t)
{
    verify(t);

    return (tp_is_wall(thing_tp(t)));
}

static inline uint8_t thing_is_player (thingp t)
{
    verify(t);

    return (tp_is_player(thing_tp(t)));
}

static inline uint8_t thing_is_shadow_caster (thingp t)
{
    verify(t);

    return (tp_is_shadow_caster(thing_tp(t)));
}

static inline uint8_t thing_is_shadow_caster_soft (thingp t)
{
    verify(t);

    return (tp_is_shadow_caster_soft(thing_tp(t)));
}

static inline uint8_t thing_is_joinable (thingp t)
{
    verify(t);

    return (tp_is_joinable(thing_tp(t)));
}

/*
 * thing.c
 */
void thing_move_set_dir(thingp t,
                        double *x,
                        double *y,
                        uint8_t up,
                        uint8_t down,
                        uint8_t left,
                        uint8_t right);

int thing_move_dir(thingp t,
                   double x,
                   double y,
                   const uint8_t up,
                   const uint8_t down,
                   const uint8_t left,
                   const uint8_t right,
                   const uint8_t fire);

void thing_move_to(thingp t, double x, double y);

/*
 * thing_move.c
 */
void thing_wid_move(thingp t,
                    double x,
                    double y,
                    uint8_t smooth);

void thing_wid_update(thingp t,
                      double x, double y,
                      uint8_t smooth);

/*
 * thing_dir.c
 */
void thing_dir(thingp t, double *dx, double *dy);
int thing_angle_to_dir(double dx, double dy);

#define FOR_ALL_THINGS(level, t) \
    { \
        TREE_WALK_UNSAFE(things, t) { \
        verify(t);

#define FOR_ALL_THINGS_END } }

extern int things_deco_total;

extern tree_root *things;

/*
 * For temporary walks.
 */
#define THING_SCRATCH_SIZE (512 * 1024)
extern thingp thing_scratch[THING_SCRATCH_SIZE];

