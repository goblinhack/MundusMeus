/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */


#include "glapi.h"
#include "tex.h"
#include "tile.h"
#include "ttf.h"
#include "ramdisk.h"
#include "time_util.h"
#include "string_ext.h"
#include "thing_tile.h"
#include "font.h"

#ifdef ENABLE_GENERATE_TTF
#include "bits.h"
#include "math_util.h"
#include "pixel.h"
#include "stb_image.h"
static void ttf_create_tex_from_char(TTF_Font *ttf, const char *name,
                                     font *f, uint8_t c);
#endif

static double tile_stretch = 1.2;
static uint8_t ttf_init_done;

uint8_t ttf_init (void)
{
#ifdef ENABLE_GENERATE_TTF
    if (TTF_Init() < 0) {
        ERR("cannot init SDL_ttf");
        return (false);
    }
#endif
    ttf_init_done = true;

    return (true);
}

void ttf_fini (void)
{
    if (ttf_init_done) {
        ttf_init_done = false;
#ifdef ENABLE_GENERATE_TTF
        TTF_Quit();
#endif
    }
}

#ifdef ENABLE_GENERATE_TTF
/*
 * Load a new font and create textures for each glyph
 */
font *ttf_new (const char *name, int32_t pointSize, int32_t style)
{
    TTF_Font *ttf;
    uint8_t c;
    font *f;

    f = (fontp)myzalloc(sizeof(*f), __FUNCTION__);

    DBG("Load TTF: %s", name);

    ttf = TTF_OpenFont(name, pointSize);
    if (!ttf) {
        DIE("cannot open font file %s", name);
    }

    f->foreground.r = 255;
    f->foreground.g = 255;
    f->foreground.b = 255;
    f->background.r = 0;
    f->background.g = 0;
    f->background.b = 0;

    TTF_SetFontStyle(ttf, style);

    for (c = TTF_GLYPH_MIN; c < TTF_GLYPH_MAX; c++) {
        ttf_create_tex_from_char(ttf, name, f, c);
    }

    TTF_CloseFont(ttf);

    return (f);
}
#endif

/*
 * Return a SDL rectangle with the size of the font
 */
void ttf_text_size (font **fpp, const char *text_in,
                    double *w,
                    double *h,
                    enum_fmt *fmt,
                    const char **col,
                    double scaling,
                    double advance,
                    uint8_t fixed_width)
{
    uint8_t found_format_string = false;
    int32_t c;
    texp tex;
    double x_start = 0;
    double x;
    enum_fmt _fmt;
    const char *text = text_in;
    font *f = *fpp;
    f->glyphs[' '].width = f->glyphs['t'].width;
    f->glyphs['('].width = f->glyphs['i'].width;
    f->glyphs[')'].width = f->glyphs['i'].width;

    x = 0;
    *w = 0;
    *h = f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].height * scaling;

    /*
     * To account for shadows that are not included in the glyph.
     */
    *h *= 1.1;

    while ((c = *text++) != '\0') {
        if (c == '\\') {
            c = ' ';
        }

	if (!found_format_string) {
	    if (c == '%') {
		found_format_string = true;
		continue;
	    }
	} else if (found_format_string) {
	    if (c == '%') {
		if (!strncmp(text, "fg=", 3)) {
		    text += 3;
                    if (col) {
                        *col = string2colorname(&text);
                    } else {
                        (void) string2color(&text);
                    }
		    found_format_string = false;
		    continue;
		} else if (!strncmp(text, "fmt=", 4)) {
		    text += 4;

                    _fmt = string2fmt(&text);
                    if (fmt) {
                        *fmt = _fmt;
                    }

		    found_format_string = false;
		    continue;
		} else if (!strncmp(text, "font=", 5)) {
		    text += 5;
                    *fpp = string2font(&text);
                    f = *fpp;
		    found_format_string = false;
		    continue;
		} else if (!strncmp(text, "tex=", 4)) {
		    text += 4;
                    tex = string2tex(&text);
		    found_format_string = false;
                    x += tex_get_width(tex) * scaling * advance;
		    continue;
		} else if (!strncmp(text, "tile=", 5)) {

		    text += 5;
                    (void)string2tile(&text);

                    x += f->glyphs[(int)TTF_FIXED_WIDTH_CHAR].width * scaling * advance * tile_stretch;

		    found_format_string = false;
		    continue;
		} else if (!strncmp(text, "tp=", 3)) {
		    text += 3;
                    tpp tp = string2tp(&text);
                    tilep tile = tp_first_tile(tp);

                    fpoint tl;
                    fpoint br;

                    double bx = x;

                    double y = 0;
                    tl.x = (x);
                    tl.y = (y);
                    br.x = (x + f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].width * scaling * tile_stretch);
                    br.y = (y + f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].height * (scaling));

                    tile_get_blit_size(tp, tile, 0, &tl, &br);

                    x = bx;
                    x += br.x - tl.x;

                    y = br.y - tl.y;
                    y *= 1.1;

                    if (y > *h) {
                        *h = y;
                    }

		    found_format_string = false;
		    continue;
		}
	    }
	}

	found_format_string = false;

        if (c == '\t') {
            x = ((((x-x_start) / TTF_TABSTOP) + 1.0) * TTF_TABSTOP);
            x = x + x_start;
        } else if (c == '~') {
            x += f->glyphs[(int) TTF_FIXED_WIDTH_CHAR].width * scaling * advance;
        } else {
            int fixed = fixed_width;

            if (fixed) {
                x += f->glyphs[(int) TTF_FIXED_WIDTH_CHAR].width * scaling * advance;
            } else {
                if (c == TTF_CURSOR_CHAR) {
                    x += f->glyphs[(int) TTF_FIXED_WIDTH_CHAR].width * scaling * advance;
                } else {
                    x += f->glyphs[(int) c].width * scaling * advance;
                }
            }
        }
    }

    *w = x;
}

/*
 * Blit the font to the screen
 */
void ttf_putc (font *f, int32_t c, double x, double y, double scaling)
{
    double texMinX = f->glyphs[c].texMinX;
    double texMaxX = f->glyphs[c].texMaxX;
    double texMinY = f->glyphs[c].texMinY;
    double texMaxY = f->glyphs[c].texMaxY;

    if (c == TTF_CURSOR_CHAR) {

        c = TTF_FIXED_WIDTH_CHAR;

        GLfloat left = (GLfloat)(x);
        GLfloat right = (GLfloat)(x + f->glyphs[c].width * scaling);
        GLfloat top = (GLfloat)(y);
        GLfloat bottom = (GLfloat)(y + f->glyphs[c].height * (scaling));

#ifdef CURSOR_FLASH
        static uint32_t last;
        static uint8_t first = true;

        glBindTexture(GL_TEXTURE_2D, 0);

        if (first) {
            first = false;
            last = time_get_time_ms();
        }

        glcolor_save();

        if (time_have_x_tenths_passed_since(10, last)) {
            glcolor(CONSOLE_CURSOR_COLOR);
            last = time_get_time_ms();
        } else if (time_have_x_tenths_passed_since(5, last)) {
            glcolor(CONSOLE_CURSOR_COLOR);
        } else {
            glcolor(CONSOLE_CURSOR_OTHER_COLOR);

            gl_blitsquare(left, top, right, bottom);

            left += 1;
            right -= 1;
            top += 1;
            bottom -= 1;

            gl_blitquad(left, top, right, bottom);

            glcolor_restore();
            return;
        }

        gl_blitquad(left, top, right, bottom);

        glcolor(BLACK);

        left += 1;
        right -= 1;
        top += 1;
        bottom -= 1;

        gl_blitquad(left, top, right, bottom);

        glcolor_restore();
        return;
#else
        glBindTexture(GL_TEXTURE_2D, 0);

        {
            color c = CONSOLE_CURSOR_COLOR;

            c.a = 200;

            glcolor(c);

            gl_blitsquare(left, top, right, bottom);

            glcolor_restore();
            return;
        }
#endif
    }

    GLfloat left = (GLfloat)(x);
    GLfloat right = (GLfloat)(x + f->glyphs[c].width * scaling);
    GLfloat top = (GLfloat)(y);
    GLfloat bottom = (GLfloat)(y + f->glyphs[c].height * (scaling));

    blit(f->tex[c].tex,
         texMinX, texMinY, texMaxX, texMaxY, left, top, right, bottom);
}

/*
 * Blit the font to the screen
 */
static void ttf_puts_internal (font *f, const char *text,
                               double x, double y,
                               double scaling, double advance,
                               uint8_t include_formatting,
                               uint8_t draw_cursor,
                               uint8_t fixed_width)
{
    color saved_color = gl_color_current();
    uint8_t color_retore_needed = false;
    uint8_t found_format_string = false;
    int32_t c;
    texp tex;
    tilep tile;
    tpp tp;
    color fg;
    double x_start = x;

    while ((c = *text++) != '\0') {
        if (c == '\\') {
            c = ' ';
        }

	if (!found_format_string) {
	    if (c == '%') {
		found_format_string = true;
		continue;
	    }
	} else if (found_format_string) {
	    if (c == '%') {
		if (!strncmp(text, "fg=", 3)) {
		    text += 3;

                    fg = string2color(&text);
                    fg.a = saved_color.a;

                    if (!color_retore_needed) {
                        color_retore_needed = true;
                        if (include_formatting) {
                            glcolor_save();
                        }
                    }

                    if (include_formatting) {
                        glcolor(fg);
                    }

		    found_format_string = false;
		    continue;

		} else if (!strncmp(text, "fmt=", 4)) {
		    text += 4;

                    (void)string2fmt(&text);

		    found_format_string = false;
		    continue;

		} else if (!strncmp(text, "font=", 5)) {
		    text += 5;
                    f = string2font(&text);
		    found_format_string = false;
		    continue;

		} else if (!strncmp(text, "tex=", 4)) {
		    text += 4;
                    tex = string2tex(&text);

                    /*
                     * Move forward half the texture size so it is centered.
                     */
                    x += (tex_get_width(tex) * scaling * advance)/2;

                    point at;
                    at.x = x;
                    at.y = y + (tex_get_height(tex) * scaling * advance)/2;
                    tex_blit(tex, at);

                    /*
                     * Now the other half.
                     */
                    x += (tex_get_width(tex) * scaling * advance)/2;

		    found_format_string = false;
		    continue;

		} else if (!strncmp(text, "tile=", 5)) {
		    text += 5;
                    tile = string2tile(&text);

                    fpoint tl;
                    fpoint br;

                    double bx = x;

                    tl.x = (x);
                    tl.y = (y);
                    br.x = (x + f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].width * scaling * tile_stretch);
                    br.y = (y + f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].height * (scaling));

                    fpoint otl = tl;
                    fpoint obr = br;

                    tile_get_blit_size(0, tile, 0, &otl, &obr);

                    double dy = ((obr.y - otl.y) - (br.y - tl.y));
                    double dx = ((obr.x - otl.x) - (br.x - tl.x));

                    tl.y += dy;
                    br.y += dy;
                    tl.x += dx/2;
                    br.x += dx/2;

                    double stretch =
                        (f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].width * scaling * tile_stretch) -
                        (f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].width * scaling);
                    tl.x -= stretch / 2.0;

                    tile_blit_fat(0, tile, 0, &tl, &br);

                    x = bx;
                    x += obr.x - otl.x;

		    found_format_string = false;
		    continue;

		} else if (!strncmp(text, "tp=", 3)) {
		    text += 3;
                    tp = string2tp(&text);
                    tilep tile = tp_first_tile(tp);

                    fpoint tl;
                    fpoint br;

                    double bx = x;

                    tl.x = (x);
                    tl.y = (y);
                    br.x = (x + f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].width * scaling * tile_stretch);
                    br.y = (y + f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].height * (scaling));

                    fpoint otl = tl;
                    fpoint obr = br;

                    double oh = br.y - tl.y;

                    tile_get_blit_size(tp, tile, 0, &otl, &obr);

                    double dx = ((obr.x - otl.x) - (br.x - tl.x));

                    tl.y += oh * tp_get_blit_top_off(tp);
                    br.y += oh * tp_get_blit_top_off(tp);
                    tl.x += dx/2;
                    br.x += dx/2;

                    double stretch =
                        (f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].width * scaling * tile_stretch) -
                        (f->glyphs[(uint32_t)TTF_FIXED_WIDTH_CHAR].width * scaling);
                    tl.x -= stretch / 2.0;

                    tile_blit_fat(tp, tile, 0, &tl, &br);

                    x = bx;
                    x += obr.x - otl.x;

		    found_format_string = false;
		    continue;
		}
	    }
	}

	found_format_string = false;

        if (c == '\t') {
            x = ((((x-x_start) / TTF_TABSTOP) + 1) * TTF_TABSTOP);
            x = x + x_start;
        } else if (c == '~') {
            x += f->glyphs[(int) TTF_FIXED_WIDTH_CHAR].width * scaling * advance;
        } else {
            double maxc = f->glyphs[(int) TTF_FIXED_WIDTH_CHAR].width;
            double thisc = f->glyphs[(int) c].width;

            int fixed = fixed_width;

            if (fixed) {
                thisc = maxc;
            }

            double pad = ((maxc - thisc) * scaling * advance) / 2.0;

            if (fixed) {
                x += pad;
            }

            if (c == TTF_CURSOR_CHAR) {
                if (draw_cursor) {
                    ttf_putc(f, c, x, y, scaling);
                }
            } else {
                ttf_putc(f, c, x, y, scaling);
            }

            if (fixed) {
                x += thisc;
                x += pad;
            } else {
                if (c == TTF_CURSOR_CHAR) {
                    x += f->glyphs[(int) TTF_FIXED_WIDTH_CHAR].width * scaling * advance;
                } else {
                    x += f->glyphs[c].width * scaling * advance;
                }
            }
        }
    }

    if (color_retore_needed) {
        glcolor(saved_color);
    }
}

void ttf_puts (font *f, const char *text, double x, double y,
               double scaling, double advance,
               uint8_t fixed_width)
{
    ttf_puts_internal(f, text, x, y, scaling, advance,
                      true, /* include formatting */
                      true, /* include cursor */
                      fixed_width);
}

void ttf_puts_no_fmt (font *f, const char *text, double x, double y,
                      double scaling, double advance,
                      uint8_t fixed_width)
{
    ttf_puts_internal(f, text, x, y, scaling, advance,
                      false, /* include formatting */
                      false, /* include cursor */
                      fixed_width);
}

//
// Create a tex from a surface. Set the alpha according to the color key.
// Pixels that match the color key get an alpha of zero while all other pixels
// get an alpha of one. We use black for the color key.
//
#ifdef ENABLE_GENERATE_TTF
static void
ttf_set_color_key (SDL_Surface *glyph_surface,
                   GLfloat *texcoord,
                   uint8_t ckr,
                   uint8_t ckg,
                   uint8_t ckb,
                   double *width,
                   double *height)
{
    SDL_Surface *tmp;
    uint32_t colorkey;

    /*
     * Use the surface width and height expanded to powers of 2
     */
    *width = glyph_surface->w;
    *height = glyph_surface->h;

    texcoord[0] = 0; // Min X
    texcoord[1] = 0; // Min Y
    texcoord[2] =
        (GLfloat)(((double)glyph_surface->w) / ((double)*width));  // Max X
    texcoord[3] =
        (GLfloat)(((double)glyph_surface->h) / ((double)*height)); // Max Y

    tmp = SDL_CreateRGBSurface(glyph_surface->flags,
                               *width, *height, 32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                               0xFF000000,
                               0x00FF0000,
                               0x0000FF00,
                               0x000000FF
#else
                               0x000000FF,
                               0x0000FF00,
                               0x00FF0000,
                               0xFF000000
#endif
                            );

    if (!tmp) {
        ERR("Failed to make RGB surface size %f %f: %s",
            *width, *height, SDL_GetError());
        SDL_ClearError();
        return;
    }

    newptr(tmp, "SDL_CreateRGBSurface");

    /*
     * Set up so that colorkey pixels become transparent
     */
    colorkey = SDL_MapRGBA(tmp->format, 0, 0, 0, 0);
    SDL_FillRect(tmp, 0, colorkey);

    colorkey = SDL_MapRGBA(glyph_surface->format, ckr, ckg, ckb, 0);
#if (SDL_MAJOR_VERSION == 2)
    SDL_SetColorKey(glyph_surface, SDL_TRUE, colorkey);
#else
    SDL_SetColorKey(glyph_surface, SDL_SRCCOLORKEY, colorkey);
#endif

    SDL_FreeSurface(tmp);
    oldptr(tmp);
}

/*
 * Given a single character, make it into an opengl tex
 */
static void
ttf_create_tex_from_char (TTF_Font *ttf, const char *name, font *f, uint8_t c)
{
    static char text[2];
    int32_t e;

    /*
     * Load the glyph info
     */
    int minx;
    int maxx;
    int miny;
    int maxy;
    int advance;

    e = TTF_GlyphMetrics(ttf, c,
                         &minx,
                         &maxx,
                         &miny,
                         &maxy,
                         &advance);
    if (e != 0) {
        ERR("error loading font glyph %u %s", c, name);
        return;
    }

    f->glyphs[c].minx = minx;
    f->glyphs[c].maxx = maxx;
    f->glyphs[c].miny = miny;
    f->glyphs[c].maxy = maxy;
    f->glyphs[c].advance = advance;

    text[0] = c;
    text[1] = '\0';

    SDL_Surface *g0 =
            TTF_RenderText_Shaded(ttf, text, f->foreground, f->background);
    if (!g0) {
        ERR("error rendering font glyph %u %s", c, name);
        return;
    }

    SDL_Surface *g1 =
            TTF_RenderText_Shaded(ttf, text, f->foreground, f->background);
    if (!g1) {
        ERR("error getting display format for font glyph %u %s", c, name);
        return;
    }

    SDL_FreeSurface(g0);

    GLfloat texcoord [4];

    f->tex[c].image = g1;
    f->tex[c].tex = 0;

    ttf_set_color_key(g1, texcoord, 0, 0, 0,
                      &f->glyphs[c].width, &f->glyphs[c].height);

    f->glyphs[c].texMinX = texcoord[0];
    f->glyphs[c].texMinY = texcoord[1];
    f->glyphs[c].texMaxX = texcoord[2];
    f->glyphs[c].texMaxY = texcoord[3];
}
#endif

font *
ttf_read_tga (fontp f, const char *name, int32_t pointsize)
{
    char filename[MAXSTR];
    uint32_t c;
    texp tex;

    if (pointsize < 0) {
        ERR("nutso font size %d", pointsize);
    }

    if (pointsize > 100) {
        ERR("nutso font size %d", pointsize);
    }

    /*
     * Read the glyph data.
     */
    snprintf(filename, sizeof(filename), "%s_pointsize%d.data",
             name, pointsize);

    const unsigned char *glyph_data = ramdisk_load(filename, 0);
    if (!glyph_data) {
        ERR("could not load font %s data", filename);
    }

    memcpy(f->glyphs, glyph_data, sizeof(f->glyphs));

    snprintf(filename, sizeof(filename), "%s_pointsize%u.tga",
             name, pointsize);

    tex = tex_load(filename,
                   filename /* to make unique for same point size */);
    if (!tex) {
        ERR("could not load font %s tex", filename);
    }

    for (c = TTF_GLYPH_MIN; c < TTF_GLYPH_MAX; c++) {
        f->tex[c].tex = tex_get_gl_binding(tex);
        f->tex[c].image = tex_get_surface(tex);
    }

    return (f);
}

#ifdef ENABLE_GENERATE_TTF
font *
ttf_write_tga (char *name, int32_t pointsize)
{
    uint32_t rmask, gmask, bmask, amask;
    double glyph_per_row;
    char filename[200];
    SDL_Surface *dst;
    uint32_t height;
    uint32_t width;
    double maxx;
    double maxy[TTF_GLYPH_MAX];
    uint32_t c;
    int x;
    int y;
    double h;
    font *f;

    snprintf(filename, sizeof(filename), "%s_pointsize%u.tga",
             name, pointsize);

    if (tex_find(filename)) {
        return (0);
    }

    /*
     * x glyphs horizontally and y vertically.
     */
    glyph_per_row = 16;

    f = ttf_new(name, pointsize, TTF_STYLE_NORMAL);
    if (!f) {
        ERR("could not create font %s", name);
    }

    maxx = 0;
    memset(maxy, 0, sizeof(maxy));

    /*
     * Find the largest font glyph pointsize.
     */
    x = 0;
    y = 0;
    height = 0;

    for (c = TTF_GLYPH_MIN; c < TTF_GLYPH_MAX; c++) {

        if (f->tex[c].image) {
            maxx = max(maxx, f->tex[c].image->w);
            maxy[y] = max(maxy[y], f->tex[c].image->h);
        }

        if (++x >= glyph_per_row) {
            x = 0;
            height += maxy[y];
            y++;
        }
    }

    if (!maxx) {
        ERR("no glyphs in font %s", name);
    }

    width = glyph_per_row * maxx;

    if (MULTIPLE_BITS(width)) {
        width = nextpoweroftwo(width);
    }

    height += 40;

    if (MULTIPLE_BITS(height)) {
        height = nextpoweroftwo(height);
    }

    /*
     * Make a large surface for all glyphs.
     */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    dst = SDL_CreateRGBSurface(0, width, height, 32,
                               rmask, gmask, bmask, amask);
    if (!dst) {
        ERR("no surface created for size %dx%d font %s", width, height, name);
    }

    newptr(dst, "SDL_CreateRGBSurface");

    /*
     * Blit each glyph to the large surface.
     */
    x = 0;
    y = 0;
    h = 0;

    for (c = TTF_GLYPH_MIN; c < TTF_GLYPH_MAX; c++) {

        if (f->tex[c].image) {
            SDL_Rect dstrect = { maxx * x, h, maxx, maxy[y] };

            SDL_BlitSurface(f->tex[c].image, 0, dst, &dstrect);
        }

        if (++x >= glyph_per_row) {
            x = 0;
            h += maxy[y];
            y++;
        }
    }

    /*
     * Convert the black border smoothing that ttf adds into alpha.
     */
    {
        double x;
        double y;

        for (x = 0; x < dst->w; x++) {
            for (y = 0; y < dst->h; y++) {

                color c;

                c = getPixel(dst, x, y);

                if ((c.a == 255) &&
                    (c.r == 255) &&
                    (c.g == 255) &&
                    (c.b == 255)) {
                    /*
                     * Do nothing.
                     */
                } else if ((c.a == 0) &&
                    (c.r == 0) &&
                    (c.g == 0) &&
                    (c.b == 0)) {
                    /*
                     * Do nothing.
                     */
                } else {
                    /*
                     * Convery gray to white with alpha.
                     */
                    c.a = (c.r + c.g + c.b) / 3;
                    c.r = 255;
                    c.g = 255;
                    c.b = 255;
                }

                putPixel(dst, x, y, c);
            }
        }
    }

#define MAX_TEXTURE_HEIGHT 4096

    if (dst->h > MAX_TEXTURE_HEIGHT) {
        ERR("ttf is too large");
    }

    uint8_t filled_pixel_row[MAX_TEXTURE_HEIGHT] = {0};

    /*
     * What the hell am I doing here? The ttf library returns incorrect
     * boounds for the top and bottom of glyphs, so I'm looking for a line of
     * no pixels above and below each glyph so I can really find the texture
     * bounds of a glyph. This allows squeezing of text together.
     */
    {
        int x;
        int y;

        for (y = 0; y < dst->h; y++) {
            for (x = 0; x < dst->w; x++) {
                color c;
                c = getPixel(dst, x, y);
                if (c.r || c.g || c.b || c.a) {
                    filled_pixel_row[y] = true;
                    break;
                }
            }
        }
    }

    /*
     * Work our the tex co-ords for each glyph in the large tex.
     */
    x = 0;
    y = 0;
    h = 0;

    int d1_max = 0;
    int d2_max = 0;

    for (c = TTF_GLYPH_MIN; c < TTF_GLYPH_MAX; c++) {

        {
            int y = (h + (h + f->glyphs[c].height)) / 2;

            int miny = y;
            int maxy = y;

            for (;;) {
                if (!filled_pixel_row[miny]) {
                    break;
                }

                miny--;
                if (miny <= 0) {
                    break;
                }
            }

            for (;;) {
                if (!filled_pixel_row[maxy]) {
                    break;
                }

                maxy++;
                if (maxy >= MAX_TEXTURE_HEIGHT - 1) {
                    break;
                }
            }

            int d1 = y - miny;
            if (d1 > d1_max) {
                d1_max = d1;
            }

            int d2 = maxy - y;
            if (d2 > d2_max) {
                d2_max = d2;
            }
        }

        if (++x >= glyph_per_row) {
            x = 0;
            h += maxy[y];
            y++;
        }
    }

    x = 0;
    y = 0;
    h = 0;

    for (c = TTF_GLYPH_MIN; c < TTF_GLYPH_MAX; c++) {

        f->glyphs[c].texMinX =
                        (double)(x * maxx) / (double)dst->w;
        f->glyphs[c].texMaxX =
                        (double)((x * maxx) + f->glyphs[c].width) /
                        (double)dst->w;

        {
            int y = (h + (h + f->glyphs[c].height)) / 2;

            int y1 = y - d1_max;
            int y2 = y + d2_max;

            if (y1 < 0) {
                y1 = 0;
            }

            f->glyphs[c].texMinY =
                            (double)(y1) /
                            (double)dst->h;
            f->glyphs[c].texMaxY =
                            (double)(y2) /
                            (double)dst->h;
        }

        if (++x >= glyph_per_row) {
            x = 0;
            h += maxy[y];
            y++;
        }
    }

    SDL_LockSurface(dst);
    stbi_write_tga(filename, dst->w, dst->h, STBI_rgb_alpha, dst->pixels);
    SDL_UnlockSurface(dst);

    texp tex;
    tex = tex_from_surface(dst, filename, filename);
    if (!tex) {
        ERR("could not convert %s to tex", filename);
    }

    /*
     * Work our the tex co-ords for each glyph in the large tex.
     */
    x = 0;
    y = 0;
    h = 0;

    for (c = TTF_GLYPH_MIN; c < TTF_GLYPH_MAX; c++) {

        f->tex[c].image = dst;
        f->tex[c].tex = tex_get_gl_binding(tex);
    }

    /*
     * Save the glyph data.
     */
    snprintf(filename, sizeof(filename), "%s_pointsize%u.data",
             name, pointsize);

    FILE *out = fopen(filename, "w");

    fwrite(f->glyphs, sizeof(f->glyphs), 1, out);

    fclose(out);

    printf("wrote %s\n",filename);
    return (f);
}
#endif

