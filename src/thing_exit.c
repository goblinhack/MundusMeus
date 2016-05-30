/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "thing.h"
#include "level.h"
#include "sound.h"
#include "wid_game_map.h"
#include "player.h"

void thing_reached_exit (levelp level, thingp t)
{
    game.level_no++;

    THING_LOG(t, "Jump to level %d", game.level_no);

    verify(t);

    t->score += 1000 * level->level_no;
    t->hp ++;

    thing_leave_level(level, t);

    level_finished(level, true /* keep player*/);

    player_wid_update(level);

    /*
     * To allow the player to be centered in the new level if it is a
     * different size.
     */
    if (game.wid_grid &&
        game.wid_grid->grid) {
        game.wid_grid->grid->bounds_locked = 0;
    }

    level = &game.level;

    if (player) {
        t = player;

        thing_move(level, t, t->x, t->y, false, false, false, false, false);
    }
}
