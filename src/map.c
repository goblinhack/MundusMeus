/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "wid_game_map.h"
#include "cloud.h"
#include "string_util.h"

#ifdef GORY_DEBUG
FILE *fp = 0;
#endif

thingp map_is_tp_at (levelp level, int32_t x, int32_t y, tpp tp)
{
    widp grid_wid;
    widp w;

    grid_wid = game.wid_grid;
    if (!grid_wid) {
        DIE("no grid wid");
    }

    /*
     * Look for a floor tile where we can place stuff.
     */
    uint8_t z;

    for (z = 0; z < Z_DEPTH; z++) {
        w = wid_grid_find_first(grid_wid, x, y, z);
        while (w) {
            thingp thing_it = wid_get_thing(w);

            if (!thing_it) {
                w = wid_grid_find_next(grid_wid, w, x, y, z);
                continue;
            }

            if (tp == wid_get_thing_template(w)) {
                return (thing_it);
            }

            w = wid_grid_find_next(grid_wid, w, x, y, z);
        }
    }

    return (0);
}

tpp map_is_x_at (levelp level,
                 int32_t x, int32_t y,
                 map_is_at_callback callback)
{
    widp grid_wid;
    widp w;

    grid_wid = game.wid_grid;
    if (!grid_wid) {
        DIE("no grid wid");
    }

    uint8_t z;

    for (z = 0; z < Z_DEPTH; z++) {
        w = wid_grid_find_first(grid_wid, x, y, z);
        while (w) {
            thingp thing_it = wid_get_thing(w);

            if (!thing_it) {
                w = wid_grid_find_next(grid_wid, w, x, y, z);
                continue;
            }

            if ((*callback)(thing_it->tp)) {
                return (thing_it->tp);
            }

            w = wid_grid_find_next(grid_wid, w, x, y, z);
        }
    }

    return (0);
}

static tpp map_is_x_at_z_depth (levelp level,
				int32_t x, int32_t y,
				map_is_at_callback callback,
				uint8_t z)
{
    widp grid_wid;
    widp w;

    grid_wid = game.wid_grid;
    if (!grid_wid) {
        DIE("no grid wid");
    }

	w = wid_grid_find_first(grid_wid, x, y, z);
	while (w) {
		thingp thing_it = wid_get_thing(w);

		if (!thing_it) {
			w = wid_grid_find_next(grid_wid, w, x, y, z);
			continue;
		}

		if ((*callback)(thing_it->tp)) {
			return (thing_it->tp);
		}

		w = wid_grid_find_next(grid_wid, w, x, y, z);
	}

	return (0);
}

uint8_t map_count_x_at (levelp level,
                        int32_t x, int32_t y,
                        map_is_at_callback callback)
{
    tpp tp;
    widp grid_wid;
    widp w;
    uint32_t count;

    count = 0;

    grid_wid = game.wid_grid;
    if (!grid_wid) {
        DIE("no grid wid");
    }

    /*
     * Look for a floor tile where we can place stuff.
     */
    uint8_t z;

    for (z = 0; z < Z_DEPTH; z++) {
        w = wid_grid_find_first(grid_wid, x, y, z);
        while (w) {
            tp = wid_get_thing_template(w);
            if (tp) {
                if ((*callback)(tp)) {
                    count++;
                }
            }

            w = wid_grid_find_next(grid_wid, w, x, y, z);
        }
    }

    return (count);
}

tpp map_is_player_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_player));
}

tpp map_is_monst_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_monst));
}

tpp map_is_door_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_door));
}

tpp map_is_chasm_smoke_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_chasm_smoke));
}

tpp map_is_wall_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_wall));
}

tpp map_is_water_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_water));
}

tpp map_is_lava_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_lava));
}

tpp map_is_floor_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_floor));
}

tpp map_is_corridor_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_corridor));
}

tpp map_is_dirt_deco_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_dirt_deco, Z_DEPTH_DIRT1));
}

tpp map_is_dirt_snow_deco_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_dirt_snow_deco, Z_DEPTH_DIRT1));
}

tpp map_is_grass_deco_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_grass_deco, Z_DEPTH_GRASS));
}

tpp map_is_grass_snow_deco_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_grass_snow_deco, Z_DEPTH_GRASS));
}

tpp map_is_gravel_deco_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_gravel_deco, Z_DEPTH_GRAVEL));
}

tpp map_is_sand_deco_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_sand_deco, Z_DEPTH_SAND));
}

tpp map_is_sand_snow_deco_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_sand_snow_deco, Z_DEPTH_SAND));
}

tpp map_is_snow_deco_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_snow_deco, Z_DEPTH_SNOW));
}

tpp map_is_dirt_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_dirt, Z_DEPTH_DIRT1));
}

tpp map_is_dirt_snow_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_dirt_snow, Z_DEPTH_DIRT1));
}

tpp map_is_grass_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_grass, Z_DEPTH_GRASS));
}

tpp map_is_grass_snow_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_grass_snow, Z_DEPTH_GRASS));
}

tpp map_is_gravel_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_gravel, Z_DEPTH_GRAVEL));
}

tpp map_is_sand_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_sand, Z_DEPTH_SAND));
}

tpp map_is_sand_snow_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_sand_snow, Z_DEPTH_SAND));
}

tpp map_is_snow_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at_z_depth(level, x, y, tp_is_snow, Z_DEPTH_SNOW));
}

tpp map_is_dusty_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_dusty));
}

tpp map_is_focus_at (levelp level, int32_t x, int32_t y)
{
    return (map_is_x_at(level, x, y, tp_is_focus));
}

thingp map_thing_is_x_at (levelp level,
                          int32_t x, int32_t y,
                          map_is_at_callback callback)
{
    tpp tp;
    widp grid_wid;
    widp w;

    grid_wid = game.wid_grid;
    if (!grid_wid) {
        DIE("no grid wid");
    }

    /*
     * Look for a floor tile where we can place stuff.
     */
    uint8_t z;

    for (z = 0; z < Z_DEPTH; z++) {
        w = wid_grid_find_first(grid_wid, x, y, z);
        while (w) {
            thingp thing_it = wid_get_thing(w);

            if (!thing_it) {
                w = wid_grid_find_next(grid_wid, w, x, y, z);
                continue;
            }

            tp = wid_get_thing_template(w);
            if (tp) {
                if ((*callback)(tp)) {
                    return (thing_it);
                }
            }

            w = wid_grid_find_next(grid_wid, w, x, y, z);
        }
    }

    return (0);
}

static void map_fixup_deco_remove (void)
{
    int s = 0;
    thingp t;

    TREE_WALK_UNSAFE(things, t) {
        if (t->is_deco) {
            if (s >= (int) ARRAY_SIZE(thing_scratch)) {
                ERR("Overflow on thing scratchpad");
                continue;
            }

            thing_scratch[s++] = t;
        }
    }

    while (s > 0) {
        thingp t = thing_scratch[--s];

        thing_destroyed_(t, "deco cleanup");
    }

    if (things_deco_total) {
        ERR("Failed to cleanup decos when cleaning level, %d", things_deco_total);
    }
}

static char tmp[SMALL_STRING_LEN_MAX];
static int count;

#define MAP_FIXUP_DECO(DECO)                                                            \
static void map_fixup_deco_ ## DECO (levelp level)                                      \
{                                                                                       \
    int x, y;                                                                           \
    int found = 0;                                                                      \
                                                                                        \
    uint8_t is_at[MAP_WIDTH][MAP_HEIGHT];                                               \
                                                                                        \
	for (x = 0; x < MAP_WIDTH; x++) {                                               \
            for (y = 0; y < MAP_HEIGHT; y++) {                                          \
                is_at[x][y] = (map_is_ ## DECO ## _at(level, x, y)) ? 1 : 0;            \
                found++;                                                                \
            }                                                                           \
	}                                                                               \
                                                                                        \
	if (!found) {                                                                   \
            return;                                                                     \
	}                                                                               \
                                                                                        \
	for (x = 1; x < MAP_WIDTH - 1; x++) {                                           \
            for (y = 1; y < MAP_HEIGHT - 1; y++) {                                      \
                                                                                        \
            thingp t;                                                                   \
                                                                                        \
            if (is_at[x][y]) {                                                          \
                continue;                                                               \
            }                                                                           \
                                                                                        \
            if (map_is_ ## DECO ## _deco_at(level, x, y)) {                             \
                continue;                                                               \
            }                                                                           \
                                                                                        \
            if (is_at[x-1][y]) {                                                        \
                itoa05(tmp, count++);                                                   \
                t = thing_new(tmp, -1 /* thing id */, #DECO "_deco");                   \
                things_deco_total++;                                                    \
                t->is_deco = true;                                                      \
                t->wid = wid_game_map_replace_tile(x, y, t);                            \
                wid_set_tilename(t->wid, #DECO "_right");                               \
            }                                                                           \
            if (is_at[x+1][y]) {                                                        \
                itoa05(tmp, count++);                                                   \
                t = thing_new(tmp, -1 /* thing id */, #DECO "_deco");                   \
                things_deco_total++;                                                    \
                t->is_deco = true;                                                      \
                t->wid = wid_game_map_replace_tile(x, y, t);                            \
                wid_set_tilename(t->wid, #DECO "_left");                                \
            }                                                                           \
            if (is_at[x][y-1]) {                                                        \
                itoa05(tmp, count++);                                                   \
                t = thing_new(tmp, -1 /* thing id */, #DECO "_deco");                   \
                things_deco_total++;                                                    \
                t->is_deco = true;                                                      \
                t->wid = wid_game_map_replace_tile(x, y, t);                            \
                wid_set_tilename(t->wid, #DECO "_bot");                                 \
            }                                               			        \
            if (is_at[x][y+1]) {                                                        \
                itoa05(tmp, count++);                                               	\
                t = thing_new(tmp, -1 /* thing id */, #DECO "_deco");                   \
                things_deco_total++;                                                    \
                t->is_deco = true;                                              	\
                t->wid = wid_game_map_replace_tile(x, y, t);                            \
                wid_set_tilename(t->wid, #DECO "_top");                                 \
            }                                               				\
            if (is_at[x-1][y-1]) {                                              	\
                itoa05(tmp, count++);                                               	\
                t = thing_new(tmp, -1 /* thing id */, #DECO "_deco");                   \
                things_deco_total++;                                                    \
                t->is_deco = true;                                              	\
                t->wid = wid_game_map_replace_tile(x, y, t);                            \
                wid_set_tilename(t->wid, #DECO "_br");                                  \
            }                                               				\
            if (is_at[x+1][y-1]) {                                              	\
                itoa05(tmp, count++);                                               	\
                t = thing_new(tmp, -1 /* thing id */, #DECO "_deco");                   \
                things_deco_total++;                                                   	\
                t->is_deco = true;                                              	\
                t->wid = wid_game_map_replace_tile(x, y, t);                            \
                wid_set_tilename(t->wid, #DECO "_bl");                                  \
            }                                               				\
            if (is_at[x-1][y+1]) {                                              	\
                itoa05(tmp, count++);                                               	\
                t = thing_new(tmp, -1 /* thing id */, #DECO "_deco");                   \
                things_deco_total++;                                                   	\
                t->is_deco = true;                                              	\
                t->wid = wid_game_map_replace_tile(x, y, t);                            \
                wid_set_tilename(t->wid, #DECO "_tr");                                  \
            }                                               				\
            if (is_at[x+1][y+1]) {                                              	\
                itoa05(tmp, count++);                                               	\
                t = thing_new(tmp, -1 /* thing id */, #DECO "_deco");                   \
                things_deco_total++;                                                   	\
                t->is_deco = true;                                              	\
                t->wid = wid_game_map_replace_tile(x, y, t);                            \
                wid_set_tilename(t->wid, #DECO "_tl");                                  \
            }                                               				\
        }                                               				\
    }                                               					\
} 					                                                \

MAP_FIXUP_DECO(grass)
MAP_FIXUP_DECO(snow)
MAP_FIXUP_DECO(sand)
MAP_FIXUP_DECO(dirt)
MAP_FIXUP_DECO(gravel)
MAP_FIXUP_DECO(grass_snow)
MAP_FIXUP_DECO(dirt_snow)
MAP_FIXUP_DECO(sand_snow)

/*
 * Add decorations. These things are never saved as part of the game.
 */
void map_fixup (levelp level)
{
    map_fixup_deco_remove();

    if (game.biome_set_is_dungeon) {
        return;
    }

    map_fixup_deco_grass(level);
    map_fixup_deco_snow(level);
    map_fixup_deco_sand(level);
    map_fixup_deco_dirt(level);
    map_fixup_deco_gravel(level);
    map_fixup_deco_grass_snow(level);
    map_fixup_deco_sand_snow(level);
    map_fixup_deco_dirt_snow(level);
}

/*
 * Remove decorations.
 */
void map_cleanup (levelp level)
{
    map_fixup_deco_remove();

    map_remove_selection_buttons(level);
}

void map_time_step (levelp level)
{
    int i;
    for (i = 0; i < 10; i++) {
        cloud_tick(false);
    }
}
