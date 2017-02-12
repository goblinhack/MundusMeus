/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */


#include "glapi.h"
#include "wid_tiles.h"
#include "tile.h"
#include "tex.h"

static int wid_tiles_init_done;
tree_root *wid_tiles_all;

uint8_t wid_tiles_init (void)
{
    wid_tiles_init_done = true;

    return (true);
}

static void wid_tiles_destroy (wid_tilesp *t)
{
}

void wid_tiles_fini (void)
{
    if (wid_tiles_init_done) {
        wid_tiles_init_done = false;

        tree_destroy(&wid_tiles_all, (tree_destroy_func)wid_tiles_destroy);
    }
}

wid_tilesp wid_tiles_load (const char *name, double scale)
{
    wid_tilesp t = wid_tiles_find(name);

    if (t) {
        return (t);
    }

    if (!name) {
        DIE("no name for wid_tiles");
        return (0);
    }

    if (!wid_tiles_all) {
        wid_tiles_all = tree_alloc(TREE_KEY_STRING, "TREE ROOT: wid_tiles");
    }

    t = (TYPEOF(t)) myzalloc(sizeof(*t), "TREE NODE: wid_tiles");
    t->tree.key = dupstr(name, "TREE KEY: wid_tiles");

    if (!tree_insert(wid_tiles_all, &t->tree.node)) {
        DIE("wid_tiles insert name [%s] failed", name);
    }

    char tmp[32];

    snprintf(tmp, sizeof(tmp) - 1, "%s_tl", name);
    tilep tile = tile_find(tmp);
    if (!tile) {
        DIE("did not find wid %s tile %s", name, tmp);
    }

    texp tex = tile_get_tex(tile);

    double tile_w = tile_get_width(tile);
    double tile_h = tile_get_height(tile);
    double tex_w = tex_get_width(tex);
    double tex_h = tex_get_height(tex);

    t->across = tex_w / tile_w;
    t->down = tex_h / tile_h;
    t->tile_w = tile_w;
    t->tile_h = tile_h;
    t->scale = scale;

    int i, j, c;

    c = 1;
    for (j = 1; j < t->down - 1; j++) {
        for (i = 1; i < t->across - 1; i++) {
            snprintf(tmp, sizeof(tmp) - 1, "%s_%d", name, c);
            tilep tile = tile_find(tmp);
            if (!tile) {
                DIE("did not find wid %s tile %s", name, tmp);
            }
            t->tile[i][j] = tile;
            c++;
        }
    }

    c = 1;
    for (i = 1; i < t->across - 1; i++) {
        j = 0;
        snprintf(tmp, sizeof(tmp) - 1, "%s_top%d", name, c);
        tilep tile = tile_find(tmp);
        if (!tile) {
            DIE("did not find wid %s tile %s", name, tmp);
        }

        t->tile[i][j] = tile;
        c++;
    }

    c = 1;
    for (i = 1; i < t->across - 1; i++) {
        j = t->down - 1;;
        snprintf(tmp, sizeof(tmp) - 1, "%s_bot%d", name, c);
        tilep tile = tile_find(tmp);
        if (!tile) {
            DIE("did not find wid %s tile %s", name, tmp);
        }

        t->tile[i][j] = tile;
        c++;
    }

    c = 1;
    for (j = 1; j < t->down - 1; j++) {
        i = 0;
        snprintf(tmp, sizeof(tmp) - 1, "%s_left%d", name, c);
        tilep tile = tile_find(tmp);
        if (!tile) {
            DIE("did not find wid %s tile %s", name, tmp);
        }

        t->tile[i][j] = tile;
        c++;
    }

    c = 1;
    for (j = 1; j < t->down - 1; j++) {
        i = t->across - 1;
        snprintf(tmp, sizeof(tmp) - 1, "%s_right%d", name, c);
        tilep tile = tile_find(tmp);
        if (!tile) {
            DIE("did not find wid %s tile %s", name, tmp);
        }

        t->tile[i][j] = tile;
        c++;
    }

    i = 0;
    j = 0;
    snprintf(tmp, sizeof(tmp) - 1, "%s_tl", name);
    tile = tile_find(tmp);
    if (!tile) {
        DIE("did not find wid %s tile %s", name, tmp);
    }

    t->tile[i][j] = tile;

    i = 0;
    j = t->down - 1;
    snprintf(tmp, sizeof(tmp) - 1, "%s_bl", name);
    tile = tile_find(tmp);
    if (!tile) {
        DIE("did not find wid %s tile %s", name, tmp);
    }

    t->tile[i][j] = tile;

    i = t->across - 1;
    j = 0;
    snprintf(tmp, sizeof(tmp) - 1, "%s_tr", name);
    tile = tile_find(tmp);
    if (!tile) {
        DIE("did not find wid %s tile %s", name, tmp);
    }

    t->tile[i][j] = tile;

    i = t->across - 1;
    j = t->down - 1;
    snprintf(tmp, sizeof(tmp) - 1, "%s_br", name);
    tile = tile_find(tmp);
    if (!tile) {
        DIE("did not find wid %s tile %s", name, tmp);
    }

    t->tile[i][j] = tile;

    return (t);
}

wid_tilesp wid_tiles_find (const char *file)
{
    wid_tiles target;
    wid_tiles *result;

    if (!file) {
        DIE("no filename given for wid_tiles find");
    }

    memset(&target, 0, sizeof(target));
    target.tree.key = (char*) file;

    result = (TYPEOF(result)) tree_find(wid_tiles_all, &target.tree.node);
    if (!result) {
        return (0);
    }

    return (result);
}
