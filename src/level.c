/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "slre.h"

#include "main.h"
#include "time_util.h"
#include "map.h"

static uint8_t level_init_done;

uint8_t level_init (void)
{
    if (level_init_done) {
        return (true);
    }

    level_init_done = true;

    return (true);
}

void level_fini (void)
{
    if (level_init_done) {
        level_init_done = false;
    }
}

void level_destroy (levelp *plevel, uint8_t keep_player)
{
    levelp level;

    if (!plevel) {
        ERR("no plevel");
    }

    level = *plevel;
    if (!level) {
        DIE("level destroy, no level");
    }

    level->is_being_destroyed = true;

    *plevel = 0;

    memset(level, 0, sizeof(*level));

    level = 0;
}

widp level_get_map (levelp level)
{
    return (game.wid_grid);
}

/*
 * Clean up the level.
 */
levelp level_finished (levelp level, int keep_player)
{
    level_destroy(&level, keep_player);

    /*
     * LEVEL CAN BE NULL HERE
     */

    return (level);
}

void level_finished_all (void)
{
    LOG("Destroy level");

    levelp level = &game.level;
    level_finished(level, false /* keep_player */);
}
