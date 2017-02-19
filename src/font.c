/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#include "main.h"
#include "ttf.h"
#include "font.h"
#ifdef ENABLE_GENERATE_TTF
#include "string_util.h"
#endif

static int32_t font_inited;

fontp fixed_font;
fontp vsmall_font;
fontp small_font;
fontp med_font;
fontp large_font;
fontp vlarge_font;
fontp vvlarge_font;

static tree_root *font_all;

static void font_destroy (fontp *t)
{
}

void font_fini (void)
{
    if (font_inited) {
        font_inited = false;

        fixed_font = 0;
        vsmall_font = 0;
        small_font = 0;
        med_font = 0;
        large_font = 0;
        vlarge_font = 0;
        vvlarge_font = 0;
    }

    tree_destroy(&font_all, (tree_destroy_func)font_destroy);
}

static fontp font_load (const char *name, const char *file, int size)
{
    fontp t = font_find(name);

    if (t) {
        return (t);
    }

    if (!name) {
        DIE("no name for font %s", __FUNCTION__);
        return (0);
    }

    if (!font_all) {
        font_all = tree_alloc(TREE_KEY_STRING, "TREE ROOT: font");
    }

    t = (TYPEOF(t)) myzalloc(sizeof(font), "TREE NODE: font");
    t->tree.key = dupstr(name, "TREE KEY: font");

    if (!tree_insert(font_all, &t->tree.node)) {
        DIE("font insert name [%s] failed", name);
    }

    ttf_read_tga(t, file, size);

    return (t);
}

fontp font_find (const char *file)
{
    font target;
    font *result;

    if (!file) {
        DIE("no filename given for font find %s", __FUNCTION__);
    }

    memset(&target, 0, sizeof(target));
    target.tree.key = (char*) file;

    result = (TYPEOF(result)) tree_find(font_all, &target.tree.node);
    if (!result) {
        return (0);
    }

    return (result);
}

uint8_t font_init (void)
{
    int32_t fixed_font_size = FIXED_FONT_SIZE;
    int32_t vsmall_font_size = VSMALL_FONT_SIZE;
    int32_t small_font_size = SMALL_FONT_SIZE;
    int32_t med_font_size = MED_FONT_SIZE;
    int32_t vlarge_font_size = VLARGE_FONT_SIZE;
    int32_t vvlarge_font_size = VVLARGE_FONT_SIZE;
    int32_t large_font_size = LARGE_FONT_SIZE;

    font_inited = true;

#if 0
    double scale = (double)game.video_pix_width / 1000.0;

    fixed_font_size  = ((double)fixed_font_size  ) * scale;
    vsmall_font_size = ((double)vsmall_font_size ) * scale;
    small_font_size  = ((double)small_font_size  ) * scale;
    med_font_size    = ((double)med_font_size    ) * scale;
    large_font_size  = ((double)large_font_size  ) * scale;
    vlarge_font_size = ((double)vlarge_font_size ) * scale;
    vvlarge_font_size = ((double)vvlarge_font_size ) * scale;
#endif

    if (fixed_font_size > MAX_FONT_SIZE - 1) {
        fixed_font_size = MAX_FONT_SIZE - 1;
    }
    if (vsmall_font_size > MAX_FONT_SIZE - 1) {
        vsmall_font_size = MAX_FONT_SIZE - 1;
    }
    if (small_font_size > MAX_FONT_SIZE - 1) {
        small_font_size = MAX_FONT_SIZE - 1;
    }
    if (med_font_size > MAX_FONT_SIZE - 1) {
        med_font_size = MAX_FONT_SIZE - 1;
    }
    if (large_font_size > MAX_FONT_SIZE - 1) {
        large_font_size = MAX_FONT_SIZE - 1;
    }
    if (vlarge_font_size > MAX_FONT_SIZE - 1) {
        vlarge_font_size = MAX_FONT_SIZE - 1;
    }
    if (vvlarge_font_size > MAX_FONT_SIZE - 1) {
        vvlarge_font_size = MAX_FONT_SIZE - 1;
    }

#ifdef ENABLE_GENERATE_TTF
    /*
     * Generate bitmaps from TTF.
     */
    char *tmp;

    int32_t i;

    for (i = 10; i < MAX_FONT_SIZE; i+=2) {
        tmp = strprepend(mybasename(FIXED_FONT, __FUNCTION__), TTF_PATH);
        ttf_write_tga((char*) tmp, i);
        myfree(tmp);

        tmp = strprepend(mybasename(VSMALL_FONT, __FUNCTION__), TTF_PATH);
        ttf_write_tga((char*) tmp, i);
        myfree(tmp);

        tmp = strprepend(mybasename(SMALL_FONT, __FUNCTION__), TTF_PATH);
        ttf_write_tga((char*) tmp, i);
        myfree(tmp);

        tmp = strprepend(mybasename(MED_FONT, __FUNCTION__), TTF_PATH);
        ttf_write_tga((char*) tmp, i);
        myfree(tmp);

        tmp = strprepend(mybasename(LARGE_FONT, __FUNCTION__), TTF_PATH);
        ttf_write_tga((char*) tmp, i);
        myfree(tmp);

        tmp = strprepend(mybasename(VLARGE_FONT, __FUNCTION__), TTF_PATH);
        ttf_write_tga((char*) tmp, i);
        myfree(tmp);

        tmp = strprepend(mybasename(VVLARGE_FONT, __FUNCTION__), TTF_PATH);
        ttf_write_tga((char*) tmp, i);
        myfree(tmp);
    }
#endif

    fixed_font = font_load("fixed",
                           (char*)FIXED_FONT, fixed_font_size & ~1);
    vsmall_font = font_load("vsmall",
                            (char*)VSMALL_FONT, vsmall_font_size & ~1);
    small_font  = font_load("small",
                            (char*)SMALL_FONT, small_font_size & ~1);
    med_font    = font_load("medium",
                            (char*)MED_FONT, med_font_size & ~1);
    large_font  = font_load("large",
                            (char*)LARGE_FONT, large_font_size & ~1);
    vlarge_font = font_load("vlarge",
                            (char*)VLARGE_FONT, vlarge_font_size & ~1);
    vvlarge_font = font_load("vvlarge",
                             (char*)VVLARGE_FONT, vvlarge_font_size & ~1);

    return (true);
}
