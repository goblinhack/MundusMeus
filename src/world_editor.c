/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#include <SDL.h>
#include <errno.h>

#include "main.h"
#include "wid.h"
#include "color.h"
#include "string_ext.h"
#include "ttf.h"
#include "thing_template.h"
#include "world_editor.h"
#include "time_util.h"
#include "timer.h"
#include "level.h"
#include "math_util.h"
#include "thing_template.h"
#include "thing_tile.h"
#include "wid_tooltip.h"
#include "wid_intro.h"
#include "tile.h"
#include "marshal.h"
#include "wid_map.h"
#include "wid_cmap.h"
#include "wid_menu.h"
#include "wid_keyboard.h"
#include "wid_game_map.h"
#include "string_util.h"
#include "wid_console.h"
#include "bits.h"
#include "thing.h"

static void world_editor_hide(void);
static void world_editor_tile_right_button_pressed(int x, int y);
static void world_editor_set_focus(world_editor_ctx *ctx, int focus_x, int focus_y);
static void world_editor_world_scroll(int dx, int dy);
static void world_editor_undo_save(void);
static void world_editor_save_level(void);
static void world_editor_save(const char *dir_and_file, int is_test_level);
static void world_editor_button_animate(widp b, tpp tp);
static void world_editor_tile_fill(int x, int y);
static void world_editor_fixup(void);

static widp world_editor_save_popup; // edit world_editor_tick if you add more
static widp world_editor_world_dialog;
static widp wid_choose_color_dialog;
static widp wid_choose_title_dialog;
static widp wid_choose_text_dialog;
widp world_editor_window;
static widp world_editor_background;
static world_editor_ctx *world_editor_window_ctx;

static uint32_t saved_level_no = -1;
static int saved_focus_x = -1;
static int saved_focus_y = -1;
static int saved_world_x = -1;
static int saved_world_y = -1;
static tpp world_editor_chosen_tile[WORLD_TILE_POOL_MAX];

static void world_editor_set_mode (int edit_mode)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    ctx->edit_mode = edit_mode;

    memset(ctx->world_highlight, 0, sizeof(ctx->world_highlight));

    ctx->got_line_start = 0;
    ctx->got_square_start = 0;
    ctx->got_cut_start = 0;
}

static void world_editor_set_new_tp (int x, int y, int z, 
                                     tpp tp)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    memset(&ctx->map.tile[x][y][z], 0, sizeof(world_editor_world_tile));
    ctx->map.tile[x][y][z].tp = tp;
}

/*
 * Replace or place a tile.
 */
widp world_editor_replace_template (levelp level,
                                    double x,
                                    double y,
                                    thingp t,
                                    tpp tp)
{
    /*
     * Can't use w as it has the level set as its context now.
     */
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    int ix = (int)x;
    int iy = (int)y;
    int z = tp_get_world_depth(tp);

    if ((ix >= WORLD_WIDTH) || (iy >= WORLD_HEIGHT) || 
        (ix < 0) || (iy < 0) ||
        (z > WORLD_DEPTH)) {
        ERR("overflow in reading position "
            "(%f,%f,%d) -> (%d,%d) in level %d, "
            "map bounds (%d,%d) -> (%d,%d)", 
            x, y, z, ix, iy, ctx->loading_level_no,
            0, 0, WORLD_DEPTH, WORLD_HEIGHT);
        return (0);
    }

    if (tp_is_player(tp)) {
        ctx->world_x = ix;
        ctx->world_y = iy;
        ctx->world_x -= WORLD_EDITOR_MENU_WORLD_ACROSS / 2;
        ctx->world_y -= WORLD_EDITOR_MENU_WORLD_DOWN / 2;

        while (ctx->world_x < 0) {
            ctx->world_x++;
        }

        while (ctx->world_y < 0) {
            ctx->world_y++;
        }

        while (ctx->world_x + WORLD_EDITOR_MENU_WORLD_ACROSS >= WORLD_WIDTH - 1) {
            ctx->world_x--;
        }

        while (ctx->world_y + WORLD_EDITOR_MENU_WORLD_DOWN >= WORLD_HEIGHT - 1) {
            ctx->world_y--;
        }
    }

    world_editor_set_new_tp(ix, iy, z, tp);

    return (0);
}

#if 0
static tpp world_editor_find_wall_at (int x, int y)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    if ((x < 0) || (y < 0) || (x >= WORLD_WIDTH) || (y >= WORLD_HEIGHT)) {
        return (0);
    }

    tpp tp = ctx->map.tile[x][y][WORLD_DEPTH_WALL].tp;
    if (tp && tp_is_wall(tp)) {
        return (tp);
    }

    return (0);
}

static tpp world_editor_find_door_at (int x, int y)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    if ((x < 0) || (y < 0) || (x >= WORLD_WIDTH) || (y >= WORLD_HEIGHT)) {
        return (0);
    }

    tpp tp = ctx->map.tile[x][y][WORLD_DEPTH_WALL].tp;
    if (tp && tp_is_door(tp)) {
        return (tp);
    }

    return (0);
}
#endif

static void world_editor_fixup (void)
{
#if 0
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    int32_t index;
    tilep tile;
    int32_t x;
    int32_t y;
    int32_t dx;
    int32_t dy;
    tpp nbrs[3][3];

    for (y = 0; y < WORLD_HEIGHT; y++) {
        for (x = 0; x < WORLD_WIDTH; x++) {

            ctx->map.world_tile[x][y] = 0;

            int ok = false;

            if (world_editor_find_wall_at(x, y)) {
                ok = true;
            } else if (world_editor_find_door_at(x, y)) {
                ok = true;
            }

            if (!ok) {
                continue;
            }

            memset(nbrs, 0, sizeof(nbrs));

            for (dx = -1; dx <= 1; dx++) {
                for (dy = -1; dy <= 1; dy++) {

                    if (world_editor_find_wall_at(x, y)) {
                        nbrs[dx + 1][dy + 1] = world_editor_find_wall_at(x + dx, y + dy);
                    }

                    if (world_editor_find_door_at(x, y)) {
                        nbrs[dx + 1][dy + 1] = world_editor_find_door_at(x + dx, y + dy);
                    }
                }
            }

            tpp a = nbrs[0][0];
            tpp b = nbrs[1][0];
            tpp c = nbrs[2][0];
            tpp d = nbrs[0][1];
            tpp e = nbrs[1][1];
            tpp f = nbrs[2][1];
            tpp g = nbrs[0][2];
            tpp h = nbrs[1][2];
            tpp i = nbrs[2][2];

            uint8_t A = (a != 0) ? 1 : 0;
            uint8_t B = (b != 0) ? 1 : 0;
            uint8_t C = (c != 0) ? 1 : 0;
            uint8_t D = (d != 0) ? 1 : 0;
            uint8_t E = (e != 0) ? 1 : 0;
            uint8_t F = (f != 0) ? 1 : 0;
            uint8_t G = (g != 0) ? 1 : 0;
            uint8_t H = (h != 0) ? 1 : 0;
            uint8_t I = (i != 0) ? 1 : 0;

            const uint32_t omask =
                (I << 8) | (H << 7) | (G << 6) | (F << 5) |
                (E << 4) | (D << 3) | (C << 2) | (B << 1) |
                (A << 0);

            uint8_t score;
            uint8_t best = 0;

            index = -1;

            uint32_t mask;

#define BLOCK(a, b, c, d, e, f, g, h, i, _index_)                               \
            mask =                                                      \
                (i << 8) | (h << 7) | (g << 6) | (f << 5) |             \
                (e << 4) | (d << 3) | (c << 2) | (b << 1) |             \
                (a << 0);                                               \
                                                                        \
            if ((mask & omask) == mask) {                               \
                uint32_t difference = mask ^ omask;                     \
                BITCOUNT(difference);                                   \
                score = 32 - difference;                                \
                if (score > best) {                                     \
                    best = score;                                       \
                    index = _index_;                                    \
                }                                                       \
            }                                                           \

            BLOCK(1,1,1,1,1,1,1,1,1, IS_JOIN_BLOCK)
            BLOCK(0,0,0,0,1,0,0,0,0, IS_JOIN_NODE)
            BLOCK(0,0,0,0,1,1,0,0,0, IS_JOIN_LEFT)
            BLOCK(0,0,0,0,1,0,0,1,0, IS_JOIN_TOP)
            BLOCK(0,0,0,1,1,0,0,0,0, IS_JOIN_RIGHT)
            BLOCK(0,1,0,0,1,0,0,0,0, IS_JOIN_BOT)
            BLOCK(0,0,0,1,1,1,0,0,0, IS_JOIN_HORIZ)
            BLOCK(0,1,0,0,1,0,0,1,0, IS_JOIN_VERT)
            BLOCK(0,0,0,0,1,1,0,1,1, IS_JOIN_TL2)
            BLOCK(0,1,1,0,1,1,0,0,0, IS_JOIN_BL2)
            BLOCK(1,1,0,1,1,0,0,0,0, IS_JOIN_BR2)
            BLOCK(0,0,0,1,1,0,1,1,0, IS_JOIN_TR2)
            BLOCK(0,0,0,0,1,1,0,1,0, IS_JOIN_TL)
            BLOCK(0,1,0,0,1,1,0,0,0, IS_JOIN_BL)
            BLOCK(0,1,0,1,1,0,0,0,0, IS_JOIN_BR)
            BLOCK(0,0,0,1,1,0,0,1,0, IS_JOIN_TR)
            BLOCK(1,1,0,1,1,0,1,1,0, IS_JOIN_T90_3)
            BLOCK(1,1,1,1,1,1,0,0,0, IS_JOIN_T180_3)
            BLOCK(0,1,1,0,1,1,0,1,1, IS_JOIN_T270_3)
            BLOCK(0,0,0,1,1,1,1,1,1, IS_JOIN_T_3)
            BLOCK(0,1,0,0,1,1,0,1,0, IS_JOIN_T270)
            BLOCK(0,1,0,1,1,1,0,0,0, IS_JOIN_T180)
            BLOCK(0,1,0,1,1,0,0,1,0, IS_JOIN_T90)
            BLOCK(0,0,0,1,1,1,0,1,0, IS_JOIN_T)
            BLOCK(0,1,1,0,1,1,0,1,0, IS_JOIN_T270_2)
            BLOCK(1,1,0,1,1,1,0,0,0, IS_JOIN_T180_2)
            BLOCK(0,1,0,1,1,0,1,1,0, IS_JOIN_T90_2)
            BLOCK(0,0,0,1,1,1,0,1,1, IS_JOIN_T_2)
            BLOCK(0,1,0,0,1,1,0,1,1, IS_JOIN_T270_1)
            BLOCK(0,1,1,1,1,1,0,0,0, IS_JOIN_T180_1)
            BLOCK(1,1,0,1,1,0,0,1,0, IS_JOIN_T90_1)
            BLOCK(0,0,0,1,1,1,1,1,0, IS_JOIN_T_1)
            BLOCK(0,1,0,1,1,1,0,1,0, IS_JOIN_X)
            BLOCK(0,1,0,1,1,1,0,1,1, IS_JOIN_X1)
            BLOCK(0,1,1,1,1,1,0,1,0, IS_JOIN_X1_270)
            BLOCK(1,1,0,1,1,1,0,1,0, IS_JOIN_X1_180)
            BLOCK(0,1,0,1,1,1,1,1,0, IS_JOIN_X1_90)
            BLOCK(0,1,0,1,1,1,1,1,1, IS_JOIN_X2)
            BLOCK(0,1,1,1,1,1,0,1,1, IS_JOIN_X2_270)
            BLOCK(1,1,1,1,1,1,0,1,0, IS_JOIN_X2_180)
            BLOCK(1,1,0,1,1,1,1,1,0, IS_JOIN_X2_90)
            BLOCK(0,1,1,1,1,1,1,1,0, IS_JOIN_X3)
            BLOCK(1,1,0,1,1,1,0,1,1, IS_JOIN_X3_180)
            BLOCK(0,1,1,1,1,1,1,1,1, IS_JOIN_X4)
            BLOCK(1,1,1,1,1,1,0,1,1, IS_JOIN_X4_270)
            BLOCK(1,1,1,1,1,1,1,1,0, IS_JOIN_X4_180)
            BLOCK(1,1,0,1,1,1,1,1,1, IS_JOIN_X4_90)

            /*
             * Single node doors need to join onto walls.
             */
            if (index == IS_JOIN_NODE) {
                if (world_editor_find_door_at(x, y)) {
                    if ( world_editor_find_wall_at(x - 1, y) &&
                         world_editor_find_wall_at(x + 1, y) &&
                        !world_editor_find_wall_at(x, y - 1) &&
                        !world_editor_find_wall_at(x, y + 1)) {
                        index = IS_JOIN_HORIZ2;
                    }
                }
            }

            if (index == IS_JOIN_NODE) {
                if (world_editor_find_door_at(x, y)) {
                    if (!world_editor_find_wall_at(x - 1, y) &&
                        !world_editor_find_wall_at(x + 1, y) &&
                         world_editor_find_wall_at(x, y - 1) &&
                         world_editor_find_wall_at(x, y + 1)) {
                        index = IS_JOIN_VERT2;
                    }
                }
            }

            if (index == -1) {
                ERR("%u%u%u %u%u%u %u%u%u not handled",
                    a ? 1 : 0,
                    b ? 1 : 0,
                    c ? 1 : 0,
                    d ? 1 : 0,
                    e ? 1 : 0,
                    f ? 1 : 0,
                    g ? 1 : 0,
                    h ? 1 : 0,
                    i ? 1 : 0);
            }

            tpp t = e;

            thing_tilep thing_tile = thing_tile_find(t, index, &tile);
            if (!thing_tile) {
                index = IS_JOIN_BLOCK;

                thing_tile = thing_tile_find(t, index, &tile);
                if (!thing_tile) {
                    index = IS_JOIN_NODE;

                    thing_tile = thing_tile_find(t, index, &tile);
                    if (!thing_tile) {
                        ERR("no joinable tile for %s", tp_name(t));
                    }
                }
            }

            if (!tile) {
                ERR("no tile for %s", tp_name(t));
            }

            const char *tilename = thing_tile_name(thing_tile);

            if (!tilename) {
                ERR("no tilename for %s", tp_name(e));
            }

            tilep tile = tile_find(tilename);
            if (!tile) {
                ERR("cannot find tilep for tile %s", tilename);
            }

            ctx->map.world_tile[x][y] = tile;
        }
    }
#endif
}

/*
 * Set the edit mode focus.
 */
static void world_editor_update_edit_mode_buttons (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    /*
     * Reset all buttons.
     */
    int i;
    for (i = 0; i < WORLD_EDITOR_MODE_MAX; i++) {
        widp b = ctx->tile[i][WORLD_EDITOR_MENU_CELLS_DOWN - 1].button;
        if (!b) {
            continue;
        }

        color c = CYAN;
        c.a = 50;
        wid_set_color(b, WID_COLOR_BG, c);

        c = WHITE;
        c.a = 100;
        wid_set_color(b, WID_COLOR_TL, c);

        c = GRAY;
        c.a = 100;
        wid_set_color(b, WID_COLOR_BR, c);

        wid_set_color(b, WID_COLOR_TEXT, GRAY);
    }

    int x;
    for (x = 0; x < WORLD_EDITOR_MODE_MAX; x++) {
        widp b = ctx->tile[x][WORLD_EDITOR_MENU_CELLS_DOWN - 1].button;
        if (!b) {
            continue;
        }

        color c = BLACK;
        c.a = 100;
        wid_set_color(b, WID_COLOR_BG, c);

        switch (x) {
        case WORLD_EDITOR_MODE_DRAW:
        case WORLD_EDITOR_MODE_LINE:
        case WORLD_EDITOR_MODE_FILL:
        case WORLD_EDITOR_MODE_DEL:
        case WORLD_EDITOR_MODE_UNDO:
        case WORLD_EDITOR_MODE_REDO:
            break;

        case WORLD_EDITOR_MODE_TOGGLE:
            if (world_editor_chosen_tile[ctx->tile_pool]) {
                tpp tp = world_editor_chosen_tile[ctx->tile_pool];

                wid_set_thing_template(b, tp);
                world_editor_button_animate(b, tp);

                wid_set_color(b, WID_COLOR_TL, RED);
                wid_set_color(b, WID_COLOR_BR, RED);
            }
            break;
        }
    }

    /*
     * Set the current focus.
     */
    widp b = ctx->tile[ctx->edit_mode][WORLD_EDITOR_MENU_CELLS_DOWN - 1].button;
    color c = RED;
    c.a = 100;
    wid_set_color(b, WID_COLOR_BG, c);
    c.a = 255;
    wid_set_color(b, WID_COLOR_TL, RED);
    wid_set_color(b, WID_COLOR_BR, RED);
    wid_set_color(b, WID_COLOR_TEXT, GREEN);
}

static tilep world_editor_tp_to_tile (tpp tp)
{
    if (!tp) {
        return (0);
    }

    int tick = time_get_time_ms() / 100;
    int which = tick % 8;

    const char *tn = tp_raw_name(tp);

    char tilename[40];

    switch (which) {
        case 0: snprintf(tilename, sizeof(tilename) - 1, "%s-right", tn); break;
        case 1: snprintf(tilename, sizeof(tilename) - 1, "%s-br", tn); break;
        case 2: snprintf(tilename, sizeof(tilename) - 1, "%s-down", tn); break;
        case 3: snprintf(tilename, sizeof(tilename) - 1, "%s-bl", tn); break;
        case 4: snprintf(tilename, sizeof(tilename) - 1, "%s-left", tn); break;
        case 5: snprintf(tilename, sizeof(tilename) - 1, "%s-tl", tn); break;
        case 6: snprintf(tilename, sizeof(tilename) - 1, "%s-up", tn); break;
        case 7: snprintf(tilename, sizeof(tilename) - 1, "%s-tr", tn); break;
    }

    tilep tile = tile_find(tilename);
    if (tile) {
        return (tile);
    }

    switch (which) {
        case 0: snprintf(tilename, sizeof(tilename) - 1, "%s1a-right", tn); break;
        case 1: snprintf(tilename, sizeof(tilename) - 1, "%s1a-br", tn); break;
        case 2: snprintf(tilename, sizeof(tilename) - 1, "%s1a-down", tn); break;
        case 3: snprintf(tilename, sizeof(tilename) - 1, "%s1a-bl", tn); break;
        case 4: snprintf(tilename, sizeof(tilename) - 1, "%s1a-left", tn); break;
        case 5: snprintf(tilename, sizeof(tilename) - 1, "%s1a-tl", tn); break;
        case 6: snprintf(tilename, sizeof(tilename) - 1, "%s1a-up", tn); break;
        case 7: snprintf(tilename, sizeof(tilename) - 1, "%s1a-tr", tn); break;
    }

    tile = tile_find(tilename);
    if (tile) {
        return (tile);
    }

    switch (which) {
        case 0: snprintf(tilename, sizeof(tilename) - 1, "%sa-right", tn); break;
        case 1: snprintf(tilename, sizeof(tilename) - 1, "%sa-br", tn); break;
        case 2: snprintf(tilename, sizeof(tilename) - 1, "%sa-down", tn); break;
        case 3: snprintf(tilename, sizeof(tilename) - 1, "%sa-bl", tn); break;
        case 4: snprintf(tilename, sizeof(tilename) - 1, "%sa-left", tn); break;
        case 5: snprintf(tilename, sizeof(tilename) - 1, "%sa-tl", tn); break;
        case 6: snprintf(tilename, sizeof(tilename) - 1, "%sa-up", tn); break;
        case 7: snprintf(tilename, sizeof(tilename) - 1, "%sa-tr", tn); break;
    }

    tile = tile_find(tilename);
    if (tile) {
        return (tile);
    }

    /*
     * Just find the first tile.
     */
    thing_tilep thing_tile;
    tree_rootp tiles;

    tiles = tp_get_tiles(tp);
    if (!tiles) {
        return (0);
    }

    thing_tile = thing_tile_first(tiles);
    if (!thing_tile) {
        return (0);
    }

    {
        const char *tilename = thing_tile_name(thing_tile);
        if (!tilename) {
            ERR("cannot find tile %s", tilename);
            return (0);
        }

        tilep tile = tile_find(tilename);

        return (tile);
    }
}

static void world_editor_button_animate (widp b, tpp tp)
{
    if (!tp) {
        return;
    }

    wid_set_thing_template(b, tp);

    tilep tile = world_editor_tp_to_tile(tp);
    if (tile) {
        wid_set_tile(b, tile);
    }

    wid_set_animate(b, false);
}

/*
 * Set the edit mode focus.
 */
static void world_editor_update_tile_mode_buttons (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    if (!ctx->tile_mode) {
        int i;

        for (i = 0; i < WORLD_TILE_POOL_MAX; i++) {
            widp b = ctx->tile[i][0].button;
            if (!b) {
                continue;
            }
            wid_set_text(b, 0);
        }

        return;
    }

    {
        int x, y;

        for (x = 0; x < WORLD_EDITOR_MENU_CELLS_ACROSS; x++) {
            for (y = 0; y < WORLD_EDITOR_MENU_CELLS_DOWN; y++) {
                widp b = ctx->tile[x][y].button;
                if (!b) {
                    continue;
                }

                wid_set_thing_template(b, 0);
            }
        }
    }

    int x = 0;
    int y = 1;
    int i;

    for (i = 0; i < TP_MAX_ID; i++) {
        tpp tp = ctx->tile_pools[ctx->tile_pool][i].tile_tp;
        if (!tp) {
            continue;
        }

        x++;
        if (x >= WORLD_EDITOR_MENU_TILES_ACROSS) {
            x = 0;
            y++;
        }

        widp b = ctx->tile[x][y].button;
        if (!b) {
            continue;
        }

        wid_set_color(b, WID_COLOR_BG, BLACK);

        if ((x == ctx->focus_x) && (y == ctx->focus_y)) {
            wid_set_color(b, WID_COLOR_TL, RED);
            wid_set_color(b, WID_COLOR_BR, RED);
        }

        if (tp == world_editor_chosen_tile[ctx->tile_pool]) {
            color c = RED;
            c.a = 100;
            wid_set_color(b, WID_COLOR_BG, c);
            wid_set_color(b, WID_COLOR_TL, RED);
            wid_set_color(b, WID_COLOR_BR, RED);
        }

        world_editor_button_animate(b, tp);

        wid_set_tooltip(b, tp_get_tooltip(tp), vsmall_font);
    }

    for (i = 0; i < WORLD_TILE_POOL_MAX; i++) {

        widp b = ctx->tile[i][0].button;
        if (!b) {
            continue;
        }

        switch (i) {
        case WORLD_TILE_MODE_LAND:
            wid_set_text(b, "Floor");
            break;
        }

        if (i == ctx->tile_pool) {
            wid_set_color(b, WID_COLOR_TL, GRAY70);
            wid_set_color(b, WID_COLOR_BR, GRAY40);
            wid_set_color(b, WID_COLOR_TEXT, WHITE);
            wid_set_color(b, WID_COLOR_TEXT, RED);
        } else if (i) {
            wid_set_color(b, WID_COLOR_TL, GRAY70);
            wid_set_color(b, WID_COLOR_BR, GRAY40);
            wid_set_color(b, WID_COLOR_BG, GRAY50);
            wid_set_color(b, WID_COLOR_BG, GRAY10);
        }

        wid_set_font(b, vsmall_font);
    }
}

static void world_editor_update_buttons (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    if (!ctx) {
        return;
    }

    double width = 1.0 / (double)(WORLD_EDITOR_MENU_CELLS_ACROSS);
    double height = 1.0 / (double)(WORLD_EDITOR_MENU_CELLS_DOWN);

    int x, y;

    ctx->b = 0;

    for (x = 0; x < WORLD_EDITOR_MENU_CELLS_ACROSS; x++) {
    for (y = 0; y < WORLD_EDITOR_MENU_CELLS_DOWN; y++) {

        widp b = ctx->tile[x][y].button;
        if (!b) {
            continue;
        }

        verify(b);

        fpoint tl;
        fpoint br;
        fontp font;
        color c;

        tl.x = ((double) x) * width;
        tl.y = ((double) y) * height;
        br.x = tl.x + 0.95 * width;
        br.y = tl.y + 0.95 * height;

        font = vsmall_font;

        int is_a_world_tile = 0;

        int mx = x + ctx->world_x;
        int my = y + ctx->world_y;

        if (!ctx->tile_mode) {
            int z;

            if ((x < WORLD_EDITOR_MENU_WORLD_ACROSS) && 
                (y < WORLD_EDITOR_MENU_WORLD_DOWN)) {
                for (z = 0; z < WORLD_DEPTH; z++) {
                    if (ctx->map.tile[mx][my][z].tp) {
                        is_a_world_tile = 1;
                        break;
                    }
                }
            }
        }

        double zoom = 0.000;
        if ((x == ctx->focus_x) && (y == ctx->focus_y)) {
            if (!is_a_world_tile) {
                tl.x -= zoom;
                tl.y -= zoom;
                br.x += zoom * 2.0;
                br.y += zoom * 2.0;
            }

            c = GREEN;
            ctx->b = b;
        } else {
            c = GRAY70;
        }

        wid_set_tl_br_pct(b, tl, br);
        wid_set_color(b, WID_COLOR_TEXT, c);
        wid_set_font(b, font);


        if ((x < WORLD_EDITOR_MENU_WORLD_ACROSS) && 
            (y < WORLD_EDITOR_MENU_WORLD_DOWN) &&
            (ctx->world_highlight[mx][my])) {
            c = WHITE;
            c.a = 5;
            wid_set_color(b, WID_COLOR_TL, c);
            wid_set_color(b, WID_COLOR_BR, c);
            wid_set_bevel(b, 0);

            color c = GREEN;
            c.a = 200;
            wid_set_color(b, WID_COLOR_BG, c);
        } else if ((x == ctx->focus_x) && (y == ctx->focus_y)) {
            c = WHITE;
            c.a = 5;
            wid_set_color(b, WID_COLOR_TL, c);
            wid_set_color(b, WID_COLOR_BR, c);
            wid_set_bevel(b, 0);

            color c = RED;
            c.a = 100;
            wid_set_color(b, WID_COLOR_BG, c);
        } else if (is_a_world_tile) {
            c = WHITE;
            c.a = 5;
            wid_set_color(b, WID_COLOR_TL, c);
            wid_set_color(b, WID_COLOR_BR, c);
            wid_set_bevel(b, 0);

            color c = WHITE;
            c.a = 0;
            wid_set_color(b, WID_COLOR_BG, c);

        } else if ((x < WORLD_EDITOR_MENU_WORLD_ACROSS) && 
                   (y < WORLD_EDITOR_MENU_WORLD_DOWN)) {
            c = WHITE;
            c.a = 20;
            wid_set_color(b, WID_COLOR_TL, c);
            wid_set_color(b, WID_COLOR_BR, c);
            wid_set_color(b, WID_COLOR_BG, c);
            wid_set_bevel(b, 0);

        } else {
            c = WHITE;
            c.a = 5;
            wid_set_color(b, WID_COLOR_TL, c);
            wid_set_color(b, WID_COLOR_BR, c);
            wid_set_bevel(b, 0);

            color c = GRAY;
            c.a = 50;
            wid_set_color(b, WID_COLOR_BG, c);
            wid_set_bevel(b, 2);
        }
    } }

    world_editor_update_edit_mode_buttons();
    world_editor_update_tile_mode_buttons();
    wid_update(world_editor_window);
}

static void world_editor_button_display (widp w, fpoint tl, fpoint br)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    if (!ctx) {
        return;
    }

    verify(ctx);
    verify(ctx->w);

    if (wid_choose_title_dialog) {
        return;
    }

    blit_init();

    int xy = (typeof(xy)) (uintptr_t) wid_get_context2(w);
    int x = (xy & 0xff);
    int y = (xy & 0xff00) >> 8;

    if (ctx->tile_mode) {
        tpp tp = ctx->tile[x][y].tile_tp;
        if (!tp) {
            return;
        }

        tilep tile = world_editor_tp_to_tile(tp);
        if (!tp) {
            return;
        }

        blit_init();
        glcolor(WHITE);
        tile_blit_fat(tp, tile, 0, tl, br);
        blit_flush();
        return;
    }

    if (x >= WORLD_EDITOR_MENU_WORLD_ACROSS) {
        return;
    }

    if (y >= WORLD_EDITOR_MENU_WORLD_DOWN) {
        return;
    }

    x += ctx->world_x;
    y += ctx->world_y;

    if ((x < 0) || (x >= WORLD_WIDTH) || (y < 0) || (y >= WORLD_HEIGHT)) {
        ERR("overflow on map coords (%d, %d)", x, y);
    }

    double width = br.x - tl.x;
    double height = br.y - tl.y;

    br.x = tl.x + width;
    tl.y = br.y - height;

    int z;
    for (z = 0; z < WORLD_DEPTH; z++) {
        tpp tp = ctx->map.tile[x][y][z].tp;
        if (!tp) {
            continue;
        }

        fpoint btl = tl;
        fpoint bbr = br;

        tilep tile = ctx->map.world_tile[x][y];
        if (tile) {
            glcolor(WHITE);
            tile_blit_fat(tp, tile, 0, btl, bbr);
            continue;
        }

        tile = world_editor_tp_to_tile(tp);
        if (!tp) {
            continue;
        }

        glcolor(WHITE);

        tile_blit_fat(tp, tile, 0, btl, bbr);
    }

    blit_flush();

    glcolor(WHITE);

    char *tmp = dynprintf("top left: %%%%fg=red$(%d, %d)", ctx->world_x, ctx->world_y);
    ttf_puts(small_font, tmp, 0, 40, 1.0, 1.0, false);
    myfree(tmp);

    tmp = dynprintf("window: %%%%fg=yellow$(%d, %d)", ctx->focus_x, ctx->focus_y);
    ttf_puts(small_font, tmp, 0, 60, 1.0, 1.0, false);
    myfree(tmp);

    tmp = dynprintf("map: %%%%fg=yellow$(%d, %d)", ctx->world_x + ctx->focus_x, ctx->world_y + ctx->focus_y);
    ttf_puts(small_font, tmp, 0, 80, 1.0, 1.0, false);
    myfree(tmp);
}

static void world_editor_focus_right (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    if (!ctx) {
        return;
    }

    ctx->focus_x++;
    if (ctx->focus_x > WORLD_EDITOR_MENU_CELLS_ACROSS - 1) {
        ctx->focus_x = WORLD_EDITOR_MENU_CELLS_ACROSS - 1;
        world_editor_world_scroll(1, 0);
    }
}

static void world_editor_focus_left (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    if (!ctx) {
        return;
    }

    ctx->focus_x--;
    if (ctx->focus_x < 0) {
        ctx->focus_x = 0;
        world_editor_world_scroll(-1, 0);
    }
}

static void world_editor_focus_down (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    if (!ctx) {
        return;
    }

    ctx->focus_y++;
    if (ctx->focus_y > WORLD_EDITOR_MENU_CELLS_DOWN - 1) {
        ctx->focus_y = WORLD_EDITOR_MENU_CELLS_DOWN - 1;
        world_editor_world_scroll(0, 1);
    }
}

static void world_editor_focus_up (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    if (!ctx) {
        return;
    }

    ctx->focus_y--;
    if (ctx->focus_y < 0) {
        ctx->focus_y = 0;
        world_editor_world_scroll(0, -1);
    }
}

static void world_editor_set_focus (world_editor_ctx *ctx, int focus_x, int focus_y)
{
    ctx->focus_x = focus_x;
    ctx->focus_y = focus_y;

    world_editor_update_buttons();
}

static void world_editor_tile_pool_find_focus (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    /*
     * If in tile mode now, find the old focus.
     */
    if (!ctx->tile_mode) {
        return;
    }

    world_editor_update_buttons();

    int x, y;

    for (x = 0; x < WORLD_EDITOR_MENU_TILES_ACROSS; x++) {
        for (y = 0; y < WORLD_EDITOR_MENU_TILES_DOWN; y++) {
            widp b = ctx->tile[x][y].button;
            if (!b) {
                continue;
            }

            tpp tp = wid_get_thing_template(b);
            if (!tp) {
                continue;
            }

            if (tp == world_editor_chosen_tile[ctx->tile_pool]) {
                ctx->focus_x = x;
                ctx->focus_y = y;
                return;
            }
        }
    }
}

static void world_editor_tile_mode_toggle (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    if (time_get_time_ms() - ctx->mode_toggled < 100) {
        return;
    }

    ctx->mode_toggled = time_get_time_ms();
    ctx->tile_mode = !ctx->tile_mode;

    /*
     * If in tile mode now, find the old focus.
     */
    if (ctx->tile_mode) {
        saved_focus_x = ctx->focus_x;
        saved_focus_y = ctx->focus_y;
        saved_world_x = ctx->world_x;
        saved_world_y = ctx->world_y;

        world_editor_tile_pool_find_focus();
    } else {
        /*
         * Now in map mode. Clear out the tile buttons.
         */
        int x, y;

        for (x = 0; x < WORLD_EDITOR_MENU_TILES_ACROSS; x++) {
            for (y = 0; y < WORLD_EDITOR_MENU_TILES_DOWN; y++) {
                widp b = ctx->tile[x][y].button;
                if (b) {
                    wid_set_thing_template(b, 0);
                }
            }
        }

        ctx->focus_x = saved_focus_x;
        ctx->focus_y = saved_focus_y;
        ctx->world_x = saved_world_x;
        ctx->world_y = saved_world_y;
    }
}

static void world_editor_tile_mode_set (int val)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    if (ctx->tile_mode == val) {
        return;
    }

    world_editor_tile_mode_toggle();
}

static void world_editor_world_scroll (int dx, int dy)
{
    static int recursion;

    if (recursion) {
        return;
    }
    recursion = 1;

    world_editor_ctx *ctx = world_editor_window_ctx;
    if (!ctx) {
        recursion = 0;
        return;
    }

    verify(ctx);
    verify(ctx->w);

    if (ctx->tile_mode) {
        recursion = 0;
        return;
    }

    if (dx > 0) {
        if (ctx->world_x + WORLD_EDITOR_MENU_WORLD_ACROSS < WORLD_WIDTH) {
            ctx->world_x++;
        } else {
            world_editor_focus_right();
        }
    } else if (dx < 0) {
        if (ctx->world_x > 0) {
            ctx->world_x--;
        } else {
            world_editor_focus_left();
        }
    }

    if (dy > 0) {
        if (ctx->world_y + WORLD_EDITOR_MENU_WORLD_DOWN < WORLD_HEIGHT) {
            ctx->world_y++;
        } else {
            world_editor_focus_down();
        }
    } else if (dy < 0) {
        if (ctx->world_y > 0) {
            ctx->world_y--;
        } else {
            world_editor_focus_up();
        }
    }

    recursion = 0;
}

static void world_editor_mx_my (int *mx, int *my)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    *mx = ctx->focus_x + ctx->world_x;
    *my = ctx->focus_y + ctx->world_y;

    if (*mx < 0) {
        *mx = 0;
    }

    if (*my < 0) {
        *my = 0;
    }

    if (*mx >= WORLD_WIDTH) {
        *mx = WORLD_WIDTH - 1;
    }

    if (*my > WORLD_HEIGHT) {
        *my = WORLD_HEIGHT - 1;
    }
}
                              
static void world_editor_world_thing_replace (int x, int y, int interactive)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    if ((x < 0) || (y < 0) ||
        (x >= WORLD_WIDTH) ||
        (y >= WORLD_HEIGHT)) {
        ERR("world_editor_world_thing_replace, bad map coord %d,%d", x, y);
        return;
    }

    tpp tp = world_editor_chosen_tile[ctx->tile_pool];
    if (!tp) {
        return;
    }

    int z = tp_get_world_depth(tp);

    world_editor_set_new_tp(x, y, z, tp);
}

static tpp world_editor_world_thing_get (int x, int y)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    if ((x < 0) || (y < 0) ||
        (x >= WORLD_WIDTH) ||
        (y >= WORLD_HEIGHT)) {
        return (0);
    }

    int z;
    for (z = WORLD_DEPTH - 1; z >= 0; z--) {
        tpp tp = ctx->map.tile[x][y][z].tp;
        if (tp) {
            return (tp);
        }
    }

    return (0);
}

static void world_editor_world_highlight_replace (int x, int y)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    if ((x < 0) || (y < 0) ||
        (x >= WORLD_WIDTH) ||
        (y >= WORLD_HEIGHT)) {
        ERR("world_editor_world_highlight_replace, bad map coord %d,%d", x, y);
        return;
    }

    ctx->world_highlight[x][y] = 1;
}

static void do_world_editor_line (int x0_in, 
                                int y0_in, 
                                int x1_in, 
                                int y1_in, 
                                int flag)
{
    double temp;
    double dx;
    double dy;
    double tdy;
    double dydx;
    double p;
    double x;
    double y;
    double i;

    double x0 = x0_in;
    double y0 = y0_in;
    double x1 = x1_in;
    double y1 = y1_in;

    if (x0 > x1) {
        temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    tdy = 2.0 * dy;
    dydx = tdy - (2.0 * dx);

    p = tdy - dx;
    x = x0;
    y = y0;

    if (flag == 0) {
        world_editor_world_thing_replace((int)x, (int)y, false /* interactive */);
    } else if (flag == 1) {
        world_editor_world_thing_replace((int)y, (int)x, false /* interactive */);
    } else if (flag == 2) {
        world_editor_world_thing_replace((int)y, (int)-x, false /* interactive */);
    } else if (flag == 3) {
        world_editor_world_thing_replace((int)x, (int)-y, false /* interactive */);
    }

    for (i = 1; i <= dx; i++){
        x++;

        if (p < 0) {
            p += tdy;
        } else {
            p += dydx;
            y++;
        }

        if (flag == 0) {
            world_editor_world_thing_replace((int)x, (int)y, false /* interactive */);
        } else if (flag == 1) {
            world_editor_world_thing_replace((int)y, (int)x, false /* interactive */);
        } else if (flag == 2) {
            world_editor_world_thing_replace((int)y, (int)-x, false /* interactive */);
        } else if (flag == 3) {
            world_editor_world_thing_replace((int)x, (int)-y, false /* interactive */);
        }
    }
}

static void world_editor_draw_line (int x0, int y0, int x1, int y1)
{
    double slope = 100.0;

    if (x0 != x1) {
        slope = (y1 - y0) * (1.0 / (x1 - x0));
    }

    if ((0 <= slope) && (slope <= 1)) {
        do_world_editor_line(x0, y0, x1, y1, 0);
    } else if ((-1 <= slope) && (slope <= 0)) {
        do_world_editor_line(x0, -y0, x1, -y1, 3);
    } else if (slope > 1) {
        do_world_editor_line(y0, x0, y1, x1, 1);
    } else {
        do_world_editor_line(-y0, x0, -y1, x1, 2);
    }
}

static void do_world_editor_highlight_line (int x0_in, 
                                          int y0_in, 
                                          int x1_in, 
                                          int y1_in, 
                                          int flag)
{
    double temp;
    double dx;
    double dy;
    double tdy;
    double dydx;
    double p;
    double x;
    double y;
    double i;

    double x0 = x0_in;
    double y0 = y0_in;
    double x1 = x1_in;
    double y1 = y1_in;

    if (x0 > x1) {
        temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    tdy = 2.0 * dy;
    dydx = tdy - (2.0 * dx);

    p = tdy - dx;
    x = x0;
    y = y0;

    if (flag == 0) {
        world_editor_world_highlight_replace((int)x, (int)y);
    } else if (flag == 1) {
        world_editor_world_highlight_replace((int)y, (int)x);
    } else if (flag == 2) {
        world_editor_world_highlight_replace((int)y, (int)-x);
    } else if (flag == 3) {
        world_editor_world_highlight_replace((int)x, (int)-y);
    }

    for (i = 1; i <= dx; i++){
        x++;

        if (p < 0) {
            p += tdy;
        } else {
            p += dydx;
            y++;
        }

        if (flag == 0) {
            world_editor_world_highlight_replace((int)x, (int)y);
        } else if (flag == 1) {
            world_editor_world_highlight_replace((int)y, (int)x);
        } else if (flag == 2) {
            world_editor_world_highlight_replace((int)y, (int)-x);
        } else if (flag == 3) {
            world_editor_world_highlight_replace((int)x, (int)-y);
        }
    }
}

static void world_editor_draw_shape (int x0, int y0, int x1, int y1, int sides)
{
    double height = fabs((double)(y1 - y0)) / 2.0;
    double width = fabs((double)(x1 - x0)) / 2.0;

    if ((width == 0.0) || (height == 0.0)) {
        return;
    }

    double cx = ((x0 + x1) / 2) + 0.5;
    double cy = ((y0 + y1) / 2) + 0.5;

    double step = RAD_360 / (double) sides;
    double rad;

    for (rad = 0; rad < RAD_360; rad += step) {

        double x0 = cos(rad);
        double y0 = sin(rad);
        double x1 = cos(rad + step);
        double y1 = sin(rad + step);

        x0 *= width;
        y0 *= height;
        x1 *= width;
        y1 *= height;

        x0 += cx;
        y0 += cy;

        x1 += cx;
        y1 += cy;

        world_editor_draw_line(x0, y0, x1, y1);
    }
}

static void world_editor_draw_square (int x0, int y0, int x1, int y1)
{
    world_editor_draw_line(x0, y0, x1, y0);
    world_editor_draw_line(x0, y0, x0, y1);
    world_editor_draw_line(x1, y1, x1, y0);
    world_editor_draw_line(x1, y1, x0, y1);

    world_editor_fixup();
    world_editor_undo_save();
}

static void world_editor_draw_circle (int x0, int y0, int x1, int y1)
{
    world_editor_draw_shape(x0, y0, x1, y1, 256);
    world_editor_fixup();
    world_editor_undo_save();
}

static void world_editor_draw_octagon (int x0, int y0, int x1, int y1)
{
    world_editor_draw_shape(x0, y0, x1, y1, 8);
    world_editor_fixup();
    world_editor_undo_save();
}

static void world_editor_paste (int mx, int my)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    int x0 = ctx->cut_start_x;
    int x1 = ctx->cut_end_x;

    int y0 = ctx->cut_start_y;
    int y1 = ctx->cut_end_y;

    if (x0 > x1) {
        swap(x0, x1);
    }
    if (y0 > y1) {
        swap(y0, y1);
    }

    int x, y, z;

    for (x = x0; x <= x1; x++) {
        for (y = y0; y <= y1; y++) {
            for (z = 0; z < WORLD_DEPTH; z++) {
                tpp tp = ctx->world_copy.tile[x][y][z].tp;
                if (!tp) {
                    continue;
                }

                int px = mx + (x - x0);
                int py = my + (y - y0);

                if ((px < 0) || (py < 0) ||
                    (px >= WORLD_WIDTH) ||
                    (py >= WORLD_HEIGHT)) {
                    continue;
                }

                world_editor_set_new_tp(px, py, z, tp);
            }
        }
    }

    world_editor_fixup();

    world_editor_undo_save();
}

static void world_editor_cut (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    int x0 = ctx->cut_start_x;
    int x1 = ctx->cut_end_x;

    int y0 = ctx->cut_start_y;
    int y1 = ctx->cut_end_y;

    if (x0 > x1) {
        swap(x0, x1);
    }
    if (y0 > y1) {
        swap(y0, y1);
    }

    int x, y, z;

    for (x = x0; x <= x1; x++) {
        for (y = y0; y <= y1; y++) {
            for (z = 0; z < WORLD_DEPTH; z++) {

                if ((x < 0) || (y < 0) ||
                    (x >= WORLD_WIDTH) ||
                    (y >= WORLD_HEIGHT)) {
                    continue;
                }

                world_editor_set_new_tp(x, y, z, 0);
            }
        }
    }

    world_editor_fixup();

    world_editor_undo_save();
}

static void world_editor_draw_highlight_square (int x0, int y0, int x1, int y1)
{
    int x, y;

    if (x0 > x1) {
        swap(x0, x1);
    }
    if (y0 > y1) {
        swap(y0, y1);
    }

    for (x = x0; x <= x1; x++) {
        world_editor_world_highlight_replace(x, y0);
        world_editor_world_highlight_replace(x, y1);
    }

    for (y = y0; y <= y1; y++) {
        world_editor_world_highlight_replace(x0, y);
        world_editor_world_highlight_replace(x1, y);
    }
}

static void world_editor_draw_highlight_cut (int x0, int y0, int x1, int y1)
{
    int x, y;

    if (x0 > x1) {
        swap(x0, x1);
    }
    if (y0 > y1) {
        swap(y0, y1);
    }

    for (x = x0; x <= x1; x++) {
        for (y = y0; y <= y1; y++) {
            world_editor_world_highlight_replace(x, y);
        }
    }
}

static void world_editor_draw_highlight_line (int x0, int y0, int x1, int y1)
{
    if ((x0 == -1) || (y0 == -1)) {
        return;
    }

    if ((x1 == -1) || (y1 == -1)) {
        return;
    }

    double slope = 100.0;

    if (x0 != x1) {
        slope = (y1 - y0) * (1.0 / (x1 - x0));
    }

    if ((0 <= slope) && (slope <= 1)) {
        do_world_editor_highlight_line(x0, y0, x1, y1, 0);
    } else if ((-1 <= slope) && (slope <= 0)) {
        do_world_editor_highlight_line(x0, -y0, x1, -y1, 3);
    } else if (slope > 1) {
        do_world_editor_highlight_line(y0, x0, y1, x1, 1);
    } else {
        do_world_editor_highlight_line(-y0, x0, -y1, x1, 2);
    }
}

static void world_editor_undo_save (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    /*
     * Check there is a change
     */
    if (ctx->undo_at != -1) {
        if (!memcmp(&ctx->world_undo[ctx->undo_at], &ctx->map, 
                    sizeof(ctx->map))) {
            return;
        }
    }

    ctx->undo_at++;
    if (ctx->undo_at >= WORLD_EDITOR_UNDO) {
        ctx->undo_at--;

        /*
         * Reached the undo limit, drop an entry off the head.
         */
        int i;

        for (i = 0; i < ctx->undo_at; i++) {
            memcpy(&ctx->world_undo[i], &ctx->world_undo[i+1], sizeof(ctx->map));
        }
    }

    if (ctx->undo_at > 0) {
        ctx->save_needed = true;
    }

    memcpy(&ctx->world_undo[ctx->undo_at], &ctx->map, sizeof(ctx->map));
    ctx->valid_undo[ctx->undo_at] = 1;
}

static void world_editor_undo (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    int old = ctx->undo_at;

    ctx->undo_at--;
    if (ctx->undo_at < 0) {
        ctx->undo_at = 0;
    }

    if (!ctx->valid_undo[ctx->undo_at]) {
        ctx->undo_at = old;
        return;
    }

    memcpy(&ctx->map, &ctx->world_undo[ctx->undo_at], sizeof(ctx->map));

    world_editor_fixup();
}

static void world_editor_redo (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    int old = ctx->undo_at;

    ctx->undo_at++;
    if (ctx->undo_at > WORLD_EDITOR_UNDO) {
        ctx->undo_at = 0;
    }

    if (!ctx->valid_undo[ctx->undo_at]) {
        ctx->undo_at = old;
        return;
    }

    memcpy(&ctx->map, &ctx->world_undo[ctx->undo_at], sizeof(ctx->map));

    world_editor_fixup();
}

static void world_editor_nuke (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    memset(&ctx->map, 0, sizeof(ctx->map));
}

static void world_editor_title_selected (widp w, const char *title)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    level_set_title(ctx->level, title);

    wid_destroy(&wid_choose_title_dialog);

    world_editor_save_level();
}

static void world_editor_title_cancelled (widp w, const char *title)
{
    wid_destroy(&wid_choose_title_dialog);
}

static void world_editor_title_choose (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    if (wid_choose_title_dialog) {
        return;
    }

    wid_choose_title_dialog = wid_keyboard(level_get_title(ctx->level),
                                           "Choose title",
                                           world_editor_title_selected,
                                           world_editor_title_cancelled);
}

static void world_editor_hflip (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    memcpy(&ctx->world_tmp, &ctx->map, sizeof(ctx->map));
    memset(&ctx->map, 0, sizeof(ctx->map));

    int x, y, z;

    for (x = 0; x < WORLD_WIDTH; x++) {
        for (y = 0; y < WORLD_HEIGHT; y++) {
            for (z = 0; z < WORLD_DEPTH; z++) {
                memcpy(&ctx->map.tile[WORLD_WIDTH - 1 - x][y][z],
                       &ctx->world_tmp.tile[x][y][z],
                       sizeof(world_editor_world_tile));
            }
        }
    }

    world_editor_fixup();

    world_editor_undo_save();
}

static void world_editor_vflip (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    memcpy(&ctx->world_tmp, &ctx->map, sizeof(ctx->map));
    memset(&ctx->map, 0, sizeof(ctx->map));

    int x, y, z;

    for (x = 0; x < WORLD_WIDTH; x++) {
        for (y = 0; y < WORLD_HEIGHT; y++) {
            for (z = 0; z < WORLD_DEPTH; z++) {
                memcpy(&ctx->map.tile[x][WORLD_HEIGHT - 1 - y][z],
                       &ctx->world_tmp.tile[x][y][z],
                       sizeof(world_editor_world_tile));
            }
        }
    }

    world_editor_fixup();

    world_editor_undo_save();
}

static void world_editor_rotate (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    memcpy(&ctx->world_tmp, &ctx->map, sizeof(ctx->map));
    memset(&ctx->map, 0, sizeof(ctx->map));

    int x, y, z;

    for (x = 0; x < WORLD_WIDTH; x++) {
        for (y = 0; y < WORLD_HEIGHT; y++) {
            for (z = 0; z < WORLD_DEPTH; z++) {
                memcpy(&ctx->map.tile[x][y][z],
                       &ctx->world_tmp.tile[y][WORLD_WIDTH - 1 - x][z],
                       sizeof(world_editor_world_tile));
            }
        }
    }

    world_editor_fixup();

    world_editor_undo_save();
}

static void world_editor_tile_fill_ (int x, int y)
{
    world_editor_ctx *ctx = world_editor_window_ctx;

    if (!world_editor_chosen_tile[ctx->tile_pool]) {
        return;
    }

    if ((x < 0) || (y < 0) ||
        (x >= WORLD_WIDTH) ||
        (y >= WORLD_HEIGHT)) {
        return;
    }

    /*
     * Check to see there is nothing here blocking us.
     */
    uint8_t min_z;
    uint8_t z;

    /*
     * Bound certain things by others. e.g. flood fill ghosts limited by 
     * walls.
     */
    tpp tp = world_editor_chosen_tile[ctx->tile_pool];
    switch (tp_get_world_depth(tp)) {
        case WORLD_DEPTH_LAND: 
            min_z = WORLD_DEPTH_LAND; 
            break;
        default:
            min_z = WORLD_DEPTH_LAND; 
            break;
    }

    for (z = min_z; z < WORLD_DEPTH; z++) {
        if (ctx->map.tile[x][y][z].tp) {
            return;
        }
    }

    z = tp_get_world_depth(tp);
    world_editor_set_new_tp(x, y, z, tp);

    world_editor_tile_fill_(x + 1, y);
    world_editor_tile_fill_(x - 1, y);
    world_editor_tile_fill_(x, y + 1);
    world_editor_tile_fill_(x, y - 1);
}

static void world_editor_tile_fill (int x, int y)
{
    if ((x == -1) || (y == -1)) {
        return;
    }

    world_editor_tile_fill_(x, y);

    world_editor_fixup();

    world_editor_undo_save();
}

static void world_editor_tile_left_button_pressed (int x, int y)
{
    if ((x == -1) || (y == -1)) {
        return;
    }

    world_editor_ctx *ctx = world_editor_window_ctx;
    int mx = -1;
    int my = -1;

    if (time_get_time_ms() - ctx->mode_toggled < 100) {
        return;
    }

    if (!ctx->tile_mode) {
        mx = x + ctx->world_x;
        my = y + ctx->world_y;
    }

    if (ctx->tile_mode) {

        widp b = ctx->tile[x][y].button;
        if (b) {
            tpp tp = wid_get_thing_template(b);
            if (tp) {
                world_editor_chosen_tile[ctx->tile_pool] = tp;

                world_editor_tile_mode_toggle();

                world_editor_set_mode(WORLD_EDITOR_MODE_DRAW);
                return;
            }
        }

        if (y == 0) {
            if (x < WORLD_TILE_POOL_MAX) {
                ctx->tile_pool = x;
                world_editor_tile_pool_find_focus();
                return;
            }
        }

    } else {
        if (world_editor_chosen_tile[ctx->tile_pool]) {
            if ((x < WORLD_EDITOR_MENU_WORLD_ACROSS) && 
                (y < WORLD_EDITOR_MENU_WORLD_DOWN)) {
                switch (ctx->edit_mode) {
                case WORLD_EDITOR_MODE_DRAW:
                    world_editor_world_thing_replace(mx, my, true /* interactive */);
                    world_editor_fixup();

                    world_editor_undo_save();
                    break;

                case WORLD_EDITOR_MODE_LINE:
                    if (!ctx->got_line_start) {
                        ctx->got_line_start = true;
                        ctx->line_start_x = mx;
                        ctx->line_start_y = my;

                    } else if ((ctx->line_start_x != mx) ||
                               (ctx->line_start_y != my)) {

                        world_editor_draw_line(ctx->line_start_x,
                                             ctx->line_start_y, mx, my);

                        world_editor_fixup();

                        world_editor_undo_save();

                        ctx->got_line_start = false;
                    }
                    break;

                case WORLD_EDITOR_MODE_SQUARE:
                    if (!ctx->got_square_start) {
                        ctx->got_square_start = true;
                        ctx->square_start_x = mx;
                        ctx->square_start_y = my;

                    } else if ((ctx->square_start_x != mx) ||
                               (ctx->square_start_y != my)) {

                        world_editor_draw_square(ctx->square_start_x,
                                               ctx->square_start_y, mx, my);
                        ctx->got_square_start = false;
                    }
                    break;

                case WORLD_EDITOR_MODE_CIRCLE:
                    if (!ctx->got_square_start) {
                        ctx->got_square_start = true;
                        ctx->square_start_x = mx;
                        ctx->square_start_y = my;

                    } else if ((ctx->square_start_x != mx) ||
                               (ctx->square_start_y != my)) {

                        world_editor_draw_circle(ctx->square_start_x,
                                               ctx->square_start_y, mx, my);
                        ctx->got_square_start = false;
                    }
                    break;

                case WORLD_EDITOR_MODE_OCTAGON:
                    if (!ctx->got_square_start) {
                        ctx->got_square_start = true;
                        ctx->square_start_x = mx;
                        ctx->square_start_y = my;

                    } else if ((ctx->square_start_x != mx) ||
                               (ctx->square_start_y != my)) {

                        world_editor_draw_octagon(ctx->square_start_x,
                                                ctx->square_start_y, mx, my);
                        ctx->got_square_start = false;
                    }
                    break;

                case WORLD_EDITOR_MODE_CUT:
                    if (!ctx->got_cut_start) {
                        ctx->got_cut_start = true;
                        ctx->cut_start_x = mx;
                        ctx->cut_start_y = my;

                    } else if ((ctx->cut_start_x != mx) ||
                               (ctx->cut_start_y != my)) {

                        /*
                         * Only do the cut if we've actually moved. Avoids
                         * double clicks messing up.
                         */
                        ctx->got_cut_start = false;
                        ctx->cut_end_x = mx;
                        ctx->cut_end_y = my;
                        world_editor_cut();
                    }
                    break;

                case WORLD_EDITOR_MODE_COPY:
                    if (!ctx->got_cut_start) {
                        ctx->got_cut_start = true;
                        ctx->cut_start_x = mx;
                        ctx->cut_start_y = my;

                    } else if ((ctx->cut_start_x != mx) ||
                               (ctx->cut_start_y != my)) {

                        memcpy(&ctx->world_copy, &ctx->map, sizeof(ctx->map));
                        ctx->got_cut_start = false;
                        ctx->cut_end_x = mx;
                        ctx->cut_end_y = my;
                    }
                    break;

                case WORLD_EDITOR_MODE_PASTE:
                    world_editor_paste(mx, my);
                    break;

                case WORLD_EDITOR_MODE_EDIT:
                case WORLD_EDITOR_MODE_YANK:
                    {
                        tpp tp = world_editor_world_thing_get(mx, my);
                        if (tp) {
                            world_editor_chosen_tile[ctx->tile_pool] = tp;

                            /*
                             * Fake a cut so a 'p' can put this back.
                             */
                            memcpy(&ctx->world_copy, &ctx->map, sizeof(ctx->map));
                            ctx->cut_start_x = mx;
                            ctx->cut_end_x = mx;
                            ctx->cut_start_y = my;
                            ctx->cut_end_y = my;
                        }
                    }

                    if (ctx->edit_mode == WORLD_EDITOR_MODE_EDIT) {
                        world_editor_world_thing_replace(mx, my, true /* interactive */);
                        world_editor_fixup();

                        world_editor_undo_save();
                    }

                    world_editor_set_mode(WORLD_EDITOR_MODE_DRAW);
                    break;

                case WORLD_EDITOR_MODE_FILL:
                    world_editor_tile_fill(mx, my);
                    break;

                case WORLD_EDITOR_MODE_DEL:
                    world_editor_tile_right_button_pressed(x, y);
                    break;
                }
            }
        }
    }

    if (x == WORLD_EDITOR_MENU_CELLS_ACROSS - 1) {
        switch (y) {
        case WORLD_EDITOR_MODE2_TITLE:
            world_editor_title_choose();
            break;
        }
    }

    if (y == WORLD_EDITOR_MENU_CELLS_DOWN - 1) {
        if (x < WORLD_EDITOR_MODE_MAX) {
            switch (x) {
            case WORLD_EDITOR_MODE_DRAW:
            case WORLD_EDITOR_MODE_LINE:
            case WORLD_EDITOR_MODE_DEL:
            case WORLD_EDITOR_MODE_FILL:
            case WORLD_EDITOR_MODE_CUT:
            case WORLD_EDITOR_MODE_COPY:
            case WORLD_EDITOR_MODE_PASTE:
            case WORLD_EDITOR_MODE_YANK:
            case WORLD_EDITOR_MODE_EDIT:
            case WORLD_EDITOR_MODE_SQUARE:
            case WORLD_EDITOR_MODE_CIRCLE:
            case WORLD_EDITOR_MODE_OCTAGON:
                world_editor_set_mode(x);
                break;

            case WORLD_EDITOR_MODE_UNDO:
                world_editor_tile_mode_set(false);
                world_editor_undo();
                break;

            case WORLD_EDITOR_MODE_REDO:
                world_editor_tile_mode_set(false);
                world_editor_redo();
                break;

            case WORLD_EDITOR_MODE_HFLIP:
                world_editor_tile_mode_set(false);
                world_editor_hflip();
                break;

            case WORLD_EDITOR_MODE_VFLIP:
                world_editor_tile_mode_set(false);
                world_editor_vflip();
                break;

            case WORLD_EDITOR_MODE_ROTATE:
                world_editor_tile_mode_set(false);
                world_editor_rotate();
                break;

            case WORLD_EDITOR_MODE_SAVE:
                world_editor_save_level();
                break;

            case WORLD_EDITOR_MODE_TOGGLE:
                world_editor_tile_mode_toggle();
                break;
            }
        }
    }
}

static void world_editor_tile_right_button_pressed (int x, int y)
{
    if ((x == -1) || (y == -1)) {
        return;
    }

    world_editor_ctx *ctx = world_editor_window_ctx;
    int mx = -1;
    int my = -1;

    if (time_get_time_ms() - ctx->mode_toggled < 100) {
        return;
    }

    if (!ctx->tile_mode) {
        mx = x + ctx->world_x;
        my = y + ctx->world_y;
    }

    if (ctx->tile_mode) {
        world_editor_tile_mode_toggle();
        return;
    } else {
        if ((x < WORLD_EDITOR_MENU_WORLD_ACROSS) && 
            (y < WORLD_EDITOR_MENU_WORLD_DOWN)) {
            int z;
            for (z = WORLD_DEPTH - 1; z >= 0; z--) {
                tpp tp = ctx->map.tile[mx][my][z].tp;
                if (tp) {
                    /*
                     * Fake a cut so a 'p' can put this back.
                     */
                    memcpy(&ctx->world_copy, &ctx->map, sizeof(ctx->map));
                    ctx->cut_start_x = mx;
                    ctx->cut_end_x = mx;
                    ctx->cut_start_y = my;
                    ctx->cut_end_y = my;

                    world_editor_set_new_tp(mx, my, z, 0);

                    world_editor_fixup();

                    world_editor_undo_save();
                    return;
                }
            }
        }
    }
}

static uint8_t world_editor_mouse_down (widp w,
                                      int mx, int my,
                                      uint32_t button)
{
    int xy = (typeof(xy)) (uintptr_t) wid_get_context2(w);
    int x = (xy & 0xff);
    int y = (xy & 0xff00) >> 8;

    if (wid_choose_title_dialog) {
        return (true);
    }

    if (button == 1) {
        world_editor_tile_left_button_pressed(x, y);
    }

    if ((button == 2) || (button == 3)) {
        world_editor_tile_right_button_pressed(x, y);
    }

    return (true);
}

static uint8_t world_editor_key_down (widp w, const SDL_KEYSYM *key)
{
    world_editor_ctx *ctx = wid_get_context(w);
    verify(ctx);

    int xy = (typeof(xy)) (uintptr_t) wid_get_context2(w);
    int x = ctx->focus_x;
    int y = ctx->focus_y;

    if (wid_choose_title_dialog) {
        return (true);
    }

    switch (key->mod) {
        case KMOD_LSHIFT:
        case KMOD_RSHIFT:
        switch (key->sym) {
            case SDLK_LEFT:
                world_editor_world_scroll(-1, 0);
                return (true);

            case SDLK_RIGHT:
                world_editor_world_scroll(1, 0);
                return (true);

            case SDLK_UP:
                world_editor_world_scroll(0, -1);
                return (true);

            case SDLK_DOWN:
                world_editor_world_scroll(0, 1);
                return (true);

            default:
                break;
        }

        default:
            break;
    }

    ctx->old_edit_mode = ctx->edit_mode;

    switch (key->sym) {
        case '`':
            return (false);

        case '\t':
            world_editor_tile_mode_toggle();
            return (true);

        case ' ':
            world_editor_tile_left_button_pressed(x, y);
            return (false);

        case SDLK_BACKSPACE:
            world_editor_tile_mode_set(false);
            world_editor_tile_right_button_pressed(x, y);
            return (true); 

        case 'l':
            world_editor_tile_mode_set(false);
            world_editor_set_mode(WORLD_EDITOR_MODE_LINE);
            world_editor_tile_left_button_pressed(x, y);
            return (true);

        case 'r':
            world_editor_tile_mode_set(false);
            world_editor_set_mode(WORLD_EDITOR_MODE_SQUARE);
            world_editor_tile_left_button_pressed(x, y);
            return (true);

        case 's':
            world_editor_tile_mode_set(false);
            world_editor_save_level();
            return (true);

        case 'd':
            world_editor_tile_mode_set(false);
            world_editor_set_mode(WORLD_EDITOR_MODE_DRAW);
            return (true);

        case 'y':
            world_editor_tile_mode_set(false);
            world_editor_set_mode(WORLD_EDITOR_MODE_YANK);
            world_editor_tile_left_button_pressed(x, y);
            return (true);

        case 'c':
            world_editor_tile_mode_set(false);
            world_editor_set_mode(WORLD_EDITOR_MODE_COPY);
            world_editor_tile_left_button_pressed(x, y);
            return (true);

        case 'v':
            world_editor_tile_mode_set(false);
            world_editor_set_mode(WORLD_EDITOR_MODE_PASTE);
            world_editor_tile_left_button_pressed(x, y);
            return (true);

        case 'f':
            world_editor_tile_mode_set(false);
            world_editor_set_mode(WORLD_EDITOR_MODE_FILL);
            world_editor_tile_left_button_pressed(x, y);
            return (true);

        case 'u':
            world_editor_tile_mode_set(false);
            world_editor_undo();
            return (true);

        case 'e':
            world_editor_tile_mode_set(false);
            world_editor_redo();
            return (true);

        case 'x':
            world_editor_tile_mode_set(false);
            world_editor_set_mode(WORLD_EDITOR_MODE_DEL);
            world_editor_tile_left_button_pressed(x, y);
            return (true);

        case 'z':
            world_editor_tile_mode_set(false);
            world_editor_nuke();
            world_editor_undo_save();
            return (true);

        case 'q':
        case SDLK_ESCAPE:

            if (ctx->got_line_start) {
                ctx->got_line_start = false;
                return (true);
            }

            if (ctx->got_square_start) {
                ctx->got_square_start = false;
                return (true);
            }

            if (ctx->got_cut_start) {
                ctx->got_cut_start = false;
                return (true);
            }

            if (ctx->edit_mode > WORLD_EDITOR_MODE_DRAW) {
                ctx->edit_mode = WORLD_EDITOR_MODE_DRAW;
                return (true);
            }

            world_editor_hide();
            return (true);

        case SDLK_RETURN: {
            return (true);

        case SDLK_HOME:
            break;

        case SDLK_END:
            break;

        default:
            return (true);
        }
    }

    world_editor_update_buttons();
    if (ctx->b) {
        wid_mouse_warp(ctx->b);
    }

    return (true);
}

static uint8_t world_editor_joy_button (widp w, int mx, int my)
{
    world_editor_ctx *ctx = wid_get_context(w);
    verify(ctx);

    if (wid_choose_title_dialog) {
        return (true);
    }

    int xy = (typeof(xy)) (uintptr_t) wid_get_context2(w);
    int x = ctx->focus_x;
    int y = ctx->focus_y;

    int ret = false;

    if (sdl_joy_buttons[SDL_JOY_BUTTON_A]) {
        world_editor_tile_left_button_pressed(x, y);
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_B]) {
        ret = true;
        world_editor_hide();
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_X]) {
        world_editor_tile_right_button_pressed(x, y);
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_Y]) {
        world_editor_tile_mode_toggle();
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_TOP_LEFT]) {
        ctx->edit_mode--;
        if (ctx->edit_mode < 0) {
            world_editor_set_mode(0);
        }
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_TOP_RIGHT]) {
        ctx->edit_mode++;
        if (ctx->edit_mode >= WORLD_EDITOR_MODE_MAX) {
            world_editor_set_mode(WORLD_EDITOR_MODE_MAX - 1);
        }
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_LEFT_STICK_DOWN]) {
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT_STICK_DOWN]) {
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_START]) {
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_XBOX]) {
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_BACK]) {
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_UP]) {
        world_editor_focus_up();
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_DOWN]) {
        world_editor_focus_down();
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_LEFT]) {
        world_editor_focus_left();
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT]) {
        world_editor_focus_right();
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_LEFT_FIRE]) {
        ret = true;
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT_FIRE]) {
        ret = true;
    }

    world_editor_update_buttons();
    if (ctx->b) {
        wid_mouse_warp(ctx->b);
    }

    return (ret);
}

static uint8_t world_editor_mouse_motion (widp w,
                                        int mx, int my,
                                        int relx, int rely,
                                        int wheelx, int wheely)
{
    if (!relx && !rely && !wheelx && !wheely) {
        return (false);
    }

    if (wid_choose_title_dialog) {
        return (true);
    }

    if (wheelx < 0) {
        world_editor_world_scroll(1, 0);
    } else if (wheelx > 0) {
        world_editor_world_scroll(-1, 0);
    }

    if (wheely < 0) {
        world_editor_world_scroll(0, 1);
    } else if (wheely > 0) {
        world_editor_world_scroll(0, -1);
    }

    int mouse_x;
    int mouse_y;
    mouse_down = SDL_GetMouseState(&mouse_x, &mouse_y);

    int xy = (typeof(xy)) (uintptr_t) wid_get_context2(w);
    int x = (xy & 0xff);
    int y = (xy & 0xff00) >> 8;

    if (mouse_down & SDL_BUTTON_LEFT) {
        world_editor_tile_left_button_pressed(x, y);
        return (true);
    }

    /*
     * MACOS seems bugged in SDL with this
     */
    if (mouse_down & SDL_BUTTON_RIGHT) {
        world_editor_tile_right_button_pressed(x, y);
        return (true);
    }

    return (true);
}

static void world_editor_mouse_over (widp w,
                                   int relx, int rely,
                                   int wheelx, int wheely)
{
    world_editor_ctx *ctx = wid_get_context(w);
    verify(ctx);

    if (wid_choose_title_dialog) {
        return;
    }

    if (!relx && !rely && !wheelx && !wheely) {
        return;
    }

    /*
     * If we recreate the map with a fixed focus we will be told about
     * a mouse over event immediately which may not be over the focus item
     * and will cause us to move. Annoying.
     */
    if (time_get_time_ms() - ctx->created < 100) {
        return;
    }

    int focus = (typeof(focus)) (uintptr_t) wid_get_context2(w);
    int focus_x = (focus & 0xff);
    int focus_y = (focus & 0xff00) >> 8;

    world_editor_set_focus(ctx, focus_x, focus_y);
}

static void world_editor_destroy (widp w)
{
    world_editor_ctx *ctx = wid_get_context(w);
    verify(ctx);

    wid_set_context(w, 0);

    int x, y, z;

    for (x = 0; x < WORLD_EDITOR_MENU_CELLS_ACROSS; x++) {
        for (y = 0; y < WORLD_EDITOR_MENU_CELLS_DOWN; y++) {

            tpp tp = ctx->tile[x][y].tile_tp;
            if (!tp) {
                continue;
            }

            ctx->tile[x][y].tile_tp = 0;

            for (z = 0; z < WORLD_DEPTH; z++) {
                world_editor_set_new_tp(x, y, z, 0);
            }
        }
    }

    saved_level_no = ctx->level_no;
    saved_focus_x = ctx->focus_x;
    saved_focus_y = ctx->focus_y;
    saved_world_x = ctx->world_x;
    saved_world_y = ctx->world_y;

    myfree(ctx);
    world_editor_window = 0;
    world_editor_window_ctx = 0;
}

static void world_editor_bg_create (void)
{
    return;

    widp wid;

    if (world_editor_background) {
        return;
    }

    {
        wid = world_editor_background = wid_new_window("bg");

        float f = (1024.0 / 680.0);

        fpoint tl = { 0.0, 0.0 };
        fpoint br = { 1.0, f };

        wid_set_tl_br_pct(wid, tl, br);

        wid_lower(wid);

        color c;
        c = WHITE;
        wid_set_mode(wid, WID_MODE_NORMAL);
        wid_set_color(wid, WID_COLOR_TL, c);
        wid_set_color(wid, WID_COLOR_BR, c);
        wid_set_color(wid, WID_COLOR_BG, c);

        wid_update(wid);
    }
}

static uint8_t world_editor_load_tile (const tree_node *node, void *arg)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    tpp tp;

    tp = (typeof(tp)) 
            (((char*) node) - STRUCT_OFFSET(struct tp_, tree2));

    if (tp_is_hidden_from_editor(tp)) {
        return (true);
    }

    int tile_pool = WORLD_TILE_MODE_LAND;

    if (tp_is_land(tp)) {
        tile_pool = WORLD_TILE_MODE_LAND;
    } else {
        return (true);
    }

    int count = ctx->tile_count[tile_pool];
    if (count >= TP_MAX_ID) {
        ERR("too many things");
    }

    ctx->tile_pools[tile_pool][count].tile_tp = tp;
    ctx->tile_count[tile_pool]++;

    /*
     * Start out with something.
     */
    if (!world_editor_chosen_tile[tile_pool]) {
        world_editor_chosen_tile[tile_pool] = tp;
    }

    return (true);
}

static void world_editor_load_tiles (void)
{
    tree_walk(thing_templates_create_order,
              world_editor_load_tile, 0 /* arg */);
}

static void world_editor_load_map (uint32_t level_no)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    LOG("Editor loading level %d", level_no);

    levelp l = level_load(level_no, 
                          true, /* is_editor */
                          false /* is_world_editor */);
    if (!l) {
        LOG("Failed to load level %d", level_no);

        l = level_reinit(level_no, 
                         true, /* is_editor */
                         false /* is_world_editor */);
        if (!l) {
            ERR("failed to create level");

        }
    }

    ctx->level = l;
    ctx->loading_level_no = level_no;
}

static void world_editor_tick (widp w)
{
    if (!wid_is_hidden(wid_console_window)) {
        return;
    }

    if (world_editor_save_popup || 
        world_editor_world_dialog || 
        wid_choose_color_dialog || 
        wid_choose_text_dialog || 
        wid_choose_title_dialog) {
        return;
    }

    world_editor_ctx *ctx = world_editor_window_ctx;
    if (!ctx) {
        return;
    }

    memset(ctx->world_highlight, 0, sizeof(ctx->world_highlight));

    int mx, my;

    world_editor_mx_my(&mx, &my);

    if (ctx->got_line_start) {
        world_editor_draw_highlight_line(ctx->line_start_x, ctx->line_start_y, 
                                       mx, my);
    }

    if (ctx->got_square_start) {
        world_editor_draw_highlight_square(ctx->square_start_x, 
                                         ctx->square_start_y,
                                         mx, my);
    }

    if (ctx->got_cut_start) {
        world_editor_draw_highlight_cut(ctx->cut_start_x, 
                                      ctx->cut_start_y,
                                      mx, my);
    }

    int moved = 0;

    if (sdl_joy_axes) {
        /*
         * Right stick
         */

        int changed = 0;

        if (sdl_joy_axes[3] > sdl_joy_deadzone) {
            changed = 1;
        }

        if (sdl_joy_axes[3] < -sdl_joy_deadzone) {
            changed = 1;
        }

        if (sdl_joy_axes[4] > sdl_joy_deadzone) {
            changed = 1;
        }

        if (sdl_joy_axes[4] < -sdl_joy_deadzone) {
            changed = 1;
        }

        if (changed) {
            static int ts;

            if (time_have_x_thousandths_passed_since(
                                DELAY_THOUSANDTHS_PLAYER_POLL * 6, ts)) {

                ts = time_get_time_ms();

                if (sdl_joy_axes[3] > sdl_joy_deadzone) {
                    world_editor_focus_right();
                    moved = 1;
                }

                if (sdl_joy_axes[3] < -sdl_joy_deadzone) {
                    world_editor_focus_left();
                    moved = 1;
                }

                if (sdl_joy_axes[4] > sdl_joy_deadzone) {
                    world_editor_focus_down();
                    moved = 1;
                }

                if (sdl_joy_axes[4] < -sdl_joy_deadzone) {
                    world_editor_focus_up();
                    moved = 1;
                }

                if (!ctx->tile_mode &&
                    (ctx->focus_x < WORLD_EDITOR_MENU_WORLD_ACROSS) && 
                    (ctx->focus_y < WORLD_EDITOR_MENU_WORLD_DOWN)) {

                    if (sdl_joy_buttons[SDL_JOY_BUTTON_A]) {
                        world_editor_tile_left_button_pressed(ctx->focus_x, 
                                                            ctx->focus_y);
                    }
                }
            }
        }

        /*
         * Left stick
         */
        changed = 0;

        if (sdl_joy_axes[0] > sdl_joy_deadzone) {
            changed = 1;
        }

        if (sdl_joy_axes[0] < -sdl_joy_deadzone) {
            changed = 1;
        }

        if (sdl_joy_axes[1] > sdl_joy_deadzone) {
            changed = 1;
        }

        if (sdl_joy_axes[1] < -sdl_joy_deadzone) {
            changed = 1;
        }

        if (changed) {
            static int ts;

            if (time_have_x_thousandths_passed_since(
                                DELAY_THOUSANDTHS_PLAYER_POLL, ts)) {

                ts = time_get_time_ms();

                if (sdl_joy_axes[0] > sdl_joy_deadzone) {
                    moved = 1;
                    if (ctx->tile_mode) {
                        world_editor_focus_right();
                    } else {
                        world_editor_world_scroll(1, 0);
                    }
                }

                if (sdl_joy_axes[0] < -sdl_joy_deadzone) {
                    moved = 1;
                    if (ctx->tile_mode) {
                        world_editor_focus_left();
                    } else {
                        world_editor_world_scroll(-1, 0);
                    }
                }

                if (sdl_joy_axes[1] > sdl_joy_deadzone) {
                    moved = 1;
                    if (ctx->tile_mode) {
                        world_editor_focus_down();
                    } else {
                        world_editor_world_scroll(0, 1);
                    }
                }

                if (sdl_joy_axes[1] < -sdl_joy_deadzone) {
                    moved = 1;
                    if (ctx->tile_mode) {
                        world_editor_focus_up();
                    } else {
                        world_editor_world_scroll(0, -1);
                    }
                }
            }
        }
    }

    if (!sdl_shift_held) {
#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION == 2 /* { */
        uint8_t *state = SDL_GetKeyState(0);
        uint8_t right = state[SDLK_RIGHT] ? 1 : 0;
        uint8_t left  = state[SDLK_LEFT] ? 1 : 0;
        uint8_t up    = state[SDLK_UP] ? 1 : 0;
        uint8_t down  = state[SDLK_DOWN] ? 1 : 0;
        uint8_t space = state[SDLK_SPACE] ? 1 : 0;
#else /* } { */
        const uint8_t *state = SDL_GetKeyboardState(0);
        uint8_t right = state[SDL_SCANCODE_RIGHT] ? 1 : 0;
        uint8_t left  = state[SDL_SCANCODE_LEFT] ? 1 : 0;
        uint8_t up    = state[SDL_SCANCODE_UP] ? 1 : 0;
        uint8_t down  = state[SDL_SCANCODE_DOWN] ? 1 : 0;
        uint8_t space = state[SDL_SCANCODE_SPACE] ? 1 : 0;
#endif /* } */

        if (up || right || down || left || space) {
            static int ts;

            if (time_have_x_thousandths_passed_since(
                                DELAY_THOUSANDTHS_PLAYER_POLL * 6, ts)) {

                ts = time_get_time_ms();

                if (right) {
                    world_editor_focus_right();
                    moved = 1;
                }

                if (left) {
                    world_editor_focus_left();
                    moved = 1;
                }

                if (down) {
                    world_editor_focus_down();
                    moved = 1;
                }

                if (up) {
                    world_editor_focus_up();
                    moved = 1;
                }

                if (!ctx->tile_mode &&
                    (
                        ctx->edit_mode == WORLD_EDITOR_MODE_DRAW
                    ) &&
                    (ctx->focus_x < WORLD_EDITOR_MENU_WORLD_ACROSS) && 
                    (ctx->focus_y < WORLD_EDITOR_MENU_WORLD_DOWN)) {

                    if (space) {
                        world_editor_tile_left_button_pressed(ctx->focus_x, 
                                                            ctx->focus_y);
                    }
                }
            }
        }
    }

    world_editor_update_buttons();

    if (moved) {
        if (ctx->b) {
            wid_mouse_warp(ctx->b);
        }
    }

    world_editor_update_buttons();
}

static void world_editor_save (const char *dir_and_file, int is_test_level)
{
    world_editor_ctx *ed = world_editor_window_ctx;

    LOG("Saving: %s", dir_and_file);

    /*
     * Write the file.
     */
    marshal_p ctx;
    ctx = marshal(dir_and_file);

    if (!ed->level) {
        ERR("no level to save");
    }

    marshal_level(ctx, ed->level);

    PUT_BRA(ctx);

    PUT_NAMED_UINT32(ctx, "width", WORLD_WIDTH);
    PUT_NAMED_UINT32(ctx, "height", WORLD_HEIGHT);

    int x, y, z;

    for (z = 0; z < WORLD_DEPTH; z++) {
        for (x = 0; x < WORLD_WIDTH; x++) {
            for (y = 0; y < WORLD_HEIGHT; y++) {

                tpp tp = ed->map.tile[x][y][z].tp;
                if (!tp) {
                    continue;
                }

                PUT_BRA(ctx);

                PUT_NAMED_UINT32(ctx, "x", x);
                PUT_NAMED_UINT32(ctx, "y", y);
                PUT_NAMED_STRING(ctx, "t", tp_name(tp));

                PUT_KET(ctx);
            }
        }
    }

    PUT_KET(ctx);
    PUT_KET(ctx); // level

    if (marshal_fini(ctx) < 0) {
        /*
         * Fail
         */
        char *popup_str = dynprintf("Failed to save %s: %s", dir_and_file,
                                    strerror(errno));

        MSG_BOX("%s", popup_str);
        myfree(popup_str);
    } else {
        if (!is_test_level) {
            /*
             * Success
             */
            char *popup_str = dynprintf("Saved %s", dir_and_file);
            widp popup = wid_tooltip(popup_str, 0.5f, 0.5f, med_font);
            wid_destroy_in(popup, ONESEC);
            myfree(popup_str);
        }

        LOG("Saved: %s", dir_and_file);
    }
}

static void world_editor_go_back (void)
{
    wid_destroy(&world_editor_background);
    wid_destroy(&world_editor_window);

    wid_map("Choose epic level", 0, 0);
}

static void world_editor_save_close_dialog (widp w)
{
    widp top = wid_get_top_parent(w);
    wid_destroy(&top);
    world_editor_save_popup = 0;
}

static void world_editor_save_level (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    uint32_t level_no = ctx->level_no;
    char *tmp = dynprintf("%s%d", WORLD_PATH, level_no);
    LOG("Save editor level %s", tmp);
    CON("Save editor level %s", tmp);
    world_editor_save(tmp, false /* is_test_level */);
    myfree(tmp);

    ctx->save_needed = false;
}

static void world_editor_save_callback_yes (widp w)
{
    world_editor_save_level();

    world_editor_save_close_dialog(w);

    world_editor_go_back();
}

static void world_editor_save_callback_no (widp w)
{
    world_editor_save_close_dialog(w);

    world_editor_go_back();
}

static void world_editor_save_callback_cancel (widp w)
{
    world_editor_save_close_dialog(w);
}

static void world_editor_save_ask (void)
{
    if (world_editor_save_popup) {
        return;
    }

    world_editor_save_popup = 
        wid_menu(0,
                vvlarge_font,
                large_font,
                0, // on_update
                0.5, /* x */
                0.5, /* y */
                0.2, /* hightlight */
                1, /* columns */
                1, /* focus */
                4, /* items */

                (int) 0, "save level?", (void*) 0,

                (int) 'y', "Yes", world_editor_save_callback_yes,

                (int) 'n', "No",  world_editor_save_callback_no,

                (int) 'c', "Cancel",  world_editor_save_callback_cancel);
}

static void world_editor_hide (void)
{
    world_editor_ctx *ctx = world_editor_window_ctx;
    verify(ctx);
    verify(ctx->w);

    if (ctx->save_needed) {
        world_editor_save_ask();
    } else {
        world_editor_go_back();
    }
}

void world_editor (uint32_t level_no)
{
    LOG("Entering level editor for level %u", level_no);
    LOG("====================================== ");

    /*
     * Create a context to hold button info so we can update it when the focus 
     * changes
     */
    world_editor_ctx *ctx = myzalloc(sizeof(*ctx), "wid editor");
    world_editor_window_ctx = ctx;

    ctx->focus_x = -1;
    ctx->focus_y = -1;
    ctx->level_no = level_no;

    widp window;
    ctx->w = world_editor_window = window = wid_new_window("wid editor");
    wid_set_context(window, ctx);
    ctx->is_new = true;

    /*
     * Main window
     */
    {
        fpoint tl = { 0.0, 0.0};
        fpoint br = { 1.0, 1.0};

        wid_set_tl_br_pct(window, tl, br);

        color c = BLACK;
        c.a = 0;
        wid_set_color(window, WID_COLOR_BG, c);

        wid_set_on_tick(window, world_editor_tick);
        wid_set_on_destroy(window, world_editor_destroy);
        wid_set_context(window, ctx);
    }

    {
        /*
         * Create the buttons
         */
        int x, y;

        for (x = 0; x < WORLD_EDITOR_MENU_CELLS_ACROSS; x++) {
        for (y = 0; y < WORLD_EDITOR_MENU_CELLS_DOWN; y++) {
            widp b = wid_new_square_button(world_editor_window, "wid tile");
            ctx->tile[x][y].button = b;
            ctx->tile[x][y].x = x;
            ctx->tile[x][y].y = y;

            wid_set_on_key_down(b, world_editor_key_down);
            wid_set_on_joy_down(b, world_editor_joy_button);
            wid_set_on_mouse_over_begin(b, world_editor_mouse_over);
            wid_set_on_mouse_motion(b, world_editor_mouse_motion);
            wid_set_on_mouse_down(b, world_editor_mouse_down);
            wid_set_on_display(b, world_editor_button_display);

            wid_set_context(b, ctx);
            int focus = (y << 8) | x;
            wid_set_context2(b, (void*) (uintptr_t) focus);

            if (x == WORLD_EDITOR_MENU_CELLS_ACROSS - 1) {
                switch (y) {
                case WORLD_EDITOR_MODE2_TITLE:
                    wid_set_text(b, "Title");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "Set level title",
                                        vsmall_font);
                    }
                    break;
                }
            }

            if (y == WORLD_EDITOR_MENU_CELLS_DOWN - 1) {
                switch (x) {
                case WORLD_EDITOR_MODE_DRAW:
                    wid_set_text(b, "Draw");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "D - draw tiles", vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_LINE:
                    wid_set_text(b, "Line");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "L - draw lines of tiles", vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_FILL:
                    wid_set_text(b, "Flood");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "f - flood fill", vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_DEL:
                    wid_set_text(b, "Del");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "x - remove tiles", vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_UNDO:
                    wid_set_text(b, "Undo");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "u - shortcut", vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_REDO:
                    wid_set_text(b, "Redo");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "e - shortcut", vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_SAVE:
                    wid_set_text(b, "Save");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "s - shortcut", vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_TOGGLE:
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "TAB - switch between tile and map mode", vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_YANK:
                    wid_set_text(b, "Yank");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "y - picks up a tile and its color",
                                        vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_EDIT:
                    wid_set_text(b, "Edit");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "edit thing attributes",
                                        vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_SQUARE:
                    wid_set_text(b, "Rect");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "r - draw a rectangle",
                                        vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_CIRCLE:
                    wid_set_text(b, "Circ");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "draw a circle",
                                        vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_OCTAGON:
                    wid_set_text(b, "Oct");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "draw an octagon",
                                        vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_CUT:
                    wid_set_text(b, "Cut");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "Remove rectangular section",
                                        vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_COPY:
                    wid_set_text(b, "Copy");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "c - copy an area into the buffer",
                                        vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_PASTE:
                    wid_set_text(b, "Paste");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "v - paste a previously cut area",
                                        vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_VFLIP:
                    wid_set_text(b, "Vflip");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "Vertical flip.", 
                                        vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_HFLIP:
                    wid_set_text(b, "Hflip");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "Horzizontal flip.", 
                                        vsmall_font);
                    }
                    break;
                case WORLD_EDITOR_MODE_ROTATE:
                    wid_set_text(b, "Rot");
                    if (!sdl_joy_axes) {
                        wid_set_tooltip(b, "Rotate level.", 
                                        vsmall_font);
                    }
                    break;
                }
            }
        }
        }
    }

    ctx->focus_x = -1;
    ctx->focus_y = -1;
    ctx->world_x = -1;
    ctx->world_y = -1;

    /*
     * Load all tiles
     */
    world_editor_load_tiles();

    /*
     * Load the level
     */
    world_editor_load_map(level_no);

    world_editor_fixup();

    /*
     * If no position was loaded from the level, use a default unless this is 
     * an edit of an old level.
     */
    if (level_no == TEST_LEVEL) {
        /*
         * Returning from testing a level? Welcome back...
         */
        ctx->level_no = saved_level_no;
        ctx->focus_x = saved_focus_x;
        ctx->focus_y = saved_focus_y;
        ctx->world_x = saved_world_x;
        ctx->world_y = saved_world_y;
    } else if (!memcmp(&saved_level_no, &ctx->level_no, sizeof(uint32_t))) {
        /*
         * Reloading the same level?
         */
        if (saved_focus_x != -1) {
            ctx->focus_x = saved_focus_x;
            ctx->focus_y = saved_focus_y;
            ctx->world_x = saved_world_x;
            ctx->world_y = saved_world_y;
        }
    }

    if (ctx->focus_x == -1) {
        ctx->focus_x = WORLD_EDITOR_MENU_CELLS_ACROSS / 2;
    }

    if (ctx->focus_y == -1) {
        ctx->focus_y = WORLD_EDITOR_MENU_CELLS_DOWN / 2;
    }

    if (ctx->world_x == -1) {
        ctx->world_x = WORLD_WIDTH / 2;
        ctx->world_x--;
        ctx->world_x--;
        ctx->world_x--;
        ctx->world_x--;
    }

    if (ctx->world_y == -1) {
        ctx->world_y = WORLD_HEIGHT / 2;
        ctx->world_y--;
        ctx->world_y--;
        ctx->world_y--;
        ctx->world_y--;
    }

    ctx->undo_at = -1;
    world_editor_undo_save();

    /*
     * Repair the context so it is not pointing at the last level loaded.
     */
    wid_set_context(window, ctx);

    world_editor_update_buttons();
    wid_update(window);
    world_editor_update_buttons();
    wid_update(window);
    world_editor_bg_create();

    ctx->created = time_get_time_ms();
    world_editor_tile_mode_set(false);
    ctx->tile_pool = WORLD_TILE_MODE_LAND;

    world_editor_set_mode(WORLD_EDITOR_MODE_DRAW);

    if (WORLD_TILE_MODE_LAST > WORLD_TILE_POOL_MAX) {
        ERR("bug, you need to resize WORLD_TILE_POOL_MAX");
    }
}
