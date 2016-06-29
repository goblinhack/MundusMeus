/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "thing_timer.h"
#include "timer.h"
#include "level.h"
#include "wid_game_map.h"
#include "math_util.h"
#include "time_util.h"

static int thing_tick_all_things (levelp level)
{
//    int count = 0;
    thingp t;

    FOR_ALL_THINGS(level, t) {

        tpp tp;
        widp w;

        /*
         * Sanity checks.
         */
        THING_SANITY(level, t);
        tp = thing_tp(t);

        if (tp_is_inactive(tp)) {
            /*
             * Even inactive things like walls need death checks else they
             * stay on the level forever!
             */
            if (thing_is_dead(t)) {
                thing_destroy(level, t, "died");
                continue;
            }

            continue;
        }

        if ((t->x >= MAP_WIDTH) ||
            (t->y >= MAP_HEIGHT) ||
            (t->x < 0) ||
            (t->y < 0)) {
            THING_ERR(t, "if off the map %f,%f", t->x, t->y);
            continue;
        }

//    count++;
        w = t->wid;
        if (likely(w != 0)) {
            verify(w);
        }

        /*
         * Thing has croaked it?
         */
        if (thing_is_dead(t)) {
            /*
             * Keep the player around until we restart the level.
             */
            if (!thing_is_player(t) &&
                !thing_is_monst(t)) {
                thing_destroy(level, t, "died");
            }

            continue;
        }

        /*
         * If a projectile, move it by the delta
         */
        int need_collision_test = false;

        if (thing_is_explosion(t)) {
            need_collision_test = true;
        }

        if (!wid_is_moving(w)) {
            /*
             * Only if it finished moving the last delta.
             */
            if ((t->dx != 0) || (t->dy != 0)) {
                /*
                 * Clean up missiles that have hit the edge and stopped 
                 * moving.
                 */
                if ((t->x <= 0) || 
                    (t->y <= 0) || 
                    (t->x >= MAP_WIDTH - 1) ||
                    (t->y >= MAP_HEIGHT - 1)) {
                    thing_dead(level, t, 0, "missile at edge of level"); 
                    continue;
                }

                need_collision_test = true;
            }
        }

        /*
         * If stopped moving, look for a next hop.
         */
        w = t->wid;

        /*
         * If waiting to update this thing to the client, like if it was newly 
         * born, then do not move it before the client gets a chance to find 
         * out.
         */
        float speed = tp_get_speed(tp);

        int look_for_new_hop = false;

        if (t->one_shot_move) {
            /*
             * Forced look for a new hop.
             */
            look_for_new_hop = true;
        } else if (thing_is_monst(t) && !wid_is_moving(w) && w && speed) {
            /*
             * Look for a new hpp.
             */
            look_for_new_hop = true;
        }

        if (look_for_new_hop) {
            double nexthop_x = -1;
            double nexthop_y = -1;

            if (thing_find_nexthop(level,
                                   t, 
                                   &nexthop_x, &nexthop_y)) {

                /*
                 * Let the slimes jump!
                 */
                if (t->one_shot_move) {
                    t->one_shot_move = false;
                }
            }
        }

        if (w) {
            switch (t->dir) {
            case THING_DIR_TL:
            case THING_DIR_BL:
            case THING_DIR_LEFT:
                if (tp_is_effect_rotate_2way(tp)) {
                    wid_flip_horiz(w, true);
                }
                break;

            case THING_DIR_TR:
            case THING_DIR_BR:
            case THING_DIR_RIGHT:
                if (tp_is_effect_rotate_2way(tp)) {
                    wid_flip_horiz(w, false);
                }
                break;

            case THING_DIR_UP:
            case THING_DIR_DOWN:
                break;
            }
        }
    }
    FOR_ALL_THINGS_END

    return (true);
}

int thing_tick_all (levelp level)
{
    if (!level) {
        return (false);
    }

    /*
     * Do per tick stuff for the level
     */
    if (!level_tick(level)) {
        return (false);
    }

    game.tick++;

#ifdef ENABLE_MAP_SANITY
    thing_map_sanity();
#endif

    if (!thing_tick_all_things(level)) {
        return (false);
    }

    return (true);
}

/*
 * Things like shop prices that respawn occasionally.
 */
static void thing_animate_slow (levelp level)
{
    int slow_tick = false;

    if (!slow_tick) {
        return;
    }

    thingp t;

    FOR_ALL_THINGS(level, t) {

        /*
         * Sanity checks.
         */
        THING_SANITY(level, t);
        tpp tp = thing_tp(t);

        /*
         * Random utterance?
         */
        uint32_t delay = tp_get_sound_random_delay_secs(tp);
        if (delay) {
            if (time_have_x_secs_passed_since(myrand() % delay, 
                                                t->timestamp_sound_random)) {
                t->timestamp_sound_random = time_get_time_ms();
                MSG_SHOUT_AT(SOUND, t, t->x, t->y, "%s", tp->sound_random);
            }
        }
    }
    FOR_ALL_THINGS_END
}

void thing_animate_all (levelp level)
{
    thingp t;

    if (!level) {
        return;
    }

    FOR_ALL_THINGS(level, t) {

        THING_SANITY(level, t);

        if (t->wid && thing_is_animated(t)) {
            thing_animate(level, t);
        }

        if (t->destroy_in_ms > 0) {
            t->destroy_in_ms -= MAIN_LOOP_DELAY;
            if (t->destroy_in_ms <= 0) {
                thing_dead(level, t, 0, "timer expired");
                continue;
            }
        }
    }

    FOR_ALL_THINGS_END

    thing_animate_slow(level);
}
