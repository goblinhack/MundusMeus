/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "math_util.h"
#include "thing.h"

static void level_place_spatter (levelp level, 
                                 thingp owner,
                                 double x, 
                                 double y,
                                 double radius,
                                 int amount,
                                 uint32_t nargs, ...)
{
    va_list args;

    while (amount--) {
        double px = gauss(x, radius);
        double py = gauss(y, radius);
        va_start(args, nargs);

        (void) level_place_explosion_at(level, 
                                        owner,
                                        px,
                                        py,
                                        px, 
                                        py, 
                                        0,
                                        true, /* epicenter */
                                        0,
                                        nargs, args);
        va_end(args);
    }
}

void level_place_bubbles (levelp level, 
                          thingp owner,
                          double x, double y)
{
    if (thing_is_dead(owner)) {
        return;
    }

    level_place_spatter(level, 
                        owner,
                        x, y,
                        0.05, // radius
                        1, // amount
                        1, // nargs
                        "bubbles1");
}

void thing_bubbles_placed (levelp level, thingp t)
{
    widp w = t->wid;

    wid_move_delta_pct_in(w, 0, -0.2, 1000);
    wid_effect_pulses(w);
    wid_fade_out(w, 500);
}
