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

static void thing_jump_end (levelp level, thingp t)
{
    t->jump_speed = 0;
}

/*
 * Try to fall down
 */
int thing_jump (levelp level, thingp t)
{
    double x = t->x;
    double y;

    y = t->y - t->jump_speed;

    if (thing_hit_solid_obstacle(level, t, x, y)) {
        t->jump_speed = 0;
        thing_jump_end(level, t);
        return (true);
    }

    thing_wid_update(level, t, x, y, true, false /* is new */);

    if (thing_is_ropetop(t)) {
        if (!thing_hit_fall_obstacle(level, t,  t->x, (int) t->y + 1.0)) {
            thing_place_rope(level, t, t->x, (int) t->y + 1.0);
        }

        if (!thing_hit_fall_obstacle(level, t,  t->x, (int) t->y + 2.0)) {
            thing_place_rope(level, t, t->x, (int) t->y + 2.0);
        }
    }

    t->jump_speed *= 0.90;

    if (fabs(t->jump_speed) < 0.01) {
        t->jump_speed = 0;
        thing_jump_end(level, t);
        return (true);
    }

    thing_handle_collisions(level, t);

    return (true);
}
