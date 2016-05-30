/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "thing.h"
#include "player.h"

void thing_use_key (levelp level, 
                    thingp owner,
                    double x, double y)
{
    if (!owner->keys) {
        return;
    }

    owner->keys--;

    player_wid_update(level);

    level_open_door(level, x, y);
}

void thing_collect_key (levelp level, 
                        thingp owner,
                        thingp it)
{
    owner->keys++;

    player_wid_update(level);

    thing_dead(level, it, owner, "collected key");
}
