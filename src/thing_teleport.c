/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "thing.h"
#include "wid_game_map.h"
#include "time_util.h"
#include "math_util.h"
#include "color.h"
#include "level.h"

void thing_reached_teleport (levelp level,
                             thingp t, 
                             thingp teleport)
{
    int tx[MAP_WIDTH*MAP_HEIGHT];
    int ty[MAP_WIDTH*MAP_HEIGHT];
    int poss = 0;

    int x, y;

    int delay = 20;

    if (!thing_is_player(t)) {
        /*
         * Only move other things sometimes.
         */
        if ((myrand() % 100) < 95) {
            return;
        }
    }

    if (!time_have_x_tenths_passed_since(delay, 
                                         t->timestamp_last_teleport)) {
        return;
    }

    for (x = 0; x < MAP_WIDTH; x++) {
        for (y = 0; y < MAP_HEIGHT; y++) {

            level_map_tile *tile = 
                &level->map_grid.tile[x][y][MAP_DEPTH_EXPLOSION];

            tpp it = tile->tp;
            if (!it) {
                continue;
            }

            if (!tp_is_teleport(it)) {
                continue;
            }

            if (DISTANCE(t->x, t->y, x, y) < 2.0) {
                continue;
            }

            color tcol;
            color col;

            tcol = teleport->data.col;

            col = tile->data.col;

            if (color_none(col) || color_none(tcol)) {
                tx[poss] = x;
                ty[poss] = y;
                poss++;
                continue;
            }

            if (color_cmp(col, tcol)) {
                tx[poss] = x;
                ty[poss] = y;
                poss++;
                continue;
            }
        }
    }

    if (!poss) {
        return;
    }

    t->timestamp_last_teleport = time_get_time_ms();

    poss = myrand() % poss;

    double nx = tx[poss];
    double ny = ty[poss];

    /*
     * Don't let lava spawn continually on top of itself.
     */
    if (thing_is_lava(t) && map_is_lava_at(level, nx, ny)) {
        return;
    }

    /*
     * Don't let water spawn continually on top of itself.
     */
    if (thing_is_water(t) && map_is_water_at(level, nx, ny)) {
        return;
    }

    /*
     * Don't let acid spawn continually on top of itself.
     */
    if (thing_is_acid(t) && map_is_acid_at(level, nx, ny)) {
        return;
    }

    /*
     * Don't let water spawn continually on top of itself.
     */
    if (thing_is_water(t) && map_is_water_at(level, nx, ny)) {
        return;
    }

    /*
     * Don't let monsters spawn continually on top of themselves.
     */
    if (thing_is_monst(t) && map_is_monst_at(level, nx, ny)) {
        return;
    }

    THING_LOG(t, "Teleport");

    wid_move_end(t->wid);

    thing_wid_update(level,
                     t, nx, ny, 
                     false /* smooth */,
                     true /* is_new */);

    /*
     * Make sure the client sees the jump
     */
    thing_handle_collisions(level, t);

    MSG_SHOUT_AT(SOUND, t, t->x, t->y, "teleport");
}
