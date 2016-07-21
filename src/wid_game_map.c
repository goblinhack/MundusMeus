/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include <SDL.h>

#include "main.h"
#include "wid.h"
#include "tile.h"
#include "string_util.h"
#include "wid_textbox.h"
#include "wid_game_map.h"
#include "wid_menu.h"
#include "thing.h"
#include "level.h"
#include "timer.h"
#include "time_util.h"
#include "thing_tile.h"
#include "wid_game_quit.h"
#include "wid_notify.h"
#include "math_util.h"
#include "thing_template.h"
#include "sound.h"
#include "player.h"
#include "python.h"

double last_playery;
double last_playerx;

uint32_t tile_width;
uint32_t tile_height;

uint32_t player_action_bar_changed_at;

uint32_t tile_width;
uint32_t tile_height;

static void wid_game_map_wid_destroy(int keep_player);

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

void wid_game_map_fini (void)
{
    py_call_void_module_void("hooks", "hook_destroy_game");

    wid_game_map_wid_destroy(false /* keep player */);
}

void wid_game_map_init (void)
{
    if (game.wid_grid) {
        return;
    }

    LOG("Create new map");

    wid_game_map_wid_create();

    /*
     * To allow the player to be centered in the new level if it is a
     * different size.
     */
    if (game.wid_grid &&
        game.wid_grid->grid) {
        game.wid_grid->grid->bounds_locked = 0;
    }

    wid_game_map_grid_create();
}

static uint8_t wid_game_map_receive_mouse_motion (
                    widp w,
                    int32_t x, int32_t y,
                    int32_t relx, int32_t rely,
                    int32_t wheelx, int32_t wheely)
{
    if (wheelx || wheely) {
        /*
         * Allow scrolling.
         */
        return (false);
    }

    /*
     * Block moving the window.
     */
    return (true);
}

void wid_game_map_scroll_adjust (levelp level, uint8_t adjust) 
{
    if (!player) {
        return;
    }

    widp w = player->wid;
    if (!w) {
        return;
    }

    verify(w);

    uint32_t gridw;
    uint32_t gridh;

    verify(game.wid_grid);
    wid_get_grid_dim(game.wid_grid, &gridw, &gridh);

    double fgridw = (double)gridw;
    double fgridh = (double)gridh;

    double winw = wid_get_width(game.wid_map);
    double winh = wid_get_height(game.wid_map);

    gridw -= winw;
    gridh -= winh;

    double playerx = wid_get_cx(w);
    double playery = wid_get_cy(w);

    playerx -= wid_get_tl_x(w->parent);
    playery -= wid_get_tl_y(w->parent);

    playerx -= winw / 2.0;
    playery -= winh / 2.0;

    playerx /= fgridw;
    playery /= fgridh;

    if (adjust || (last_playery != playery)) {
        wid_move_to_vert_pct(game.wid_vert_scroll, playery);
    }

    if (adjust || (last_playerx != playerx)) {
        wid_move_to_horiz_pct(game.wid_horiz_scroll, playerx);
    }

    /*
     * Lock the level scroll bar so we don't adjust the level size when 
     * explosions or anything else leaks over the edge.
     */
    game.wid_grid->grid->bounds_locked = 1;

    last_playerx = playerx;
    last_playery = playery;
}

/*
 * Create the wid_game_map
 */
void wid_game_map_grid_create (void)
{
    {
        fpoint tl = {0.00f, 0.00f};
        fpoint br = {1.00f, 1.00f};

        game.wid_grid = wid_new_container(game.wid_map,
                                            "wid game client grid container");

        wid_set_no_shape(game.wid_grid);

        wid_set_color(game.wid_grid, WID_COLOR_TL, BLACK);
        wid_set_color(game.wid_grid, WID_COLOR_BG, BLACK);
        wid_set_color(game.wid_grid, WID_COLOR_BR, BLACK);
        wid_set_on_mouse_motion(game.wid_grid,
                                wid_game_map_receive_mouse_motion);

        wid_set_tl_br_pct(game.wid_grid, tl, br);

        wid_set_on_key_down(game.wid_grid, 
                            player_key);

        wid_set_on_joy_down(game.wid_grid, 
                            player_joy);

        LOG("Created map container window");
    }

    {
        game.tile_width =
                ((1.0f / ((double)TILES_SCREEN_WIDTH)) *
                    (double)wid_get_width(game.wid_map));

        game.tile_height =
                ((1.0f / ((double)TILES_SCREEN_HEIGHT)) *
                    (double)wid_get_height(game.wid_map));

        fpoint tl = { 0, 0 };
        fpoint br = { 0, 0 };

        br.x += game.tile_width;
        br.y += game.tile_height;

        tile_width = br.x - tl.x;
        tile_height = br.y - tl.y;

        wid_new_grid(game.wid_grid,
                     MAP_WIDTH,
                     MAP_HEIGHT, tile_width, tile_height);

        LOG("Created map container window grid");
    }

    game.wid_vert_scroll =
        wid_new_vert_scroll_bar(game.wid_map,
                                game.wid_grid);
    game.wid_horiz_scroll =
        wid_new_horiz_scroll_bar(game.wid_map,
                                 game.wid_grid);

    wid_visible(wid_get_parent(game.wid_vert_scroll), 0);
    wid_visible(wid_get_parent(game.wid_horiz_scroll), 0);
    wid_visible(game.wid_vert_scroll, 0);
    wid_visible(game.wid_horiz_scroll, 0);

    wid_update(game.wid_vert_scroll);
    wid_update(game.wid_horiz_scroll);

    wid_visible(game.wid_grid, true);
}

void wid_game_map_wid_create (void)
{
    if (sdl_is_exiting()) {
        return;
    }

    if (game.wid_map) {
        return;
    }

    wid_notify_flush();

    LOG("Create map");

    {
        fpoint tl = {0.0f, 0.0f};
        fpoint br = {1.0f, 1.0f};

        game.wid_map = wid_new_square_window("wid_game_map");
        wid_lower(game.wid_map);
        wid_set_movable(game.wid_map, false);
        wid_set_do_not_raise(game.wid_map, true);
        wid_set_mode(game.wid_map, WID_MODE_NORMAL);
        wid_set_no_shape(game.wid_map);
        wid_set_tl_br_pct(game.wid_map, tl, br);

        wid_set_on_key_down(game.wid_map, 
                            player_key);

        wid_set_on_joy_down(game.wid_map, 
                            player_joy);
    }

    {
        game.tile_width =
                ((1.0f / ((double)TILES_SCREEN_WIDTH)) *
                    (double)wid_get_width(game.wid_map));

        game.tile_height =
                ((1.0f / ((double)TILES_SCREEN_HEIGHT)) *
                    (double)wid_get_height(game.wid_map));

        fpoint tl = { 0, 0 };
        fpoint br = { 0, 0 };

        br.x += game.tile_width;
        br.y += game.tile_height;

        tile_width = br.x - tl.x;
        tile_height = br.y - tl.y;
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

    thing_wid_update(t, x + dx, y + dy, false /* smooth */);

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

static void wid_game_map_wid_destroy (int keep_player)
{
    if (game.wid_map) {
        if (!keep_player) {
            level_finished_all();
        }

        LOG("Destroy game window");

        wid_destroy(&game.wid_map);
        game.wid_map = 0;
        game.wid_grid = 0;
        game.wid_horiz_scroll = 0;
        game.wid_vert_scroll = 0;
    }
}
