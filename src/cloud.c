/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "cloud.h"
#include "glapi.h"
#include "tile.h"
#include "wid.h"
#include "math_util.h"

#define nclouds 10

typedef struct {
    int active;
    double x;
    double y;
    double scale;
} cloud;

static cloud clouds[nclouds];

void cloud_tick (int display)
{
    double w = game.video_gl_width;
    double h = game.video_gl_height;

    double cloud_w = w / 30.0;
    double cloud_h = w / 40.0;
    static double wind = 0.3;

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
        if (!f->active) {
            f->active = true;

            f->x = myrand() % (int)w;
            f->y = myrand() % (int)h;
            f->scale = gauss(1.5, 0.2);
        }

        double dw = cloud_w * (f->scale + 0.2);
        double dh = cloud_h * (f->scale + 0.2);

        tl.x = f->x - dw;
        tl.y = f->y - dh;
        br.x = f->x + dw;
        br.y = f->y + dh;

        color c = BLACK;
        c.a = 100.0;
        glcolor(c);
        tl.x += w / 20;
        tl.y += w / 12;
        br.x += w / 20;
        br.y += w / 12;

        if (display) {
            tile_blit_fat(0, tile, 0, &tl, &br);
        }

        f++;
    }

    f = clouds;

    while (f < f_eo) {
        double dw = cloud_w * f->scale;
        double dh = cloud_h * f->scale;

        tl.x = f->x - dw;
        tl.y = f->y - dh;
        br.x = f->x + dw;
        br.y = f->y + dh;

        f->x += wind;

        color c = WHITE;
        c.a = 150.0;
        glcolor(c);

        if (display) {
            tile_blit_fat(0, tile, 0, &tl, &br);
        }

        f++;
    }

    blit_flush();

    cloud_move(false);
}

void cloud_move (int jumped)
{
    cloud *f;
    cloud *f_eo = clouds + nclouds;

    f = clouds;

    if (!player) {
        return;
    }

    widp w = player->wid;
    if (!w) {
        return;
    }

    verify(w);

    double playerx = wid_get_cx(w);
    double playery = wid_get_cy(w);

    static double last_playerx;
    static double last_playery;

    if (!last_playerx && !last_playery) {
        last_playerx = playerx;
        last_playery = playery;
    }

    double dx = playerx - last_playerx;
    double dy = playery - last_playery;

    last_playerx = playerx;
    last_playery = playery;

    if (jumped) {
        return;
    }

    f = clouds;

    while (f < f_eo) {

        f->x -= dx;
        f->y -= dy;

        double w = game.video_gl_width;
        double h = game.video_gl_height;

        if (f->x > w * 1.5) {
            f->x = -(w / 2) - myrand() % (int)(w / 2);
            f->y = myrand() % (int)h;
        }

        if (f->x < -w * 1.5) {
            f->x = w + myrand() % (int)(w / 2);
            f->y = myrand() % (int)h;
        }

        f++;
    }
}
