/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "thing.h"
#include "time_util.h"
#include "player.h"

void thing_torch_update_count (levelp level, thingp t, int force)
{
    if (force || time_have_x_ticks_passed_since(30 * TICKS_PER_SEC, t->tick_torch)) {
        t->tick_torch = game.tick; 

        /*
         * Use up one torch unit.
         */
        if (t->torches) {
            t->torches--;
            if (!t->torches) {
                MSG_SHOUT_AT(INFO, t, 0, 0, "Your light fizzles out");
            }
        }
    }

    /*
     * Work out the torch light radius. Each torch lights 0.5 radius units.
     */
    float torch_light_radius = (double) t->torches / 2.0;

    if (torch_light_radius > tp_get_light_radius(thing_tp(t))) {
        torch_light_radius = tp_get_light_radius(thing_tp(t));
    }

    if (torch_light_radius != t->torch_light_radius) {
        t->torch_light_radius = torch_light_radius;
        t->torch_light_radius_set = true;
    }
}

thingp thing_place_torch (levelp level, 
                         thingp owner,
                         double x, double y)
{
    if (!owner->torches) {
        return (0);
    }

    owner->torches--;

    player_wid_update(level);

    widp w = thing_place(level,
                         owner,
                         id_to_tp(THING_TORCH));
    if (!w) {
        ERR("could not place torch");
        return (0);
    }

    thingp t = wid_get_thing(w);

    /*
     * Set no owner. torchs should kill their owner too!
     *
    thing_set_owner(t, owner);
     */

    thing_wake(level, t);

    return (t);
}

void thing_collect_torch (levelp level, 
                         thingp owner,
                         thingp it)
{
    owner->torches++;

    player_wid_update(level);

    thing_dead(level, it, owner, "collected torch");
}
