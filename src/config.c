/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#include <string.h>

#include "main.h"
#include "marshal.h"
#include "string_util.h"
#include "python.h"

struct game_ game;

static const char *config_dir_and_file = "mundusmeus-config.py";
static int32_t config_inited;

void config_fini (void)
{
    if (config_inited) {
        config_inited = false;
    }
}

uint8_t config_init (void)
{
    config_inited = true;

    return (true);
}

static uint8_t demarshal_config (demarshal_p ctx, struct game_ *p)
{
    uint8_t rc;

    rc = true;

    rc = rc && GET_OPT_NAMED_INT32(ctx, "width", p->video_pix_width);
    rc = rc && GET_OPT_NAMED_INT32(ctx, "height", p->video_pix_height);
    rc = rc && GET_OPT_NAMED_INT32(ctx, "sound_volume", p->sound_volume);
    rc = rc && GET_OPT_NAMED_INT32(ctx, "music_volume", p->music_volume);
    rc = rc && GET_OPT_NAMED_INT32(ctx, "display_sync", p->display_sync);
    rc = rc && GET_OPT_NAMED_INT32(ctx, "full_screen", p->full_screen);
    rc = rc && GET_OPT_NAMED_INT32(ctx, "fps_counter", p->fps_counter);

    return (rc);
}

static void marshal_config (marshal_p ctx, struct game_ *p)
{
    PUT_NAMED_INT32(ctx, "width", p->video_pix_width);
    PUT_NAMED_INT32(ctx, "height", p->video_pix_height);
    PUT_NAMED_INT32(ctx, "sound_volume", p->sound_volume);
    PUT_NAMED_INT32(ctx, "music_volume", p->music_volume);
    PUT_NAMED_INT32(ctx, "display_sync", p->display_sync);
    PUT_NAMED_INT32(ctx, "full_screen", p->full_screen);
    PUT_NAMED_INT32(ctx, "fps_counter", p->fps_counter);
}

uint8_t config_save (void)
{
    char *file = dynprintf("%s", config_dir_and_file);
    marshal_p ctx;

    LOG("Saving config to: %s", file);

    ctx = marshal(file);
    if (!ctx) {
        WARN("Failed to save: %s", file);
        myfree(file);
        return (false);
    }

    marshal_config(ctx, &game);

    if (marshal_fini(ctx) < 0) {
        WARN("Failed to finalize: %s", file);
        myfree(file);
        return (false);
    }

    myfree(file);

    return (true);
}

uint8_t config_load (void)
{
    char *file = dynprintf("%s", config_dir_and_file);
    demarshal_p ctx;

    /*
     * Default settings.
     */
    game.video_pix_width = 0;
    game.video_pix_height = 0;
    game.sound_volume = 10;
    game.music_volume = 5;
    game.display_sync = 0;
    game.full_screen = 0;
    game.fps_counter = 0;

    if (!(ctx = demarshal(file))) {
        myfree(file);
        return (true);
    }

    if (!demarshal_config(ctx, &game)) {
        MSG_BOX("Failed to parse: %s", file);
    }

    myfree(file);
    demarshal_fini(ctx);

    return (true);
}

void set_game_video_pix_width (int width)
{
    game.video_pix_width = width;

    py_call_void_module_int("config", "set_game_video_pix_width", game.video_pix_width);
}

int get_game_video_pix_width (void)
{
    game.video_pix_width = 
        py_call_int_module_void("config", "get_game_video_pix_width");

    return (game.video_pix_width);
}
