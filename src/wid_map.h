/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#pragma once

#define WID_MAP_EXITS_MAX 255

typedef struct wid_map_tile_ {
    tpp tp;
    tilep tile;

    /*
     * Data associated with individual tiles.
     */
    thing_template_data data;
} wid_map_tile;

typedef struct wid_map_cell_ {
    char name[MAXSTR];
    int x;
    int y;

    /*
     * Used for level preview.
     */
    wid_map_tile tiles[MAP_WIDTH][MAP_HEIGHT][MAP_DEPTH];

    /*
     * Player (well one of them) start at
     */
    int px;
    int py;
    int player_count;
} wid_map_level;

typedef void(*wid_map_event_t)(widp);
typedef void(*on_selected_t)(const uint32_t);
typedef void(*on_cancelled_t)(void);

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
     * Entered text.
     */
    char text[MAXSTR];

    /*
     * Item currently in focus
     */
    int focusx;
    int focusy;

    /*
     * When the map was made.
     */
    uint32_t created;

    /*
     * Items in the map
     */
    widp buttons[LEVELS_ACROSS][LEVELS_DOWN];
    wid_map_level levels[LEVELS_ACROSS][LEVELS_DOWN];

    /*
     * What to call on events
     */
    wid_map_event_t selected;
    wid_map_event_t cancelled;

    /*
     * Just created?
     */
    int is_new;

    /*
     * Current level being loaded.
     */
    int loading_level_no;

    on_selected_t on_selected;
    on_cancelled_t on_cancelled;

} wid_map_ctx;

widp wid_map(const char *title, on_selected_t, on_cancelled_t);

widp wid_editor_level_map_thing_replace_template(levelp,
                                                 double x,
                                                 double y,
                                                 thingp t,
                                                 tpp tp,
                                                 tpp_data);
extern void wid_map_cell_play(void);
extern void wid_map_cell_load(void);
extern void wid_map_visible(void);
extern void wid_map_preview(widp);
extern void wid_map_preview_thumbnail(widp);
