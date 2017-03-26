/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "tile.h"
#include "wid_game_map.h"
#include "thing_tile.h"
#include "wid_notify.h"
#include "math_util.h"
#include "player.h"
#include "python.h"
#include "cloud.h"

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

    /*
     * Ensure placement of random offsets is the same for this thing
     */
    mysrand(t->thing_id);

    thing_tiles = tp_get_tiles(tp);
    if (thing_tiles) {
        thing_tilep thing_tile;

        /*
         * Get the first anim tile.
         */
        thing_tile = (TYPEOF(thing_tile)) tree_root_get_random(thing_tiles);

        /*
         * Find the real tile that corresponds to this name.
         */
        tilename = thing_tile_name(thing_tile);
        tile = tile_find(tilename);

        if (!tile) {
            DIE("tile name [%s] from thing [%s] not found",
                tilename,
                tp_short_name(tp));
            return (0);
        }
    }

    /*
     * Make a new thing.
     */
    child = wid_new_square_button(game.wid_grid, "map tile");

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

    if (tp_is_tree(tp)) {
        /*
         * Side offsets make the trees move into rocks...
         */
        dx = gauss(0.0, 0.25);
        dy = gauss(0.0, 0.3);
        scale = gauss(1.2, 0.10);
    }

    if (tp_is_snow_mound(tp)) {
        /*
         * Side offsets make the trees move into rocks...
         */
        dx = gauss(0.0, 0.1);
        dy = gauss(0.0, 0.2);
        scale = gauss(1.0, 0.05);
    }

    if (tp_is_plant(tp)) {
        dx = gauss(0.0, 0.3);
        dy = gauss(0.0, 0.3);
        scale = gauss(1.0, 0.05);
    }

    if (tp_is_marsh_plant(tp)) {
        dx = gauss(0.0, 0.5);
        dy = gauss(0.0, 0.5);
        scale = gauss(1.0, 0.05);
    }

    if (tp_is_ice(tp)) {
        dx = gauss(0.0, 0.2);
        dy = gauss(0.0, 0.2);
        scale = gauss(1.0, 0.3);

        if ((myrand() % 100) < 5) {
            wid_bounce_to_pct_in(child, 0.1, 1.0,
                                 myrand() % 20000 + 5000,
                                 99999);
        }
    }

    if (scale <= 0) {
        scale = 1.0;
    }

    if (scale != 1.0) {
        wid_scaling_blit_to_pct_in(child, scale, scale, 500, 9999999);
    }

    if (x + dx >= MAP_WIDTH - 1) {
        dx = -dx;
    }

    if (y + dy >= MAP_HEIGHT - 1) {
        dy = -dy;
    }

    if (x + dx <= 1) {
        dx = -dx;
    }

    if (y + dy <= 1) {
        dy = -dy;
    }

    thing_wid_update(t, x + dx, y + dy, false /* smooth */);

    if (tp_is_snow(tp)) {
        if ((myrand() % 100) < 20) {
            t->depth = myrand() % 15;
        }
    }

    if (tp_is_dirt(tp) ||
            tp_is_gravel(tp) ||
            tp_is_gravel_snow(tp) ||
            tp_is_gravel_snow_deco(tp) ||
            tp_is_gravel_deco(tp) ||
            tp_is_rock(tp) ||
            tp_is_landrock(tp) ||
            tp_is_dirt_deco(tp) ||
            tp_is_dirt_snow_deco(tp) ||
            tp_is_grass(tp) ||
            tp_is_grass_snow(tp) ||
            tp_is_grass_deco(tp) ||
            tp_is_grass_snow_deco(tp) ||
            tp_is_sand(tp) ||
            tp_is_sand_snow(tp) ||
            tp_is_sand_deco(tp) ||
            tp_is_sand_snow_deco(tp) ||
            tp_is_snow(tp) ||
            tp_is_snow_deco(tp)) {
        t->depth = myrand() % 75;
    }

    /*
     * If this is a pre-existing thing perhaps being recreated ona new level
     * then it will have a direction already. Update it.
     */
    if (tp_is_animated(thing_tp(t))) {
        thing_animate(t);
    }

    if (tp_is_player(thing_tp(t))) {
        player = t;
    }

    /*
     * This adds it to the grid wid.
     */
    wid_update(child);

    return (child);
}

void wid_game_map_scroll_chunk (int dx, int dy)
{
    widp w = game.wid_grid;

    w->grid->bounds_locked = true;

    if (!w) {
        return;
    }

    widgrid *grid;

    grid = w->grid;
    if (!grid) {
        return;
    }

    int32_t x, y, z;

    double px = -((double)grid->pixwidth) * CHUNK_WIDTH * (double) dx;
    double py = -((double)grid->pixheight) * CHUNK_HEIGHT * (double) dy;
    double tdx = -CHUNK_WIDTH * (double) dx;
    double tdy = -CHUNK_HEIGHT * (double) dy;

    int s = 0;

    for (z = 0; z < Z_DEPTH; z++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            for (y = 0; y < MAP_HEIGHT; y++) {

                tree_root **tree;
                tree = grid->grid_of_trees[z] + (y * grid->width) + x;

                widgridnode *node;

                TREE_WALK_REVERSE_UNSAFE_INLINE(
                            *tree, node,
                            tree_prev_tree_wid_compare_func_fast) {

                    if (s >= (int) ARRAY_SIZE(wid_scratch) - 1) {
                        ERR("exceeded scratch pad size when moving things");
                        continue;
                    }

                    wid_scratch[s++] = node->wid;
                }
            }
        }
    }

    while (s > 0) {
        widp w = wid_scratch[--s];

        thingp t = wid_get_thing(w);
        if (t) {
            if (t->is_focus) {
                continue;
            }
        }

        wid_move_end(w);
        wid_move_delta(w, px, py);

        if (t) {
            thing_move_to(t, t->x + tdx, t->y + tdy);
        }
    }

    /*
     * Scroll the various lighting effects we overlay on the map.
     */
    double copy_light_pulse_amount[MAP_WIDTH][MAP_HEIGHT];
    int copy_light_pulse_dir[MAP_WIDTH][MAP_HEIGHT];
    double copy_floor_offset[MAP_WIDTH][MAP_HEIGHT];

    extern double light_pulse_amount[MAP_WIDTH][MAP_HEIGHT];
    extern int light_pulse_dir[MAP_WIDTH][MAP_HEIGHT];
    extern double floor_offset[MAP_WIDTH][MAP_HEIGHT];

    for (x = 0; x < MAP_WIDTH; x++) {
        uint32_t nx = ((uint32_t)(x + tdx)) % MAP_WIDTH;

        for (y = 0; y < MAP_HEIGHT; y++) {
            uint32_t ny = ((uint32_t)(y + tdy)) % MAP_HEIGHT;

            copy_light_pulse_amount[nx][ny] = light_pulse_amount[x][y];
            copy_light_pulse_dir[nx][ny] = light_pulse_dir[x][y];
            copy_floor_offset[nx][ny] = floor_offset[x][y];
        }
    }

    memcpy(light_pulse_amount, copy_light_pulse_amount, sizeof(light_pulse_amount));
    memcpy(light_pulse_dir, copy_light_pulse_dir, sizeof(light_pulse_dir));
    memcpy(floor_offset, copy_floor_offset, sizeof(floor_offset));

    wid_update(game.wid_grid);

    cloud_move(true /* jumped */);
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

    if (game.wid_game_vert_scroll) {
        wid_move_to_vert_pct(game.wid_game_vert_scroll, playery);
    }

    if (game.wid_game_horiz_scroll) {
        wid_move_to_horiz_pct(game.wid_game_horiz_scroll, playerx);
    }
}
