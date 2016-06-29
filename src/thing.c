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
#include "thing_tile.h"
#include "player.h"
#include "time_util.h"

int things_total;
int monst_things_total;

static uint8_t thing_init_done;

/*
 * What things live on the map.
 */
thing_map_t thing_map;

uint8_t thing_init (void)
{
    thing_init_done = true;

    return (true);
}

void thing_fini (void)
{
    if (thing_init_done) {
        thing_init_done = false;
    }
}

/*
 * Create a new thing.
 */
static void thing_try_to_flush_ids_ (levelp level)
{
    thingp t;

    FOR_ALL_THINGS(level, t)

        if (thing_is_cloud_effect(t)) {
            thing_destroy(level, t, "too many things");
        }

    FOR_ALL_THINGS_END
}

static void thing_try_to_flush_ids (levelp level)
{
    thing_try_to_flush_ids_(level);
}

static void thing_map_dump_ (thing_map_t *map, const char *name)
{
    uint32_t i;
    uint32_t x;
    uint32_t y;

    FILE *fp;

    fp = fopen(name, "w");

    uint32_t width = 0;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            thing_map_cell *cell = &map->cells[x][y];

            width = max(width, cell->count);
        }
    }

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            thing_map_cell *cell = &map->cells[x][y];

            for (i = 0; i < width; i++) {
                uint32_t m = cell->id[i];

                if (!m) {
                    fprintf(fp, "----- ");
                    continue;
                }

                fprintf(fp, "%5u ", m);
            }

            fprintf(fp, "|");
        }
        fprintf(fp, "\n");
    }
}

void thing_map_dump (void)
{
    thing_map_dump_(&thing_map, "thing.map");
}

static void thing_map_sanity_ (levelp level, thing_map_t *map)
{
    uint32_t i;
    uint32_t x;
    uint32_t y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            thing_map_cell *cell = &map->cells[x][y];

            uint8_t found_end = 0;

            for (i = 0; i < MAP_THINGS_PER_CELL; i++) {
                uint32_t m = cell->id[i];

                if (!m) {
                    found_end = true;
                    continue;
                }

                if (found_end) {
                    thing_map_dump();

                    ERR("map elements are not contiguous at %d,%d", x, y);
                }

                thingp t = id_to_thing(m);
                if (!t) {
                    ERR("thing %p id %08X is invalid and on map", t, m);
                }

                verify(t);
            }
        }
    }
}

void thing_map_sanity (levelp level)
{
    thing_map_sanity_(level, &thing_map);
}

void thing_map_remove (levelp level, thingp t)
{
    int i;

    verify(t);

    int32_t x = t->map_x;
    int32_t y = t->map_y;

    /*
     * Check not on the map.
     */
    if ((x == -1) || (y == -1)) {
        return;
    }

    thing_map_t *map = level_map(level);
    thing_map_cell *cell = &map->cells[x][y];

    if (!cell->count) {
        ERR("map count mismatch");
        return;
    }

    /*
     * Remove from the map.
     */
    for (i = 0; i < cell->count; i++) {
        uint32_t m = cell->id[i];
        if (m != thing_id(t)) {
            continue;
        }

        if (i == cell->count - 1) {
            /*
             * Popping last element.
             */
            cell->id[i] = 0;
        } else {
            /*
             * Pop and swap last element.
             */
            cell->id[i] = cell->id[cell->count - 1];
            cell->id[cell->count - 1] = 0;
        }

        cell->count--;

        t->map_x = -1;
        t->map_y = -1;

        return;
    }

    ERR("did not find id %08X/%s on map at %d,%d to remove",
        thing_id(t), thing_logname(t), x, y);
}

void thing_map_add (levelp level, thingp t, int32_t x, int32_t y)
{
    verify(t);

    if (!t->thing_id) {
        DIE("cannot add thing %s ID of 0", thing_logname(t));
    }

    if (x < 0) {
        DIE("map underflow for thing %s", thing_logname(t));
    }

    if (y < 0) {
        DIE("map y underflow for thing %s", thing_logname(t));
    }

    if (x >= MAP_WIDTH) {
        DIE("map x overflow for thing %s", thing_logname(t));
    }

    if (y >= MAP_HEIGHT) {
        DIE("map y overflow for thing %s", thing_logname(t));
    }

    thing_map_t *map = level_map(level);

    /*
     * Check not on the map.
     */
    if ((t->map_x != -1) || (t->map_y != -1)) {
        ERR("thing %s already on map at %d,%d", thing_logname(t),
            t->map_x, t->map_y);
        return;
    }

    uint32_t i;

    /*
     * Sanity check we're not on already.
     */
    thing_map_cell *cell = &map->cells[x][y];

#ifdef ENABLE_MAP_SANITY
    for (i = 0; i < cell->count; i++) {
        uint32_t m = cell->id[i];

        if (!m) {
            continue;
        }

        /*
         * Something is on the map.
         */
        if (m == t->thing_id) {
            /*
             * It's us?
             */
            ERR("already found on map");
        }

        thingp p = id_to_thing(m);
        if (p == t) {
            ERR("already found thing %s on map", thing_logname(t));
        }
    }
#endif

    if (cell->count == MAP_THINGS_PER_CELL) {
        /*
         * Try to find something we can boot out.
         */
        if (thing_is_cloud_effect(t)    ||
            thing_is_explosion(t)       ||
            thing_is_projectile(t)) {
            /*
             * Don't bother. This is a transient thing.
             */
            return;
        }

        /*
         * This is a more important thing. Try and boot out something less
         * important.
         */
        for (i = 0; i < cell->count; i++) {
            uint32_t m = cell->id[i];
            if (!m) {
                DIE("expected to find a map id on the map here");
            }

            thingp p = id_to_thing(m);
            if (!p) {
                DIE("expected to find a thing on the map here at slot %d", m);
            }

            if (thing_is_cloud_effect(p)    ||
                thing_is_explosion(p)       ||
                thing_is_projectile(t)) {
                /*
                 * Kick out this transient thing.
                 */
                thing_map_remove(level, p);
                break;
            }
        }
    }

    if (cell->count == MAP_THINGS_PER_CELL) {
        /*
         * We're hosed.
         */
        ERR("Out of map slots trying to add %s", thing_logname(t));

        for (i = 0; i < cell->count; i++) {
            uint32_t m = cell->id[i];
            thingp p = id_to_thing(m);

            LOG("  slot [%d] id %08X %s", i, m, thing_logname(p));
        }

        return;
    }

    cell->id[cell->count] = t->thing_id;
    cell->count++;

    t->map_x = x;
    t->map_y = y;
}

/*
 * Find a new thing slot on the lvel
 */
static uint32_t thing_new_id (levelp level)
{
    /*
     * Use a different base for monsters so that the IDs we create are going
     * to be contiguous and allows us to optimize when sending map updates.
     */
    uint32_t *next;
    uint32_t id;
    uint32_t min;
    uint32_t max;

    /*
     * Reserve thing id 0 for unused.
     */
    next = &level->next_thing_id;
    id = level->next_thing_id;
    min = 1;
    max = MAX_THINGS_PER_LEVEL - 1;

    if (!id) {
        id = min;
    }

    /*
     * Find a free thing slot
     */
    int looped = 0;

    while (level->things[id].thing_id) {
        id++;
        if (id >= max) {
            id = min;
            looped++;

            /*
             * Try hard to reclaim space.
             */
            if (looped == 2) {
                thing_try_to_flush_ids(level);
            }

            if (looped == 3) {
                ERR("out of thing ids, min %u max %u!", min, max);
            }
        }
    }

    *next = id + 1;
    if (*next >= max) {
        *next = min;
    }

    if (!id) {
        DIE("sanity check, ID 0 never used min %u max %u", min, max);
    }

    return (id);
}

/*
 * Create a new thing.
 */
thingp thing_new (levelp level,
                  const char *name,
                  double x, double y)
{
    thingp t;
    tpp tp;

    tp = tp_find(name);
    if (!tp) {
        ERR("thing [%s] has no template", name);
    }

    uint32_t id = thing_new_id(level);

    t = &level->things[id];
    newptr(t, "thing");
    memset(t, 0, sizeof(*t));

    t->thing_id = id | (myrand() & THING_ID_RAND_MASK);

    t->tp_id = tp_to_id(tp);

    /*
     * New items are top quality.
     */
    t->is_sleeping = tp_is_sleeping(tp);
    t->tick_born = game.tick;
    t->timestamp_sound_random = time_get_time_ms();

    /*
     * Start our with max stats.
     */
    t->hp = tp->max_hp;
    t->max_hp = tp->max_hp;

#if 0
    if (thing_is_player(t)) {
        t->torches = 10;
        t->ropes = 5;
        t->bombs = 20;
        thing_wield(level, t, id_to_tp(THING_SWORD1));
    }
#endif

    thing_reinit(level, t, x, y);

    if (thing_is_monst(t)) {
        monst_things_total++;
    }

    things_total++;

    return (t);
}

/*
 * Reinit this player on a new level
 */
void thing_reinit (levelp level, thingp t, double x, double y)
{
    verify(t);

    /*
     * Start out not on the map.
     */
    t->last_x = -1.0;
    t->last_y = -1.0;
    t->map_x = -1.0;
    t->map_y = -1.0;

    t->x = x;
    t->y = y;
}

void thing_restarted (levelp level, thingp t)
{
    verify(t);

    t->current_tile = 0;

    if (!thing_is_dead(t)) {
        return;
    }

    thing_set_is_dead(t, false);
}

void thing_destroy (levelp level, thingp t, const char *why)
{
    verify(t);

    /*
     * Stop all timers.
     */
    thing_timers_destroy(level, t);

    if (t->wid) {
        thing_map_remove(level, t);
        thing_set_wid(level, t, 0);
    }

    /*
     * Record that the client player may have died so we do not disconnect.
     */
    if (t == player) {
        player_wid_destroy(level);

        player = 0;
    }

    if (thing_is_monst(t)) {
        monst_things_total--;
    }

    things_total--;

    t->thing_id = 0;

    oldptr(t);
}

void thing_destroy_in (levelp level, thingp t, int32_t ms)
{
    t->destroy_in_ms = ms;
}

void thing_wake (levelp level, thingp t)
{
    verify(t);

    if (!t->is_sleeping) {
        return;
    }

    THING_LOG(t, "Wake");

    thing_set_is_sleeping(t, false);
}

void thing_hide (levelp level, thingp t)
{
    verify(t);

    widp w = t->wid;
    if (!w) {
        return;
    }

    if (wid_this_is_hidden(w)) {
        return;
    }

    wid_this_hide(w, 0);
}

void thing_visible (levelp level, thingp t)
{
    verify(t);

    widp w = t->wid;
    if (!w) {
        return;
    }

    if (!wid_this_is_hidden(w)) {
        return;
    }

    /*
     * Reveal the thing.
     */
    wid_this_visible(w, 0);
}

uint8_t thing_is_visible (levelp level, thingp t)
{
    verify(t);

    widp w = t->wid;
    if (w) {
        return (!wid_is_hidden(w));
    }

    return (false);
}

void thing_leave_level (levelp level, thingp t)
{
    if (thing_is_player(t)) {
        THING_LOG(t, "Leave level");
    }

    /*
     * Still in a shop? Sneaky.
     */
    t->gold_owed = 0;

    thing_map_remove(level, t);
    thing_set_wid(level, t, 0);
}

void thing_join_level (levelp level, thingp t)
{
    if (thing_is_player(t)) {
        THING_LOG(t, "Join level");
    }
}

void things_level_destroyed (levelp level, uint8_t keep_player)
{
    thingp t;

    /*
     * Ensure no stale pointers.
     */
    {
        {
            if (keep_player) {
                LEVEL_LOG(level, "Destroy things, keep player");
            } else {
                LEVEL_LOG(level, "Destroy things and player");
            }

            FOR_ALL_THINGS(level, t)

                if (keep_player &&
                    !thing_is_animation(t)) {

                    thing_map_remove(level, t);
                    thing_set_wid(level, t, 0);
                    continue;
                }

                thing_destroy(level, t, "level destroyed");

            FOR_ALL_THINGS_END
        }
    }
}

const char *thing_logname (thingp t)
{
    /*
     * Return constant strings from a small pool.
     */
    static char tmp[10][MAXSTR];
    static int loop;

    verify(t);

    if (loop >= 10) {
        loop = 0;
    }

    snprintf(tmp[loop], sizeof(tmp[loop]) - 1,
             "%s[%p, id %08X]", thing_short_name(t), t, t->thing_id);

    return (tmp[loop++]);
}

const char *thing_short_name (thingp t)
{
    verify(t);

    return (tp_short_name(thing_tp(t)));
}

uint8_t thing_z_depth (thingp t)
{
    verify(t);

    return (tp_get_z_depth(thing_tp(t)));
}

uint8_t thing_z_order (thingp t)
{
    verify(t);

    return (tp_get_z_order(thing_tp(t)));
}

tree_rootp thing_tile_tiles (thingp t)
{
    verify(t);

    return (tp_get_tiles(thing_tp(t)));
}

widp thing_wid (thingp t)
{
    verify(t);

    return (t->wid);
}

void thing_set_wid (levelp level, thingp t, widp w)
{
    verify(t);

    if (w) {
        verify(w);
    } else {
        /*
         * If setting the wid to 0, we're destroying it.
         */
        if (t->wid) {
            verify(t->wid);
            wid_set_thing(t->wid, 0, 0);
            wid_fade_out(t->wid, 100);
            wid_destroy_in(t->wid, 100);
        }
    }

    t->wid = w;
}

uint8_t thing_is_light_source (thingp t)
{
    verify(t);

    return (thing_tp(t)->is_light_source);
}

void thing_set_is_candle_light (thingp t, uint8_t val)
{
    verify(t);

    thing_tp(t)->is_candle_light = val;
}

uint8_t thing_is_candle_light (thingp t)
{
    verify(t);

    return (thing_tp(t)->is_candle_light);
}

void thing_set_is_sleeping (thingp t, uint8_t val)
{
    verify(t);

    t->is_sleeping = val;
}

void thing_set_is_collected (thingp t, uint8_t val)
{
    verify(t);

    t->is_collected = val;
}

uint8_t thing_is_collected (thingp t)
{
    verify(t);

    return (t->is_collected);
}

void thing_set_is_open (thingp t, uint8_t val)
{
    verify(t);

    t->is_open = val;
}

void thing_set_is_angry (thingp t, uint8_t val)
{
    verify(t);

    t->is_angry = val;
}

void thing_set_is_dead (thingp t, uint8_t val)
{
    verify(t);

    t->is_dead = val;
}

const char *thing_name (thingp t)
{
    verify(t);

    return (tp_short_name(thing_tp(t)));
}

const char * thing_tooltip (thingp t)
{
    verify(t);

    return (tp_get_tooltip(thing_tp(t)));
}

tree_rootp thing_tiles (thingp t)
{
    verify(t);

    return (tp_get_tiles(thing_tp(t)));
}

thing_tilep thing_current_tile (thingp t)
{
    verify(t);

    if (!t->current_tile) {
        return (0);
    }

    return (&thing_tile_arr[t->current_tile]);
}

void thing_move_to (thingp t, double x, double y)
{
    verify(t);

    if ((t->last_x == -1.0) && (t->last_y == -1.0)) {
        t->last_x = x;
        t->last_y = y;
    } else {
        t->last_x = t->x;
        t->last_y = t->y;
    }

    t->x = x;
    t->y = y;
}

void thing_move_set_dir (levelp level,
                         thingp t,
                         double *x,
                         double *y,
                         uint8_t up,
                         uint8_t down,
                         uint8_t left,
                         uint8_t right)
{
    /*
     * Projectiles like the fireball anim have their direction set when 
     * created and don't need upating; and if we do we get the angles not 
     * quite right
     */
    if (thing_is_projectile(t)) {
        return;
    }

    double ox = t->x;
    double oy = t->y;

    if (*x < 0) {
        *x = 0;
    }

    if (*y < 0) {
        *y = 0;
    }

    if (*x > MAP_WIDTH - 1) {
        *x = MAP_WIDTH - 1;
    }

    if (*y > MAP_HEIGHT - 1) {
        *y = MAP_HEIGHT - 1;
    }

    if (*x > ox) {
        right = true;
    }

    if (*x < ox) {
        left = true;
    }

    if (*y > oy) {
        down = true;
    }

    if (*y < oy) {
        up = true;
    }

    if (up) {
        if (left) {
            thing_set_dir_tl(level, t);
        } else if (right) {
            thing_set_dir_tr(level, t);
        } else {
            thing_set_dir_up(level, t);
        }
    } else if (down) {
        if (left) {
            thing_set_dir_bl(level, t);
        } else if (right) {
            thing_set_dir_br(level, t);
        } else {
            thing_set_dir_down(level, t);
        }
    } else if (left) {
        thing_set_dir_left(level, t);
    } else if (right) {
        thing_set_dir_right(level, t);
    }
}

void thing_dead (levelp level, 
                 thingp t, 
                 thingp killer, 
                 const char *reason, ...)
{
    ERR("thing dead TBD");
}

