/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file.
 */

#pragma once

tpp map_is_monst_at(levelp, int32_t x, int32_t y);
tpp map_is_player_at(levelp, int32_t x, int32_t y);
tpp map_is_wall_at(levelp, int32_t x, int32_t y);
tpp map_is_cwall_at(levelp, int32_t x, int32_t y);
tpp map_is_hwall_at(levelp, int32_t x, int32_t y);
tpp map_is_water_at(levelp, int32_t x, int32_t y);
tpp map_is_lava_at(levelp, int32_t x, int32_t y);
tpp map_is_floor_at(levelp, int32_t x, int32_t y);
tpp map_is_corridor_at(levelp, int32_t x, int32_t y);
tpp map_is_dusty_at(levelp, int32_t x, int32_t y);
tpp map_is_door_at(levelp, int32_t x, int32_t y);
tpp map_is_chasm_smoke_at(levelp, int32_t x, int32_t y);
tpp map_is_dirt_deco_at(levelp level, int32_t x, int32_t y);
tpp map_is_dirt_snow_deco_at(levelp level, int32_t x, int32_t y);
tpp map_is_grass_deco_at(levelp level, int32_t x, int32_t y);
tpp map_is_grass_snow_deco_at(levelp level, int32_t x, int32_t y);
tpp map_is_carpet_deco_at(levelp level, int32_t x, int32_t y);
tpp map_is_gravel_deco_at(levelp level, int32_t x, int32_t y);
tpp map_is_gravel_snow_deco_at(levelp level, int32_t x, int32_t y);
tpp map_is_sand_deco_at(levelp level, int32_t x, int32_t y);
tpp map_is_sand_snow_deco_at(levelp level, int32_t x, int32_t y);
tpp map_is_snow_deco_at(levelp level, int32_t x, int32_t y);
tpp map_is_dirt_at(levelp level, int32_t x, int32_t y);
tpp map_is_dirt_snow_at(levelp level, int32_t x, int32_t y);
tpp map_is_grass_at(levelp level, int32_t x, int32_t y);
tpp map_is_grass_snow_at(levelp level, int32_t x, int32_t y);
tpp map_is_carpet_at(levelp level, int32_t x, int32_t y);
tpp map_is_gravel_at(levelp level, int32_t x, int32_t y);
tpp map_is_gravel_snow_at(levelp level, int32_t x, int32_t y);
tpp map_is_sand_at(levelp level, int32_t x, int32_t y);
tpp map_is_sand_snow_at(levelp level, int32_t x, int32_t y);
tpp map_is_snow_at(levelp level, int32_t x, int32_t y);
tpp map_is_focus_at(levelp level, int32_t x, int32_t y);

void map_fixup(levelp);
void map_time_step(levelp);
void map_cleanup(levelp);
void map_add_selection_buttons(levelp level);
void map_remove_selection_buttons(levelp level);
void map_clear_selection_buttons(levelp level);
void map_set_selection_buttons(levelp level, int x, int y, const char *what);

#include "tree.h"

typedef struct tree_thing_node_ {
    tree_key_pointer tree;
} tree_thing_node;

typedef uint8_t (*map_is_at_callback)(tpp);

uint8_t map_count_x_at(levelp level,
                       int32_t x, int32_t y,
                       map_is_at_callback callback);

thingp map_is_tp_at(levelp level, int32_t x, int32_t y, tpp tp);

#include "enum.h"

#define Z_DEPTH_ENUMS(list_macro)                             \
    list_macro(Z_DEPTH_BURIED,            "buried"),          \
    list_macro(Z_DEPTH_LAVA,              "lava"),            \
    list_macro(Z_DEPTH_ROCK,              "rock"),            \
    list_macro(Z_DEPTH_SAND,              "sand"),            \
    list_macro(Z_DEPTH_DIRT,              "dirt"),            \
    list_macro(Z_DEPTH_LAWN,              "lawn"),            \
    list_macro(Z_DEPTH_GRAVEL,            "gravel"),          \
    list_macro(Z_DEPTH_GRASS,             "grass"),           \
    list_macro(Z_DEPTH_SNOW,              "snow"),            \
    list_macro(Z_DEPTH_ICE,               "ice"),             \
    list_macro(Z_DEPTH_ROAD,              "road"),            \
    list_macro(Z_DEPTH_BRIDGE,            "bridge"),          \
    list_macro(Z_DEPTH_FLOOR,             "floor"),           \
    /* level start */                                         \
    list_macro(Z_DEPTH_FLOOR2,            "floor2"),          \
    /* focus */                                               \
    list_macro(Z_DEPTH_WALL,              "wall"),            \
    list_macro(Z_DEPTH_TREASURE,          "treasure"),        \
    list_macro(Z_DEPTH_PLAYER,            "player"),          \
    list_macro(Z_DEPTH_EXPLOSION,         "explosion"),       \
    list_macro(Z_DEPTH_ACTIONS,           "actions"),         \

#define Z_DEPTH 19

ENUM_DEF_H(Z_DEPTH_ENUMS, z_depth)

static inline int map_ok (int x, int y)
{
    if ((x >= MAP_WIDTH) || (x < 0) || (y >= MAP_HEIGHT) || (y < 0)) {
        return (false);
    }

    return (true);
}

thingp map_thing_is_x_at(levelp level,
                         int32_t x, int32_t y,
                         map_is_at_callback callback);

tpp map_is_x_at(levelp level,
                int32_t x, int32_t y,
                map_is_at_callback callback);
