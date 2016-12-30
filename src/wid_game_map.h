/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

void wid_game_map_init(void);
void wid_game_map_fini(void);

void wid_game_map_grid_create(void);
void wid_game_map_wid_create(void);
void wid_game_map_scroll_adjust(levelp levle, uint8_t adjust);
uint8_t wid_game_map_player_move(levelp level);

widp wid_game_map_replace_tile(double x, double y, thingp t);
void wid_game_map_scroll_chunk(int dx, int dy);
