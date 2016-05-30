/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "thing_timer.h"
#include "wid_game_map.h"
#include "string_util.h"
#include "sound.h"
#include "timer.h"
#include "tile.h"
#include "wid_hiscore.h"
#include "thing_shop.h"
#include "thing_tile.h"
#include "player.h"

static void thing_dead_ (levelp level, thingp t, char *reason)
{
    /*
     * Detach from the owner
     */
    thing_remove_hooks(level, t);

    if (!reason) {
        ERR("thing %s dead for no reason? why? why? why?!", thing_logname(t));
    }

    if (thing_is_dungeon_floor(t)   || 
        thing_is_ladder(t)          || 
        thing_is_door(t)            || 
        thing_is_cloud_effect(t)    || 
        thing_is_explosion(t)       || 
        thing_is_projectile(t)      || 
        thing_is_rock(t)            ||
        thing_is_wall(t)) {
        /*
         * Too boring to log.
         */
    } else {
        THING_LOG(t, "Dead, why(%s)", reason);
    }

    if (thing_is_player(t)) {
        /*
         * Tell the poor player they've croaked it.
         */
        hiscore_try_to_add("Happless human", reason, t->score);
    }

    const char *sound = tp_sound_on_death(thing_tp(t));
    if (sound) {
        MSG_SHOUT_AT(SOUND, t, t->x, t->y, "%s", sound);
    }

    /*
     * Move the weapon behind the poor thing.
     */
    thing_set_weapon_placement(level, t);

    myfree(reason);
}

void thing_dead (levelp level, 
                 thingp t, 
                 thingp killer, 
                 const char *reason, ...)
{
    /*
     * If in a shop, this might be the shopkeeper.
     */
    thingp owner = thing_owner(level, t);

    /*
     * If an arrow, this might be an elf.
     */
    thingp real_killer = 0;

    if (killer) {
        real_killer = thing_owner(level, killer);
        if (!real_killer) {
            real_killer = killer;
        }
    }

    va_list args;

    verify(t);

    tpp tp = thing_tp(t);

    /*
     * If the reason of death was collection, some things we do not want
     * to do.
     */
    if (!t->is_collected) {
        /*
         * When it dies, doth it polymorph and thus avoid the reaper?
         * e.g. a mob spawner dying and creating a smaller one.
         */
        const char *polymorph = tp_polymorph_on_death(tp);

        if (thing_is_sawblade(t) && killer) {
            /*
             * Skip polymorph if there is a killer. We want the blades to
             * just vanish and not get more bloody. That only happens if
             * there is no killer and we force a polymorph.
             */
        } else if (polymorph) {
            tpp what = tp_find(polymorph);
            if (!what) {
                ERR("could now find %s to polymorph into on %s death",
                    polymorph, thing_logname(t));
            }

            /*
             * It doth polymorph.
             */
            t->tp_id = tp_to_id(what);
            t->hp = what->max_hp;
            return;
        }

        /*
         * Or perhaps it does die, but spawns something else, like the
         * player dying and creating a mob spawner.
         */
        const char *spawn = tp_spawn_on_death(tp);
        if (spawn) {
            thingp newt = thing_mob_spawn_on_death(level, t);

            /*
             * If this is the player death then give the gravestone a lot of 
             * health or it can be immediately killed by a lingering explosion 
             * that killed the player too.
             */
            if (newt && thing_is_player(t)) {
                newt->hp = 200;
            }
        }
    }

    /*
     * If a wall is gone, remove the decorations.
     */
    if (thing_is_wall(t)) {
        if (!level->is_being_destroyed) {
            /*
             * Keep walls on edge of level to stop things falling off.
             */
            if ((int)t->x <= 0) {
                return;
            }
            if ((int)t->x >= MAP_WIDTH-1) {
                return;
            }
            if ((int)t->y <= 0) {
                return;
            }
            if ((int)t->y >= MAP_HEIGHT-1) {
                return;
            }

            int dx, dy;
            for (dx = -1; dx <= 1; dx++) {
                for (dy = -1; dy <= 1; dy++) {
                    widp w;
                    while (map_find_wall_deco_at(level, t->x + dx, t->y + dy, &w)) {
                        thing_destroy(level, wid_get_thing(w), __FUNCTION__);
                    }
                }
            }
        }
    }
    /*
     * You only die once.
     */
    if (thing_is_dead(t)) {
        return;
    }

    thing_set_is_dead(t, true);

    /*
     * Bounty for the killer?
     */
    uint32_t xp = tp_get_bonus_score_on_death(tp);
    if (xp && real_killer) {
        /*
         * Did someone throw this weapon and gets the xp?
         */
        int32_t val = tp_get_bonus_score_on_death(tp);

        if (val) {
            real_killer->score += val;

            if (thing_is_player(real_killer)) {
#if 0
                if (thing_is_cloud_effect(killer)) {
                    /*
                        * Too many packets if we kill a lot of things in one
                        * go.
                        *
                        * But it looks nice... 8)
                        */
                } else {
#endif
                    MSG_SHOUT_AT(OVER_THING,
                                 t,
                                 0, 0,
                                 "%%%%font=%s$%%%%fg=%s$+%d XP",
                                 "large", "gold",
                                 val);
#if 0
                }
#endif
            }
        }
    }

    /*
     * Flash briefly red on death.
     */
    if (thing_is_monst(t)        ||
        thing_is_mob_spawner(t)  ||
        thing_is_rock(t)         ||
        thing_is_wall(t)         ||
        thing_is_door(t)) {

        widp w = t->wid;
        if (w) {
            wid_set_mode(w, WID_MODE_ACTIVE);
            if (!wid_is_hidden(w)) {
                wid_set_color(w, WID_COLOR_BLIT, RED);
            }
        }
    }

    /*
     * Boom! If this bomb is not being collected then make it blow up.
     */
    {
#if 0
if (thing_is_treasure(t)) {
CON("%s destroyed",thing_logname(t));
if (owner) {
CON("  %s owner is keeper",thing_logname(owner));
}
if (killer) {
CON("  %s killer ",thing_logname(killer));
}
if (real_killer) {
CON("  %s real_killer ",thing_logname(real_killer));
}
}
#endif
        if (!t->is_collected) {
            if (thing_is_bomb(t)        || 
                thing_is_fireball(t)    ||
                thing_is_bullet(t)) {
                level_place_explosion(level,
                                      0, /* owner */
                                      thing_tp(t),
                                      t->x, t->y,
                                      t->x, t->y);
            }

            /*
             * Breaking stuff in a shop? bad idea.
             */
            if (thing_is_treasure(t)) {
                if (owner && thing_is_shopkeeper(owner)) {
                    if (thing_is_player(real_killer)) {
                        shop_break_message(level, real_killer, owner);
                    } else {
                        shop_whodunnit_break_message(level, real_killer, owner);
                    }
                }
            }
        } else {
            /*
             * Collecting a thing?
             */
            if (thing_is_treasure(t)) {
                if (owner && thing_is_shopkeeper(owner)) {
                    if (thing_is_player(real_killer)) {
                        shop_collect_message(level, real_killer, t);
                    }
                }
            }
        }
    }

    /*
     * Stop bouncing or swaying.
     */
    if (t->wid) {
        if (tp_is_effect_pulse(tp)) {
            wid_scaling_to_pct_in(t->wid, 1.0, 1.0, 0, 0);
        }

        if (tp_is_effect_sway(tp)) {
            wid_rotate_to_pct_in(t->wid, 0, 0, 0, 0);
        }
    }

    /*
     * Log the means of death!
     */
    if (reason) {
        va_start(args, reason);
        thing_dead_(level, t, dynvprintf(reason, args));
        va_end(args);
    } else {
        thing_dead_(level, t, 0);
    }
}
