/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

// REMOVED #include <SDL.h>

#include "main.h"
// REMOVED #include "wid.h"
#include "tile.h"
// REMOVED #include "string_util.h"
// REMOVED #include "wid_textbox.h"
#include "wid_game_map.h"
// REMOVED #include "thing.h"
// REMOVED #include "level.h"
// REMOVED #include "timer.h"
// REMOVED #include "time_util.h"
#include "thing_tile.h"
#include "wid_notify.h"
#include "math_util.h"
// REMOVED #include "thing_template.h"
// REMOVED #include "sound.h"
#include "player.h"
#include "python.h"

double last_playery;
double last_playerx;

uint32_t tile_width;
uint32_t tile_height;

uint32_t player_action_bar_changed_at;

uint32_t tile_width;
uint32_t tile_height;

static void wid_game_map_set_thing_template (widp w, tpp t)
{
    wid_set_thing_template(w, t);

    if (tp_is_effect_pulse(t)) {
        wid_scaling_to_pct_in(w, 1.0, 1.05, gauss(500, 10), 9999999);
    }

    if (tp_is_effect_sway(t)) {
        wid_rotate_to_pct_in(w, -2, 2, 500, 9999999);
    }

    if (tp_is_effect_fade_in_out(t)) {
        wid_fade_in_out(w, 4000, 99999, 1);
    }
}

/*
 * Replace or place a tile.
 */
widp
wid_game_map_replace_tile (double x, double y, thingp t)
{
    tree_rootp thing_tiles;
    const char *tilename;
    tpp tp = thing_tp(t);
    tilep tile = 0;
    widp child;

    if ((x < 0) || (y < 0) || (x >= MAP_WIDTH) || (y >= MAP_HEIGHT)) {
        /*
         * Trying to fire a weapon off the end of the level?
         */
        LOG("Thing template [%s] cannot be placed at %f %f, "
            "out of bounds, max %d %d",
            tp_short_name(tp), x, y, MAP_WIDTH, MAP_HEIGHT);
        return (0);
    }

    double dx = 0;
    double dy = 0;

    /*
     * Grow tl and br to fit the template thing. Use the first tile.
     */
    if (!tp) {
        ERR("no thing template to place thing");
        return (0);
    }

    thing_tiles = tp_get_tiles(tp);
    if (thing_tiles) {
        thing_tilep thing_tile;

        /*
         * Get the first anim tile.
         */
        thing_tile = (typeof(thing_tile)) tree_root_get_random(thing_tiles);

        /*
         * Find the real tile that corresponds to this name.
         */
        tilename = thing_tile_name(thing_tile);
        tile = tile_find(tilename);

        if (!tile) {
            DIE("tile name %s from thing %s not found",
            tilename,
            tp_short_name(tp));
            return (0);
        }
    }

    /*
     * Make a new thing.
     */
    child = wid_new_square_button(game.wid_grid,
                                  "map tile");

    wid_set_mode(child, WID_MODE_NORMAL);
    wid_set_no_shape(child);

    /*
     * "paint" the thing.
     */
    wid_set_thing_template(child, tp);

    wid_set_thing(child, t);
    if (tile) {
        wid_set_tile(child, tile);
    }

    /*
     * "paint" the thing.
     */
    wid_game_map_set_thing_template(child, tp);

    /*
     * Does it appear as a different size on screen?
     */
    double scale = tp_get_scale(tp);

    if (scale != 1.0) {
        wid_scaling_blit_to_pct_in(child, scale, scale, 500, 9999999);
    }

    if (tp_is_chasm_smoke(tp)) {
        wid_scaling_blit_to_pct_in(child, 1.0, 2.0, 5000, 9999999);
    }

    thing_wid_update(t, x + dx, y + dy, false /* smooth */);

    if (tp_is_corridor(tp) || tp_is_dusty(tp) || tp_is_bridge(tp)) {
        double d = t->depth;
        if (d > 7) {
            d = 7;
        }
        wid_set_blit_y_offset(child, wid_get_height(child) * -d * 0.10);
    }

    /*
     * If this is a pre-existing thing perhaps being recreated ona new level
     * then it will have a direction already. Update it.
     */
    if (tp_is_animated(thing_tp(t))) {
        thing_animate(t);
    }

    /*
     * This adds it to the grid wid.
     */
#ifdef DEBUG_CLIENT_THING
    wid_update(child);
    char name[20];
    sprintf(name, "%d",thing_id(t));
    wid_set_text(child,name);
#endif

    /*
     * This adds it to the grid wid.
     */
    wid_update(child);

    return (child);
}
