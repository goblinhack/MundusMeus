/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "thing.h"
#include "wid_game_map.h"

uint8_t thing_mob_spawn (levelp level, thingp t)
{
    /*
     * Not too many things
     */
    if (monst_things_total > THING_MAX_MONSTS_SPAWN_LIMIT) {
        return (false);
    }

    const char *mob_spawn = tp_mob_spawn(thing_tp(t));
    if (!mob_spawn) {
        return (false);
    }

    tpp what = tp_find(mob_spawn);
    if (!what) {
        ERR("cannot spawn %s", mob_spawn);
        return (false);
    }

    uint32_t tries = 10;

    while (tries-- > 0) {
        double x;
        double y;
        uint32_t r = myrand() % 100;

        thing_real_to_fmap(t, &x, &y);

        if (r < 12) {
            x -= 1.0;
        } else if (r < 50) {
            x += 1.0;
        } else if (r < 75) {
            y -= 1.0;
        } else {
            y += 1.0;
        }

        /*
         * Protect against mobs at the edge!
         */
        if ((x < 0) || (x >= MAP_WIDTH)) {
            continue;
        }

        if ((y < 0) || (y >= MAP_HEIGHT)) {
            continue;
        }

        /*
         * Things not to spawn onto.
         */
        if (thing_is_lava(t) ||
            thing_is_water(t) ||
            thing_is_acid(t)) {

            if (map_is_wall_at(level, x, y)   ||
                map_is_rock_at(level, x, y)   ||
                map_is_lava_at(level, x, y)   ||
                map_is_water_at(level, x, y)  ||
                map_is_acid_at(level, x, y)   ||
                map_is_exit_at(level, x, y)) {

                thing_real_to_fmap(t, &x, &y);

                uint32_t r = myrand() % 100;

                if (r < 12) {
                    x -= 1.0;
                    y -= 1.0;
                } else if (r < 50) {
                    x += 1.0;
                    y += 1.0;
                } else if (r < 75) {
                    x -= 1.0;
                    y += 1.0;
                } else {
                    x -= 1.0;
                    y += 1.0;
                }

                if (map_is_wall_at(level, x, y)   ||
                    map_is_rock_at(level, x, y)   ||
                    map_is_lava_at(level, x, y)   ||
                    map_is_water_at(level, x, y)  ||
                    map_is_acid_at(level, x, y)   ||
                    map_is_exit_at(level, x, y)) {
                    continue;
                }
            }
        } else {
            if (map_is_wall_at(level, x, y)          ||
                map_is_rock_at(level, x, y)          ||
                map_is_monst_at(level, x, y)         ||
                map_is_cobweb_at(level, x, y)        ||
                map_is_door_at(level, x, y)          ||
                map_is_player_at(level, x, y)        ||
                map_is_treasure_at(level, x, y)      ||
                map_is_food_at(level, x, y)          ||
                map_is_mob_spawner_at(level, x, y)   ||
                map_is_exit_at(level, x, y)) {

                uint32_t r = myrand() % 100;

                thing_real_to_fmap(t, &x, &y);

                if (r < 12) {
                    x -= 1.0;
                    y -= 1.0;
                } else if (r < 50) {
                    x += 1.0;
                    y += 1.0;
                } else if (r < 75) {
                    x -= 1.0;
                    y += 1.0;
                } else {
                    x -= 1.0;
                    y += 1.0;
                }

                if (map_is_wall_at(level, x, y)          ||
                    map_is_rock_at(level, x, y)          ||
                    map_is_monst_at(level, x, y)         ||
                    map_is_cobweb_at(level, x, y)        ||
                    map_is_door_at(level, x, y)          ||
                    map_is_player_at(level, x, y)        ||
                    map_is_treasure_at(level, x, y)      ||
                    map_is_food_at(level, x, y)          ||
                    map_is_mob_spawner_at(level, x, y)   ||
                    map_is_exit_at(level, x, y)) {

                    continue;
                }
            }
        }

        if (x <= 0) {
            continue;
        }
        if (y <= 0) {
            continue;
        }
        if (x >= MAP_WIDTH-1) {
            continue;
        }
        if (y >= MAP_HEIGHT-1) {
            continue;
        }

        /*
         * Round down so mob spawners on an in between position will not
         * get mixed up. This can happen when we place a mob spawner where
         * a player died, overlapping a wall and we then try to spawn into
         * a cell we cannot move in.
         */
        x = floor(x);
        y = floor(y);

        /*
         * Flames on lava pits spawn on the lava.
         */
        if (tp_is_spawns_under(what)) {
            thing_real_to_fmap(t, &x, &y);
        }

        wid_game_map_replace_tile(level,
                                  x,
                                  y,
                                  0, /* thing */
                                  what,
                                  0 /* tpp data */);

        if (thing_is_single_mob_spawner(t)) {
            thing_dead(level, t, 0, "finished single mob spawn");
        }

        return (true);
    }

    return (false);
}

thingp thing_mob_spawn_on_death (levelp level, thingp t)
{
    const char *mob_spawn = tp_spawn_on_death(thing_tp(t));
    if (!mob_spawn) {
        return (0);
    }

    tpp what = tp_find(mob_spawn);
    if (!what) {
        THING_ERR(t, "cannot spawn %s on death", mob_spawn);
        return (0);
    }

    uint32_t tries = 100;
    uint8_t first = 1;

    while (tries-- > 0) {
        double x;
        double y;
        uint32_t r = myrand() % 100;

        thing_real_to_fmap(t, &x, &y);

        if (r < 25) {
            x -= 1.0;
        } else if (r < 50) {
            x += 1.0;
        } else if (r < 75) {
            y -= 1.0;
        } else if (r < 100) {
            y += 1.0;
        } else {
            // centered over player
        }

        /*
         * Protect against mobs at the edge!
         */
        if ((x < 0) || (x >= MAP_WIDTH)) {
            continue;
        }

        if ((y < 0) || (y >= MAP_HEIGHT)) {
            continue;
        }

        if (first) {
            thing_real_to_fmap(t, &x, &y);
            first = 0;
        }

        /*
         * Round down so mob spawners on an in between position will not
         * get mixed up. This can happen when we place a mob spawner where
         * a player died, overlapping a wall and we then try to spawn into
         * a cell we cannot move in.
         */
        x = floor(x);
        y = floor(y);

        /*
         * Flames on lava pits spawn on the lava.
         */
        if (tp_is_spawns_under(what)) {
            thing_real_to_fmap(t, &x, &y);
        }

        if (tp_is_cobweb(what)) {
            /*
             * Don't want too many sticky things in the same place.
             */
            if (map_is_cobweb_at(level, x, y)) {
                continue;
            }
        } else {
            /*
             * Things not to spawn onto.
             */
            if (map_is_wall_at(level, x, y)          ||
                map_is_rock_at(level, x, y)          ||
                map_is_door_at(level, x, y)          ||
                map_is_exit_at(level, x, y)) {
                continue;
            }
        }

        widp w = wid_game_map_replace_tile(level,
                                           x,
                                           y,
                                           0, /* thing */
                                           what,
                                           0 /* tpp data */);

        THING_LOG(t, "place %s on death", mob_spawn);

        return (wid_get_thing(w));
    }

    THING_LOG(t, "Failed to place %s on death", mob_spawn);

    return (0);
}
