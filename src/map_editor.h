/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#pragma once

#define MAP_EDITOR_MENU_CELLS_ACROSS    26
#define MAP_EDITOR_MENU_CELLS_DOWN      16

#define MAP_EDITOR_MENU_MAP_ACROSS      25
#define MAP_EDITOR_MENU_MAP_DOWN        15

#define MAP_EDITOR_MENU_TILES_ACROSS    20
#define MAP_EDITOR_MENU_TILES_DOWN      14

#define MAP_EDITOR_UNDO                 30

enum {
    MAP_EDITOR_MODE_TOGGLE,
    MAP_EDITOR_MODE_COLOR,
    MAP_EDITOR_MODE_DRAW,
    MAP_EDITOR_MODE_PAINT,
    MAP_EDITOR_MODE_LINE,
    MAP_EDITOR_MODE_SQUARE,
    MAP_EDITOR_MODE_CIRCLE,
    MAP_EDITOR_MODE_OCTAGON,
    MAP_EDITOR_MODE_FILL,
    MAP_EDITOR_MODE_DEL,
    MAP_EDITOR_MODE_YANK,
    MAP_EDITOR_MODE_EDIT,
    MAP_EDITOR_MODE_CUT,
    MAP_EDITOR_MODE_COPY,
    MAP_EDITOR_MODE_PASTE,
    MAP_EDITOR_MODE_UNDO,
    MAP_EDITOR_MODE_REDO,
    MAP_EDITOR_MODE_SAVE,
    MAP_EDITOR_MODE_UNUSED_2,
    MAP_EDITOR_MODE_UNUSED_3,
    MAP_EDITOR_MODE_VFLIP,
    MAP_EDITOR_MODE_HFLIP,
    MAP_EDITOR_MODE_ROTATE,
    MAP_EDITOR_MODE_TEST,
    MAP_EDITOR_MODE_STYLE,
    MAP_EDITOR_MODE_RANDOM,
    MAP_EDITOR_MODE_MAX,
};

enum {
    MAP_EDITOR_MODE2_TITLE,
    MAP_EDITOR_MODE2_OUTLINE,
    MAP_EDITOR_MODE2_UNUSED_5,
    MAP_EDITOR_MODE2_UNUSED_6,
    MAP_EDITOR_MODE2_UNUSED_7,
    MAP_EDITOR_MODE2_UNUSED_8,
    MAP_EDITOR_MODE2_FILTER_ACTIONS,
    MAP_EDITOR_MODE2_FILTER_OBJ,
    MAP_EDITOR_MODE2_FILTER_WALL,
    MAP_EDITOR_MODE2_FILTER_ALL,
    MAP_EDITOR_MODE2_MAX,
};

enum {
    WID_TILE_MODE_NONE,
    WID_TILE_MODE_WALLS,
    WID_TILE_MODE_FLOORS,
    WID_TILE_MODE_MOB_SPAWNERS,
    WID_TILE_MODE_MONST,
    WID_TILE_MODE_WEAPONS,
    WID_TILE_MODE_FOOD,
    WID_TILE_MODE_TREASURE,
    /*
     * Add to here, update WID_TILE_POOL_MAX
     */
    WID_TILE_MODE_KEYS,
    WID_TILE_MODE_TRAP,
    WID_TILE_MODE_ITEMS,
    WID_TILE_MODE_PLAYER,
    WID_TILE_MODE_EXITS,
    WID_TILE_MODE_ACTIONS,
    WID_TILE_MODE_LAST,
    /*
     * Add to here, update WID_TILE_POOL_MAX
     */
};
#define WID_TILE_POOL_MAX 14

typedef struct map_editor_tile_ {
    int x;
    int y;

    /*
     * Used for level preview.
     */
    tpp tile_tp;

    widp button;
} map_editor_tile;

typedef struct map_editor_map_tile_ {
    tpp tp;

    /*
     * Data associated with individual tiles.
     */
    thing_template_data data;
} map_editor_map_tile;

typedef struct map_editor_map_grid_ {
    map_editor_map_tile tile[MAP_WIDTH][MAP_HEIGHT][MAP_DEPTH];

    /*
     * For joined up walls etc.
     */
    tilep map_tile[MAP_WIDTH][MAP_HEIGHT];
} map_editor_map_grid;

typedef void(*map_editor_event_t)(widp);

typedef struct {
    /*
     * Parent widget
     */
    widp w;

    /*
     * Current button
     */
    widp b;

    /*
     * Current color
     */
    const char *col_name;
    color col;

    /*
     * Item currently in focus
     */
    int focus_x;
    int focus_y;

    /*
     * When the map was made.
     */
    uint32_t created;

    /*
     * When we last changed from tile to map mode.
     */
    uint32_t mode_toggled;

    /*
     * Items in the map
     */
    map_editor_tile 
        tile[MAP_EDITOR_MENU_CELLS_ACROSS][MAP_EDITOR_MENU_CELLS_DOWN];

    /*
     * Tile pools
     */
    map_editor_tile tile_pools[WID_TILE_POOL_MAX][THING_MAX];
    int tile_count[WID_TILE_POOL_MAX];

    /*
     * Map tiles
     */
    map_editor_map_grid map;
    map_editor_map_grid map_undo[MAP_EDITOR_UNDO];
    map_editor_map_grid map_copy;
    map_editor_map_grid map_tmp;

    /*
     * When we change layer, this holds onto all the unmodified layers.
     */
    map_editor_map_grid map_preserved_layers;

    uint8_t valid_undo[MAP_EDITOR_UNDO];

    /*
     * For line drawing.
     */
    int map_highlight[MAP_WIDTH][MAP_HEIGHT];

    /*
     * Just created?
     */
    int is_new;

    /*
     * Modified and need to save?
     */
    int save_needed;

    /*
     * Current level.
     */
    levelp level;
    uint32_t level_no;

    /*
     * Current level being loaded.
     */
    int loading_level_no;

    int map_x;
    int map_y;

    /*
     * Where in the undo array are we
     */
    int undo_at;

    int tile_mode;
    int tile_pool;

    /*
     * Which slice are we editing.
     */
    int layer_mode;

    /*
     * Drawing or lines or?
     */
    int edit_mode;
    int old_edit_mode;

    /*
     * Line drawing state.
     */
    int line_start_x;
    int line_start_y;
    int got_line_start;

    int square_start_x;
    int square_start_y;
    int got_square_start;

    int cut_start_x;
    int cut_start_y;
    int cut_end_x;
    int cut_end_y;
    int got_cut_start;

} map_editor_ctx;

void map_editor(uint32_t);

widp map_editor_replace_template(levelp,
                                 double x,
                                 double y,
                                 thingp t,
                                 tpp tp,
                                 tpp_data);

extern void map_editor_preview(widp);
extern void map_editor_preview_thumbnail(widp);

extern widp map_editor_window;
