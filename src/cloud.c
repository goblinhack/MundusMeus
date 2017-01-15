/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "cloud.h"
#include "glapi.h"
#include "tile.h"

#define nclouds 10

typedef struct {
    int active;
    double x;
    double y;
} cloud;

static cloud clouds[nclouds];

void cloud_tick (int intensity)
{
    double w = game.video_gl_width;
    double h = game.video_gl_height;

    double cloud_w = w / 30.0;
    double cloud_h = w / 40.0;
    static double wind = 0.1;

    static tilep tile;
    if (!tile) {
        tile = tile_find("snow_mound5");
    }

    fpoint tl, br;

    blit_init();

    cloud *f;
    cloud *f_eo = clouds + nclouds;

    f = clouds;

    while (f < f_eo) {

        double scale = 1.2;
        double dw = cloud_w * scale;
        double dh = cloud_h * scale;

        tl.x = f->x - dw;
        tl.y = f->y - dh;
        br.x = f->x + dw;
        br.y = f->y + dh;

        color c = BLACK;
        c.a = 100.0;
        glcolor(c);
        tl.x += w / 20;
        tl.y += w / 10;
        br.x += w / 20;
        br.y += w / 10;
        tile_blit_fat(0, tile, 0, tl, br);

        f++;
    }

    f = clouds;

    while (f < f_eo) {
        if (!f->active) {
            f->active = true;

            f->x = myrand() % (int)w;
            f->y = myrand() % (int)h;
        }

        double scale = 1.0;
        double dw = cloud_w * scale;
        double dh = cloud_h * scale;

        tl.x = f->x - dw;
        tl.y = f->y - dh;
        br.x = f->x + dw;
        br.y = f->y + dh;

        if (f->x > w * 2) {
            f->x -= w * 2;
            f->y = myrand() % (int)h;
        }

        f->x += wind;

        color c = WHITE;
        c.a = 150.0;
        glcolor(c);
        tile_blit_fat(0, tile, 0, tl, br);

        f++;
    }

    blit_flush();
}
