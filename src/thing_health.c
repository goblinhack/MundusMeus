/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "color.h"
#include "thing.h"
#include "time_util.h"

void thing_health_tick (levelp level, thingp t)
{
    if (!time_have_x_ticks_passed_since(10, t->tick_health)) {
        return;
    }

    t->tick_health = game.tick;

    /*
     * Auto heal
     */
    if (t->hp < t->max_hp) {
        /*
         * Only heal if not being actively attacked.
         */
        if (time_have_x_tenths_passed_since(600,
                                            t->timestamp_last_attacked)) {
            t->hp += 1;
        }
    }

    if (t->hp <= 0) {
        thing_dead(level, t, 0, "heath ran our");
    }

    /*
     * If over max health, return to max.
     */
    int delta = t->hp - t->max_hp;;
    if (delta > 0) {
        THING_LOG(t, "Over max hp, hp will auto decrease");

        t->hp -= 1;
    }
}
