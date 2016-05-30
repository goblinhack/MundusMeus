/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "thing.h"
#include "player.h"

thingp thing_place_bomb (levelp level, 
                         thingp owner,
                         double x, double y)
{
    if (!owner->bombs) {
        return (0);
    }
    owner->bombs--;

    player_wid_update(level);

    widp w = thing_place(level,
                         owner,
                         id_to_tp(THING_BOMB));
    if (!w) {
        ERR("could not place bomb");
        return (0);
    }

    thingp t = wid_get_thing(w);

    /*
     * Set no owner. Bombs should kill their owner too!
     *
    thing_set_owner(t, owner);
     */

    thing_wake(level, t);

    return (t);
}

void thing_collect_bomb (levelp level, 
                         thingp owner,
                         thingp it)
{
    owner->bombs++;

    player_wid_update(level);

    thing_dead(level, it, owner, "collected bomb");
}
