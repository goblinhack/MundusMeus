/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "wid.h"
#include "color.h"
#include "wid_popup.h"
#include "string_util.h"
#include "thing_template.h"
#include "math_util.h"
#include "thing.h"
#include "wid_game_map.h"
#include "level.h"

void level_place_light (levelp level, double x, double y)
{
    int ix = (int)x & ~7;
    int iy = (int)y & ~7;

    if (level->map_grid.lit[ix][iy]) {
        return;
    }

    level->map_grid.lit[ix][iy] = 1;

    (void) wid_game_map_replace_tile(level, x, y,
                                     0, /* thing */
                                     id_to_tp(THING_LIGHT),
                                     0 /* tpp_data */);
}
