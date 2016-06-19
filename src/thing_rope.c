/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "thing.h"
#include "wid_game_map.h"
#include "player.h"

thingp thing_place_mountain (levelp level, 
                            thingp owner,
                            double x, double y)
{
    return (0);
#if 0
    if (!owner->ropes) {
        return (0);
    }
    owner->ropes--;

    player_wid_update(level);

    widp w = wid_game_map_replace_tile(level, x, y, 
                                       0, /* thing */
                                       id_to_tp(THING_ROPETOP),
                                       0 /* tpp_data */);
    if (!w) {
        ERR("could not place rope");
        return (0);
    }

    thingp t = wid_get_thing(w);

    thing_wake(level, t);

    t->jump_speed = 0.5;

    return (t);
#endif
}

thingp thing_place_rope (levelp level, 
                         thingp owner,
                         double x, double y)
{
        return (0);
#if 0
    widp w = wid_game_map_replace_tile(level, x, y, 
                                       0, /* thing */
                                       id_to_tp(THING_ROPE),
                                       0 /* tpp_data */);
    if (!w) {
        ERR("could not place rope");
        return (0);
    }

    thingp t = wid_get_thing(w);

    thing_wake(level, t);

    return (t);
#endif
}

void thing_collect_rope (levelp level, 
                         thingp owner,
                         thingp it)
{
    owner->ropes++;

    player_wid_update(level);

    thing_dead(level, it, owner, "collected rope");
}
