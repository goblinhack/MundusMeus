/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "thing_tile.h"
#include "wid_game_map.h"
#include "python.h"

tree_root *things;

thingp thing_scratch[THING_SCRATCH_SIZE];

int things_deco_total;

static void thing_destroy_internal(thingp t);
static int thing_init_done;

uint8_t thing_init (void)
{
    thing_init_done = true;

    return (true);
}

void thing_fini (void)
{
    if (thing_init_done) {
        thing_init_done = false;

        tree_destroy(&things,
            (tree_destroy_func)thing_destroy_internal);
    }
}

thingp thing_new (const char *name,
                  long int thing_id,
                  const char *tp_name)
{
    thingp t;

#ifdef ENABLE_THING_NAME_IS_UNIQUE_CHECK
    if (thing_find(name)) {
        ERR("thing name [%s] already used", name);
    }
#endif

    if (!things) {
        things = tree_alloc(TREE_KEY_STRING, "TREE ROOT: thing");
    }

    t = (thingp) myzalloc(sizeof(*t), "a thing");

    t->tree.key = dupstr(name, "TREE KEY: thing");
    t->thing_id = thing_id;

    if (!tree_insert(things, &t->tree.node)) {
        ERR("thing insert name [%s] failed", name);
    }

    t->tp = tp_find(tp_name);
    if (!t->tp) {
        THING_ERR(t, "thing [%s] not found", tp_name);
    }

    /*
     * Too slow with level changes.
     */
//    THING_LOG(t, "Created thing");

    verify(t);

    return (t);
}

static void thing_destroy_internal (thingp t)
{
    if (t->wid) {
        wid_set_thing(t->wid, 0);
        wid_destroy_nodelay(&t->wid);

        if (t->is_deco) {
            things_deco_total--;
        }
    }
}

void thing_destroyed_ (thingp t, const char *reason)
{
    verify(t);

    /*
     * Too slow with level changes.
     */
//    THING_LOG(t, "Destroyed thing");

    tree_remove_found_node(things, &t->tree.node);

    thing_destroy_internal(t);

    myfree(t);
}

void thing_set_tilename_ (thingp t, const char *tile)
{
    verify(t);

    wid_set_tilename(t->wid, tile);
}

void thing_set_tp_ (thingp t, const char *tp_name)
{
    verify(t);

    t->tp = tp_find(tp_name);
    if (!t->tp) {
        THING_ERR(t, "thing [%s] not found", tp_name);
    }
}

void thing_set_depth_ (thingp t, double value)
{
    verify(t);

    t->depth = value;
}

void thing_move_ (thingp t, double x, double y)
{
    verify(t);

    thing_wid_update(t, x, y, true /* smooth */);
}

PyObject *thing_push_ (thingp t, double x, double y)
{
    verify(t);

    if (!game.tile_width) {
        game.tile_width = get_game_tile_width();
        game.tile_height = get_game_tile_height();
        if (!game.tile_width) {
            DIE("python did not set tile size prior to thing push");
        }
    }

    t->wid = wid_game_map_replace_tile(x, y, t);

    return (Py_BuildValue("K", (uintptr_t) t->wid));
}

void thing_pop_ (thingp t)
{
    verify(t);

    wid_set_thing(t->wid, 0);
    wid_destroy_nodelay(&t->wid);
}

/*
 * Find an existing thing.
 */
thingp thing_find (const char *name)
{
    thing_template target;
    thingp result;

    if (!name) {
        ERR("no name for thing find");
    }

    // memset(&target, 0, sizeof(target)); intentional for speed
    target.tree.key = (char*) name;

    result = (TYPEOF(result)) tree_find(things, &target.tree.node);
    if (!result) {
        return (0);
    }

    return (result);
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

    snprintf(tmp[loop], sizeof(tmp[loop]) - 1, "%s", t->tree.key);

    return (tmp[loop++]);
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

void thing_set_wid (thingp t, widp w)
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
            wid_set_thing(t->wid, 0);
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

uint8_t thing_is_candle_light (thingp t)
{
    verify(t);

    return (thing_tp(t)->is_candle_light);
}

uint8_t thing_is_explosion (thingp t)
{
    verify(t);

    return (thing_tp(t)->is_explosion);
}

void thing_set_is_sleeping (thingp t, uint8_t val)
{
    verify(t);

    t->is_sleeping = val;
}

void thing_set_is_dead (thingp t, uint8_t val)
{
    verify(t);

    t->is_dead = val;
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

    if (tp_is_animated_lr_flip(thing_tp(t))) {
        if (fabs(t->x - t->last_x) <= 1) {
            if (t->x > t->last_x) {
                wid_flip_horiz(t->wid, true);
            } else if (t->x < t->last_x) {
                wid_flip_horiz(t->wid, false);
            }
        }
    }
}

void thing_move_set_dir (thingp t,
                         double *x,
                         double *y,
                         uint8_t up,
                         uint8_t down,
                         uint8_t left,
                         uint8_t right)
{
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
            thing_set_dir_tl(t);
        } else if (right) {
            thing_set_dir_tr(t);
        } else {
            thing_set_dir_up(t);
        }
    } else if (down) {
        if (left) {
            thing_set_dir_bl(t);
        } else if (right) {
            thing_set_dir_br(t);
        } else {
            thing_set_dir_down(t);
        }
    } else if (left) {
        thing_set_dir_left(t);
    } else if (right) {
        thing_set_dir_right(t);
    }
}

void thing_dead (thingp t,
                 thingp killer,
                 const char *reason, ...)
{
    ERR("thing dead TBD");
}

