/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "thing.h"
#include "wid_game_map.h"
#include "tile.h"
#include "math_util.h"
#include "time_util.h"
#include "player.h"

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

    x *= game.tile_width;
    y *= game.tile_height;
    x += game.tile_width / 2;
    y += game.tile_height / 2;

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
            tile_width *= scale_x;
            tile_height *= scale_y;
        }

        br.x += tile_width / 2.0;
        br.y += tile_height / 2.0;
        tl.x -= tile_width / 2.0;
        tl.y -= tile_height / 2.0;
    }

    /*
     * Now the tile itself has a shadow that is 1/4 of the pixels.
     * The center is 24x24 and with shadow it is 32x32. We need to
     * stretch the tile so it overlaps so the inner 24x24 if seamless.
     */
#if 0
    double tile_width = ((br.x - tl.x) / 
                         (double)TILE_PIX_WIDTH) * 
                            (double)TILE_PIX_WITH_SHADOW_WIDTH;

    double tile_height = ((br.y - tl.y) / 
                         (double)TILE_PIX_HEIGHT) * 
                            (double)TILE_PIX_WITH_SHADOW_HEIGHT;
    double tile_width = (br.x - tl.x);
    double tile_height = (br.y - tl.y);

    tile_width *= 0.50;
    tile_height *= 0.50;
#endif

#if 0
    double tile_width = (br.x - tl.x);
    double tile_height = (br.y - tl.y);
#endif

#if 0
    if (thing_is_wall(t) || thing_is_door(t)) {
        tl.y -= game.tile_height / 3.0;
        br.x += game.tile_width / 3.0;
    }
#endif

#if 0
    if (scale_x == 1) {
        tl.y -= tile_height / 4.0;
        br.x += tile_width / 4.0;
    }
#endif

#if 0
    /*
     * Stretch vertically a bit and then recented. Don't do this. Overlapping 
     * wall pixels with shadows mess up.
     */
    tl.y -= tile_height / 8.0;
    tl.y += tile_height / 16.0;
    br.y += tile_height / 16.0;
#endif

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

    double ms = 1000;
    ms = ms / (1.0 / dist);

    /*
     * Add a bit of jumpiness to confuse the player.
     */
    if (thing_is_monst(t)) {
        ms -= 10.0 * gauss(tp_get_speed(thing_tp(t)), 10);
    }

    if (smooth) {
        wid_move_to_abs_poffset_in(w, tl, ms);
    } else {
        wid_set_tl_br(w, tl, br);
    }

    double bounce_ms = 300;

    wid_bounce_to_pct_in(w, 0.1, 0.9, bounce_ms, 4);

    if (thing_is_player(t) || 
        thing_is_monst(t)) {

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
