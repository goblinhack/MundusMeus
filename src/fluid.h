/*
 * Copyright (C) 2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

void fluid_init(levelp level);
void fluid_tick(levelp level);
void fluid_update(levelp level);
void fluid_render(widp, int, int, int, int);
void fluid_add_droplets(levelp level);
void fluid_remove_water_radius(levelp level, int x, int y, int radius);
