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

uint8_t thing_init(void);
void thing_fini(void);
thingp thing_load(const char *name);
thingp thing_find(const char *name);
void thing_sanity(levelp, thingp);
int thing_tick_all(levelp level);
void thing_animate_all(levelp level);
void thing_wake(levelp, thingp t);
void thing_dead(levelp, thingp, thingp killer,
                const char *fmt, ...) __attribute__ ((format (printf, 4, 5)));
void thing_set_wid(levelp, thingp, widp);
widp thing_wid(thingp);
const char *thing_logname(thingp);
uint8_t thing_z_depth(thingp);
uint8_t thing_z_order(thingp);
tree_rootp thing_tile_tiles(thingp);
void thing_animate(levelp, thingp);
typedef uint8_t (*thing_is_fn)(thingp t);

uint8_t thing_is_light_source(thingp t);
uint8_t thing_is_candle_light(thingp t);
uint8_t thing_is_explosion(thingp t);

void thing_set_dir_none(levelp, thingp t);
uint8_t thing_is_dir_none(thingp t);
void thing_set_dir_down(levelp, thingp t);
uint8_t thing_is_dir_down(thingp t);
void thing_set_dir_up(levelp, thingp t);
uint8_t thing_is_dir_up(thingp t);
void thing_set_dir_left(levelp, thingp t);
uint8_t thing_is_dir_left(thingp t);
void thing_set_dir_right(levelp, thingp t);
uint8_t thing_is_dir_right(thingp t);

void thing_set_dir_tl(levelp, thingp t);
uint8_t thing_is_dir_tl(thingp t);
void thing_set_dir_bl(levelp, thingp t);
uint8_t thing_is_dir_bl(thingp t);
void thing_set_dir_tr(levelp, thingp t);
uint8_t thing_is_dir_tr(thingp t);
void thing_set_dir_br(levelp, thingp t);
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
void thing_place_timed(levelp,
                       tpp t, 
                       double x,
                       double y,
                       uint32_t ms, 
                       uint32_t jitter);
void thing_place_and_destroy_timed(levelp,
                                   tpp t, 
                                   thingp owner,
                                   double x,
                                   double y,
                                   uint32_t ms, 
                                   uint32_t destroy_in, 
                                   uint32_t jitter,
                                   uint8_t is_epicenter);
void thing_wid_update(levelp,
                      thingp t, 
                      double x, double y, 
                      uint8_t smooth, 
                      uint8_t is_new);
uint8_t thing_use(thingp t, uint32_t id);

typedef struct {
    /*
     * First expiry
     */
    int16_t fire_in;

    /*
     * Second expiry
     */
    int16_t destroy_in;

    tpp tp;
    float x;
    float y;

    uint32_t owner_id;

    /*
     * Center of an explosion.
     */
    uint8_t is_epicenter:1;

} thing_place_context_t;

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
     * Pointer to common settings for this thing.
     */
    uint16_t tp_id;

    /*
     * Widget for displaying thing.
     */
    widp wid;

    /*
     * Grid coordinates.
     */
    float x;
    float y;

    /*
     * Last anim frame position. To be able to detect moving things.
     */
    float anim_x;
    float anim_y;

    /*
     * For animation.
     */
    uint16_t current_tile;

    /*
     * Previous hop where we were. We use this to interpolate the real 
     * position when moving.
     */
    float last_x;
    float last_y;

    /*
     * For moving
     */
    float dx;
    float dy;

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

    return (id_to_tp(t->tp_id));
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
 * thing_move.c
 */
void thing_move_set_dir(levelp,
                        thingp t,
                        double *x,
                        double *y,
                        uint8_t up,
                        uint8_t down,
                        uint8_t left,
                        uint8_t right);

/*
 * thing.c
 */
uint8_t thing_player_move(levelp,
                          thingp t,
                          double x,
                          double y,
                          const uint8_t up,
                          const uint8_t down,
                          const uint8_t left,
                          const uint8_t right,
                          const uint8_t fire);
int thing_move(levelp,
               thingp t,
               double x,
               double y,
               const uint8_t up,
               const uint8_t down,
               const uint8_t left,
               const uint8_t right,
               const uint8_t fire);

void thing_move_to(thingp t, double x, double y);

void thing_wid_move(levelp level,
                    thingp t, 
                    double x, 
                    double y, 
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

extern tree_root *things;
