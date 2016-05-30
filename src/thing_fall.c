/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "thing.h"
#include "wid_game_map.h"
#include "tile.h"
#include "math_util.h"
#include "time_util.h"
#include "player.h"

/*
 * Hit the ground hard?
 */
static int thing_hit_ground (levelp level, 
                             thingp t, 
                             thingp it)
{
    if (t->fall_speed > THING_FALL_SPEED_HIT_MONST) {
        if (thing_is_monst(it)) {
            THING_LOG(t, "fell (%f) and landed on monst", t->fall_speed);
            (void) thing_hit(level, it, t, 1);
        }
    }

    if (t->falling_too_fast) {
        t->falling_too_fast = 0;

        if (thing_is_player(t) ||
            thing_is_monst(t) ||
            thing_is_bomb(t)) {

            THING_LOG(t, "fell too fast (%f) and hit the ground", t->fall_speed);

            (void) thing_hit(level, t, it, 1);
        }
    }

    t->fall_speed = 0;
    return (true);
}

/*
 * Try to fall down
 */
int thing_fall (levelp level, thingp t)
{
    double x = t->x;
    double y = t->y + 0.015;
    thingp it;

    if (thing_is_monst(t)  ||
        thing_is_ladder(t) ||
        thing_is_player(t)) {

        it = thing_overlaps(level, t, t->x, t->y, thing_is_ladder);
        if (it) {
            thing_hit_ground(level, t, it);
            return (false);
        }
    }

    if (t->jump_speed) {
        t->fall_speed = 0;
        return (false);
    }

    if (thing_is_monst(t)  ||
        thing_is_player(t)) {

        if (t->fall_speed > THING_FALL_SPEED_HIT_SPIKES) {
            it = thing_overlaps(level, t, t->x, t->y, thing_is_spikes);
            if (it) {
                THING_LOG(t, "fell onto spikes (%f)", t->fall_speed);

                (void) thing_hit(level, t, it, 1);
            }
        }
    }

    it = thing_hit_fall_obstacle(level, t, x, y);
    if (it) {
        thing_hit_ground(level, t, it);
        return (false);
    }

    t->fall_speed += THING_FALL_SPEED_GRAVITY;

    t->falling_too_fast = false;
    if (t->fall_speed - t->jump_speed > THING_FALL_SPEED_TOO_FAST) {
        t->falling_too_fast = true;
    }

    if (t->fall_speed > 1) {
        t->fall_speed = 1;
    }

    y = t->y + t->fall_speed;
    it = thing_hit_fall_obstacle(level, t, x, y);
    if (it) {
        y = t->y + t->fall_speed / 2;
        it = thing_hit_fall_obstacle(level, t, x, y);
        if (it) {
            y = t->y + t->fall_speed / 4;
            it = thing_hit_fall_obstacle(level, t, x, y);
            if (it) {
                y = t->y + t->fall_speed / 4;
                it = thing_hit_fall_obstacle(level, t, x, y);
                if (it) {
                    thing_hit_ground(level, t, it);
                    return (false);
                }
            }
        }
    }

    thing_wid_update(level, t, x, y, true, false /* is new */);

    thing_handle_collisions(level, t);

    return (true);
}
