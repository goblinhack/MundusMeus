/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#pragma once

#define WORLD_EDITOR_MENU_CELLS_ACROSS    26
#define WORLD_EDITOR_MENU_CELLS_DOWN      16

#define WORLD_EDITOR_MENU_WORLD_ACROSS    25
#define WORLD_EDITOR_MENU_WORLD_DOWN      15

#define WORLD_EDITOR_MENU_TILES_ACROSS    20
#define WORLD_EDITOR_MENU_TILES_DOWN      14

#define WORLD_EDITOR_UNDO                 30

enum {
    WORLD_EDITOR_MODE_TOGGLE,
    WORLD_EDITOR_MODE_DRAW,
    WORLD_EDITOR_MODE_LINE,
    WORLD_EDITOR_MODE_SQUARE,
    WORLD_EDITOR_MODE_CIRCLE,
    WORLD_EDITOR_MODE_OCTAGON,
    WORLD_EDITOR_MODE_FILL,
    WORLD_EDITOR_MODE_DEL,
    WORLD_EDITOR_MODE_YANK,
    WORLD_EDITOR_MODE_EDIT,
    WORLD_EDITOR_MODE_CUT,
    WORLD_EDITOR_MODE_COPY,
    WORLD_EDITOR_MODE_PASTE,
    WORLD_EDITOR_MODE_UNDO,
    WORLD_EDITOR_MODE_REDO,
    WORLD_EDITOR_MODE_SAVE,
    WORLD_EDITOR_MODE_UNUSED_2,
    WORLD_EDITOR_MODE_UNUSED_3,
    WORLD_EDITOR_MODE_VFLIP,
    WORLD_EDITOR_MODE_HFLIP,
    WORLD_EDITOR_MODE_ROTATE,
    WORLD_EDITOR_MODE_MAX,
};

enum {
    WORLD_EDITOR_MODE2_TITLE,
    WORLD_EDITOR_MODE2_MAX,
};

enum {
    WORLD_TILE_MODE_NONE,
    WORLD_TILE_MODE_LAND,
    WORLD_TILE_MODE_PLACES,
    WORLD_TILE_MODE_LAST,
    /*
     * Add to here, update WORLD_TILE_POOL_MAX
     */
};
#define WORLD_TILE_POOL_MAX 3

typedef struct world_editor_tile_ {
    int x;
    int y;

    /*
     * Used for level preview.
     */
    tpp tile_tp;

    widp button;
} world_editor_tile;

typedef struct world_editor_world_tile_ {
    tpp tp;
} world_editor_world_tile;

typedef struct world_editor_world_grid_ {
    world_editor_world_tile tile[WORLD_WIDTH][WORLD_HEIGHT][WORLD_DEPTH];

    /*
     * For joined up walls etc.
     */
    tilep world_tile[WORLD_WIDTH][WORLD_HEIGHT];
} world_editor_world_grid;

typedef void(*world_editor_event_t)(widp);

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
    world_editor_tile 
        tile[WORLD_EDITOR_MENU_CELLS_ACROSS][WORLD_EDITOR_MENU_CELLS_DOWN];

    /*
     * Tile pools
     */
    world_editor_tile tile_pools[WORLD_TILE_POOL_MAX][THING_MAX];
    int tile_count[WORLD_TILE_POOL_MAX];

    /*
     * Map tiles
     */
    world_editor_world_grid map;
    world_editor_world_grid world_undo[WORLD_EDITOR_UNDO];
    world_editor_world_grid world_copy;
    world_editor_world_grid world_tmp;

    uint8_t valid_undo[WORLD_EDITOR_UNDO];

    /*
     * For line drawing.
     */
    int world_highlight[WORLD_WIDTH][WORLD_HEIGHT];

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

    int world_x;
    int world_y;

    /*
     * Where in the undo array are we
     */
    int undo_at;

    int tile_mode;
    int tile_pool;

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

} world_editor_ctx;

void world_editor(uint32_t);

widp world_editor_replace_template(levelp,
                                 double x,
                                 double y,
                                 thingp t,
                                 tpp tp);

extern void world_editor_preview(widp);
extern void world_editor_preview_thumbnail(widp);

extern widp world_editor_window;
