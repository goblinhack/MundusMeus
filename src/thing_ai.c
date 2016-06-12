/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#define __STDC_LIMIT_MACROS
#include "main.h"
#include "thing.h"
#include "math_util.h"
#include "level.h"
#include "time_util.h"
#include "wid_game_map.h"

static FILE *fp;
static const int8_t is_a_wall = 63;
static const int8_t not_preferred = 62;

/*
 * Final dmaps
 */
static level_walls dmap[DMAP_MAP_MAX];
static level_walls dmap_scratchpad[DMAP_MAP_MAX];

static int update_player_dmap;
static double move_delta = 0.05;

/*
 * Djkstra maps so we can quickly tell the next hop.
 *
 * This is for every single map position, how to get there.
 */
static level_walls dmap_map_wander[MAP_WIDTH][MAP_HEIGHT];

/*
 * Non straight line distance, avoiding walls.
 *
 * TAKE CARE: takes some seconds for all points to be updated at level start.
 */
int dmap_distance_between_points (int target_x, int target_y, int source_x, int source_y)
{
    if ((target_x >= MAP_WIDTH) || (target_x < 0)) {
        return (-1);
    }

    if ((target_y >= MAP_HEIGHT) || (target_y < 0)) {
        return (-1);
    }

    if ((source_x >= MAP_WIDTH) || (source_x < 0)) {
        return (-1);
    }

    if ((source_y >= MAP_HEIGHT) || (source_y < 0)) {
        return (-1);
    }

    int distance = dmap_map_wander[target_x][target_y].walls[source_x][source_y];

    if (distance >= not_preferred) {
        return (-1);
    }

    return (distance);
}

/*
 * Non straight line distance to player, avoiding walls.
 *
 * Is updated each player move to a new cell
 */
int dmap_distance_to_player (int source_x, int source_y)
{
    if ((source_x >= MAP_WIDTH) || (source_x < 0)) {
        return (-1);
    }

    if ((source_y >= MAP_HEIGHT) || (source_y < 0)) {
        return (-1);
    }

    int distance = (dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS].walls[source_x][source_y]);

    if (distance >= not_preferred) {
        /*
         * Try bordering spaces. This works for say a rock that is a blocking 
         * obstacle that we do not have a path too. One of the bordering cells
         * might be reachable.
         */
        if (source_x > 0) {
            int distance = (dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS].walls[source_x-1][source_y]);
            if (distance < not_preferred) {
                return (distance);
            }
        }

        if (source_y > 0) {
            int distance = (dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS].walls[source_x][source_y-1]);
            if (distance < not_preferred) {
                return (distance);
            }
        }

        if (source_x < MAP_WIDTH - 1) {
            int distance = (dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS].walls[source_x+1][source_y]);
            if (distance < not_preferred) {
                return (distance);
            }
        }

        if (source_y < MAP_HEIGHT - 1) {
            int distance = (dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS].walls[source_x][source_y+1]);
            if (distance < not_preferred) {
                return (distance);
            }
        }

        return (-1);
    }

    return (distance);
}

/*
 * Print the Dijkstra map scores shared by all things of the same type.
 */
static void dmap_print (levelp level, level_walls *dmap)
{
    int8_t x;
    int8_t y;

    if (!fp) {
        fp = fopen("map.txt", "w");
    }

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if (map_is_exit_at(level, x, y)) {
                fprintf(fp, " Ex ");
                continue;
            }
            if (map_is_door_at(level, x, y)) {
                fprintf(fp, " Do ");
                continue;
            }
            if (map_is_mob_spawner_at(level, x, y)) {
                fprintf(fp, " Gn ");
                continue;
            }
            if (map_is_food_at(level, x, y)) {
                fprintf(fp, " Fd ");
                continue;
            }

            if (!map_is_dungeon_floor_at(level, x, y)) {
                fprintf(fp, " -- ");
                continue;
            }

            fprintf(fp, "%4d", dmap->walls[x][y]);
        }

        fprintf(fp, "\n");
    }

    fprintf(fp, "\n");
}

/*
 * Print the Dijkstra map scores shared by all things of the same type.
 */
static void dmap_thing_print (thingp t, 
                              int8_t nexthop_x,
                              int8_t nexthop_y)
{
    int8_t x;
    int8_t y;

    if (!fp) {
        fp = fopen("map.txt", "w");
    }

    double tx;
    double ty;

    thing_real_to_map(t, &tx, &ty);

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if ((nexthop_x == x) && (nexthop_y == y)) {
                fprintf(fp, " Nh ");
            } else {
                if ((x == tx) && (y == ty)) {
                    fprintf(fp, " Mo ");
                } else {
                    fprintf(fp, "%4d", 
                            dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS].walls[x][y]);
                }
            }
        }

        fprintf(fp, "\n");
    }

    fprintf(fp, "\n");
}

/*
 * http://www.roguebasin.com/index.php?title=The_Incredible_Power_of_Dijkstra_Maps
 *
 * To get a Dijkstra map, you start with an integer array representing your
 * map, with some set of goal cells set to zero and all the rest set to a very
 * high number. 
 *
 * Iterate through the map's "floor" cells -- skip the impassable wall cells.
 * If any floor tile has a value that is at least 2 greater than its 
 * lowest-value floor neighbor, set it to be exactly 1 greater than its lowest 
 * value neighbor. Repeat until no changes are made. The resulting grid of 
 * numbers represents the number of steps that it will take to get from any 
 * given tile to the nearest goal. 
 */
static void dmap_process (level_walls *dmap, level_walls *dmap_final)
{
    uint8_t x;
    uint8_t y;
    int8_t a;
    int8_t b;
    int8_t c;
    int8_t d;
    int8_t *e;
    int8_t f;
    int8_t g;
    int8_t h;
    int8_t i;
    int8_t lowest;
    uint8_t changed;
    static uint8_t valid[MAP_WIDTH][MAP_HEIGHT];
    static uint8_t new_valid[MAP_WIDTH][MAP_HEIGHT];
    static uint8_t orig_valid[MAP_WIDTH][MAP_HEIGHT];

    memset(valid, 1, sizeof(valid));
    memset(orig_valid, 1, sizeof(valid));

    for (x = 1; x < MAP_WIDTH - 1; x++) {
        for (y = 1; y < MAP_HEIGHT - 1; y++) {
            e = &dmap->walls[x  ][y];
            if (*e != is_a_wall) {
                continue;
            }

            valid[x][y] = 0;
            orig_valid[x][y] = 0;
        }
    }

#if 0
    int count = 0;

    if (1) {
        for (x = 1; x < MAP_WIDTH - 1; x++) {
            for (y = 1; y < MAP_HEIGHT - 1; y++) {
                e = &dmap->walls[x  ][y];
                if (*e == is_a_wall) {
                    printf("  XX");
                    continue;
                }

                printf("%4d", *e);
            }
            printf("\n");
        }
        printf("\n");
    }
#endif

    do {
        changed = false;
        memset(new_valid, 0, sizeof(new_valid));

#if 0
        printf("run %d %d %d\n", count, x, y);
        count++;
#endif
        for (x = 1; x < MAP_WIDTH - 1; x++) {
            for (y = 1; y < MAP_HEIGHT - 1; y++) {
                if (!valid[x][y]) {
#if 0
printf(" ");
#endif
                    continue;
                }

                if (!orig_valid[x][y]) {
#if 0
printf("w");
#endif
                    continue;
                }

                e = &dmap->walls[x  ][y];

                a =  dmap->walls[x-1][y-1] * 2;
                b =  dmap->walls[x  ][y-1];
                c =  dmap->walls[x+1][y-1] * 2;

                d =  dmap->walls[x-1][y];
                f =  dmap->walls[x+1][y];
                 
                g =  dmap->walls[x-1][y+1] * 2;
                h =  dmap->walls[x  ][y+1];
                i =  dmap->walls[x+1][y+1] * 2;

                if (a < b) {
                    lowest = a;
                } else {
                    lowest = b;
                }

                if (c < lowest) { lowest = c; }
                if (d < lowest) { lowest = d; }
                if (f < lowest) { lowest = f; }
                if (g < lowest) { lowest = g; }
                if (h < lowest) { lowest = h; }
                if (i < lowest) { lowest = i; }

                if (*e - lowest >= 2) {

                    new_valid[x-1][y-1] = 1;
                    new_valid[x-1][y  ] = 1;
                    new_valid[x-1][y+1] = 1;
                    new_valid[x  ][y-1] = 1;
                    new_valid[x  ][y  ] = 1;
                    new_valid[x  ][y+1] = 1;
                    new_valid[x+1][y-1] = 1;
                    new_valid[x+1][y  ] = 1;
                    new_valid[x+1][y+1] = 1;

                    *e = lowest + 1;
                    changed = true;
#if 0
printf("*");
                } else {
printf(".");
                }
            }
printf("\n");
#else
                }
            }
#endif
        }

        memcpy(valid, new_valid, sizeof(new_valid));

    } while (changed);

    /*
     * Only update the map when done so the monsters never see a map work
     * in progress.
     */
    memcpy(dmap_final, dmap, sizeof(level_walls));
#if 0
    if (1) {
        printf("final:\n");
        for (x = 1; x < MAP_WIDTH - 1; x++) {
            for (y = 1; y < MAP_HEIGHT - 1; y++) {
                e = &dmap->walls[x  ][y];
                if (*e == is_a_wall) {
                    printf("  XX");
                    continue;
                }

                printf("%4d", *e);
            }
            printf("\n");
        }
        printf("\n");
    }
#endif
}

/*
 * Generate goal points with a low value.
 */
static uint32_t dmap_generate_for_player_target_set_goals (uint8_t test, level_walls *dmap)
{
    uint32_t checksum = 0;
    thingp thing_it;
    double x;
    double y;

    thing_it = player;
    if (thing_it) {
        /*
         * Ignore dead player.
         */
        if (thing_is_dead(thing_it)) {
            return (0);
        }

        /*
         * Aim for center of tile.
         */
        thing_real_to_map(thing_it, &x, &y);

        if (!test) {
            dmap->walls[(int)x][(int)y] = 0;
        }

        checksum ^= (int)x | ((int)y << 16);
        checksum = checksum << 1;
    }

    return (checksum);
}

/*
 * Initialize the djkstra map with high values.
 */
static void dmap_init (level_walls *dmap, const level_walls *map)
{
    int8_t x;
    int8_t y;

    for (x = 0; x < MAP_WIDTH; x++) {
        for (y = 0; y < MAP_HEIGHT; y++) {
            if (map->walls[x][y] != ' ') {
                dmap->walls[x][y] = is_a_wall;
                continue;
            }

            dmap->walls[x][y] = not_preferred;
        }
    }
}

static int dmap_generate_for_player_target (levelp level, int force)
{
    if (!force) {
        static uint32_t dmap_checksum;

        /*
         * Only reprocess the djkstra map if something has changed on the map
         * We use a checksum of the goals to indicate this with reasonable 
         * certainty.
         */
        uint32_t checksum = dmap_generate_for_player_target_set_goals(true /* test */, 0);

        if (dmap_checksum == checksum) {
            return (0);
        }

        dmap_checksum = checksum;
    }

    update_player_dmap = true;

    dmap_init(&dmap_scratchpad[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE],
              &level->dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE]);
    dmap_generate_for_player_target_set_goals(false /* test */,
                   &dmap_scratchpad[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE]);

    dmap_init(&dmap_scratchpad[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS],
              &level->dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS]);
    dmap_generate_for_player_target_set_goals(false /* test */,
                   &dmap_scratchpad[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS]);

    return (1);
}

/*
 * Generate maps to allow things to wander to any location.
 */
void dmap_generate_map_wander (levelp level)
{
    static level_walls map_player_target_treat_doors_as_walls;

    memcpy(&dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS],
            &level->dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS],
            sizeof(dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS]));

    level_walls tmp;
    uint32_t x, y;

    for (x = 0; x < MAP_WIDTH; x++) {
        for (y = 0; y < MAP_HEIGHT; y++) {
            /*
             * If a wall then we can't get to it, period.
             */
            if (map_player_target_treat_doors_as_walls.walls[x][y] != ' ') {
                continue;
            }

            dmap_init(&tmp, &map_player_target_treat_doors_as_walls);

            /*
             * Set the goal.
             */
            tmp.walls[x][y] = 0;

            dmap_process(&tmp, &dmap_map_wander[x][y]);
        }
    }
}

/*
 * Generate a djkstra map for the thing.
 */
void dmap_generate (levelp level, int force)
{
    if (!player) {
        return;
    }

    int modified;
    
    modified = dmap_generate_for_player_target(level, force);

    /*
     * Now wake the dmap processor.
     */
    if (!modified) {
        return;
    }

    /*
     * Start with a clean dmap for each set of obstacles to consider.
     */
    if (update_player_dmap) {
        dmap_process(&dmap_scratchpad[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE],
                        &dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE]);
        dmap_process(&dmap_scratchpad[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS],
                        &dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS]);
        update_player_dmap = false;
    }

#ifdef ENABLE_MAP_DEBUG
    if (1)
#else
    if (/* DISABLES CODE */ (0))
#endif
    dmap_print(level, &dmap[DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE]);
}

static uint8_t thing_find_nexthop_dmap (thingp t, 
                                        level_walls *dmap,
                                        double *nexthop_x, 
                                        double *nexthop_y)
{
    double fx;
    double fy;
    int x;
    int y;

    thing_real_to_map(t, &fx, &fy);

    x = (int)fx;
    y = (int)fy;

    int8_t a;
    int8_t b;
    int8_t c;
    int8_t d;
    int8_t e;
    int8_t f;
    int8_t g;
    int8_t h;
    int8_t i;
    int lowest;

    a = dmap->walls[x-1][y-1];
    b = dmap->walls[x  ][y-1];
    c = dmap->walls[x+1][y-1];

    d = dmap->walls[x-1][y];
    e = dmap->walls[x  ][y];
    f = dmap->walls[x+1][y];
        
    g = dmap->walls[x-1][y+1];
    h = dmap->walls[x  ][y+1];
    i = dmap->walls[x+1][y+1];

    /*
     * Some things like archers want to stay a distance away
     * from the player.
     */
    tpp tp;
    tp = thing_tp(t);
    uint32_t min_distance = tp_get_approach_distance(tp);
    if (min_distance) {
        if ((uint32_t)a < min_distance) { a += not_preferred; }
        if ((uint32_t)b < min_distance) { b += not_preferred; }
        if ((uint32_t)c < min_distance) { c += not_preferred; }
        if ((uint32_t)d < min_distance) { d += not_preferred; }
        if ((uint32_t)e < min_distance) { e += not_preferred; }
        if ((uint32_t)f < min_distance) { f += not_preferred; }
        if ((uint32_t)g < min_distance) { g += not_preferred; }
        if ((uint32_t)h < min_distance) { h += not_preferred; }
        if ((uint32_t)i < min_distance) { i += not_preferred; }
    }

    lowest = min(a, min(b, min(c, min(d, min(e, min(f, min(g, min(h,i))))))));

    if (a != lowest) { a += not_preferred; }
    if (b != lowest) { b += not_preferred; }
    if (c != lowest) { c += not_preferred; }
    if (d != lowest) { d += not_preferred; }
    if (e != lowest) { e += not_preferred; }
    if (f != lowest) { f += not_preferred; }
    if (g != lowest) { g += not_preferred; }
    if (h != lowest) { h += not_preferred; }
    if (i != lowest) { i += not_preferred; }

    lowest = min(a, min(b, min(c, min(d, min(e, min(f, min(g, min(h,i))))))));

    int8_t dx = 0;
    int8_t dy = 0;

    if (a == lowest) { dx = -1; dy = -1; }
    else if (b == lowest) { dx =  0; dy = -1; }
    else if (c == lowest) { dx = +1; dy = -1; }
    else if (d == lowest) { dx = -1; dy =  0; }
    else if (e == lowest) { dx =  0; dy =  0; }
    else if (f == lowest) { dx = +1; dy =  0; }
    else if (g == lowest) { dx = -1; dy =  1; }
    else if (h == lowest) { dx =  0; dy =  1; }
    else if (i == lowest) { dx = +1; dy =  1; }
    else {
        return (false);
    }

    /*
     * Best hop is into something we can't move onto?
     */
    if (lowest == not_preferred) {
        return (false);
    }

    if (lowest == is_a_wall) {
        return (false);
    }

    if (lowest > (int) tp_get_vision_distance(tp)) {
        return (false);
    }

    /*
     * Success.
     */
    *nexthop_x = x + dx;
    *nexthop_y = y + dy;

#ifdef ENABLE_MAP_DEBUG
    if (1)
#else
    if (/* DISABLES CODE */ (0))
#endif
    dmap_thing_print(t, *nexthop_x, *nexthop_y);

    return (true);
}

static uint8_t thing_try_nexthop (levelp level,
                                  thingp t,
                                  const double *nexthop_x, 
                                  const double *nexthop_y)
{
    if (thing_hit_solid_obstacle(level, t,
                                 *nexthop_x, *nexthop_y)) {
        return (false);
    }

    if (!thing_hit_fall_obstacle(level, t,
                                 *nexthop_x, *nexthop_y + 1)) {
        /*
         * If the player is below us, try to drop down.
         */
        if (player && (player->y > t->y)) {
            if (thing_hit_fall_obstacle(level, t,
                                        *nexthop_x, *nexthop_y + 2)) {
                /*
                 * A small drop is ok
                 */
            } else {
                if (thing_hit_fall_obstacle(level, t,
                                            *nexthop_x, *nexthop_y + 3)) {
                    /*
                     * A small drop is ok
                     */
                } else {
                    /*
                     * Too large a drop.
                     */
                    return (false);
                }
            }
        } else {
            return (false);
        }
    }

    if (thing_move(level,
                   t, *nexthop_x, *nexthop_y,
                   *nexthop_y < t->y, 
                   *nexthop_y > t->y,
                   *nexthop_x < t->x, 
                   *nexthop_x > t->x, 
                   false /* fire */)) {
        return (true);
    }

    return (false);
}

static uint8_t thing_dmap_try_nexthop (levelp level,
                                       thingp t,
                                       int dmap_id,
                                       level_walls *dmap_in,
                                       double *nexthop_x, 
                                       double *nexthop_y,
                                       uint8_t can_change_dir_without_moving)
{
    level_walls *d;
    
    if (dmap_id) {
        d = &dmap[dmap_id];
    } else {
        d = dmap_in;
    }

    if (thing_find_nexthop_dmap(t, d, nexthop_x, nexthop_y)) {

        if (!can_change_dir_without_moving) {
            if (thing_hit_solid_obstacle(level, t,
                                         *nexthop_x, *nexthop_y)) {
                return (false);
            }
        }

        if (thing_move(level,
                       t, *nexthop_x, *nexthop_y,
                       *nexthop_y < t->y, 
                       *nexthop_y > t->y,
                       *nexthop_x < t->x, 
                       *nexthop_x > t->x, 
                       false /* fire */
                       )) {
            return (true);
        }
    }

    return (false);
}

static int thing_chase_closest_player (thingp t, double *nexthop_x, double *nexthop_y)
{
    double distance = 9999;
    thingp best = 0;
    thingp thing_it;

    thing_it = player;
    if (thing_it) {
        /*
         * Ignore dead player.
         */
        if (!thing_is_dead(thing_it)) {
            if (DISTANCE(thing_it->x, thing_it->y, t->x, t->y) < distance) {
                best = thing_it;
            }
        }
    }

    if (!best) {
        return (false);
    }

    *nexthop_x = t->x;
    *nexthop_y = t->y;

    if (best->x > t->x) {
        *nexthop_x = t->x + move_delta;
    }

    if (best->x < t->x) {
        *nexthop_x = t->x - move_delta;
    }

    if (best->y > t->y) {
        *nexthop_y = t->y + move_delta;
    }

    if (best->y < t->y) {
        *nexthop_y = t->y - move_delta;
    }

    return (true);
}

int thing_run_from (levelp level, thingp t, double *nexthop_x, double *nexthop_y, tpp tp)
{
    double distance = 9999;
    thingp best = 0;
    thingp thing_it;

    FOR_ALL_THINGS(level, thing_it)

        /*
         * Avoid the chosen thing
         */
        if (thing_tp(thing_it) != tp) {
            continue;
        }

        /*
         * Ignore dead instances.
         */
        if (thing_is_dead(thing_it)) {
            continue;
        }

        if (DISTANCE(thing_it->x, thing_it->y, t->x, t->y) < distance) {
            best = thing_it;
        }

    FOR_ALL_THINGS_END

    if (!best) {
        return (false);
    }

    *nexthop_x = t->x;
    *nexthop_y = t->y;

    if (best->x > t->x) {
        *nexthop_x = t->x - move_delta;
    }

    if (best->x < t->x) {
        *nexthop_x = t->x + move_delta;
    }

    if (best->y > t->y) {
        *nexthop_y = t->y - move_delta;
    }

    if (best->y < t->y) {
        *nexthop_y = t->y + move_delta;
    }

    return (true);
}

static int 
thing_wander_in_straight_lines (levelp level,
                                thingp t, 
                                double *nexthop_x, 
                                double *nexthop_y)
{
    if ((t->dx < 0) && (t->dy == 0)) {
        *nexthop_x = (t->x);
        *nexthop_y = (t->y - move_delta);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

        *nexthop_x = (t->x);
        *nexthop_y = (t->y + move_delta);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

        *nexthop_x = (t->x + move_delta);
        *nexthop_y = (t->y);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

    } else if ((t->dx > 0) && (t->dy == 0)) {

        *nexthop_x = (t->x);
        *nexthop_y = (t->y + move_delta);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

        *nexthop_x = (t->x);
        *nexthop_y = (t->y - move_delta);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

        *nexthop_x = (t->x - move_delta);
        *nexthop_y = (t->y);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

    } else if ((t->dx == 0) && (t->dy < 0)) {

        *nexthop_x = (t->x + move_delta);
        *nexthop_y = (t->y);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

        *nexthop_x = (t->x - move_delta);
        *nexthop_y = (t->y);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

        *nexthop_x = (t->x);
        *nexthop_y = (t->y + move_delta);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

    } else if ((t->dx == 0) && (t->dy > 0)) {

        *nexthop_x = (t->x - move_delta);
        *nexthop_y = (t->y);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

        *nexthop_x = (t->x + move_delta);
        *nexthop_y = (t->y);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

        *nexthop_x = (t->x);
        *nexthop_y = (t->y - move_delta);
        t->dx = (*nexthop_x - t->x);
        t->dy = (*nexthop_y - t->y);
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }
    }

    return (false);
}

static int 
thing_wander_in_straight_lines_lr (levelp level,
                                   thingp t, 
                                   double *nexthop_x, 
                                   double *nexthop_y)
{
    if ((myrand() % 100) < 50) {
        *nexthop_x = (t->x + move_delta);
        *nexthop_y = (t->y);
        t->dx = (*nexthop_x - t->x);
        t->dy = 0;
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

        *nexthop_x = (t->x - move_delta);
        *nexthop_y = (t->y);
        t->dx = (*nexthop_x - t->x);
        t->dy = 0;
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

    } else {

        *nexthop_x = (t->x - move_delta);
        *nexthop_y = (t->y);
        t->dx = (*nexthop_x - t->x);
        t->dy = 0;
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }

        *nexthop_x = (t->x + move_delta);
        *nexthop_y = (t->y);
        t->dx = (*nexthop_x - t->x);
        t->dy = 0;
        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) { return (true); }
    }

    return (false);
}

uint8_t thing_find_nexthop (levelp level,
                            thingp t, 
                            double *nexthop_x, 
                            double *nexthop_y)
{
    if (thing_is_wanderer(t)) {
        /*
         * Like a juggernaut
         */
        if (!t->dx && !t->dy) {
            if (thing_is_wanderer_lr(t)) {
                switch (myrand() % 4) {
                    case 0: t->dx = -move_delta; t->dy = 0; break;
                    case 1: t->dx =  move_delta; t->dy = 0; break;
                    case 2: t->dx =  0; t->dy = 0; break;
                    case 3: t->dx =  0; t->dy = 0; break;
                }
            } else {
                switch (myrand() % 4) {
                    case 0: t->dx = -move_delta; t->dy = 0;  break;
                    case 1: t->dx =  move_delta; t->dy = 0;  break;
                    case 2: t->dx =  0; t->dy = -move_delta; break;
                    case 3: t->dx =  0; t->dy =  move_delta; break;
                }
            }
        }

        *nexthop_x = t->x + (double)t->dx;
        *nexthop_y = t->y + (double)t->dy;

        if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) {
            return (true);
        }

        if (thing_is_wanderer_lr(t)) {
            if (thing_wander_in_straight_lines_lr(level, t, nexthop_x, nexthop_y)) {
                return (true);
            }
        } else {
            if (thing_wander_in_straight_lines(level, t, nexthop_x, nexthop_y)) {
                return (true);
            }
        }

        return (false);

    } else if (thing_is_ethereal(t)) {
        /*
         * Walk through walls to get to the player?
         */
        if (thing_chase_closest_player(t, nexthop_x, nexthop_y)) {

            if (thing_try_nexthop(level, t, nexthop_x, nexthop_y)) {
                return (true);
            }

            return (true);
        }
    }

    /*
     * Start out with treating doors as passable.
     */
    if (!t->dmap_id) {
        if (thing_is_shopkeeper(t)) {
            if (thing_is_angry(t)) {
                t->dmap_id = DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE;
            } else {
                t->dmap_id = DMAP_MAP_NONE;
            }
        } else {
            t->dmap_id = DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE;
        }
    }

    /*
     * Try the current map.
     */
    if (t->dmap_id) {
        if (thing_dmap_try_nexthop(level, t, 
                                   t->dmap_id, 
                                   0, 
                                   nexthop_x, 
                                   nexthop_y,
                                   true /* can_change_dir_without_moving */)) {
            return (true);
        }
    }

    /*
     * Try the alternative map.
     */
    if (thing_is_shopkeeper(t)) {
        /*
         * Chase player only if angry.
         */
        if (thing_is_angry(t)) {
            if (t->dmap_id == DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE) {
                t->dmap_id = DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS;
            } else {
                t->dmap_id = DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE;
            }
        } else {
            t->dmap_id = 0;
        }
    }

    if (t->dmap_id) {
        if (thing_dmap_try_nexthop(level,
                                   t, 
                                   t->dmap_id, 
                                   0, 
                                   nexthop_x, 
                                   nexthop_y,
                                   false /* can_change_dir_without_moving */)) {
            return (true);
        }
    }

    /*
     * Still nowhere to move? Try the wander map.
     */
    if (t->dmap_wander_x || t->dmap_wander_y) {
        if (!dmap_map_wander[t->dmap_wander_x][t->dmap_wander_y].walls[(int)t->x][(int)t->y]) {
            /*
             * Reached the destination. Try a new map.
             */
            t->dmap_wander_x = 0;
            t->dmap_wander_y = 0;
        }
    }

    /*
     * Find a wander destination.
     */
    uint32_t x;
    uint32_t y;
    uint32_t tries = 0;

    thing_map_t *map = level_map(level);

    for (;;) {
        x = myrand() % MAP_WIDTH;
        y = myrand() % MAP_HEIGHT;

        tries++;
        if (tries > 100) {
            break;
        }

        /*
         * Make sure we can at least reach the target we chose
         */
        int cost_to_this_target = dmap_map_wander[x][y].walls[(int)t->x][(int)t->y];
        if (cost_to_this_target >= not_preferred) {
            continue;
        }

        thing_map_cell *cell = &map->cells[x][y];
        int done = false;
        uint32_t i;
        for (i = 0; i < cell->count; i++) {
            thingp it;
            
            it = id_to_thing(cell->id[i]);

            if (thing_is_shopkeeper(t)) {
                if (!thing_is_angry(t)) {
                    /*
                     * Not angry. Just wander the shop floor.
                     */
                    if (thing_is_shop_floor(it)) {
                        done = true;
                        break;
                    }

                    /*
                     * Make sure we stay in the same shop.
                     */
                    if (DISTANCE(x, y, t->x, t->y) > 4.0) {
                        continue;
                    }

                    /*
                     * Keep looking for a shop floor.
                     */
                    continue;
                } else {
                    /*
                     * Lost sight of the player. Hunting.
                     */
                    done = true;
                    break;
                }
            } else {
                /*
                 * Monsters just wander anywhere.
                 */
                done = true;
                break;
            }

            done = true;
            break;
        }

        if (done) {
            break;
        }
    }

    if (!t->dmap_wander_x && !t->dmap_wander_y) {
        t->dmap_wander_x = x;
        t->dmap_wander_y = y;
    }

    if (thing_dmap_try_nexthop(level,
                               t, 
                               0, 
                               &dmap_map_wander[t->dmap_wander_x][t->dmap_wander_y], 
                               nexthop_x, nexthop_y,
                               false /* can_change_dir_without_moving */)) {
        return (true);
    }

    /*
     * Try a new wander map next time.
     */
//    t->dmap_wander = &dmap_map_wander[x][y];
//    shouldn't this be 0 to get a new map?
    t->dmap_wander_x = 0;
    t->dmap_wander_y = 0;

    return (false);
}
