/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

void wid_game_map_init(void);
void wid_game_map_fini(void);

void wid_game_map_grid_create(levelp);
void wid_game_map_wid_create(void);
void wid_game_map_scroll_adjust(levelp levle, uint8_t adjust);
void wid_game_map_go_back_to_editor(void);
uint8_t wid_game_map_player_move(levelp level);

extern widp
wid_game_map_replace_tile(levelp,
                          double x, double y,
                          thingp t,
                          tpp tp,
                          tpp_data data);

