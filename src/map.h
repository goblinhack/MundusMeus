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
tpp map_is_door_at(levelp, int32_t x, int32_t y);

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

#define MAP_DEPTH_ENUMS(list_macro)                             \
    list_macro(MAP_DEPTH_LAVA = 0,          "lava"),            \
    list_macro(MAP_DEPTH_FLOOR,             "floor"),           \
    list_macro(MAP_DEPTH_OBJ,               "obj"),             \
    list_macro(MAP_DEPTH_MONST,             "monst"),           \
    list_macro(MAP_DEPTH_PLAYER,            "player"),          \
    list_macro(MAP_DEPTH_WALL,              "wall"),            \
    list_macro(MAP_DEPTH_EXPLOSION,         "explosion"),       \
    list_macro(MAP_DEPTH_ACTIONS,           "actions"),         \

#define MAP_DEPTH 8

ENUM_DEF_H(MAP_DEPTH_ENUMS, map_depth)

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
