/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

typedef struct wid_tiles_ {
    tree_key_string tree;
    int across;
    int down;
    int tile_w;
    int tile_h;
    double scale;
    tilep tile[32][32];
} wid_tiles;

uint8_t wid_tiles_init(void);
void wid_tiles_fini(void);
wid_tilesp wid_tiles_load(const char *name, double scale);
wid_tilesp wid_tiles_find(const char *name);
