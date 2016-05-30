/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "thing.h"
#include "thing_timer.h"
#include "timer.h"
#include "level.h"
#include "wid_game_map.h"
#include "sound.h"

static void thing_action_timer_callback_dead (levelp level, 
                                              thing_place_context_t *place)
{
    thingp thing = id_to_thing(place->thing_id);

    if (thing) {
        verify(thing);

        thing_dead(level, thing, 0, "callback");
    }

    place->thing_id = 0;
}

static void thing_timer_place_and_destroy_callback (levelp level,
                                                    thing_place_context_t *place)
{
    if (!place->tp) {
        ERR("no thing to place");
    }

    widp w;

    w = wid_game_map_replace_tile(level,
                                  place->x,
                                  place->y,
                                  0, /* thing */
                                  place->tp,
                                  0 /* tpp data */);

    /*
     * Some things like projectiles can go offscrenn.
     */
    if (!w) {
        return;
    }

    /*
     * Just pass the same context along as it has the expire time but add
     * the newborn thing.
     */
    thingp t = wid_get_thing(w);

    if (place->is_epicenter) {
        t->is_epicenter = 1;

        const char *sound = tp_sound_on_creation(place->tp);
        if (sound) {
            sound_play_at(sound, t->x, t->y);
        }

        if (thing_is_explosion(t)) {
            thing_explosion_placed(level, t);
        }

        if (thing_is_bubbles(t)) {
            thing_bubbles_placed(level, t);
        }
    }

    place->thing_id = t->thing_id;

    /*
     * Save the owner of this new thing. This could be who cast a spell.
     */
    thing_set_owner_id(level, t, place->owner_id);
}

static void thing_timer_place_callback (levelp level,
                                        thing_place_context_t *place)
{
    widp w;

    w = wid_game_map_replace_tile(level,
                                  place->x,
                                  place->y,
                                  0, /* thing */
                                  place->tp,
                                  0 /* tpp data */);

    /*
     * Some things like projectiles can go offscrenn.
     */
    if (!w) {
        return;
    }

    thingp t = wid_get_thing(w);

    /*
     * Save the owner of this new thing. This could be who cast a spell.
     */
    thing_set_owner_id(level, t, place->owner_id);
}

/*
 * Various thing timers.
 */
void thing_timers_destroy (levelp level, thingp t)
{
    uint32_t id = thing_id(t);
    thing_place_context_t *p;
    int i;

    if (!id) {
        DIE("no thing id to remove");
    }

    for (i = 0; i < MAX_TIMERS_PER_LEVEL; i++) {
        p = &level->timers[i];
        if (p->thing_id == id) {
            memset(p, 0, sizeof(*p));
        }

        if (p->owner_id == id) {
            memset(p, 0, sizeof(*p));
        }
    }
}

void thing_timers_tick (levelp level)
{
    thing_place_context_t *p;
    int total = 0;
    int i;

    for (i = 0; i < MAX_TIMERS_PER_LEVEL; i++) {
        p = &level->timers[i];
        if (p->fire_in) {
            p->fire_in -= LEVEL_TICK_DELAY_TENTHS * 100;

            if (p->fire_in <= 0) {
                if (p->destroy_in) {
                    thing_timer_place_and_destroy_callback(level, p);
                } else {
                    thing_timer_place_callback(level, p);
                }
                memset(p, 0, sizeof(*p));
            }
            total++;
            continue;
        }

        if (p->destroy_in) {
            p->destroy_in -= LEVEL_TICK_DELAY_TENTHS * 100;

            if (p->destroy_in <= 0) {
                thing_action_timer_callback_dead(level, p);
                memset(p, 0, sizeof(*p));
            }
            total++;
            continue;
        }
    }

#if 0
    CON("total timers %u", total);
#endif
}

/*
 * Place a thing after a delay.
 */
void thing_place_timed (levelp level,
                        tpp tp,
                        double x,
                        double y,
                        uint32_t ms,
                        uint32_t jitter)
{
    thing_place_context_t context = {0};

    context.x = x;
    context.y = y;
    context.tp = tp;
    context.fire_in = ms + jitter;

    thing_place_context_t *p;
    int i;

    for (i = 0; i < MAX_TIMERS_PER_LEVEL; i++) {
        p = &level->timers[i];
        if (!p->fire_in && !p->destroy_in) {
            memcpy(p, &context, sizeof(*p));
#ifdef THING_TIMER_DEBUG
            LOG("%p: place [%s] via timer, %u ms", p, tp_short_name(tp), ms);
#endif
            return;
        }
    }

    ERR("out of timers trying to place [%s]", tp_short_name(tp));
}

/*
 * Place a thing after a delay.
 */
void thing_place_and_destroy_timed (levelp level,
                                    tpp tp,
                                    thingp owner,
                                    double x,
                                    double y,
                                    uint32_t ms,
                                    uint32_t destroy_in,
                                    uint32_t jitter,
                                    uint8_t is_epicenter)
{
    thing_place_context_t context = {0};

    context.x = x;
    context.y = y;
    context.fire_in = ms + jitter;
    context.destroy_in = destroy_in;
    context.tp = tp;
    context.is_epicenter = is_epicenter ? 1 : 0;

    if (owner) {
        context.owner_id = owner->thing_id;
    }

    thing_place_context_t *p;
    int i;

    for (i = 0; i < MAX_TIMERS_PER_LEVEL; i++) {
        p = &level->timers[i];
        if (!p->fire_in && !p->destroy_in) {
            memcpy(p, &context, sizeof(*p));
#ifdef THING_TIMER_DEBUG
            LOG("%p: place [%s] via timer, in %u ms, destroy in %u ms", p, tp_short_name(tp), ms, destroy_in);
#endif
            return;
        }
    }

    ERR("out of timers trying to place [%s]", tp_short_name(tp));
}
