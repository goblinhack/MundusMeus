/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file.
 */

#pragma once

#include "thing_template.h"
#include "thing.h"
#include "map.h"

#define DMAP_MAP_MAX 8

void set_game_video_pix_width(int width);
typedef struct {
    int8_t walls[MAP_WIDTH][MAP_HEIGHT];
} level_walls;

typedef struct level_map_tile_ {
    tpp tp;
} level_map_tile;

typedef struct level_t_ {
    uint8_t is_being_destroyed:1;
} level_t;

uint8_t level_init(void);
void level_fini(void);

levelp level_reinit(uint32_t,
                 int is_editor,
                 int is_map_editor);
void level_destroy(levelp *, uint8_t keep_player);
levelp level_finished(levelp, int keep_player);
void level_finished_all(void);
int level_tick(levelp);
void level_update_now(levelp);
levelp level_load_new(int level_no);
levelp level_load(uint32_t,
                  int is_editor,
                  int is_map_editor);
void level_pause(levelp);
void level_resume(levelp);
levelp level_load_random(uint32_t level,
                         int is_editor,
                         int is_map_editor);
void level_update_slow(levelp);
const char *level_get_title(levelp);
void level_set_title(levelp, const char *val);
const char *level_get_destroy_reason(levelp);
void level_set_destroy_reason(levelp, const char *val);
widp level_get_map(levelp);
void level_set_map(levelp, widp val);

uint32_t level_get_level_no(levelp);
void level_set_level_no(levelp, uint32_t val);

uint32_t level_get_seed(levelp);
void level_set_seed(levelp, uint32_t val);

const char *level_get_logname(levelp);
uint32_t level_get_tick_started(levelp);
void level_set_tick_started(levelp, uint32_t val);
uint8_t level_is_editor(levelp);
void level_set_is_editor(levelp, uint8_t val);
uint8_t level_is_map_editor(levelp);
void level_set_is_map_editor(levelp, uint8_t val);
void level_set_exit_has_been_reached(levelp, uint8_t val);
uint8_t level_exit_has_been_reached(levelp);
void level_set_needs_fixup(levelp, uint8_t val);
uint8_t level_needs_fixup(levelp);
void level_set_is_ready_to_be_destroyed(levelp, uint8_t val);
uint8_t level_is_ready_to_be_destroyed(levelp);
void level_set_is_ready_to_fade_out(levelp, uint8_t val);
uint8_t level_is_ready_to_fade_out(levelp);

void level_set_has_shop(levelp, uint8_t val);
uint8_t level_has_shop(levelp);
