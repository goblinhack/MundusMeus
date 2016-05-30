/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include <SDL.h>

#include "main.h"
#include "wid.h"
#include "wid_intro.h"
#include "wid_popup.h"
#include "wid_intro_about.h"
#include "wid_intro_settings.h"
#include "wid_menu.h"
#include "wid_game_over.h"
#include "wid_game_map.h"
#include "wid_hiscore.h"
#include "wid_notify.h"
#include "thing_template.h"
#include "music.h"
#include "level.h"
#include "timer.h"
#include "glapi.h"
#include "wid_keyboard.h"
#include "thing_template.h"
#include "wid_editor.h"
#include "wid_map.h"
#include "wid_cmap.h"
#include "wid_tooltip.h"
#include "thing.h"

static widp wid_intro;
static widp wid_intro_menu;
static widp wid_intro_title;
static timerp wid_change_level_timer;

static void wid_change_level(void *context);
static uint8_t wid_intro_is_hidden;
static uint8_t wid_intro_is_visible;
static uint8_t wid_intro_init_done;

static void wid_intro_quit_selected(void);
static void wid_intro_create(void);
static void wid_intro_menu_create(void);
static void wid_intro_menu_destroy(void);

static int intro_effect_delay = 500;
static int intro_effect_delay_zoom = 1000;
static int wid_intro_ready;

static int saved_focus = 0;


static widp wid_intro_help;
static void wid_intro_help_create(void);

static void wid_intro_help_destroy (void)
{
    wid_destroy(&wid_intro_help);
}

static void wid_intro_help_hide (void)
{
    wid_intro_help_destroy();

    wid_intro_visible();
}

static void wid_intro_help_visible (void)
{
    wid_intro_help_create();
}

static void wid_intro_help_callback_close (widp wid)
{
    wid_intro_help_hide();
}

static void wid_intro_help_create (void)
{
    if (wid_intro_help) {
        return;
    }

    wid_intro_help = 
        wid_menu(0,
                vlarge_font,
                large_font,
                0, // on_update
                0.5, /* x */
                0.5, /* y */
                2, /* columns */
                4, /* focus */
                9, /* items */

                /*
                 * Column widths
                 */
                (double) 0.25, (double) 0.25,

                (int) '0',
                "%%fmt=left$%%fg=white$Space",
                "%%fg=purple$Fire",
                (void*) 0,

                (int) '0', 
                "%%fmt=left$%%fg=white$Movement",
                "%%fg=purple$Cursor keys, shift to run", 
                (void*) 0,

                (int) '0', 
                "%%fmt=left$%%fg=white$Jump",
                "%%fg=purple$z", 
                (void*) 0,

                (int) '0', 
                "%%fmt=left$%%fg=white$Drop bomb",
                "%%fg=purple$b", 
                (void*) 0,

                (int) '0', 
                "%%fmt=left$%%fg=white$Fire rope",
                "%%fg=purple$r", 
                (void*) 0,

                (int) '0', 
                "%%fmt=left$%%fg=white$Drop torch",
                "%%fg=purple$t", 
                (void*) 0,

                (int) '0', 
                "%%fmt=left$%%fg=white$Pay in shop",
                "%%fg=purple$p",
                (void*) 0,

                (int) '0', 
                "%%fmt=left$%%fg=white$Quit game",
                "%%fg=purple$q",
                (void*) 0,

                (int) 'b', 
                "%%fmt=left$Back", 
                (char*) 0,
                wid_intro_help_callback_close);
}

uint8_t wid_intro_init (void)
{
    if (!wid_intro_init_done) {
        wid_intro_create();
    }

    wid_intro_init_done = true;

    return (true);
}

void wid_intro_fini (void)
{
    if (wid_intro_init_done) {
        wid_intro_init_done = false;

        if (wid_intro) {
            wid_destroy(&wid_intro);
            wid_intro_menu_destroy();
            wid_destroy_in(wid_intro_title, wid_hide_delay * 2);
        }
    }
}

void wid_intro_hide (void)
{
    if (wid_intro_is_hidden) {
        return;
    }

    LOG("Intro screen hide");

    wid_intro_is_hidden = true;
    wid_intro_is_visible = false;

    music_halt();

    if (!wid_intro) {
        return;
    }

    wid_fade_out(wid_intro_title, intro_effect_delay_zoom);

    wid_hide(wid_intro, 0);
    wid_raise(wid_intro);
    wid_update(wid_intro);

    wid_destroy_in(wid_intro, wid_hide_delay * 2);

    wid_intro_menu_destroy();
    wid_destroy_in(wid_intro_title, wid_hide_delay * 2);

    wid_intro = 0;
    wid_intro_title = 0;
}

void wid_intro_visible (void)
{
    wid_game_map_fini();

    music_halt();

    wid_intro_is_visible = true;
    wid_intro_is_hidden = false;

    wid_intro_create();
    wid_intro_menu_create();

    wid_notify_flush();

    LOG("Intro screen show");

    music_play_intro();

    if (game.game_over) {
        game.game_over = false;
        wid_game_over_visible();
        return;
    }

    if (!wid_intro) {
        return;
    }

    wid_visible(wid_intro, 0);
    wid_raise(wid_intro);
    wid_update(wid_intro);

    wid_fade_in(wid_intro_title, intro_effect_delay);
}

static double y;
static double dy;

static void wid_intro_tick_reset (void)
{
    y = -0.20;
    dy = 0.00026;
}

static void wid_intro_tick (widp wid)
{
    if (!wid_intro_title) {
        return;
    }

    wid_move_to_pct_centered(wid_intro_title, 0.5f, y);

    y += dy;

    static const double wall_start = 0.20;
    static const double accell_down = 1.10;
    static const double friction_up = 0.8;
    static const double elasticity = 0.09;

    if (y > wall_start) {
        y = wall_start;
        dy = -dy * elasticity;
        y += dy;
    }

    if (dy < 0) {
        dy *= friction_up;

        if (dy > -0.0001) {
            dy = 0.0001;
        }
    } else {
        dy *= accell_down;
    }
}

static void wid_intro_bg_create (void)
{
    if (!wid_intro_title) {
        widp wid = wid_intro_title = wid_new_window("bg");
        fpoint tl = { 0.0, 0.0 };
        fpoint br = { 0.75, 0.15 };

        wid_set_tl_br_pct(wid, tl, br);

        wid_set_tex(wid, 0, "main_title");

        wid_lower(wid);

        color c;
        c = WHITE;
        wid_set_mode(wid, WID_MODE_NORMAL);
        wid_set_color(wid, WID_COLOR_TL, c);
        wid_set_color(wid, WID_COLOR_BR, c);
        wid_set_color(wid, WID_COLOR_BG, c);

        wid_update(wid);
        wid_move_to_pct_centered(wid_intro_title, 0.5f, -2.1f);
        wid_set_on_tick(wid, wid_intro_tick);
        wid_intro_tick_reset();
        wid_set_do_not_lower(wid, true);
    }
}

static widp wid_intro_quit_popup;

static void wid_intro_quit_callback_yes (widp wid)
{
    wid_destroy(&wid_intro_quit_popup);

    sdl_exit();
}

static void wid_intro_quit_callback_no (widp wid)
{
    wid_destroy(&wid_intro_quit_popup);

    wid_intro_visible();
}

static void wid_intro_quit_selected (void)
{
    LOG("Intro quit selected");

    if (wid_intro_quit_popup) {
        return;
    }

    wid_intro_menu_destroy();

    wid_intro_quit_popup = 
        wid_menu(0,
                 vvlarge_font,
                 large_font,
                 0,
                 0.5, /* x */
                 0.5, /* y */
                 1, /* columns */
                 2, /* focus */
                 3, /* items */

                 (int) 0, "Quit game?", (void*)0,

                 (int) 'y', "Yes", wid_intro_quit_callback_yes,

                 (int) 'n', "No",  wid_intro_quit_callback_no);
}

static uint8_t wid_menu_settings_selected (widp w, 
                                           int32_t x, int32_t y,
                                           uint32_t button)
{
    wid_intro_menu_destroy();
    wid_intro_settings_visible();

    return (true);
}

static uint8_t wid_menu_level_editor_selected (widp w,
                                               int32_t x, int32_t y,
                                               uint32_t button)
{
    if (!wid_intro_ready) {
        return (true);
    }

    if (wid_change_level_timer) {
        action_timer_destroy(&wid_timers, wid_change_level_timer);
        wid_change_level_timer = 0;
    }

    SDL_ShowCursor(0);
    wid_intro_hide();
    wid_game_map_fini();

    wid_map("Choose epic level to edit", 0, 0);

    return (true);
}

static uint8_t wid_menu_quick_start_selected (widp w,
                                              int32_t x, int32_t y,
                                              uint32_t button)
{
    SDL_ShowCursor(0);

    /*
     * Check the level is ready
     */
    if (!game.level.is_ready) {
        return (true);
    }

    if (wid_change_level_timer) {
        action_timer_destroy(&wid_timers, wid_change_level_timer);
        wid_change_level_timer = 0;
    }

    wid_intro_menu_destroy();
    wid_intro_hide();

    levelp level = &game.level;
    level_resume(level);

    return (true);
}

static uint8_t wid_menu_past_legends_selected (widp w,
                                               int32_t x, int32_t y,
                                               uint32_t button)
{
    wid_intro_menu_destroy();
    wid_hiscore_visible();

    return (true);
}

static uint8_t wid_menu_help_selected (widp w,
                                       int32_t x, int32_t y,
                                       uint32_t button)
{
    wid_intro_menu_destroy();
    wid_intro_help_visible();

    return (true);
}

static uint8_t wid_menu_credits_selected (widp w,
                                          int32_t x, int32_t y,
                                          uint32_t button)
{
    wid_intro_menu_destroy();
    wid_intro_about_visible();

    return (true);
}

static void wid_intro_create (void)
{
    if (wid_intro) {
        return;
    }

    music_play_intro();

    wid_intro = wid_new_window("intro");

    wid_set_no_shape(wid_intro);

    fpoint tl = {0.0f, 0.0f};
    fpoint br = {1.0f, 1.0f};
    wid_set_tl_br_pct(wid_intro, tl, br);

    color col = BLACK;
    col.a = 0;
    glcolor(col);

    wid_set_mode(wid_intro, WID_MODE_NORMAL);
    wid_set_color(wid_intro, WID_COLOR_TL, col);
    wid_set_color(wid_intro, WID_COLOR_BR, col);
    wid_set_color(wid_intro, WID_COLOR_BG, col);

    wid_intro_bg_create();
    wid_update(wid_intro);

    wid_move_to_pct_centered(wid_intro, 0.5f, 0.5f);
    wid_fade_in(wid_intro_title, intro_effect_delay*2);

    wid_intro_menu_create();
}

static void wid_version_make_visible (void *context)
{
    wid_intro_ready = true;

    widp w = wid_popup(
                  "%%fg=green$MundusMeus -- " VERSION,
                  "",               /* title */
                  0.9f, 0.95f,      /* x,y postition in percent */
                  0,                /* title font */
                  vsmall_font,      /* body font */
                  0,                /* button font */
                  0);               /* number args */
    wid_set_no_shape(w);
    wid_destroy_in(w, 10000);

    wid_move_end(w);
    wid_move_to_pct_centered(w, 0.9f, 0.95);

    wid_game_map_fini();
    game.level_no = 0;
    wid_game_map_init();

    if (!wid_change_level_timer) {
        wid_change_level_timer = action_timer_create(
            &wid_timers,
            (action_timer_callback)wid_change_level,
            (action_timer_destroy_callback)0,
            0, /* context */
            "change level",
            5000,
            0 /* jitter */);
    }
}

static void wid_change_level (void *context)
{
    wid_change_level_timer = 0;

    wid_game_map_fini();
    game.level_no = 0;
    wid_game_map_init();

    wid_change_level_timer = action_timer_create(
        &wid_timers,
        (action_timer_callback)wid_change_level,
        (action_timer_destroy_callback)0,
        0, /* context */
        "change level",
        5000,
        0 /* jitter */);
}

static void wid_intro_menu_create (void)
{
    wid_destroy_delay_ms = 500;

    if (wid_intro_menu) {
        return;
    }

    wid_intro_menu = 
        wid_menu(0,
                 vvlarge_font,
                 vlarge_font,
                 0, // on_update
                 0.5, /* x */
                 0.55, /* y */
                 1, /* columns */
                 saved_focus, /* focus */
                 7, /* items */

                 (int) ' ', "Play game", wid_menu_quick_start_selected,

                 (int) 'S', "Settings", wid_menu_settings_selected,

                 (int) 'e', "Editor", wid_menu_level_editor_selected,

                 (int) 'c', "Credits", wid_menu_credits_selected,

                 (int) 'h', "Hiscores", wid_menu_past_legends_selected,

                 (int) '?', "Help me!", wid_menu_help_selected,

                 (int) 'q', "Quit", wid_intro_quit_selected);

    {
        action_timer_create(
            &wid_timers,
            (action_timer_callback)wid_version_make_visible,
            (action_timer_destroy_callback)0,
            0, /* context */
            "init game version",
            500,
            0 /* jitter */);
    }
}

static void wid_intro_menu_destroy (void)
{
    if (!wid_intro_menu) {
        return;
    }

    wid_menu_ctx *ctx = 
                    (typeof(ctx)) wid_get_context(wid_intro_menu);
    verify(ctx);

    /*
     * Save the focus so when we remake the menu we are at the same entry.
     */
    saved_focus = ctx->focus;

    wid_destroy(&wid_intro_menu);
}
