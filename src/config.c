/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#include <string.h>

#include "main.h"
#include "string_util.h"
#include "python.h"

struct game_ game;

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

uint8_t config_save (void)
{
    set_game_video_pix_width(game.video_pix_width);
    set_game_video_pix_height(game.video_pix_height);
    set_game_sound_volume(game.sound_volume);
    set_game_music_volume(game.music_volume);
    set_game_display_sync(game.display_sync);
    set_game_full_screen(game.full_screen);
    set_game_fps_counter(game.fps_counter);

    return (true);
}

uint8_t config_load (void)
{
    game.video_pix_width = get_game_video_pix_width();
    game.video_pix_height = get_game_video_pix_height();
    game.sound_volume = get_game_sound_volume();
    game.music_volume = get_game_music_volume();
    game.display_sync = get_game_display_sync();
    game.full_screen = get_game_full_screen();
    game.fps_counter = get_game_fps_counter();

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

void set_game_video_pix_height (int height)
{
    game.video_pix_height = height;

    py_call_void_module_int("config", "set_game_video_pix_height", game.video_pix_height);
}

int get_game_video_pix_height (void)
{
    game.video_pix_height = 
        py_call_int_module_void("config", "get_game_video_pix_height");

    return (game.video_pix_height);
}

void set_game_sound_volume (int sound_volume)
{
    game.sound_volume = sound_volume;

    py_call_void_module_int("config", "set_game_sound_volume", game.sound_volume);
}

int get_game_sound_volume (void)
{
    game.sound_volume = 
        py_call_int_module_void("config", "get_game_sound_volume");

    return (game.sound_volume);
}

void set_game_music_volume (int music_volume)
{
    game.music_volume = music_volume;

    py_call_void_module_int("config", "set_game_music_volume", game.music_volume);
}

int get_game_music_volume (void)
{
    game.music_volume = 
        py_call_int_module_void("config", "get_game_music_volume");

    return (game.music_volume);
}

void set_game_display_sync (int display_sync)
{
    game.display_sync = display_sync;

    py_call_void_module_int("config", "set_game_display_sync", game.display_sync);
}

int get_game_display_sync (void)
{
    game.display_sync = 
        py_call_int_module_void("config", "get_game_display_sync");

    return (game.display_sync);
}

void set_game_full_screen (int full_screen)
{
    game.full_screen = full_screen;

    py_call_void_module_int("config", "set_game_full_screen", game.full_screen);
}

int get_game_full_screen (void)
{
    game.full_screen = 
        py_call_int_module_void("config", "get_game_full_screen");

    return (game.full_screen);
}

void set_game_fps_counter (int fps_counter)
{
    game.fps_counter = fps_counter;

    py_call_void_module_int("config", "set_game_fps_counter", game.fps_counter);
}

int get_game_fps_counter (void)
{
    game.fps_counter = 
        py_call_int_module_void("config", "get_game_fps_counter");

    return (game.fps_counter);
}
