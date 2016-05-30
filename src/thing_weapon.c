/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include <SDL.h>

#include "main.h"
#include "thing.h"
#include "wid.h"
#include "level.h"
#include "wid_game_map.h"
#include "math_util.h"

void thing_set_weapon_carry_anim_id (levelp level,
                                     thingp t,
                                     uint32_t weapon_carry_anim_id)
{
    thingp weapon_carry_anim;

    if (!weapon_carry_anim_id) {
        thing_set_weapon_carry_anim(level, t, 0);
        return;
    }

    weapon_carry_anim = id_to_thing(weapon_carry_anim_id);

    thing_set_weapon_carry_anim(level, t, weapon_carry_anim);
}

void thing_set_weapon_carry_anim (levelp level,
                                  thingp t, 
                                  thingp weapon_carry_anim)
{
    if (weapon_carry_anim) {
        verify(weapon_carry_anim);
    }

    thingp old_weapon_carry_anim = 
                    thing_weapon_carry_anim(level, t);

    if (old_weapon_carry_anim) {
        if (old_weapon_carry_anim == weapon_carry_anim) {
            return;
        }

        if (weapon_carry_anim) {
            THING_LOG(t, "Weapon carry changed, %s->%s",
                      thing_logname(old_weapon_carry_anim),
                      thing_logname(weapon_carry_anim));
        } else {
            THING_LOG(t, "Remove weapon carry animation, %s",
                      thing_logname(old_weapon_carry_anim));
        }
    } else {
        if (weapon_carry_anim) {
            THING_LOG(t, "Weapon carry anim now, %s",
                      thing_logname(weapon_carry_anim));
        }
    }

    if (weapon_carry_anim) {
        t->weapon_carry_anim_thing_id = thing_id(weapon_carry_anim);

        /*
         * Scale up weapons so they look the same size as the carryer.
         */
        double scale = tp_get_scale(thing_tp(t));
        if (scale != 1.0) {
            wid_scaling_blit_to_pct_in(weapon_carry_anim->wid, scale, scale, 500, 9999999);
        }
    } else {
        t->weapon_carry_anim_thing_id = 0;
    }
}

void thing_set_weapon_swing_anim_id (levelp level,
                                     thingp t,
                                     uint32_t weapon_swing_anim_id)
{
    thingp weapon_swing_anim;

    if (!weapon_swing_anim_id) {
        thing_set_weapon_swing_anim(level, t, 0);
        return;
    }

    weapon_swing_anim = id_to_thing(weapon_swing_anim_id);

    thing_set_weapon_swing_anim(level, t, weapon_swing_anim);
}

void thing_set_weapon_swing_anim (levelp level,
                                  thingp t, thingp weapon_swing_anim)
{
    if (weapon_swing_anim) {
        verify(weapon_swing_anim);
    }

    thingp old_weapon_swing_anim = 
                    thing_weapon_swing_anim(level, t);

    if (old_weapon_swing_anim) {
        if (old_weapon_swing_anim == weapon_swing_anim) {
            return;
        }

        if (weapon_swing_anim) {
            THING_LOG(t, "Weapon_swing_anim change %s->%s",
                      thing_logname(old_weapon_swing_anim),
                      thing_logname(weapon_swing_anim));
        } else {
#ifdef THING_DEBUG
            THING_LOG(t, "Remove weapon_swing_anim %s", thing_logname(old_weapon_swing_anim));
#endif
        }
    } else {
        if (weapon_swing_anim) {
#ifdef THING_DEBUG
            THING_LOG(t, "Weapon_swing_anim %s", thing_logname(weapon_swing_anim));
#endif
        }
    }

    if (weapon_swing_anim) {
        t->weapon_swing_anim_thing_id = weapon_swing_anim->thing_id;

        /*
         * Scale up weapons so they look the same size as the carryer.
         */
        double scale = tp_get_scale(thing_tp(t));
        if (scale != 1.0) {
            wid_scaling_blit_to_pct_in(weapon_swing_anim->wid, scale, scale, 500, 9999999);
        }
    } else {
        t->weapon_swing_anim_thing_id = 0;
    }
}

void thing_weapon_swing_offset (levelp level,
                                thingp t, 
                                double *dx, double *dy)
{
    tpp weapon = thing_weapon(t);
    if (!weapon) {
        return;
    }

    *dx = 0;
    *dy = 0;

    double dist_from_player = 
        ((double)tp_get_swing_distance_from_player(weapon)) / 10.0;

    /*
     * Try current direction.
     */
    if (thing_is_dir_tl(t)) {
        *dx = -dist_from_player;
        *dy = -dist_from_player;
        return;
    }

    /*
     * Careful here - change dy too much and you hit through walls
     */

    if (thing_is_dir_tr(t)) {
        *dx = dist_from_player;
        *dy = -dist_from_player;
        return;
    }

    if (thing_is_dir_bl(t)) {
        *dx = -dist_from_player;
        *dy = dist_from_player;
        return;
    }

    if (thing_is_dir_br(t)) {
        *dx = dist_from_player;
        *dy = dist_from_player;
        return;
    }

    if (thing_is_dir_down(t)) {
        *dy = dist_from_player;
        return;
    }

    if (thing_is_dir_up(t)) {
        *dy = -dist_from_player;
        return;
    }

    if (thing_is_dir_right(t)) {
        *dx = dist_from_player;
        return;
    }

    if (thing_is_dir_left(t)) {
        *dx = -dist_from_player;
        return;
    }

    *dy = dist_from_player + 0.3;
}

thingp thing_weapon_carry_anim (levelp level,
                                thingp t)
{
    thingp weapon_carry_anim = 0;

    if (t->weapon_carry_anim_thing_id) {
        weapon_carry_anim = 
            id_to_thing(t->weapon_carry_anim_thing_id);
    }

    return (weapon_carry_anim);
}

thingp thing_weapon_swing_anim (levelp level,
                                thingp t)
{
    /*
     * If this weapon_swing_anim has its own thing id for animations then 
     * destroy that.
     */
    thingp weapon_swing_anim = 0;

    if (t->weapon_swing_anim_thing_id) {
        weapon_swing_anim = id_to_thing(t->weapon_swing_anim_thing_id);
    }

    return (weapon_swing_anim);
}

void thing_wield_next_weapon (levelp level,
                              thingp t)
{
    if (t->weapon_tp_id) {
        thing_wield(level, t, id_to_tp(t->weapon_tp_id));
    }
}

void thing_unwield (levelp level, 
                    thingp t, 
                    const char *why)
{
    tpp weapon = thing_weapon(t);
    if (!weapon) {
        return;
    }

    THING_LOG(t, "Unwielding current weapon %s, why: %s", 
              tp_short_name(weapon), why);

    thing_sheath(level, t);
}

void thing_sheath (levelp level, thingp t)
{
    tpp weapon = thing_weapon(t);
    if (!weapon) {
        return;
    }

    THING_LOG(t, "Sheathing %s", tp_short_name(weapon));

    /*
     * If this weapon has its own thing id for animations then destroy that.
     */
    thingp weapon_carry_anim = thing_weapon_carry_anim(level, t);
    if (weapon_carry_anim) {
        thing_dead(level, weapon_carry_anim, 0, "owner weapon");
        thing_set_weapon_carry_anim(level, t, 0);
    }

    thingp weapon_swing_anim = thing_weapon_swing_anim(level, t);
    if (weapon_swing_anim) {
        thing_dead(level, weapon_swing_anim, 0, "owner weapon");
        thing_set_weapon_swing_anim(level, t, 0);
    }
}

void thing_wield (levelp level,
                  thingp t, tpp weapon)
{
    if (thing_weapon(t) == weapon) {
        widp weapon_wid = thing_get_weapon_carry_anim_wid(level, t);
        if (weapon_wid) {
            THING_LOG(t, "Already wiedling: %s", tp_short_name(weapon));
            return;
        }
    }

    THING_LOG(t, "Want to wield: %s", tp_short_name(weapon));

    thing_unwield(level, t, "wield new weapon");

    const char *carry_as = tp_weapon_carry_anim(weapon);
    if (!carry_as) {
        THING_ERR(t, "Could not wield weapon %s", tp_short_name(weapon));
        return;
    }

    tpp what = tp_find(carry_as);
    if (!what) {
        THING_ERR(t, "Could not find %s to wield", carry_as);
        return;
    }

    t->weapon_tp_id = tp_to_id(weapon);

    /*
     * Update the thing stats so the weapon inventory changes.
     */
    widp weapon_carry_anim_wid;

    weapon_carry_anim_wid = wid_game_map_replace_tile(
                            level,
                            t->x,
                            t->y,
                            0, /* thing */
                            what,
                            0 /* tpp data */);

    /*
     * Save the thing id so the client wid can keep track of the weapon.
     */
    thingp child = wid_get_thing(weapon_carry_anim_wid);
    thing_set_weapon_carry_anim(level, t, child);

    child->dir = t->dir;

    /*
     * Attach to the thing.
     */
    thing_set_owner(level, child, t);
}

void thing_swing (levelp level, thingp t)
{
    if (t->weapon_swing_anim_thing_id) {
        /*
         * Still swinging.
         */
        return;
    }

    tpp weapon = thing_weapon(t);
    if (!weapon) {
        THING_ERR(t, "No weapon to swing");
        return;
    }

    const char *swung_as = tp_weapon_swing_anim(weapon);
    if (!swung_as) {
        THING_ERR(t, "could not swing %s", tp_short_name(weapon));
        return;
    }

    tpp what = tp_find(swung_as);
    if (!what) {
        THING_ERR(t, "could not find %s to wield", swung_as);
        return;
    }

    /*
     * Put the swing anim on the map
     */
    widp weapon_swing_anim_wid;

    weapon_swing_anim_wid = wid_game_map_replace_tile(
                                    level,
                                    t->x,
                                    t->y,
                                    0, /* thing */
                                    what,
                                    0 /* tpp data */);

    /*
     * Save the thing id so the client wid can keep track of the weapon.
     */
    thingp child = wid_get_thing(weapon_swing_anim_wid);

    /*
     * Attach to the parent thing.
     */
    thing_set_owner(level, child, t);

    child->dir = t->dir;

    thing_set_weapon_swing_anim(level, t, child);

    thing_destroy_in(level, child, 500);

    /*
     * Hide the weapon too or it just floats in the air.
     */
    thingp weapon_carry_anim = thing_weapon_carry_anim(level, t);
    if (weapon_carry_anim) {
        thing_hide(level, weapon_carry_anim);
    }
}

widp thing_get_weapon_carry_anim_wid (levelp level,
                                      thingp t)
{
    thingp weapon = thing_weapon_carry_anim(level, t);
    if (!weapon) {
        return (0);
    }

    return (thing_wid(weapon));
}

/*
 * If a player carrying a weapon moves around, adjust the placement of the 
 * weapon so it is not always in front.
 */
void thing_set_weapon_placement (levelp level,
                                 thingp t)
{
    widp owner_wid = thing_wid(t);
    if (!owner_wid) {
        return;
    }

    widp weapon_wid = thing_get_weapon_carry_anim_wid(level, t);
    if (!weapon_wid) {
        return;
    }

    int depth_modifier = 0;


#if 0
    if (thing_is_dead(t)) {
        depth_modifier = -1;
    } else {
        switch (t->dir) {
        case THING_DIR_NONE:
            depth_modifier = 1;
            break;
        case THING_DIR_LEFT:
            depth_modifier = 1;
            break;
        case THING_DIR_RIGHT:
            depth_modifier = 1;
            break;
        case THING_DIR_UP:
            depth_modifier = 0;
            break;
        case THING_DIR_DOWN:
            depth_modifier = 1;
            break;
        case THING_DIR_TL:
            depth_modifier = 0;
            break;
        case THING_DIR_BL:
            depth_modifier = 0;
            break;
        case THING_DIR_TR:
            depth_modifier = 1;
            break;
        case THING_DIR_BR:
            depth_modifier = 1;
            break;
        }
    }
#endif
    depth_modifier = -1;

    int weapon_depth = wid_get_z_depth(weapon_wid);
    int player_depth = wid_get_z_depth(owner_wid);
    int new_weapon_depth = player_depth + depth_modifier;

    if (new_weapon_depth == weapon_depth) {
        return;
    }

    wid_set_z_depth(weapon_wid, new_weapon_depth);
}
