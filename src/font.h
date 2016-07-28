/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#define TTF_FIXED_WIDTH_CHAR 'Q'
#define TTF_GLYPH_MIN ' '
#define TTF_GLYPH_MAX 126
#define TTF_TABSTOP 80.0

#ifdef ENABLE_GENERATE_TTF
#include "SDL_ttf.h"
#endif

typedef struct {
    double width;
    double height;
    double minx;
    double maxx;
    double miny;
    double maxy;
    double advance;
    double texMinX;
    double texMaxX;
    double texMinY;
    double texMaxY;
} glyph;

typedef struct {
    SDL_Surface *image;
    uint32_t tex;
} glyphtex;

typedef struct font {
    tree_key_string tree;
    glyph glyphs[TTF_GLYPH_MAX+1];
    glyphtex tex[TTF_GLYPH_MAX+1];
    SDL_Color foreground;
    SDL_Color background;
} font;

void font_fini(void);
uint8_t font_init(void);
fontp font_find(const char *f);
