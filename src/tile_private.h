/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include "tree.h"

typedef struct tile_ {
    tree_key_string tree;
    /*
     * Index within the overall texture, left to right, top to bottom.
     */
    uint32_t index;
    uint32_t pix_width;
    uint32_t pix_height;
    /*
     * Texture co-ordinates within the image.
     */
    double x1;
    double y1;
    double x2;
    double y2;
    /*
     * As above but not clipped 0.5 pixels.
     */
    double ox1;
    double oy1;
    double ox2;
    double oy2;
    /*
     * Percentage points that indicate the start of the pixels within the tile
     * texture for use in collisions.
     */
    double pct_width;
    double pct_height;
    double px1;
    double py1;
    double px2;
    double py2;
    int32_t gl_surface_binding;
    int32_t gl_surface_binding_black_and_white;
    texp tex;
} tile;

extern tree_root *tiles;
