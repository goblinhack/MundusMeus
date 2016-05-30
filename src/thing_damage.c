/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "color.h"
#include "thing.h"
#include "time_util.h"

int32_t thing_stats_get_total_damage (thingp t)
{
    double damage = t->damage;

    if (!damage) {
        damage = tp_get_damage(thing_tp(t));
    }

    double modifier = 0;

    tpp weapon = thing_weapon(t);
    if (weapon) {
        if (!damage) {
            damage = tp_get_damage(weapon);
        }

        /*
         * A wand does no damage, but what if fires does...
         */
        tpp projectile = tp_fires(weapon);
        if (projectile) {
            weapon = projectile;
        }
    }

    /*
     * Modifier of 1 maps to +10 % in damage.
     */
    double final_damage = ceil(damage + (damage * (modifier / 10.0)));

    if (thing_is_monst(t) || thing_is_player(t)) {
        if (weapon) {
            THING_LOG(t, "Attack damage (from %s), mod %d, damage %d -> %d", 
                      weapon ? tp_name(weapon) : 0,
                      (int) modifier, (int) damage, (int) final_damage);
        } else {
            THING_LOG(t, "Attack damage, modifier %d, damage %d -> %d", 
                      (int) modifier, (int) damage, (int) final_damage);
        }
    }

    if (!final_damage) {
        return (final_damage);
    }

    if (final_damage <= 1) {
        return (final_damage);
    }

    /*
     * 1dx damage.
     */
    return (myrand() % (int)final_damage);
}
