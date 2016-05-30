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
#include "wid_editor.h"
#include "sound.h"
#include "thing_shop.h"
#include "player.h"

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
        if (tp_is_lava(t)) {
            wid_scaling_to_pct_in(w, 1.0, 1.05, gauss(500, 10), 9999999);
        } else if (tp_is_water(t)) {
            wid_scaling_to_pct_in(w, 1.0, 1.02, gauss(500, 10), 9999999);
        } else {
            wid_scaling_to_pct_in(w, 1.0, 1.05, gauss(500, 10), 9999999);
        }
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
    wid_game_map_wid_destroy(false /* keep player */);
}

void wid_game_map_init (void)
{
    if (game.wid_grid) {
        DIE("Asking to recreate the map when it's already created");
    }

    LOG("Create new map");

    wid_game_map_wid_create();

    levelp level = level_load_new(game.level_no);
    if (!level) {
        DIE("no level active");
    }

    level = &game.level;

    tpp tp = tp_find("player");
    if (!tp) {
        DIE("no player template found");
    }

    /*
     * Pop the player off of the map.
     */
    widp w = 
        wid_game_map_replace_tile(level,
                                  0, 0, 
                                  0, /* thing */
                                  tp,
                                  0 /* tpp_data */);
    if (!w) {
        ERR("no player");
    }

    thingp t = wid_get_thing(w);
    player = t;

    thing_move(level, player, t->x, t->y, false, false, false, false, false);

    /*
     * To allow the player to be centered in the new level if it is a
     * different size.
     */
    if (game.wid_grid &&
        game.wid_grid->grid) {
        game.wid_grid->grid->bounds_locked = 0;
    }

    /*
     * Fluid code needs the level pointer.
     */
    game.wid_grid->level = level;

    /*
     * Need one tick for dmap light source generation.
     */
    level_tick(level);

    /*
     * Don't want to call tick all things, just tick the players torch.
     */
    thing_torch_update_count(level, player, false /* force */);

    {
        fpoint tl = {0.00f, 0.00f};
        fpoint br = {1.00f, 1.00f};

        widp tmp = wid_new_container(game.wid_map, "gwid game client grid container");

        wid_set_tl_br_pct(tmp, tl, br);

        wid_set_color(tmp, WID_COLOR_TL, BLACK);
        wid_set_color(tmp, WID_COLOR_BG, BLACK);
        wid_set_color(tmp, WID_COLOR_BR, BLACK);

        wid_raise(tmp);

        wid_fade_out(tmp, 1000);
        wid_destroy_in(tmp, 2000);
    }
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

void wid_game_map_go_back_to_editor (void)
{
    wid_game_map_fini();

    wid_editor(TEST_LEVEL);
}

/*
 * Create the wid_game_map
 */
void wid_game_map_grid_create (levelp level)
{
    {
        fpoint tl = {0.00f, 0.00f};
        fpoint br = {1.00f, 1.00f};

        game.wid_grid = wid_new_container(game.wid_map,
                                            "wid game client grid container");

        wid_set_context(game.wid_grid, level);

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

static void level_set_new_tp (levelp level,
                              int x, int y, int z, 
                              tpp tp, tpp_data data)
{
    memset(&level->map_grid.tile[x][y][z], 0, sizeof(level_map_tile));
    level->map_grid.tile[x][y][z].tp = tp;

    if (data) {
        level->map_grid.tile[x][y][z].data = *data;
    }
}

static void level_get_tp (levelp level,
                          int x, int y, int z, 
                          tpp *tp)
{
    *tp = level->map_grid.tile[x][y][z].tp;
}

/*
 * Replace or place a tile.
 */
widp
wid_game_map_replace_tile (levelp level,
                           double x, double y,
                           thingp t,
                           tpp tp,
                           tpp_data data)
{
#if 0
    static levelp last_level;
    static int flip_x;
    static int flip_y;
    static int rot;

    /*
     * Does not handle rotation of triggers correctly, so commented out for 
     * now
     */
    if (game.level_is_being_loaded) {
        if (level != last_level) {
            last_level = level;

            /*
             * Need to change direction triggers too... Not sure if this is 
             * worth the complexity...
             */
            flip_x = 0;
            flip_y = 0;
            rot = 0;

            if ((myrand() % 100) < 50) {
                flip_x = 1;
            }

            if ((myrand() % 100) < 50) {
                flip_y = 1;
            }

            if ((myrand() % 100) < 50) {
                rot = 1;
            }
        }

        if (flip_x) {
            x = MAP_WIDTH - x - 1;
        }

        if (flip_y) {
            y = MAP_HEIGHT - y - 1;
        }

        if (rot) {
            swap(x, y);
        }

        if ((x < 0) || (y < 0) || (x >= MAP_WIDTH) || (y >= MAP_HEIGHT)) {
            ERR("%s placing thing at bad co-ords %f %f", tp_name(tp), x, y);
            return (0);
        }
    }
#endif


    int z = tp_get_z_depth(tp);
    tree_rootp thing_tiles;
    const char *tilename;
    tilep tile = 0;
    widp child;
    int ix = x;
    int iy = y;
    int depth = level->level_no;

    if (game.level_is_being_loaded == 1) {
        /*
         * Map random things to real things.
         */
        tpp otp = tp;

        /*
         * Some things are only valid in shops.
         */
        tpp floor_tp;
        level_get_tp(level, x, y, MAP_DEPTH_FLOOR, &floor_tp);
        int shop_floor = false;

        if (floor_tp) {
            if (tp_is_shop_floor(floor_tp)) {
                shop_floor = true;
            }
        }

        switch (tp_to_id(tp)) {
            case THING_FOOD_ANY:
                tp = random_food();
                break;
            case THING_MOB_ANY:
                tp = random_mob(depth);
                break;
            case THING_MONST_ANY:
                tp = random_monst(depth);
                break;
            case THING_TREASURE_ANY:
                tp = random_treasure(shop_floor);
                break;
            case THING_WEAPON_ANY:
                tp = random_weapon(shop_floor);
                break;
        }

        if (!tp) {
            ERR("failed to make random %s", tp_name(otp));
            return (0);
        }
    } else {
        switch (tp_to_id(tp)) {
            case THING_FOOD_ANY:
            case THING_MOB_ANY:
            case THING_MONST_ANY:
            case THING_TREASURE_ANY:
            case THING_WEAPON_ANY:
                level_get_tp(level, x, y, z, &tp);
                break;
        }
    }

    /*
     * First pass? Only interested in location of triggers.
     */
    if (game.level_is_being_loaded == 1) {
        /*
         * Record what triggers exist on the level.
         */
        if (tp_is_action_trigger(tp)) {
            if (!data) {
                ERR("expecting trigger data for %s", tp_name(tp));
                return (0);
            }

            level_trigger_alloc(level, data->col);
        }

        level_set_new_tp(level, x, y, z, tp, data);

        return (0);
    }

    /*
     * Second pass, do not create things that are to only be created on 
     * triggers.
     */
    if (game.level_is_being_loaded == 2) {
        /*
         * If there is a trigger here 
         */
        if (z > MAP_DEPTH_FLOOR) {
            tpp trigger = level->map_grid.tile[ix][iy][MAP_DEPTH_ACTIONS].tp;

            if (trigger) {
                if (tp_is_action_sleep(trigger)) {
                    return (0);
                }
            }
        }
    }

    /*
     * Second pass, create the things.
     */

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

    if (tp_is_smallrock(tp)) {
        dx = gaussrand(0.0, 0.5);
        tp = random_smallrock();
    }

    /*
     * If we find a player, it is really a placeholder of where to put a 
     * future player who joins.
     */
    if (tp_is_player(tp)) {
        if ((x == 0) && (y == 0)) {
            /*
             * Grab a position from the list loaded.
             */
            x = level->player_start_position.x;
            y = level->player_start_position.y;
            level->player_start_at++;

            if (level->player_start_at >= level->player_start_max) {
                level->player_start_at = 0;
            }
        } else {
            /*
             * Append to the array of possible start positions.
             */
            level->player_start_position.x = x;
            level->player_start_position.y = y;
            level->player_start_max++;

            return (0);
        }
    }

    /*
     * Grow tl and br to fit the template thing. Use the first tile.
     */
    if (!tp) {
        ERR("no thing template to place on server map");
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

    if (!t) {
        t = thing_new(level, tp_name(tp), x, y);
    } else {
        thing_reinit(level, t, x, y);
    }

    if (data) {
        memcpy(&t->data, data, sizeof(thing_template_data));
        if (!color_none(t->data.col)) {
            wid_set_color(child, WID_COLOR_BLIT, t->data.col);
        }
    }

    wid_set_thing(child, level, t);
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

    /*
     * So we have baby and bigger slimes. But alas this is visual only and has 
     * no effect on hp.
     */
    if (thing_is_variable_size(t)) {
        scale += gaussrand(0.0, 0.05);
    }

    if (scale != 1.0) {
        wid_scaling_blit_to_pct_in(child, scale, scale, 500, 9999999);
    }

    thing_wid_update(level,
                     t, 
                     x + dx, y + dy, false /* smooth */, true /* is new */);

    /*
     * If this is a pre-existing thing perhaps being recreated ona new level
     * then it will have a direction already. Update it.
     */
    if (thing_is_animated(t)) {
        thing_animate(level, t);
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

    const char *sound = tp_sound_on_creation(tp);
    if (sound) {
        if (thing_is_explosion(t) ||
            thing_is_cloud_effect(t)) {

            /*
             * Let the caller do this for the epicenter
             */
        } else {
            sound_play_at(sound, t->x, t->y);
        }
    }

    if (thing_is_explosion(t)) {
        static int flip;
        flip = !flip;
        wid_flip_horiz(child, flip);
    }
if (thing_can_roll(t)) {
t->fall_speed = gauss(0, 0.0);
t->momentum = gauss(0, 0.15);
}
    /*
     * This adds it to the grid wid.
     */
    wid_update(child);

    /*
     * Avoid doing the map fixup all the time as it is expensive.
     */
    if (game.level_is_being_loaded) {
        return (child);
    }

    /*
     * STUFF ADDED HERE WILL BE SKIPPED BY THE ABOVE CHECK
     */

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
