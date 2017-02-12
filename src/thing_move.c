/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "tile.h"
#include "math_util.h"

void thing_wid_move (thingp t,
                     double x,
                     double y,
                     uint8_t smooth)
{
    double dist = DISTANCE(t->x, t->y, x, y);

    if (smooth) {
        if (dist == 0.0) {
            return;
        }
    }

    widp w = t->wid;

    thing_move_to(t, x, y);

    double height = game.tile_height;
    double width = game.tile_width;

    x *= width;
    y *= height;

    x += width / 2;
    y += height / 2;

    fpoint tl = { x, y };
    fpoint br = { x, y };

    if (!w) {
        /*
         * Player has left the level?
         */
        THING_LOG(t, "Trying to move but has no widget");
        DIE("bug");
        return;
    }

    tilep tile = wid_get_tile(w);
    if (tile) {
        double tw = tile_get_width(tile);
        double th = tile_get_height(tile);
        double scale_x = tw / TILE_WIDTH;
        double scale_y = th / TILE_HEIGHT;

        double tile_width = game.tile_width;
        double tile_height = game.tile_height;

        if (scale_x > 1) {
            br.x -= tile_width / 2.0;
            br.y -= tile_height / 2.0;
            tl.x -= tile_width / 2.0;
            tl.y -= tile_height / 2.0;

            tile_width *= scale_x;
            tile_height *= scale_y;
        }

        br.x += tile_width / 2.0;
        br.y += tile_height / 2.0;
        tl.x -= tile_width / 2.0;
        tl.y -= tile_height / 2.0;
    }

    /*
     * Off the map? Perhaps between levels.
     */
    if (!w) {
        return;
    }

    /*
     * For teleporting things, no smooth move!
     */
    if (dist > 2.0) {
        wid_move_end(w);
        smooth = false;
    }

    double ms = 200;
    ms = ms / (1.0 / dist);

    /*
     * Add a bit of jumpiness to confuse the player.
     */
    if (thing_is_monst(t)) {
        ms -= 10.0 * gauss(tp_get_speed(thing_tp(t)), 10);
    }

    if (smooth) {
        wid_move_to_abs_poffset_in(w, tl.x, tl.y, ms);
    } else {
        wid_set_pos(w, tl, br);
    }

    double bounce_ms = 300;

    if (thing_is_player(t) ||
        thing_is_monst(t)) {

        wid_bounce_to_pct_in(w, 0.1, 0.9, bounce_ms, 1);

        {
            if (!w->bouncing) {
                wid_bounce_to_pct_in(w, 0.1, 0.9, bounce_ms, 0);
            }
        }
    }
}

void thing_wid_update (thingp t,
                       double x, double y,
                       uint8_t smooth)
{
    verify(t);

    thing_wid_move(t, x, y, smooth);
}
