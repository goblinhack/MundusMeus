/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "rain.h"
#include "glapi.h"
#include "tile.h"

#define ndrops 20000

typedef struct {
    int active;
    double fading;
    double x;
    double y;
    double z;
} drop;

static drop drops[ndrops];

void rain_tick (int intensity)
{
    double w = game.video_gl_width;
    double h = game.video_gl_height;

    double drop_w = w / 400.0;
    double drop_h = drop_w;

    double tile_height = h / TILES_SCREEN_HEIGHT;
    double fall_speed = 5.00;
    double drop_fade = 255.0 / TILES_SCREEN_HEIGHT;

    static double wind = 0.5;

    static tilep tile;
    if (!tile) {
        tile = tile_find("rain_drop");
    }

    fpoint tl, br;
    color c = WHITE;

    blit_init();

    drop *f;
    drop *f_eo = drops + ndrops;

    f = drops;

    static int active_count = 0;
    if (!intensity && !active_count) {
        return;
    }

    while (f < f_eo) {
        if (intensity && !f->active) {
            intensity--;
            f->active = true;
            active_count++;

            f->x = myrand() % (int)w;
            f->y = 0;
            f->z = myrand() % TILES_SCREEN_HEIGHT;
        }

        if (!f->active) {
            f++;
            continue;
        }

        c.a = 255.0 - (f->z * drop_fade);
        if (f->fading> 0) {
            c.a /= (double) f->fading;
        }

        glcolor(c);

        double scale = 1.0 + (f->z * 0.5);
        double dw = drop_w * scale;
        double dh = drop_h * scale;

        tl.x = f->x - dw;
        tl.y = f->y - dh;
        br.x = f->x + dw;
        br.y = f->y + dh;

        f->y += drop_h * (fall_speed / 10.0) * f->z;

        if (f->y > tile_height * f->z) {
            f->fading += 0.1;
            if (f->fading > 10) {
                f->fading = 0;
                f->active = false;
                active_count--;
            }
        }

        f->x += wind - 0.5;

        if ((myrand() % 1000) < 50) {
            f->x += wind - 0.5;
        }

        tile_blit_fat(0, tile, 0, &tl, &br);

        f++;
    }

    if (!active_count) {
        return;
    }
    blit_flush();

    if ((myrand() % 1000) < 50) {
        wind += 0.1;
        if (wind > 1.0) {
            wind = 1.0;
        }
    }

    if ((myrand() % 1000) < 50) {
        wind -= 0.1;
        if (wind < -1.0) {
            wind = -1.0;
        }
    }
}
