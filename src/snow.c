/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "snow.h"
#include "glapi.h"
#include "tile.h"

#define nflakes 10000

typedef struct {
    int active;
    double fading;
    double x;
    double y;
    double z;
} flake;

static flake flakes[nflakes];

void snow_tick (int intensity)
{
    double w = game.video_gl_width;
    double h = game.video_gl_height;

    double flake_w = w / 400.0;
    double flake_h = flake_w;

    double tile_height = h / TILES_SCREEN_HEIGHT;
    double fall_speed = 1.80;
    double flake_fade = 255.0 / TILES_SCREEN_HEIGHT;

    static double wind = 0.1;

    static tilep tile;
    if (!tile) {
        tile = tile_find("snow_flake");

    }

    fpoint tl, br;
    color c = WHITE;

    blit_init();

    flake *f;
    flake *f_eo = flakes + nflakes;

    f = flakes;
    while (f < f_eo) {
        if (intensity && !f->active) {
            intensity--;
            f->active = true;
            f->x = myrand() % (int)w;
            f->y = 0;
            f->z = myrand() % TILES_SCREEN_HEIGHT;
        }

        c.a = 255.0 - (f->z * flake_fade);
        if (f->fading> 0) {
            c.a /= (double) f->fading;
        }

        glcolor(c);

        double scale = 1.0 + (f->z * 0.5);
        double dw = flake_w * scale;
        double dh = flake_h * scale;

        tl.x = f->x - dw;
        tl.y = f->y - dh;
        br.x = f->x + dw;
        br.y = f->y + dh;

        f->y += flake_h * (fall_speed / 10.0) * f->z;

        if (f->y > tile_height * f->z) {
            f->fading += 0.1;
            if (f->fading > 10) {
                f->fading = 0;
                f->active = false;
            }
        }

        f->x += wind - 0.5;

        if ((myrand() % 1000) < 50) {
            f->x += wind - 0.5;
        }

        tile_blit_fat(0, tile, 0, tl, br);

        f++;
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
