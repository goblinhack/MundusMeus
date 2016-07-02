/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include <SDL.h>
#include <errno.h>
#include "slre.h"

#include "main.h"
#include "string_util.h"
#include "wid.h"
#include "level.h"
#include "thing.h"
#include "command.h"
#include "time_util.h"
#include "wid_game_map.h"
#include "map.h"
#include "timer.h"
#include "sound.h"
#include "file.h"
#include "map.h"
#include "player.h"

static uint8_t level_init_done;
static uint8_t level_init_done;
static void level_set_walls(levelp level);
static void level_update_incremental(levelp level);

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

    oldptr(level);

    level = 0;
}

void level_update_slow (levelp level)
{
    map_fixup(level);

    level_set_walls(level);

    /*
     * One time generate of expensive wander map
     */
#if 0
    dmap_generate_map_wander(level);
#endif

    level_update_incremental(level);
}

static void level_update_incremental (levelp level)
{
    level_set_walls(level);

    /*
     * Regenerate player dmaps as things like doors may have been opened.
     */
#if 0
    dmap_generate(level, true /* force */);
#endif

    map_fixup(level);
}

widp level_get_map (levelp level)
{
    return (game.wid_grid);
}

static void level_set_walls (levelp level)
{
    int32_t x;
    int32_t y;
    int32_t z;

    widp w = game.wid_grid;
    if (!w) {
        return;
    }

    if (!level) {
        return;
    }

    int i;
    for (i = 0; i < DMAP_MAP_MAX; i++) {
        memset(level->dmap[i].walls, '+',
            sizeof(level->dmap[i].walls));
        memset(level->dmap[i].walls, '+',
            sizeof(level->dmap[i].walls));
    }

    memset(level->walls.walls, '+',
           sizeof(level->walls.walls));
    memset(level->doors.walls, '+',
           sizeof(level->doors.walls));

    for (z = MAP_DEPTH_WALL; z < MAP_DEPTH; z++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            for (y = 0; y < MAP_HEIGHT; y++) {

                tree_root **tree = 
                    w->grid->grid_of_trees[z] + (y * w->grid->width) + x;
                widgridnode *node;

                TREE_WALK_REVERSE_UNSAFE_INLINE(
                                    *tree, 
                                    node,
                                    tree_prev_tree_wid_compare_func) {

                    widp w = node->wid;

                    thingp t = wid_get_thing(w);
                    if (!t) {
                        continue;
                    }

                    /*
                     * Identify obstacles.
                     */
                    if (thing_is_door(t)) {
                        level->dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS].walls[x][y] = '+';
                        continue;
                    }

                    /*
                     * Same as above, but treat doors as passable.
                     */
                    if (thing_is_door(t)) {
                        level->dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE].walls[x][y] = ' ';
                        continue;
                    }

                    char c = 0;

                    if (thing_is_wall(t)) {
                        if (!map_is_wall_at(level, x, y - 1, 0)) {
                            c = ' ';
                        }
                    }

                    if (c) {
                        level->dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS].walls[x][y] = c;
                        level->dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE].walls[x][y] = c;
                        level->doors.walls[x][y] = c;
                    }
                }
            }
        }
    }

#if 0
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            printf("%c", level->doors.walls[x][y]);
        }
        printf("\n");
    }
    printf("\n");
#endif
}

/*
 * Clean up the level. It's over! the exit was reached and a delay passed to 
 * warn the other player.
 */
levelp level_finished (levelp level, int keep_player)
{
    if (keep_player) {
        LEVEL_LOG(level, "Finish level but keep player");
    } else {
        LEVEL_LOG(level, "Finish level and kill player");
    }

    wid_detach_from_grid(game.wid_grid);
    wid_destroy_grid(game.wid_grid);
    wid_destroy(&game.wid_grid);

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

int level_tick (levelp level)
{
    if (!level) {
        return (false);
    }

    static uint32_t last_tick;
    if (!time_have_x_tenths_passed_since(LEVEL_TICK_DELAY_TENTHS, last_tick)) {
        return (true);
    }

    last_tick = time_get_time_ms();

    level_update_incremental(level);

    return (true);
}
