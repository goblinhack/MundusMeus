/*
 * Copyright (C) 2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#include "main.h"
#include "sdl.h"
#include "tile.h"
#ifdef ENABLE_WIDGET_CODE
#include "wid.h"
#endif
#include "time_util.h"
#include "thing_template.h"
#include "thing_tile.h"
#include "ttf.h"
#include "string_util.h"
#include <string.h>
#include <stdlib.h>
#include "map.h"
#include "dmap.h"

void dmap_print (dmap *d)
{
    int16_t x;
    int16_t y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            int16_t e = d->val[x][y];
            if (e == DMAP_IS_WALL) {
                printf("# ");
                continue;
            }

            if (e > 0) {
                printf("%2d", e);
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

/*
 * An optimized djkstra map that only looks at map areas that change.
 */
void dmap_process (dmap *D)
{
    int16_t x;
    int16_t y;
    int16_t a;
    int16_t b;
    int16_t c;
    int16_t d;
    int16_t *e;
    int16_t f;
    int16_t g;
    int16_t h;
    int16_t i;
    int16_t lowest;
    int16_t changed;
    static uint16_t valid[MAP_WIDTH][MAP_HEIGHT];
    static uint16_t new_valid[MAP_WIDTH][MAP_HEIGHT];
    static uint16_t orig_valid[MAP_WIDTH][MAP_HEIGHT];

    memset(valid, 1, sizeof(valid));
    memset(orig_valid, 1, sizeof(valid));

    for (y = 1; y < MAP_HEIGHT - 1; y++) {
        for (x = 1; x < MAP_WIDTH - 1; x++) {
            e = &D->val[x ][y];
            if (*e != DMAP_IS_WALL) {
                continue;
            }

            valid[x][y] = 0;
            orig_valid[x][y] = 0;
        }
    }

#if 0
    int16_t count = 1;
#endif

    do {
        changed = false;
        memset(new_valid, 0, sizeof(new_valid));

#if 0
        printf("run %d %d %d\n", count, x, y);
        count++;
#endif
        for (y = 1; y < MAP_HEIGHT - 1; y++) {
            for (x = 1; x < MAP_WIDTH - 1; x++) {
                if (!orig_valid[x][y]) {
#if 0
printf("# ");
#endif
                    continue;
                }

                if (!valid[x][y]) {
#if 0
printf("  ");
#endif
                    continue;
                }

                e = &D->val[x  ][y];

                a =  D->val[x-1][y-1] * 2;
                b =  D->val[x  ][y-1];
                c =  D->val[x+1][y-1] * 2;

                d =  D->val[x-1][y];
                f =  D->val[x+1][y];
                 
                g =  D->val[x-1][y+1] * 2;
                h =  D->val[x  ][y+1];
                i =  D->val[x+1][y+1] * 2;

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
printf("* ");
                } else {
printf("%-2d", lowest);
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
}
