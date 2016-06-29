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

/*
 * This is movement on the client of the player initiated by the player.
 */
int thing_move (levelp level,
                thingp t,
                double x,
                double y,
                const uint8_t up,
                const uint8_t down,
                const uint8_t left,
                const uint8_t right,
                const uint8_t fire)
{
    int rc = true;

    if (thing_is_dead(t)) {
        return (false);;
    }

#if 0
    if (t->wid) {
        if (thing_hit_solid_obstacle(level, t, x, y)) {
            if (!thing_hit_solid_obstacle(level, t, x, t->y)) {
                y = t->y;
            } else if (!thing_hit_solid_obstacle(level, t, t->x, y)) {
                x = t->x;
            } else {
                x = t->x;
                y = t->y;

                rc = false;
            }
        } else if ((x != 0) && (y != 0)) {
            if (thing_hit_solid_obstacle(level, t, x, t->y)) {
                x = t->x;
            } 

            if (thing_hit_solid_obstacle(level, t, t->x, y)) {
                y = t->y;
            }
        }
    }
#endif

    thing_move_set_dir(level, t, &x, &y, up, down, left, right);

    /*
     * If no widget yet then this can be a dummy move during thing creation
     * just to set the weapon anim correctly.
     */
    if (!t->wid) {
        return (false);
    }

    if (up || down || left || right) {
        thing_wid_update(level, t, x, y, true, false /* is new */);
    }

    if (fire) {
        thing_fire(level, t, up, down, left, right);
    }

    /*
    thing_handle_collisions(level, t);
     */

    return (rc);
}

void thing_wid_move (levelp level,
                     thingp t, 
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

    /*
     * Breaks snow_settlement bounce if not
     */
    if (thing_can_roll(t)) {
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
        if (t->is_jumping) {
            if (wid_is_moving(w)) {
                wid_move_end(w);
            }
        }

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

void thing_wid_update (levelp level,
                       thingp t, 
                       double x, double y, 
                       uint8_t smooth,
                       uint8_t is_new)
{
    verify(t);

    thing_wid_move(level, t, x, y, smooth);
}
