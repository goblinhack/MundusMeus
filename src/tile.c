/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */


#include "glapi.h"
#include "tile.h"
#include "tex.h"
#include "pixel.h"

tree_root *tiles;

static uint8_t tile_init_done;

uint8_t tile_init (void)
{
    tile_init_done = true;

    return (true);
}

static void tile_destroy (tile *t)
{
}

void tile_fini (void)
{
    if (tile_init_done) {
        tile_init_done = false;

        tree_destroy(&tiles, (tree_destroy_func)tile_destroy);
    }
}

void tile_load_arr (const char *tex_name,
                    const char *tex_name_black_and_white,
                    uint32_t width, uint32_t height,
                    uint32_t nargs, const char *arr[])
{
    texp tex = tex_load(0, tex_name);
    texp tex_black_and_white;

    if (tex_name_black_and_white && *tex_name_black_and_white) {
        tex_black_and_white = tex_load(0, tex_name_black_and_white);
    } else {
        tex_black_and_white = 0;
    }

    float fw = 1.0 / (((float)tex_get_width(tex)) / ((float)width));
    float fh = 1.0 / (((float)tex_get_height(tex)) / ((float)height));
    float pw = 1.0 / ((float)tex_get_width(tex));
    float ph = 1.0 / ((float)tex_get_height(tex));
    pw /= 2.0;
    ph /= 2.0;

    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t idx = 0;

    size pixel_size;

    pixel_size.width = width;
    pixel_size.height = height;

    while (nargs--) {

        const char *name = arr[idx++];

        if (name && *name) {
            tile *t;

            if (tile_find(name)) {
                ERR("tile name [%s] already used", name);
            }

            if (!tiles) {
                tiles = tree_alloc(TREE_KEY_STRING, "TREE ROOT: tile");
            }

            t = (TYPEOF(t)) myzalloc(sizeof(*t), "TREE NODE: tile");
            t->tree.key = dupstr(name, "TREE KEY: tile");

            if (!tree_insert(tiles, &t->tree.node)) {
                ERR("tile insert name [%s] failed", name);
            }

            t->index = idx - 1;
            t->pix_width = width;
            t->pix_height = height;
            t->tex = tex;
            t->gl_surface_binding = tex_get_gl_binding(tex);

            if (tex_black_and_white) {
                t->gl_surface_binding_black_and_white =
                                tex_get_gl_binding(tex_black_and_white);
            }

            t->x1 = fw * (float)(x);
            t->y1 = fh * (float)(y);
            t->x2 = t->x1 + fw;
            t->y2 = t->y1 + fh;

            t->ox1 = t->x1;
            t->oy1 = t->y1;
            t->ox2 = t->x2;
            t->oy2 = t->y2;

            /*
             * Why? Texture atlas and GL_LINEAR will cause problems blending
             * with tiles adjacent in the atlas, so we trim 0.5 of a pixel
             * all around.
             */
            t->x1 += pw;
            t->x2 -= pw;
            t->y1 += ph;
            t->y2 -= ph;

            t->pct_width = fw;
            t->pct_height = fh;

            DBG("Tile: %-10s %ux%u (%u, %u)", name, width, height, x, y);

            SDL_Surface *s = tex_get_surface(tex);

	    point AT = {
                pixel_size.width * x,
                pixel_size.height * y
            };

	    point MAX = {
                pixel_size.width * x,
		pixel_size.height * y
            };

	    point MIN = {
               (pixel_size.width * x) + pixel_size.width - 1,
	       (pixel_size.height * y) + pixel_size.height - 1
            };

            int x1, y1;

	    for (y1=pixel_size.height-1; y1>=0; y1--) {
		for (x1=0; x1<pixel_size.width; x1++) {

		    point at = {
                        (pixel_size.width * x) + x1,
			(pixel_size.height * y) + y1
                    };

		    color p = getPixel(s, at.x, at.y);

                    /*
                     * If solid...
                     */
		    if (p.a >= 0xef) {
			MIN.x = min(at.x, MIN.x);
			MIN.y = min(at.y, MIN.y);
			MAX.x = max(at.x, MAX.x);
			MAX.y = max(at.y, MAX.y);
#ifdef DEBUG_TILE
                        printf("X");
#endif
                    } else if (p.a > 0) {
#ifdef DEBUG_TILE
                        printf(".");
#endif
		    } else {
#ifdef DEBUG_TILE
                        printf(" ");
#endif
                    }
		}
#ifdef DEBUG_TILE
                printf("\n");
#endif
	    }

            t->px1 = ((double) (MIN.x - AT.x)) / (double) pixel_size.width;
            t->px2 = ((double) (MAX.x - AT.x + 1)) / (double) pixel_size.width;
            t->py1 = ((double) (MIN.y - AT.y)) / (double) pixel_size.height;
            t->py2 = ((double) (MAX.y - AT.y + 1)) / (double) pixel_size.height;

#ifdef DEBUG_TILE
            printf("^^^  %s %f %f %f %f min x %d %d min y %d %d\n",name,t->px1,t->py1,t->px2,t->py2, MIN.x,MAX.x,MIN.y,MAX.y);
#endif
        }

        x++;

        if (x * width >= tex_get_width(tex)) {
            x = 0;
            y++;
        }

        if (y * height > tex_get_height(tex)) {
            if (name) {
                ERR("overflow reading tile arr[%s]", name);
            } else {
                ERR("overflow reading tile arr at x %d y %d", x, y);
            }
        }
    }
}

/*
 * Find an existing tile.
 */
tilep tile_find (const char *name)
{
    tile target;
    tile *result;

    if (!name) {
        ERR("no name for tile find");
    }

    memset(&target, 0, sizeof(target));
    target.tree.key = (char*) name;

    result = (TYPEOF(result)) tree_find(tiles, &target.tree.node);
    if (!result) {
        return (0);
    }

    verify(result->tex);

    return (result);
}

int32_t tile_get_gl_binding (tile *tile)
{
    return (tile->gl_surface_binding);
}

int32_t tile_get_width (tile *tile)
{
    return (tile->pix_width);
}

int32_t tile_get_height (tile *tile)
{
    return (tile->pix_height);
}

texp tile_get_tex (tile *tile)
{
    return (tile->tex);
}

uint32_t tile_get_index (tile *tile)
{
    return (tile->index);
}

const char *tile_name (tile *tile)
{
    return (tile->tree.key);
}

void tile_get_coords (tile *tile, float *x1, float *y1, float *x2, float *y2)
{
    *x1 = tile->x1;
    *y1 = tile->x1;
    *x2 = tile->x2;
    *x2 = tile->x2;
}

tilep string2tile (const char **s)
{
    static char tmp[MAXSTR];
    static const char *eo_tmp = tmp + MAXSTR;
    const char *c = *s;
    char *t = tmp;

    while (t < eo_tmp) {
        if ((*c == '\0') || (*c == '$')) {
            break;
        }

        *t++ = *c++;
    }

    if (c == eo_tmp) {
        return (0);
    }

    *t++ = '\0';
    *s += (t - tmp);

    tile find;
    tile *target;

    memset(&find, 0, sizeof(find));
    find.tree.key = tmp;

    target = (TYPEOF(target)) tree_find(tiles, &find.tree.node);
    if (!target) {
        DIE("unknown tile [%s]", tmp);
    }

    return (target);
}
