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
#include "wid_map.h"
#include "map.h"
#include "timer.h"
#include "sound.h"
#include "map_editor.h"
#include "map_jigsaw.h"
#include "file.h"
#include "map.h"
#include "thing_shop.h"
#include "fluid.h"
#include "player.h"
#include "thing_timer.h"

static uint8_t level_init_done;
static uint8_t level_init_done;
static void level_reset_player(levelp level);
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

static uint8_t level_side_one_time_init (void)
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

levelp level_reinit (uint32_t level_no, 
                     int is_editor, 
                     int is_map_editor)
{
    levelp level;

    level = &game.level;
    newptr(level, "level");
    LOG("Level allocated %p", level);

    level->is_valid = true;

    level->last_moved = 
    level->last_hit_obstacle =
    level->last_jumped =
    level->last_bomb =
    level->last_rope =
    level->last_torch = time_get_time_ms();

    if (!is_editor && !is_map_editor) {
        wid_game_map_grid_create(level);
    }

    if (level_no == TEST_LEVEL) {
        level->is_test_level = true;
    }

    level_set_level_no(level, level_no);
    level_set_is_editor(level, is_editor);
    level_set_is_map_editor(level, is_map_editor);

    LEVEL_LOG(level, "Reset fresh level %d", level_no);

    return (level);
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

    if (keep_player) {
        LEVEL_LOG(level, "Destroy level but keep player");
    } else {
        LEVEL_LOG(level, "Destroy level and kill player too");
    }

    /*
     * Kill all humans!
     */
    things_level_destroyed(level, keep_player);

    level->title[0] = 0;

    *plevel = 0;

    thing tmp;
    if (keep_player) {
        thing_leave_level(level, player);
        memcpy(&tmp, player, sizeof(tmp));
    }

    memset(level, 0, sizeof(*level));

    if (keep_player) {
        memcpy(player, &tmp, sizeof(tmp));
        LOG("Level freed %p but kept player %p", level, player);

        THING_LOG(player, "Player lives on beyond end of level");
    } else {
        LOG("Level freed %p", level);
    }

    oldptr(level);

    level = 0;
}

void level_pause (levelp level)
{
    level->is_paused = true;
}

void level_resume (levelp level)
{
    level->is_paused = false;

    player_wid_update(level);
}

void level_update_slow (levelp level)
{
    map_fixup(level);

    shop_fixup(level);

    level_set_walls(level);

    /*
     * One time generate of expensive wander map
     */
    dmap_generate_map_wander(level);

    level_update_incremental(level);
}

static int level_update_needed (levelp level)
{
    static uint64_t last_checksum;
    uint64_t checksum = 0;

    int tot = 0;
    int x, y, z;

    widp w = game.wid_grid;
    if (!w) {
        return (false);
    }

    verify(w);

    z = MAP_DEPTH_WALL;
    {
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

                    if (thing_is_wall(t)        ||
                        thing_is_rock(t)        ||
                        thing_is_door(t)) {

                        tot++;
                        checksum += x + y * MAP_WIDTH;
                        checksum += 1000;
                    }
                }
            }
        }
    }

    if (checksum != last_checksum) {
        last_checksum = checksum;
        return (true);
    }

    return (false);
}

static void level_update_incremental (levelp level)
{
    if (!level_update_needed(level)) {
        return;
    }

    level_set_walls(level);

    /*
     * Regenerate player dmaps as things like doors may have been opened.
     */
    dmap_generate(level, true /* force */);

    map_fixup(level);

    fluid_update(level);
}

levelp level_load_new (int level_no)
{
    levelp level;
    level = &game.level;

    LOG("New level, %d", level_no);

    char *tmp = dynprintf("%s%d", LEVELS_PATH, level_no);

    /*
     * Mostly random levels.
     */
    int fixed = false;

    if (game.level_no == TEST_LEVEL) {
        fixed = true;
    }

    int r = (myrand() % 100);
    if ((r < 20) && file_exists(tmp)) {
        fixed = true;
    }

    if (fixed) {
        LOG("Level %s exists, create fixed level", tmp);

        level = level_load(game.level_no,
                           false /* is_editor */,
                           false /* is_map_editor */);
    } else {
        LOG("Level %s does not exist, create random level", tmp);

        level = level_load_random(game.level_no,
                                  false /* is_editor */,
                                  false /* is_map_editor */);
    }

    myfree(tmp);

    LOG("New level, %d done loading", level_no);

    if (!level) {
        ERR("failed to load level %d", game.level_no);
        return (0);
    }

    level_update_slow(level);

    level->is_ready = true;

    return (level);
}

/*
 * Routines done for both random and static levels.
 */
static void level_loaded_common (levelp level)
{
    /*
     * Activate any triggers that have no activators.
     */
    if (!level->is_editor && !level->is_map_editor) {
        level_trigger_activate_default_triggers(level);

        fluid_init(level);
    }
}

levelp level_load (uint32_t level_no, 
                   int is_editor,
                   int is_map_editor)
{
    levelp level;

    level_side_one_time_init();

    level = level_reinit(level_no, is_editor, is_map_editor);

    level_set_tick_started(level, time_get_time_ms());

    char *dir_and_file;

    dir_and_file = dynprintf("%s%d", LEVELS_PATH, level_no);

    if (!file_exists(dir_and_file)) {
        myfree(dir_and_file);

        dir_and_file = dynprintf("data/levels/%d", level_no);
    }

    int pass;
    int max_pass;

    if (level_is_map_editor(level) || 
        level_is_editor(level)) {

        max_pass = 1;

        LEVEL_LOG(level, "Level %s: loading editor, max passes %d", 
                  dir_and_file, max_pass);

    } else {
        /*
         * Need 2 passes for levels being read into the game. First pass is 
         * just to learn where the action triggers are for spawned things and 
         * the like.
         *
         * Second pass creates the things.
         */
        max_pass = 2;

        LEVEL_LOG(level, "Level %s: loading, max passes %d", dir_and_file,
                  max_pass);
    }

    for (pass = 0; pass < max_pass; pass++) {
        LEVEL_LOG(level, "Level %s: loading pass %d", dir_and_file, pass);
    }

    game.level_is_being_loaded = 0;

    myfree(dir_and_file);

    if (!level_is_map_editor(level) &&
        !level_is_editor(level)) {
        level_update_slow(level);
    }

    if (!level_is_map_editor(level) &&
        !level_is_editor(level)) {
        level_reset_player(level);
    }

    level_loaded_common(level);

    return (level);
}

levelp level_load_random (uint32_t level_no, 
                          int is_editor,
                          int is_map_editor)
{
    levelp level;

    level_side_one_time_init();

    level = level_reinit(level_no, 
                         is_editor, 
                         is_map_editor);

    level_set_tick_started(level, time_get_time_ms());

    LEVEL_LOG(level, "Level generating");

    map_jigsaw_generate(level, 0, wid_game_map_replace_tile);

    level_update_slow(level);

    level_reset_player(level);

    level_loaded_common(level);

    return (level);
}

const char *level_get_title (levelp level)
{
    return (level->title);
}

void level_set_title (levelp level, const char *val)
{
    strncpy(level->title, val, sizeof(level->title));
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
                        if (!map_is_wall_at(level, x, y - 1)) {
                            c = ' ';
                        }
                    }

                    if (thing_is_ladder(t)) {
                        c = ' ';
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
 * New level; update player.
 */
void level_reset_player (levelp level)
{
    thingp t;

    /*
     * Rewield weapons at start.
     */
    LEVEL_LOG(level, "Reset player at start of level");

    FOR_ALL_THINGS(level, t) {
        if (thing_is_player(t)) {
            THING_LOG(t, "Reset player thing at start of level");

            tpp weapon = thing_weapon(t);
            if (weapon) {
                THING_LOG(t, "Rewield weapon at start of level");

                thing_wield(level, t, weapon);
            }

            /*
             * Need this else we are in darkness at level start
             */
            thing_torch_update_count(level, t, true);
        }
    }
    FOR_ALL_THINGS_END
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

    if (!level->is_valid) {
        LEVEL_LOG(level, "Level is no longer valid, do not free");
        return (0);
    }

    if (level->is_test_level) {
        LEVEL_LOG(level, "Test level finished");
        level_destroy(&level, false /* keep player */);
        wid_game_map_go_back_to_editor();
        return (0);
    }

    thingp t;

    /*
     * Force the death of all things on the level.
     */
    FOR_ALL_THINGS(level, t) {
        if (!thing_is_player(t)) {
            thing_leave_level(level, t);
            thing_set_is_dead(t, true);
        }
    }
    FOR_ALL_THINGS_END

    wid_detach_from_grid(game.wid_grid);
    wid_destroy_grid(game.wid_grid);
    wid_destroy(&game.wid_grid);

    int next_level = level->level_no + 1;

    level_destroy(&level, keep_player);

    /*
     * LEVEL CAN BE NULL HERE
     */

    if (game.game_over) {
        return (0);
    }

    if (!keep_player) {
        return (0);
    }

    /*
     * Create the new level, random or otherwise.
     */
    level = level_load_new(next_level);

    LEVEL_LOG(level, "Move player to new level");

    /*
     * Move player to the new level.
     */
    FOR_ALL_THINGS(level, t) {
        if (!thing_is_player(t)) {
            continue;
        }

        thing_map_remove(level, t);

        wid_game_map_replace_tile(
                level,
                0, 0,
                t,
                thing_tp(t),
                0 /* tpp_data */);

        thing_join_level(level, t);

        level_reset_player(level);
    }
    FOR_ALL_THINGS_END

    level_update_slow(level);

    sound_play_level_end();

    /*
     * To allow the player to be centered in the new level if it is a
     * different size.
     */
    if (game.wid_grid &&
        game.wid_grid->grid) {
        game.wid_grid->grid->bounds_locked = 0;
    }

    /*
     * Fluid code needs the level pointer.
     */
    game.wid_grid->level = level;

    verify(player);

    thing_move(level, player, player->x, player->y, false, false, false, false, false);

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

    thing_timers_tick(level);

    last_tick = time_get_time_ms();

    level_update_incremental(level);

    return (true);
}

uint32_t level_get_level_no (levelp level)
{
    return (level->level_no);
}

void level_set_level_no (levelp level, uint32_t val)
{
    level->level_no = val;
}

uint32_t level_get_seed (levelp level)
{
    return (level->seed);
}

void level_set_seed (levelp level, uint32_t val)
{
    level->seed = val;
}

uint32_t level_get_tick_started (levelp level)
{
    return (level->tick_started);
}

void level_set_tick_started (levelp level, uint32_t val)
{
    level->tick_started = val;
}

uint8_t level_is_editor (levelp level)
{
    return (level->is_editor);
}

void level_set_is_editor (levelp level, uint8_t val)
{
    level->is_editor = val;
}

uint8_t level_is_map_editor (levelp level)
{

    return (level->is_map_editor);
}

void level_set_is_map_editor (levelp level, uint8_t val)
{

    level->is_map_editor = val;
}

uint8_t level_has_shop (levelp level)
{

    return (level->has_shop > 0);
}

void level_set_has_shop (levelp level, uint8_t val)
{

    if (val) {
        level->has_shop++;
    } else {
        if (level->has_shop) {
            level->has_shop--;
        }
    }
}

static uint32_t level_count_is_x (levelp level, map_is_at_callback callback)
{
    uint32_t count;
    int32_t x;
    int32_t y;

    count = 0;

    for (x = 0; x < MAP_WIDTH; x++) {
        for (y = 0; y < MAP_HEIGHT; y++) {
            count += map_count_x_at(level, x, y, callback);
        }
    }

    return (count);
}

uint32_t level_count_is_exit (levelp level)
{
    return (level_count_is_x(level, tp_is_exit));
}

static thingp this_door[MAP_WIDTH][MAP_HEIGHT];

static void door_flood (levelp level, int32_t x, int32_t y)
{
    if (this_door[x][y]) {
        return;
    }

    if (!(this_door[x][y] = map_thing_is_door_at(level, x, y))) {
        this_door[x][y] = (thingp) (void*)-1;
        return;
    }

    door_flood(level, x-1, y);
    door_flood(level, x+1, y);
    door_flood(level, x, y-1);
    door_flood(level, x, y+1);
}

void level_open_door (levelp level, int32_t ix, int32_t iy)
{
    int32_t x;
    int32_t y;

    memset(this_door, 0, sizeof(this_door));

    door_flood(level, ix, iy);

    for (x = 0; x < MAP_WIDTH; x++) {
        for (y = 0; y < MAP_HEIGHT; y++) {
            if (!this_door[x][y]) {
                continue;
            }

            if (this_door[x][y] == (void*)-1) {
                continue;
            }

            thing_dead(level, this_door[x][y], 0 /* killer */, "open");
        }
    }

    level_update_incremental(level);

    MESG(SOUND, "door");
}
