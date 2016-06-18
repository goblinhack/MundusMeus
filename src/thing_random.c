/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "thing.h"

tpp random_wall (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (tp_is_corridor_wall(tp)) {
            continue;
        }

        if (tp_is_wall(tp)) {
            /*
             * Exclude lit walls as we run out of light sources
             */
            if (tp_is_light_source(tp)) {
                continue;
            }

            return (tp);
        }
    }
}

tpp random_corridor_wall (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (tp_is_corridor_wall(tp)) {
            return (tp);
        }
    }
}

tpp random_door (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (tp_is_door(tp)) {
            return (tp);
        }
    }
}

tpp random_obstacle (void)
{
#if 0
    int any = false;
    int loop = 0;

    for (;;) {

        if (loop++ > 1000) {
            any = true;
        }

        uint32_t id = myrand() % TP_MAX_ID;

        if (id == THING_BOULDER2) {
            continue;
        }

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_obstacle(tp)) {
            if (any) {
                return (tp);
            }

            uint32_t r =  myrand() % 10000;
            if (r < tp_get_d10000_chance_of_appearing(tp)) {
                return (tp);
            }
        }
    }
#endif
    return (0);
}

tpp random_spikes (void)
{
    int any = false;
    int loop = 0;

    for (;;) {

        if (loop++ > 1000) {
            any = true;
        }

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_spikes(tp)) {
            if (any) {
                return (tp);
            }

            int r =  myrand() % 10000;
            if (r < tp_get_d10000_chance_of_appearing(tp)) {
                return (tp);
            }
        }
    }
}

tpp random_floor (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (tp_is_shop_floor(tp)) {
            continue;
        }

        if (tp_is_light_source(tp)) {
            continue;
        }

        if (tp_is_corridor(tp)) {
            continue;
        }

        if (tp_is_dungeon_floor(tp)) {
            return (tp);
        }
    }
}

tpp random_corridor (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (tp_is_shop_floor(tp)) {
            continue;
        }

        if (tp_is_light_source(tp)) {
            continue;
        }

        if (tp_is_corridor(tp)) {
            return (tp);
        }
    }
}

tpp random_player (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (tp_is_player(tp)) {
            return (tp);
        }
    }
}

tpp random_exit (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (tp_is_exit(tp)) {
            return (tp);
        }
    }
}

tpp random_entrance (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (tp_is_entrance(tp)) {
            return (tp);
        }
    }
}

tpp random_food (void)
{
    int loop = 0;
    int any = false;

    for (;;) {

        if (loop++ > 1000) {
            any = true;
        }

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (tp_is_food(tp)) {
            if (any) {
                return (tp);
            }

            int r =  myrand() % 10000;
            if (r < tp_get_d10000_chance_of_appearing(tp)) {
                return (tp);
            }
        }
    }
}

tpp random_treasure (int shop_floor)
{
    int loop = 0;
    int any = false;

    for (;;) {

        if (loop++ > 1000) {
            any = true;
        }

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        /*
         * Valid for shops?
         */
        if (shop_floor && !tp_get_cost(tp)) {
            continue;
        }

        if (tp_is_treasure(tp)) {
            if (any) {
                return (tp);
            }

            int r =  myrand() % 10000;
            if (r < tp_get_d10000_chance_of_appearing(tp)) {
                return (tp);
            }
        }
    }
}

tpp random_weapon (int shop_floor)
{
    int loop = 0;
    int any = false;

    for (;;) {

        if (loop++ > 1000) {
            any = true;
        }

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        /*
         * Valid for shops?
         */
        if (shop_floor && !tp_get_cost(tp)) {
            continue;
        }

        if (tp_is_weapon(tp)) {
            if (any) {
                return (tp);
            }

            int r =  myrand() % 10000;
            if (r < tp_get_d10000_chance_of_appearing(tp)) {
                return (tp);
            }
        }
    }
}

tpp random_rock (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (!tp_is_rock(tp)) {
            continue;
        }

        if (tp_is_light_source(tp)) {
            continue;
        }

        int r = myrand() % 10000;
        if (r < tp_get_d10000_chance_of_appearing(tp)) {
            return (tp);
        }
    }
}

tpp random_smallrock (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (!tp_is_smallrock(tp)) {
            continue;
        }

        int r = myrand() % 10000;
        if (r < tp_get_d10000_chance_of_appearing(tp)) {
            return (tp);
        }
    }
}

tpp random_lava (void)
{
    for (;;) {

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (tp_is_lava(tp)) {
            return (tp);
        }
    }
}

tpp random_monst (int depth)
{
    int loop = 0;
    int any = false;

    for (;;) {

        if (loop++ > 1000) {
            any = true;
        }

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (!tp_is_monst(tp)) {
            continue;
        }

        if (tp_is_trap(tp)) {
            continue;
        }

        /*
         * Unique? like death?
         */
        if (!tp_get_d10000_chance_of_appearing(tp)) {
            continue;
        }

        if (any) {
            return (tp);
        }

        if (depth < tp_get_min_appear_depth(tp)) {
            continue;
        }

        if (depth > tp_get_max_appear_depth(tp)) {
            continue;
        }

        int r = myrand() % 10000;
        if (r < (tp_get_d10000_chance_of_appearing(tp) + depth)) {
            LOG("Level depth %d, adding monster, %s", depth, tp_name(tp));
            return (tp);
        }
    }
}

tpp random_trap (int depth)
{
    int loop = 0;
    int any = false;

    for (;;) {

        if (loop++ > 1000) {
            any = true;
        }

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (!tp_is_trap(tp)) {
            continue;
        }

        if (any) {
            return (tp);
        }

        int r = myrand() % 10000;
        if (r < (tp_get_d10000_chance_of_appearing(tp) + depth)) {
            return (tp);
        }
    }
}

tpp random_mob (int depth)
{
    int loop = 0;
    int any = false;

    for (;;) {

        if (loop++ > 1000) {
            any = true;
        }

        uint32_t id = myrand() % TP_MAX_ID;

        tpp tp = id_to_tp(id);
        if (!tp) {
            continue;
        }

        if (tp_is_internal(tp)) {
            continue;
        }

        if (!tp_is_mob_spawner(tp)) {
            continue;
        }

        if (any) {
            return (tp);
        }

        int r =  myrand() % 10000;
        if (r < (tp_get_d10000_chance_of_appearing(tp) + depth)) {
            return (tp);
        }
    }
}
