/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#include "main.h"
#include "thing.h"
#include "player.h"
#include "time_util.h"
#include "level.h"
#include "wid_game_map.h"
#include "color.h"
#include "glapi.h"
#include "ttf.h"
#include "wid.h"
#include "wid_popup.h"
#include "wid_textbox.h"

thingp player;

uint8_t player_init (void)
{
    return (true);
}

void player_fini (void)
{
    player = 0;
}

uint8_t player_move (levelp level)
{
    uint8_t right = 0;
    uint8_t left  = 0;
    uint8_t up    = 0;
    uint8_t down  = 0;
    uint8_t fire  = 0;
    uint8_t jump  = 0;
    uint8_t bomb  = 0;
    uint8_t rope  = 0;
    uint8_t torch  = 0;
    uint8_t run = sdl_shift_held;

#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION == 2 /* { */
    {
        uint8_t *state = SDL_GetKeyState(0);

        right  = state[SDLK_RIGHT] ? 1 : 0;
        left   = state[SDLK_LEFT] ? 1 : 0;
        up     = state[SDLK_UP] ? 1 : 0;
        down   = state[SDLK_DOWN] ? 1 : 0;
        fire   = state[SDLK_SPACE] ? 1 : 0;
        jump   = state[SDLK_z] ? 1 : 0;
        bomb   = state[SDLK_b] ? 1 : 0;
        rope   = state[SDLK_s] ? 1 : 0;
        torch  = state[SDLK_t] ? 1 : 0;
    }
#else /* } { */
    {
        const uint8_t *state = SDL_GetKeyboardState(0);

        right  = state[SDL_SCANCODE_RIGHT] ? 1 : 0;
        left   = state[SDL_SCANCODE_LEFT] ? 1 : 0;
        up     = state[SDL_SCANCODE_UP] ? 1 : 0;
        down   = state[SDL_SCANCODE_DOWN] ? 1 : 0;
        fire   = state[SDL_SCANCODE_SPACE] ? 1 : 0;
        jump   = state[SDL_SCANCODE_Z] ? 1 : 0;
        bomb   = state[SDL_SCANCODE_B] ? 1 : 0;
        rope   = state[SDL_SCANCODE_R] ? 1 : 0;
        torch  = state[SDL_SCANCODE_T] ? 1 : 0;
    }
#endif /* } */

    if (sdl_joy_buttons[SDL_JOY_BUTTON_UP]) {
        up = true;
    }

    if (sdl_joy_buttons[SDL_JOY_BUTTON_DOWN]) {
        down = true;
    }

    if (sdl_joy_buttons[SDL_JOY_BUTTON_LEFT]) {
        left = true;
    }

    if (sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT]) {
        right = true;
    }

    if (sdl_joy_buttons[SDL_JOY_BUTTON_LEFT_FIRE]) {
        fire = true;
    } else if (sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT_FIRE]) {
        run = true;
    } else if (sdl_joy_buttons[SDL_JOY_BUTTON_A]) {
        rope = true;
    } else if (sdl_joy_buttons[SDL_JOY_BUTTON_X]) {
        bomb = true;
    } else if (sdl_joy_buttons[SDL_JOY_BUTTON_Y]) {
        torch = true;
    }

    if (sdl_joy_axes) {
        if (sdl_joy_axes[3] > sdl_joy_deadzone) {
            right = true;
        }

        if (sdl_joy_axes[3] < -sdl_joy_deadzone) {
            left = true;
        }

        if (sdl_joy_axes[4] > sdl_joy_deadzone) {
            down = true;
        }

        if (sdl_joy_axes[4] < -sdl_joy_deadzone) {
            up = true;
        }

        if (sdl_joy_axes[0] > sdl_joy_deadzone) {
            right = true;
        }

        if (sdl_joy_axes[0] < -sdl_joy_deadzone) {
            left = true;
        }

        if (sdl_joy_axes[1] > sdl_joy_deadzone) {
            down = true;
        }

        if (sdl_joy_axes[1] < -sdl_joy_deadzone) {
            up = true;
        }
    }

    if (!player) {
        LOG("No player, cannot move");
        return (false);
    }

#if 0
    if (wid_menu_visible) {
        /*
         * Noisy
         *
        LOG("Menu present, ignore moves");
         */
        return (false);
    }
#endif

    if (!up && !down && !left && !right &&
        !fire &&
        !jump &&
        !bomb &&
        !rope &&
        !torch) {
        return (false);
    }

    static uint32_t last_tick;
    if (time_have_x_tenths_passed_since(1, last_tick)) {
        player_wid_update(level);
        last_tick = time_get_time_ms();
    }

    /*
     * If no key then we allow the console.
     */
    return (up || down || left || right || fire || jump || torch || bomb || rope);
}

uint8_t player_key (widp w, const SDL_KEYSYM *key)
{
#if 0
    if (wid_menu_visible) {
        return (false);
    }
#endif

    if (!player) {
        return (false);
    }

    {
        switch (key->sym) {
        case 'z':
            switch (key->mod) {
                case KMOD_LCTRL:
                case KMOD_RCTRL:
                    debug = !debug;
                    CON("debug %d", debug);
                    break;
                default:
                    break;
            }
            return (true);

        case 'p':

            MSG_SHOUT_AT(INFO, player, 0, 0, "pay TBD");
            break;

        case 'q':
#if 0
            wid_game_quit_visible();
#endif
            return (true);

        case SDLK_RETURN:
        case ' ':
        case '`':
            return (false);

        default:
            return (false);
        }
    }

    return (true);
}

uint8_t player_joy (widp w, int x, int y)
{
#if 0
    if (wid_menu_visible) {
        return (false);
    }
#endif

    if (sdl_joy_buttons[SDL_JOY_BUTTON_A]) {
        SDL_KEYSYM key = {0};
        key.sym = 'p';
        return (player_key(w, &key));
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_B]) {
        SDL_KEYSYM key = {0};
        key.sym = 'q';
        player_key(w, &key);
        return (true);
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_X]) {
        SDL_KEYSYM key = {0};
        key.sym = 'd';
        return (player_key(w, &key));
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_Y]) {
        SDL_KEYSYM key = {0};
        key.sym = '\t';
        return (player_key(w, &key));
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_TOP_LEFT]) {
        SDL_KEYSYM key = {0};
        key.mod = KMOD_SHIFT;
        key.sym = SDLK_LEFT;
        return (player_key(w, &key));
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_TOP_RIGHT]) {
        SDL_KEYSYM key = {0};
        key.mod = KMOD_SHIFT;
        key.sym = SDLK_RIGHT;
        return (player_key(w, &key));
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_LEFT_STICK_DOWN]) {
        SDL_KEYSYM key = {0};
        key.mod = KMOD_SHIFT;
        key.sym = SDLK_LEFT;
        return (player_key(w, &key));
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT_STICK_DOWN]) {
        SDL_KEYSYM key = {0};
        key.mod = KMOD_SHIFT;
        key.sym = SDLK_RIGHT;
        return (player_key(w, &key));
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_START]) {
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_XBOX]) {
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_BACK]) {
        SDL_KEYSYM key = {0};
        key.sym = 'q';
        player_key(w, &key);
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_UP]) {
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_DOWN]) {
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_LEFT]) {
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT]) {
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_LEFT_FIRE]) {
    }
    if (sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT_FIRE]) {
    }
    return (true);
}

void player_wid_update (levelp level)
{
    static char tmp[MAXSTR];

    if (!player) {
        return;
    }

    player_wid_destroy(level);

    {
        snprintf(tmp, sizeof(tmp), "%07u", player->score);

        game.wid_score_textbox = wid_popup(tmp,
                    "score",         /* title */
                    0.92f, 0.08,      /* x,y postition in percent */
                    small_font,      /* title font */
                    vlarge_font,      /* body font */
                    0,               /* button font */
                    0);              /* number args */
        wid_move_end(game.wid_score_textbox);
        wid_set_no_shape(game.wid_score_textbox);
    }

    {
        snprintf(tmp, sizeof(tmp), "%%%%fg=gold$$%%%%fg=white$%u", player->gold);

        game.wid_gold_textbox = wid_popup(tmp,
                    "gold",         /* title */
                    0.5f, 0.08,      /* x,y postition in percent */
                    small_font,      /* title font */
                    vlarge_font,      /* body font */
                    0,               /* button font */
                    0);              /* number args */
        wid_move_end(game.wid_gold_textbox);
        wid_set_no_shape(game.wid_gold_textbox);
    }

    /*
     * Health and torches and ropes
     */
    {
        double y1 = 0.07;
        double y2 = 0.09;
        double dx = 0.02;
        fpoint tl = { 0.0, 0.0 };
        fpoint br = { 0.04, 0.06 };
        fpoint br2 = { 0.25, 0.18 };

        {
            double x1 = 0.05;

            {
                game.wid_hp_icon = wid_new_window("icon-hp");
                wid_set_pos_pct(game.wid_hp_icon, tl, br);

                if (thing_is_dead(player)) {
                    wid_set_tilename(game.wid_hp_icon, "icon-skull");
                } else if (player->hp > 0) {
                    wid_set_tilename(game.wid_hp_icon, "icon-hp");
                } else {
                    wid_set_tilename(game.wid_hp_icon, "icon-skull");
                }

                wid_set_no_shape(game.wid_hp_icon);
                wid_set_do_not_lower(game.wid_hp_icon, true);
                wid_move_to_pct_centered(game.wid_hp_icon, x1, y1);
            }

            if (player->hp > 0) {

                game.wid_hp_text = wid_new_window("text-hp");
                wid_set_pos_pct(game.wid_hp_text, tl, br2);
                wid_set_no_shape(game.wid_hp_text);
                wid_set_do_not_lower(game.wid_hp_text, true);
                wid_move_to_pct_centered(game.wid_hp_text, x1 + dx, y2);

                wid_set_font(game.wid_hp_text, vlarge_font);
                snprintf(tmp, sizeof(tmp), "x %u", player->hp);
                wid_set_text(game.wid_hp_text, tmp);
                wid_set_text_outline(game.wid_hp_text, true);
            }
        }
    }
}

void player_wid_destroy (levelp level)
{
    if (game.wid_score_textbox) {
        wid_destroy_nodelay(&game.wid_score_textbox);
        game.wid_score_textbox = 0;
    }

    if (game.wid_gold_textbox) {
        wid_destroy_nodelay(&game.wid_gold_textbox);
        game.wid_gold_textbox = 0;
    }


    if (game.wid_hp_icon) {
        wid_destroy_nodelay(&game.wid_hp_icon);
        game.wid_hp_icon = 0;
    }

    if (game.wid_hp_text) {
        wid_destroy_nodelay(&game.wid_hp_text);
        game.wid_hp_text = 0;
    }
}
