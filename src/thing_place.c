/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "thing.h"
#include "wid_game_map.h"

static widp thing_place__ (levelp level,
                           thingp t, 
                           tpp tp, 
                           const int under, 
                           const int behind,
                           thing_hit_obstacle_fn fn)
{
    double dx = 0;
    double dy = 0;

    if (t && under) {
        double x;
        double y;

        thing_real_to_fmap(t, &x, &y);

        /*
         * Try to place in front of the player.
         */
        if (!fn || !(*fn)(level, t, x, y)) {
            widp w = wid_game_map_replace_tile(level, x, y,
                                               0, /* thing */
                                               tp,
                                               0 /* tpp_data */);
            return (w);
        }
    }

    thing_dir(t, &dx, &dy);

    /*
     * Sanity check we got one dir.
     */
    if ((dx == 0.0) && (dy == 0.0)) {
        dx = 1.0;
        dy = 0.0;
    }

    if (behind) {
        dx *= -1.0;
        dy *= -1.0;
    }

    dx /= 2.0;

    /*
     * The player sinks into the ground a bit.
     */
    dy = -0.3;

    double x;
    double y;
    thing_real_to_fmap(t, &x, &y);
    x += dx;
    y += dy;

    /*
     * Try to place in front of the player.
     */
    if (!fn || !(*fn)(level, t, x, y)) {
        widp w = wid_game_map_replace_tile(level, x, y,
                                           0, /* thing */
                                           tp,
                                           0 /* tpp_data */);
        return (w);
    }

    dx /= 2.0;

    thing_real_to_fmap(t, &x, &y);

    x += dx;
    y += dy;

    /*
     * Try to place in front of the player.
     */
    if (!fn || !(*fn)(level, t, x, y)) {
        widp w = wid_game_map_replace_tile(level, x, y,
                                           0, /* thing */
                                           tp,
                                           0 /* tpp_data */);
        return (w);
    }

    dx *= 3.0;

    thing_real_to_fmap(t, &x, &y);

    x += dx;
    y += dy;

    /*
     * Try to place in front of the player.
     */
    if (!fn || !(*fn)(level, t, x, y)) {
        widp w = wid_game_map_replace_tile(level, x, y,
                                           0, /* thing */
                                           tp,
                                           0 /* tpp_data */);
        return (w);
    }

    dx = 0.0;

    thing_real_to_fmap(t, &x, &y);

    x += dx;
    y += dy;

    /*
     * Try to place in front of the player.
     */
    if (!fn || !(*fn)(level, t, x, y)) {
        widp w = wid_game_map_replace_tile(level, x, y,
                                           0, /* thing */
                                           tp,
                                           0 /* tpp_data */);
        return (w);
    }

    if (!behind) {
        return (0);
    }

    /*
     * Just place anywhere free.
     */
    for (dx = -1.0; dx <= 1.0; dx += 0.5) {
        dy = 0; {
            if ((dx == 0.0) && (dy == 0.0)) {
                continue;
            }

            double x;
            double y;
            thing_real_to_fmap(t, &x, &y);
            x += dx;
            y += dy;

            if (x < 0) {
                continue;
            }
            if (y < 0) {
                continue;
            }

            if (x >= MAP_WIDTH) {
                continue;
            }
            if (y >= MAP_HEIGHT) {
                continue;
            }

            if (!fn || !(*fn)(level, t, x, y)) {
                widp w = wid_game_map_replace_tile(level, x, y, 
                                                   0, /* thing */
                                                   tp,
                                                   0 /* tpp_data */);
                return (w);
            }
        }
    }

    thing_real_to_fmap(t, &x, &y);

    /*
     * Last resort, just place on the player.
     */
    widp w = wid_game_map_replace_tile(level, x, y, 
                                       0, /* thing */
                                       tp,
                                       0 /* tpp_data */);
    return (w);
}

static widp thing_place_ (levelp level,
                          thingp t, 
                          tpp tp, 
                          const int under, 
                          const int behind)
{
    thing_hit_obstacle_fn fn;

    if (tp_is_bomb(tp)) {
        fn = 0;
    } else if (tp_is_torch(tp)) {
        fn = thing_hit_solid_obstacle;
    } else if (tp_is_rope(tp)) {
        fn = thing_hit_fall_obstacle;
    } else {
        fn = thing_hit_any_obstacle;
    }

    widp w = thing_place__(level,
                          t, 
                          tp, 
                          under, 
                          behind,
                          fn);

    if (!w) {
        return (0);
    }

    thingp it = wid_get_thing(w);

    it->fall_speed = t->fall_speed;
    it->jump_speed = t->jump_speed;
    it->momentum = t->momentum;

    return (w);
}

widp thing_place (levelp level, thingp t, tpp tp)
{
    return (thing_place_(level,
                         t, tp, false /* under */, false /* behind */));
}

widp thing_place_behind (levelp level, thingp t, tpp tp)
{
    return (thing_place_(level,
                         t, tp, false /* under */, true /* behind */));
}

widp thing_place_behind_or_under (levelp level, thingp t, tpp tp)
{
    return (thing_place_(level,
                         t, tp, true /* under */, true /* behind */));
}

