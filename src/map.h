/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file.
 */

#pragma once

tpp map_is_monst_at(levelp, int32_t x, int32_t y);
tpp map_is_player_at(levelp, int32_t x, int32_t y);
tpp map_is_wall_at(levelp, int32_t x, int32_t y);
tpp map_is_floor_at(levelp, int32_t x, int32_t y);
tpp map_is_corridor_at(levelp, int32_t x, int32_t y);
tpp map_is_dusty_at(levelp, int32_t x, int32_t y);
tpp map_is_door_at(levelp, int32_t x, int32_t y);
tpp map_is_chasm_smoke_at(levelp, int32_t x, int32_t y);

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
    list_macro(Z_DEPTH_STALACTITE,        "stalactite"),      \
    list_macro(Z_DEPTH_LAVA,              "lava"),            \
    list_macro(Z_DEPTH_FLOOR,             "floor"),           \
    list_macro(Z_DEPTH_WALL,              "wall"),            \
    list_macro(Z_DEPTH_MONST,             "monst"),           \
    list_macro(Z_DEPTH_PLAYER,            "player"),          \
    list_macro(Z_DEPTH_EXPLOSION,         "explosion"),       \
    list_macro(Z_DEPTH_ACTIONS,           "actions"),         \

#define Z_DEPTH 8

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

tree_rootp map_all_things_is_x_at(levelp level,
                                  int32_t x, int32_t y,
                                  map_is_at_callback callback);
tree_rootp map_all_things_is_x(levelp level,
                               map_is_at_callback callback);
