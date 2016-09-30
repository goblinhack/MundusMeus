/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include "SDL.h"
#include "tile_private.h"
#include "glapi.h"

uint8_t tile_init(void);
void tile_fini(void);
void tile_load(const char *file, uint32_t width, uint32_t height,
               uint32_t nargs, ...);
void tile_load_arr(const char *tex,
                   const char *tex_black_and_white,
                   uint32_t width, uint32_t height,
                   uint32_t nargs, const char *arr[]);
tilep tile_find(const char *name);
tilep tile_from_surface(SDL_Surface *surface,
                        const char *optional_file,
                        const char *name);
void tile_blit_mask_fat(tilep tile, char *name, fpoint tl, fpoint br);
void blit_quad(point tl, point br);
int32_t tile_get_gl_binding(tilep);
int32_t tile_get_width(tilep);
int32_t tile_get_height(tilep);
const char *tile_name(tilep);
texp tile_get_tex(tilep);
uint32_t tile_get_index(tilep);
tilep string2tile(const char **s);
void tile_get_coords(tilep, float *x1, float *y1, float *x2, float *y2);

/*
 * Blits a whole tile. Y co-ords are inverted.
 */
static inline
void tile_blit_fat (tpp tp, tile *tile, char *name, fpoint tl, fpoint br)
{
    if (unlikely(!tile)) {
        if (!name) {
            DIE("no name for tile blit");
        }

        tile = tile_find(name);

        if (unlikely(!tile)) {
            return;
        }
    }

    double x1;
    double x2;
    double y1;
    double y2;

    if (tp && tp_is_wall(tp)) {
        /*
         * Clipped 0.5 pixels
         */
        x1 = tile->x1;
        x2 = tile->x2;
        y1 = tile->y1;
        y2 = tile->y2;
    } else {
        /*
         * Unclipped 0.5 pixels
         */
        x1 = tile->ox1;
        x2 = tile->ox2;
        y1 = tile->oy1;
        y2 = tile->oy2;
    }

    if (unlikely(tp != 0)) {
        double left_off  = (double)tp_get_blit_left_off(tp);
        double right_off = (double)tp_get_blit_right_off(tp);
        double top_off   = (double)tp_get_blit_top_off(tp);
        double bot_off   = (double)tp_get_blit_bot_off(tp);

        double pct_w     = tile->pct_width;
        double pct_h     = tile->pct_height;
        double pix_w     = br.x - tl.x;
        double pix_h     = br.y - tl.y;

        tl.x -= left_off  * pix_w;
        br.x += right_off * pix_w;
        tl.y -= top_off   * pix_h;
        br.y += bot_off   * pix_h;

        x1 -= left_off  * pct_w;
        x2 += right_off * pct_w;
        y1 -= top_off   * pct_h;
        y2 += bot_off   * pct_h;
    }

    blit(tile->gl_surface_binding,
         x1, y2, x2, y1, tl.x, br.y, br.x, tl.y);
}

/*
 * Blits a whole tile. Y co-ords are inverted. Does not expand tl, br.
 */
static inline
void tile_blit_fat2 (tpp tp, tile *tile, char *name, fpoint tl, fpoint br)
{
#if 0
    if (!tile) {
        if (!name) {
            DIE("no name for tile blit");
        }

        tile = tile_find(name);
    }

    if (!tile) {
        return;
    }
#endif

    double x1;
    double x2;
    double y1;
    double y2;

    if (tp && tp_is_wall(tp)) {
        /*
         * Clipped 0.5 pixels
         */
        x1 = tile->x1;
        x2 = tile->x2;
        y1 = tile->y1;
        y2 = tile->y2;
    } else {
        /*
         * Unclipped 0.5 pixels
         */
        x1 = tile->ox1;
        x2 = tile->ox2;
        y1 = tile->oy1;
        y2 = tile->oy2;
    }

    if (unlikely(tp != 0)) {
        double left_off  = (double)tp_get_blit_left_off(tp);
        double right_off = (double)tp_get_blit_right_off(tp);
        double top_off   = (double)tp_get_blit_top_off(tp);
        double bot_off   = (double)tp_get_blit_bot_off(tp);

        double pct_w     = tile->pct_width;
        double pct_h     = tile->pct_height;
        double pix_w     = br.x - tl.x;
        double pix_h     = br.y - tl.y;

        tl.x -= left_off  * pix_w;
        br.x += right_off * pix_w;
        tl.y -= top_off   * pix_h;
        br.y += bot_off   * pix_h;

        x1 -= left_off  * pct_w;
        x2 += right_off * pct_w;
        y1 -= top_off   * pct_h;
        y2 += bot_off   * pct_h;
    }

    blit(tile->gl_surface_binding,
         x1, y2, x2, y1, tl.x, br.y, br.x, tl.y);
}

/*
 * Blits a whole tile. Y co-ords are inverted.
 */
static inline
void tile_blit_fat_black_and_white (tpp tp, tile *tile, char *name, fpoint tl, fpoint br)
{
#if 0
    if (!tile) {
        if (!name) {
            DIE("no name for tile blit");
        }

        tile = tile_find(name);
    }
#endif
    if (!tile) {
        return;
    }

    double x1 = tile->x1;
    double x2 = tile->x2;
    double y1 = tile->y1;
    double y2 = tile->y2;

    if (unlikely(tp != 0)) {
        double left_off  = (double)tp_get_blit_left_off(tp);
        double right_off = (double)tp_get_blit_right_off(tp);
        double top_off   = (double)tp_get_blit_top_off(tp);
        double bot_off   = (double)tp_get_blit_bot_off(tp);

        double pct_w     = tile->pct_width;
        double pct_h     = tile->pct_height;
        double pix_w     = br.x - tl.x;
        double pix_h     = br.y - tl.y;

        tl.x -= left_off  * pix_w;
        br.x += right_off * pix_w;
        tl.y -= top_off   * pix_h;
        br.y += bot_off   * pix_h;

        x1 -= left_off  * pct_w;
        x2 += right_off * pct_w;
        y1 -= top_off   * pct_h;
        y2 += bot_off   * pct_h;
    }

    if (!tile->gl_surface_binding_black_and_white) {
        blit(tile->gl_surface_binding,
            x1, y2, x2, y1, tl.x, br.y, br.x, tl.y);
    } else {
        blit(tile->gl_surface_binding_black_and_white,
            x1, y2, x2, y1, tl.x, br.y, br.x, tl.y);
    }
}

/*
 * Blits a whole tile.
 */
static inline
void tile_blit_at (tile *tile, char *name, point tl, point br)
{
#if 0
    if (!tile) {
        if (!name) {
            DIE("no name for tile blit");
        }

        tile = tile_find(name);
    }
#endif

    blit(tile->gl_surface_binding,
         tile->x1, tile->y2, tile->x2, tile->y1, tl.x, tl.y, br.x, br.y);
}

/*
 * Blits a whole tile.
 */
static inline
void tile_blit (tile *tile, char *name, point at)
{
    point tl, br;

#if 0
    if (!tile) {
        if (!name) {
            DIE("no name for tile blit");
        }

        tile = tile_find(name);
    }
#endif

    tl.x = at.x - tile->pix_width/2;
    br.y = at.y - tile->pix_height/2;
    br.x = at.x + tile->pix_width/2;
    tl.y = at.y + tile->pix_height/2;

    tile_blit_at(tile, name, tl, br);
}

/*
 * Blits a whole tile. Y co-ords are inverted.
 */
static inline
void tile_blit_colored_fat (tpp tp,
                            tile *tile,
                            char *name,
                            fpoint tl,
                            fpoint br,
                            color color_tl,
                            color color_tr,
                            color color_bl,
                            color color_br)
{
#if 0
    if (!tile) {
        if (!name) {
            DIE("no name for tile blit");
        }

        tile = tile_find(name);
    }

    if (!tile) {
        return;
    }
#endif

    double x1 = tile->x1;
    double x2 = tile->x2;
    double y1 = tile->y1;
    double y2 = tile->y2;

    if (unlikely(tp != 0)) {
        double left_off  = (double)tp_get_blit_left_off(tp);
        double right_off = (double)tp_get_blit_right_off(tp);
        double top_off   = (double)tp_get_blit_top_off(tp);
        double bot_off   = (double)tp_get_blit_bot_off(tp);

        double pct_w     = tile->pct_width;
        double pct_h     = tile->pct_height;
        double pix_w     = br.x - tl.x;
        double pix_h     = br.y - tl.y;

        tl.x -= left_off  * pix_w;
        br.x += right_off * pix_w;
        tl.y -= top_off   * pix_h;
        br.y += bot_off   * pix_h;

        x1 -= left_off  * pct_w;
        x2 += right_off * pct_w;
        y1 -= top_off   * pct_h;
        y2 += bot_off   * pct_h;
    }

    blit_colored(tile->gl_surface_binding,
                 x1, y2, x2, y1,
                 tl.x, br.y, br.x, tl.y,
                 color_tl,
                 color_tr,
                 color_bl,
                 color_br);
}
