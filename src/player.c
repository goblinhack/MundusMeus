/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#include "main.h"
#include "player.h"
#include "time_util.h"

thingp player;

uint8_t player_init (void)
{
    return (true);
}

void player_fini (void)
{
    player = 0;
}
