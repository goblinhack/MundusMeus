/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "math_util.h"
#include "wid_game_map.h"
#include "thing.h"
#include "fluid.h"

int level_explosion_flash_effect;

/*
 * Place an explosion
 */
void level_place_explosion_at (levelp level,
                               thingp owner,
                               double ox, 
                               double oy, 
                               double x, 
                               double y, 
                               uint8_t dist,
                               uint8_t is_epicenter,
                               const char *epicenter,
                               uint32_t nargs,
                               va_list args)
{
    /*
     * Choose one of the things in the args list to place.
     */
    const char *name = 0;
    uint32_t r;

    if (is_epicenter) {
        r = 1;

        name = epicenter;
    } else {
        r = (myrand() % nargs) + 1;
    }

    if (!name) {
        while (r--) {
            name = va_arg(args, char *);
        }
    }

    if (!name) {
        ERR("cannot place explosion thing");
        return;
    }

    tpp tp = tp_find(name);
    if (!tp) {
        ERR("no explosion for name %s", name);
        return;
    }

    double delay = DISTANCE(ox, oy, x, y) * 100;

    uint32_t destroy_in;
    uint32_t jitter;

    destroy_in = 5000;
    jitter = 10;

    thing_place_and_destroy_timed(level,
                                  tp,
                                  owner,
                                  x,
                                  y,
                                  delay,
                                  destroy_in,
                                  jitter,
                                  is_epicenter);

    if (is_epicenter) {
        if (tp_is_explosion(tp)) {
            int i;
            for (i = 0; i < 100; i++) {
                fluid_add_droplets(level);
            }

            fluid_remove_water_radius(level, x, y, 3);
        }
    }
}

static double this_explosion[MAP_WIDTH][MAP_HEIGHT];
static uint8_t this_explosion_x;
static uint8_t this_explosion_y;
static uint8_t this_explosion_radius;

void explosion_flood (levelp level, uint8_t x, uint8_t y)
{
    if (x < 1) {
        return;
    }

    if (y < 1) {
        return;
    }

    if (x > MAP_WIDTH - 1) {
        return;
    }

    if (y > MAP_HEIGHT - 1) {
        return;
    }

    if (this_explosion[x][y] > 0.0) {
        return;
    }

    double distance = DISTANCE(x, y, this_explosion_x, this_explosion_y);

    if (distance > this_explosion_radius) {
        return;
    }

    if (map_find_wall_at(level, x, y, 0) ||
        map_find_door_at(level, x, y, 0) ||
        map_find_rock_at(level, x, y, 0)) {
        /*
         * Don't go any further but allow the explosion to overlap into this 
         * tile.
         */
        this_explosion[x][y] = distance;
        return;
    }

#if 0
    /*
     * Why do this ? Explosions can't bend around corners.
     */
    if (!can_see(level, x, y, this_explosion_x, this_explosion_y)) {
        /*
         * Don't go any further but allow the explosion to overlap into this 
         * tile.
         */
        this_explosion[x][y] = distance;
        return;
    }
#endif

    this_explosion[x][y] = distance;
    explosion_flood(level, x-1, y);
    explosion_flood(level, x+1, y);
    explosion_flood(level, x, y-1);
    explosion_flood(level, x, y+1);
}

#ifdef DEBUG_EXPLOSION
static FILE *fp;

static void debug_explosion (levelp level, int ix, int iy)
{
    int32_t x;
    int32_t y;
    widp w;
                
    if (!fp) {
        fp = fopen("exp.txt","w");
    }

    fprintf(fp,"test level %p\n",level);

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {

            widp mywid = 0;

            if ((x == ix) && (y == iy)) {
                fprintf(fp,"*");
                continue;
            }
                
            if (map_find_wall_at(level, x, y, &w)) {
                fprintf(fp,"x");
                mywid = w;
            } else if (map_find_door_at(level, x, y, &w)) {
                fprintf(fp,"D");
                mywid = w;
            }

            if (!mywid) {
                fprintf(fp," ");
                continue;
            }
        }
        fprintf(fp,"\n");
    }
    fprintf(fp,"\n");
    fprintf(fp,"\n");
}
#endif

/*
 * Place an explosion
 */
static void level_place_explosion_ (levelp level, 
                                    thingp owner,
                                    double ox, 
                                    double oy,
                                    double x, 
                                    double y,
                                    int radius,
                                    double density,
                                    const char *epicenter,
                                    uint32_t nargs, ...)
{
    va_list args;
 
    /*
     * However, as the player can move close to walls, the current tile might 
     * end up being a wall. If this is the case, look around for a closer tile 
     * that has no wall.
     */
    if (0)
    if (map_find_wall_at(level, x, y, 0) ||
        map_find_door_at(level, x, y, 0) ||
        map_find_rock_at(level, x, y, 0)) {

        double dx, dy;
        double best_x, best_y;
        double best_distance;
        int gotone = false;

        best_x = -1;
        best_y = -1;
        best_distance = 999;

        for (dx = -0.5; dx <= 0.5; dx+=0.5) {
            for (dy = -0.5; dy <= 0.5; dy+=0.5) {
                double tx, ty;

                tx = x + dx;
                ty = y + dy;

                if (tx < 0) {
                    continue;
                }
                if (ty < 0) {
                    continue;
                }

                if (tx >= MAP_WIDTH) {
                    continue;
                }
                if (ty >= MAP_HEIGHT) {
                    continue;
                }

                if (map_find_wall_at(level, tx, ty, 0) ||
                    map_find_door_at(level, tx, ty, 0) ||
                    map_find_rock_at(level, tx, ty, 0)) {
                    continue;
                }

                double distance = DISTANCE(x, y, tx, ty);
                if (!gotone || (distance < best_distance)) {
                    best_distance = distance;
                    best_x = tx;
                    best_y = ty;
                    gotone = true;
                }
            }
        }

        if (gotone) {
            x = best_x;
            y = best_y;
        }
    }

    /*
     * Record the start of this explosion. We will do a map flood fill to find 
     * out the extent of the detonation.
     */
    int ix, iy;

    for (ix = 0; ix < MAP_WIDTH; ix++) {
        for (iy = 0; iy < MAP_HEIGHT; iy++) {
            this_explosion[ix][iy] = 0.0;
        }
    }

    this_explosion_x = x;
    this_explosion_y = y;
    this_explosion_radius = radius;
    explosion_flood(level, x, y);

#ifdef DEBUG_EXPLOSION
    if (0) {
        debug_explosion(level, x, y);
    }

    printf("explosion at x %d y %d\n",(int)x,(int)y);

    for (iy = 0; iy < MAP_HEIGHT; iy++) {
        for (ix = 0; ix < MAP_WIDTH; ix++) {
            if (((int)x == ix) && ((int)y == iy)) {
                printf("*");
                continue;
            }

            if (map_find_wall_at(level, ix, iy, 0) ||
                map_find_rock_at(level, ix, iy, 0)) {
                printf("+");
                continue;
            }

            if (this_explosion[ix][iy]) {
                printf("%u", this_explosion[ix][iy]);
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
#endif

    /*
     * Place the epicenter.
     */
    va_start(args, nargs);

    (void) level_place_explosion_at(level, 
                                    owner,
                                    ox,
                                    oy,
                                    x, 
                                    y, 
                                    0,
                                    true, /* epicenter */
                                    epicenter,
                                    nargs, args);
    va_end(args);

    if (!radius) {
        return;
    }

    for (ix = x - radius; ix <= x + radius; ix++) {
        if (ix < 1) {
            continue;
        }

        if (ix > MAP_WIDTH - 1) {
            continue;
        }

        for (iy = y - radius; iy <= y + radius; iy++) {
                                
            if (iy < 1) {
                continue;
            }

            if (iy > MAP_HEIGHT - 1) {
                continue;
            }

            int8_t distance = this_explosion[ix][iy];
            if (!distance) {
                continue;
            }

            double dx, dy;

            if ((radius == 1) || (density == 1.0)) {
                va_start(args, nargs);
                (void) level_place_explosion_at(level, 
                                                owner,
                                                ox,
                                                oy,
                                                ix, 
                                                iy, 
                                                distance,
                                                false, /* epicenter */
                                                epicenter,
                                                nargs, args);
                va_end(args);
            } else {
                for (dx = -0.5; dx <= 0.5; dx += density) {
                    for (dy = -0.5; dy <= 0.5; dy += density) {
                        double ex = ix + dx;
                        double ey = iy + dy;

                        va_start(args, nargs);
                        (void) level_place_explosion_at(level, 
                                                        owner,
                                                        ox,
                                                        oy,
                                                        ex, 
                                                        ey, 
                                                        distance,
                                                        false, /* epicenter */
                                                        epicenter,
                                                        nargs, args);
                        va_end(args);
                    }
                }
            }
        }
    }
}

void level_place_explosion (levelp level, 
                            thingp owner,
                            tpp tp,
                            double ox, double oy,
                            double x, double y)
{
    const char *explodes_as = 0;
    double explosion_radius = 1.0;
    int id = 0;

    tpp otp = tp;

    if (tp) {
        if (tp_is_cloud_effect(tp)) {
            explodes_as = tp_name(tp);
            if (explodes_as) {
                tp = tp_find(explodes_as);
            }
        } else {
            explodes_as = tp_explodes_as(tp);
            if (explodes_as) {
                tp = tp_find(explodes_as);
            }
        }

        explosion_radius = tp_get_explosion_radius(tp);
        id = tp_to_id(tp);
    }

    if (!explodes_as) {
        DIE("combustable, but no explosion for name %s", tp_name(tp));
        return;
    }

    tpp non_explosive_gas_cloud = tp_find(explodes_as);
    if (!non_explosive_gas_cloud) {
        DIE("no explosion for name %s", explodes_as);
        return;
    }

    /*
     * Used for fire potions and bombs as it gives a layered effect.
     */
    if (tp_to_id(otp) == THING_BOMB) {

        level_explosion_flash_effect = 10;

        char tmp[20];
        static int which;
        which++;
        snprintf(tmp, sizeof(tmp) - 1, "explosion%d", (which % 3) + 1);

        level_place_explosion_(level, 
                               owner,
                               ox, oy,
                               x, y,
                               tp_get_explosion_radius(non_explosive_gas_cloud),
                               1.0, // density
                               tmp, // epicenter
                               3, // nargs
                               "explosion1",
                               "explosion2",
                               "explosion3");
        return;
    }

CON("todo %s", tp_name(otp));
return;
    level_place_explosion_(level, 
                           owner,
                           ox, oy,
                           x, y,
                           tp_get_explosion_radius(non_explosive_gas_cloud),
                           0.5, // density
                           explodes_as, // epicenter
                           1, // nargs
                           explodes_as);
}

static void level_place_small_rocks (levelp level, 
                                     thingp t,
                                     double x, 
                                     double y,
                                     double radius,
                                     int amount)
{
    while (amount--) {
        double px = gauss(x, radius);
        double py = y - gauss(1, radius);

        wid_game_map_replace_tile(level, px, py,
                                  0, /* thing */
                                  tp_find("smallrock1"),
                                  0 /* tpp_data */);
    }
}

void thing_explosion_placed (levelp level, thingp t)
{
    level_place_small_rocks(level, t,
                            t->x, t->y,
                            2.05, // radius
                            5 // amount
                        );

    things_throw(level, t);
}
