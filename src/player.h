/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

uint8_t player_init(void);
void player_fini(void);

uint8_t player_move(levelp level);
uint8_t player_key(widp w, const SDL_KEYSYM *key);
uint8_t player_joy(widp w, int x, int y);
void player_wid_update(levelp);
void player_wid_destroy(levelp);

#endif
