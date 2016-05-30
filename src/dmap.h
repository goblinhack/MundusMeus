/*
 * Copyright (C) 2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

typedef struct dmap_ {
    int16_t val[MAP_WIDTH][MAP_HEIGHT];
} dmap;

void dmap_process(dmap *d);
void dmap_print(dmap *d);

#define DMAP_IS_WALL 999
