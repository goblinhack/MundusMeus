/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "wid_game_map.h"
#include "cloud.h"
#include "python.h"
#include "string_util.h"

static uint8_t
map_mouse_motion (widp w,
                int32_t x, int32_t y,
                int32_t relx, int32_t rely,
                int32_t wheelx, int32_t wheely)
{

    thingp t = wid_get_thing(w);
    if (!t) {
        return (false);
    }

    py_call_void_module_ptr_iiiii("game",
                                  "map_mouse_over",
                                  w,
                                  t->x, t->y,
                                  wheelx, wheely,
                                  mouse_down);
    return (true);
}

static void
map_mouse_over (widp w,
                int32_t relx, int32_t rely,
                int32_t wheelx, int32_t wheely)
{

    thingp t = wid_get_thing(w);
    if (!t) {
        return;
    }

    py_call_void_module_ptr_iiiii("game",
                                  "map_mouse_over",
                                  w,
                                  t->x, t->y,
                                  wheelx, wheely,
                                  mouse_down);
}

static uint8_t
map_mouse_down (widp w, int32_t x, int32_t y, uint32_t button)
{
    int ret;
    thingp t = wid_get_thing(w);

    if (!t) {
        return (false);
    }

    ret = py_call_int_module_ptr_iii("game",
                                     "map_mouse_down",
                                     w,
                                     t->x, t->y, button);
    return (ret);
}

static uint8_t
map_key_down (widp w, const struct SDL_KEYSYM *key)
{
    int ret;

    ret = py_call_int_module_ptr_ii("game",
                                    "map_key_down",
                                    w,
                                    key->sym, key->mod);
    return (ret);
}

static thingp selection_buttons[MAP_WIDTH][MAP_HEIGHT];

void map_add_selection_buttons (levelp level)
{
    static char tmp[SMALL_STRING_LEN_MAX];
    static int count;

    tpp tp_none = tp_find("none");
    int x, y;

    strncpy(tmp, "focus", sizeof(tmp) -1);

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {

            if (selection_buttons[x][y]) {
                continue;
            }

            itoa05(tmp + sizeof("focus") - 1, count++);
            thingp t = thing_new(tmp, -1 /* thing id */, "focus1");
            t->wid = wid_game_map_replace_tile(x, y, t);

            /*
             * The focus graphic is larger, scale to ti player size.
             */
            wid_set_tilename(t->wid, "player1.1");
            thing_wid_update(t, t->x, t->y, false /* smooth*/);

            t->tp = tp_none;
            t->is_focus = true;
            selection_buttons[x][y] = t;

            wid_set_on_m_motion(t->wid, map_mouse_motion);
            wid_set_on_m_over_b(t->wid, map_mouse_over);
            wid_set_on_m_down(t->wid, map_mouse_down);
            wid_set_on_key_down(t->wid, map_key_down);
        }
    }
}

void map_clear_selection_buttons (levelp level)
{
    tpp tp_none = tp_find("none");
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {

            thingp t = selection_buttons[x][y];
            if (t) {
                t->tp = tp_none;
            }
        }
    }
}

void map_set_selection_buttons (levelp level, int x, int y, const char *what)
{
    tpp tp_what = tp_find(what);

    if (!tp_what) {
        ERR("cannot set map to tp %s", what);
        return;
    }

    thingp t = selection_buttons[x][y];
    if (t) {
        t->tp = tp_what;
    }
}

void map_remove_selection_buttons (levelp level)
{
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {

            thingp t = selection_buttons[x][y];
            if (t) {
                thing_destroyed_(t, "focus cleanup");
            }
        }
    }

    memset(selection_buttons, 0, sizeof(selection_buttons));
}
