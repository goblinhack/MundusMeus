/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include <SDL.h>

#include "main.h"
#include "wid.h"
#include "wid_popup.h"
#include "wid_game_quit.h"
#include "wid_game_map.h"
#include "wid_intro.h"
#include "string_util.h"
#include "wid_menu.h"

static widp wid_really_quit_game_yes_no;

static uint8_t wid_game_quit_init_done;
static void wid_game_quit_create(void);
static void wid_game_quit_destroy(void);

uint8_t wid_game_quit_init (void)
{
    if (!wid_game_quit_init_done) {
    }

    wid_game_quit_init_done = true;

    return (true);
}

void wid_game_quit_fini (void)
{
    if (wid_game_quit_init_done) {
        wid_game_quit_init_done = false;

        wid_game_quit_destroy();
    }
}

void wid_game_quit_hide (void)
{
    wid_game_quit_destroy();
}

void wid_game_quit_visible (void)
{
    wid_game_quit_create();
}

static void wid_game_quit_destroy (void)
{
    wid_destroy(&wid_really_quit_game_yes_no);
}

static void wid_game_quit (void)
{
    wid_game_map_fini();
    wid_game_quit_destroy();
}

static void wid_game_quit_really_quit_callback_yes (widp wid)
{
    LOG("Really quit, yes");
    wid_destroy(&wid_really_quit_game_yes_no);
    wid_game_quit();
    wid_intro_visible();
}

static void wid_game_quit_really_quit_callback_no (widp wid)
{
    LOG("Really quit, no");
    wid_destroy(&wid_really_quit_game_yes_no);
}

static void wid_game_quit_create (void)
{
    if (sdl_is_exiting()) {
        return;
    }

    static const char *messages[] = {
        "I thought you were made of the right stuff",
        "I thought you were tougher",
        "Cluck cluck!",
        "You gonna bail on me?",
        "Cmon, this is no fun?",
        "Keep going you coward",
        "Look I'll be nice, keep playing",
        "When the going gets tough, you just leave?",
        "Ho hum. Such high hopes...",
        "What? Wait",
        "Why? Why? Why?",
        "What did I do to you? Except all the monsters...",
        "Don't do this to me",
        "Please Dave, don't do it",
        "You will not quit, soldier",
        "Perhaps you should play snap instead?",
        "Too tough for you eh?",
        "I give in, looks like you do too",
        "You canny take it cap'n?",
        "Just one more level, cmon",
        "It gets easier, honest",
        "I'll be nice in the next level",
        "Go on then, go off an cry",
        "Is it something I said?",
        "Is it something I killed?",
        "Is it something I manifested?",
        "Let's not break up just yet!",
        "It wont get better if you quit",
        "You quit now, I'll make the next level worse!",
    };

    if (wid_really_quit_game_yes_no) {
        return;
    }

    wid_really_quit_game_yes_no = 
        wid_menu(0,
                 vvlarge_font,
                 large_font,
                 0, // on_update
                 0.5, /* x */
                 0.3, /* y */
                 0.2, /* hightlight */
                 1, /* columns */
                 3, /* focus */
                 4, /* items */

                 (int) 0, "%%fg=red$Quit game?", (void*) 0,

                 (int) 0, messages[myrand() % ARRAY_SIZE(messages)], (void*) 0,

                 (int) 'y', "Yes", wid_game_quit_really_quit_callback_yes,

                 (int) 'n', "No",  wid_game_quit_really_quit_callback_no);
}