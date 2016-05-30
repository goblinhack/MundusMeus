/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include <SDL.h>
#include <math.h>

#include "main.h"
#include "thing.h"
#include "map.h"
#include "level.h"
#include "time_util.h"

void shop_enter(levelp, thingp t, thingp floor);
void shop_leave(levelp, thingp t, thingp floor);
void shop_collect_message(levelp, thingp t, thingp item);
void shop_pay_for_items(levelp, thingp t);
void shop_deposit_message(levelp, thingp t, thingp item);
void shop_steal_message(levelp, thingp t);
void shop_attack_message(levelp, thingp t);
void shop_fixup(levelp);
void thing_shop_item_tick(levelp, thingp t);
void shop_break_message(levelp, thingp t, thingp shopkeeper);
void shop_whodunnit_break_message(levelp, thingp t, thingp shopkeeper);
int shop_inside(levelp, thingp t);
void shop_on_level(double x, double y);
