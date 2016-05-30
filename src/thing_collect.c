/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "thing.h"
#include "player.h"

void thing_collect (levelp level, 
                    thingp owner,
                    thingp it)
{
    owner->gold += tp_get_bonus_gold_on_collect(thing_tp(it));
    owner->hp += tp_get_bonus_hp_on_collect(thing_tp(it));

    player_wid_update(level);

    thing_dead(level, it, owner, "collected");
}
