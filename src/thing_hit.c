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

static int thing_hit_ (levelp level,
                       thingp t, 
                       thingp orig_hitter, 
                       thingp real_hitter, 
                       thingp hitter, 
                       int32_t damage)
{
    int32_t orig_damage = damage;

    verify(t);

    /*
     * Cruel to let things keep on hitting you when you're dead
     */
    if (thing_is_dead(t)) {
        return (false);
    }

    if (!damage) {
        /*
         * Could be a spider silkball
         */
        return (false);
    }

    /*
     * Protect player from multiple impact - landing hard on a spike.
     */
    if (thing_is_player(t)) {
        if (!time_have_x_tenths_passed_since(10, t->timestamp_last_hit)) {
            return (false);
        }
        t->timestamp_last_hit = time_get_time_ms();
    }

    t->timestamp_last_attacked = time_get_time_ms();

    if (hitter) {
        if (thing_is_spikes(hitter)) {
            /*
             * Spikes only hurt if you land on them!
             */
            if (t->fall_speed < 0.05) {
                return (false);
            }

            if (thing_is_monst(t) ||
                thing_is_player(t)) {
                hitter->is_bloodied = true;
            }
        }
    }

    /*
     * Take note of the hit so we can send an event to the client.
     *
     * Send this for both player and monsters so we see monsters being 
     * pumelled!
     */
#if 0
CON("%s hit success on %s hitter %s",
    thing_logname(t), 
    hitter ? thing_logname(hitter) : "",
    real_hitter ? thing_logname(real_hitter) : "");
#endif

    thing_effect_hit_success(level, t);

    if (damage > t->hp / 10) {
        thing_effect_hit_crit(level, t);
    }

    /*
     * Keep hitting until all damage is used up or the thing is dead.
     */
#if 0
    THING_LOG(t, "Is hit! hp %d, damage %d", t->hp, damage);
#endif

    /*
     * Clash of swords?
     */
    const char *sound = 0;

    if (orig_hitter) {
        tp_sound_on_hitting_something(thing_tp(orig_hitter));
    } else if (hitter) {
        tp_sound_on_hitting_something(thing_tp(hitter));
    }

    if (sound) {
        if (thing_is_player(real_hitter)) {
            MSG_SHOUT_AT(SOUND, 
                                real_hitter, 
                                t->x, t->y,
                                "%s", sound);
        } else if (thing_is_player(t)) {
            /*
             * Orc hitting player
             */
            MSG_SHOUT_AT(SOUND, t, t->x, t->y, "%s", sound);
        }
    }

    while (damage > 0) {
        if (t->hp <= damage) {
            damage -= t->hp;

            t->hp = 0;

            /*
             * Record who dun it.
             */
            thing_dead(level, t, orig_hitter, "%s",
                       tp_short_name(thing_tp(real_hitter)));

            /*
             * Explodes on death ala Sith Lord? Only a lesser one, mind.
             */
            if (tp_is_combustable(thing_tp(t))) {
                level_place_explosion(level,
                                      0, // owner
                                      thing_tp(t),
                                      t->x, t->y,
                                      t->x, t->y);
            }

            THING_LOG(t, "Is hit terminally, hp %d, damage %d, now dead", 
                      t->hp, damage);

            if (t == orig_hitter) {
                THING_ERR(t, "Hitting thyself? wth.");
                DIE("hitting thyself");
                return (false);
            }

            /*
             * If polymorphed, hit again?
             */
            if (!t->hp) {
                /*
                 * No it really died.
                 */
                break;
            }

            if (!damage) {
                /*
                 * If polymorphed, but we're out of damage.
                 */
                break;
            }
        } else {
            /*
             * A hit, but not enough to kill the thing.
             */
            t->hp -= damage;

            if (t->hp < 0) {
                t->hp = 0;
            }

            THING_LOG(t, "Is hit by (%s) for %u, hp now %d",
                      thing_logname(orig_hitter), damage,
                      t->hp);

            if (t == orig_hitter) {
                THING_ERR(t, "Hitting thyself? wth.");
                DIE("hitting thyself");
                return (false);
            }

            damage = 0;
        }
    }

    /*
     * If a thing that modifies the level dies, update it.
     */
    if (thing_is_dead(t)) {
        /*
         * Destroying one door opens all doors.
         */
        if (thing_is_door(t)) {
            level_open_door(level, t->x-1, t->y);
            level_open_door(level, t->x+1, t->y);
            level_open_door(level, t->x, t->y-1);
            level_open_door(level, t->x, t->y+1);
        }
    } else {
        if (orig_damage > 0) {
            const char *color = "white";
            const char *font = "small";

            if (orig_damage > 20) {
                font = "vlarge";
            } else if (orig_damage > 10) {
                font = "large";
            } else if (orig_damage > 5) {
                font = "medium";
            } else if (orig_damage > 2) {
                font = "small";
            } else {
                font = "vsmall";
            }

            if (thing_is_player(t)) {
                color = "red";
            }

            MSG_SHOUT_AT(OVER_THING, t,
                                t->x, t->y,
                                "%%%%font=%s$%%%%fg=%s$-%d HP",
                                font, color, orig_damage);
        }
    }

    return (true);
}

int thing_hit (levelp level, thingp t, thingp hitter, uint32_t damage)
{
    thingp orig_hitter = hitter;

    verify(t);

    if (t == hitter) {
        THING_ERR(t, "Hitting thyself? wth.");
        DIE("hitting thyself");
        return (false);
    }

    /*
     * If an arrow, this might be an elf.
     */
    thingp real_hitter = 0;

    if (hitter) {
        real_hitter = thing_owner(level, hitter);
        if (!real_hitter) {
            real_hitter = hitter;
        }
    }

    tpp weapon = 0;

#if 0
    if (hitter && thing_owner(level, hitter)) {
        THING_CON(t, "is being hit by %s, owner %s",
                  thing_logname(hitter), 
                  thing_logname(thing_owner(level, hitter)));
    } else {
        THING_CON(t, "is being hit by %s",
                  thing_logname(hitter));
    }
#endif

    verify(t);
    if (hitter) {
        verify(hitter);
    }

    /*
     * Cruel to let things keep on hitting you when you're dead
     */
    if (thing_is_dead(t)) {
        return (false);
    }

    if (hitter && thing_is_dead(hitter)) {
        /*
         * This case is hit if a ghost runs into a player. The ghost takes
         * damage. We don't want the player to keep absorbing hits when
         * already dead though.
         */
        return (false);
    }

    /*
     * Explosions are only dangerous in the intitial blast.
     */
    if (hitter && thing_is_explosion(hitter)) {
        if (time_have_x_ticks_passed_since(10, hitter->tick_born)) {
            return (false);
        }
    }

    /*
     * If this is a thing on the edge of the level acting as a indestructable
     * wall, then don't allow it to be destroyed.
     */
    if (thing_is_joinable(t)) {
        if ((t->x <= 0) || (t->x >= MAP_WIDTH - 1) ||
            (t->y <= 0) || (t->y >= MAP_HEIGHT - 1)) {
            return (false);
        }
    }

    /*
     * Check to see if this is a thing that can be damaged by the hitter.
     */
    if (hitter) {
        /*
         * Walls and doors and other solid object are not damaged by poison
         * or similar effects. Limit it to explosions and the like.
         */
        if (thing_is_door(t)            ||
            thing_is_rock(t)            ||
            thing_is_wall(t)) {

            if (!thing_is_explosion(hitter)     &&
                !thing_is_projectile(hitter)    &&
                !thing_is_weapon_swing_effect(hitter)) {
                /*
                 * Not something that typically damages walls.
                 */
                return (false);
            }
        }

        if (thing_is_weapon_swing_effect(hitter)) {
            if (!hitter->owner_thing_id) {
                /*
                 * Happens with rapid swings as we only allow one active swing
                 * per owner.
                 *
                ERR("swung weapon %s has no owner ID", thing_logname(hitter));
                 */
                return (false);
            }

            /*
             * Get the player swinging the weapon as the hitter.
             */
            hitter = thing_owner(level, hitter);
            if (!hitter) {
                return (false);
            }

            verify(hitter);

            /*
             * Don't let a sword hit too far away else we end up hitting
             * through walls! Actually I now like the effect. Keep it.
             */
#if 0
            double dist = DISTANCE(hitter->x, hitter->y, t->x, t->y);
            if (dist > 1.0) {
                /*
                 * Too far.
                 */
                return (false);
            }
#endif

            /*
             * Get the damage from the weapon being used to swing.
             */
            weapon = thing_weapon(hitter);
            if (!weapon) {
                return (false);
            }

            if (!damage) {
                damage = tp_get_damage(weapon);
            }

        } else if (hitter->owner_thing_id) {
            /*
             * Get the player firing the weapon as the hitter.
             */
            hitter = thing_owner(level, hitter);
            if (!hitter) {
                return (false);
            }

            verify(hitter);
        }

        /*
         * Don't let our own potion hit ourselves!
         */
        if (hitter == t) {
            return (false);
        }

        /*
         * Don't allow one player's potion effect to kill another player.
         */
        if (thing_is_player(hitter) && thing_is_player(t)) {
            return (false);
        }

        if (orig_hitter &&
            (orig_hitter->tp_id == THING_SILKBALL)) {
            /*
             * No inherited spider damage from silk ball.
             */
        } else {
            /*
             * Take the damage from the weapon that hits first.
             */
            if (!damage) {
                if (orig_hitter) {
                    damage = thing_stats_get_total_damage(orig_hitter);
                }
            }

            /*
             * If still no damage, use the thing that did the hitting.
             */
            if (!damage) {
                if (hitter) {
                    damage = thing_stats_get_total_damage(hitter);
                }
            }
        }
    }

    /*
     * Attack a shopkeeper? Not a wise move...
     */
    if (thing_is_shopkeeper(t)) {
        if (hitter && thing_is_player(hitter)) {
            shop_attack_message(level, hitter);
        }
    }

    /*
     * Allow no more hits than x per second by the hitter.
     */
    if (orig_hitter) {
        /*
         * We want the orig hitter, i.e. the sword and not the playet.
         */
        uint32_t delay =
            tp_get_hit_delay_tenths(thing_tp(orig_hitter));

        if (delay) {
            if (!time_have_x_tenths_passed_since(
                    delay, orig_hitter->timestamp_i_attacked)) {

                return (false);
            }

            orig_hitter->timestamp_i_attacked = time_get_time_ms();
        }

        if (t == orig_hitter) {
            THING_ERR(t, "Hitting thyself? wth.");
            DIE("hitting thyself");
            return (false);
        }
    }

    /*
     * Flash briefly red on attempted hits.
     */
    if (thing_is_monst(t)               ||
        thing_is_mob_spawner(t)         ||
        thing_is_rock(t)                ||
        thing_is_wall(t)                ||
        thing_is_door(t)) {

        /*
         * Assume missed due to the logic below where we detect chance.
         */
        if (orig_hitter && thing_is_cloud_effect(orig_hitter)) {
            /*
             * No flashing
             */
        } else {
            thing_effect_hit_miss(level, t);
        }
    }

    /*
     * Does the thing get off being hit.
     */
    if (orig_hitter) {
        /*
         * Only for the player. Player sourced explosions always hit!
         */
        if (hitter && thing_is_player(hitter)) {
            uint32_t can_be_hit_chance = tp_get_can_be_hit_chance(thing_tp(t));
            if (can_be_hit_chance) {
                uint32_t chance = myrand() % (can_be_hit_chance + 1);

                if (chance > damage) {
                    return (false);
                }
            }
        }
    }

    int r;

    r = thing_hit_(level, t, orig_hitter, real_hitter, hitter, damage);

    /*
     * Do we need to kill the original hitter?
     */
    if (orig_hitter) {
        if (thing_is_fragile(orig_hitter)) {
            /*
             * Sawblades get more covered in blood each time they kill
             * something that is warm blooded. But we don't want that to
             * happen for damage from say a bomb. However if the damage is
             * really high then we just stop the blade.
             */
            if (thing_is_sawblade(orig_hitter)) {
                if (thing_is_warm_blooded(t)) {
                    /*
                     * Move to the next most bloody blade
                     */
                    thing_dead(level, orig_hitter, 0, "blood splatter");
                } else {
                    /*
                     * Keep on spinning those blades.
                     */
                }
            } else {
                thing_dead(level, orig_hitter, 0, "self destruct on hitting");
            }
        }
    }

    return (r);
}
