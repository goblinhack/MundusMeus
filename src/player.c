/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#include "main.h"
#include "thing.h"
#include "player.h"
#include "wid_menu.h"
#include "time_util.h"
#include "level.h"
#include "thing_shop.h"
#include "wid_game_map.h"
#include "wid_game_quit.h"
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

    if (wid_menu_visible) {
        /*
         * Noisy
         *
        LOG("Menu present, ignore moves");
         */
        return (false);
    }

    if (!up && !down && !left && !right && 
        !fire && 
        !jump && 
        !bomb && 
        !rope && 
        !torch) {
        return (false);
    }

    if (thing_is_dead(player)) {
        return (false);
    }

    /*
     * If no longer visible it may mean we have finished the level and are 
     * waiting for others to finish.
     */
    if (!thing_is_visible(level, player)) {
        LOG("Player is not visible, cannot move");
        return (false);
    }

    static uint32_t last_tick;
    if (time_have_x_tenths_passed_since(1, last_tick)) {
        player_wid_update(level);
        last_tick = time_get_time_ms();
    }

    /*
     * Check if we are allowed to fire our gun again so soon.
     */
    if (fire) {
        tpp weapon = thing_weapon(player);

        if (!weapon) {
            THING_LOG(player, "Tried to fire but no weapon");
            fire = 0;
        }

        if (fire) {
            static uint32_t last_fired = 0;

            uint32_t delay = tp_get_weapon_fire_delay_hundredths(weapon);
            
            if (!time_have_x_hundredths_passed_since(delay, last_fired)) {
                fire = 0;

                if (!up && !down && !left && !right && !jump) {
                    return (false);
                }
            }

            if (fire) {
                last_fired = time_get_time_ms();
            }
        }
    }

    double submerged = thing_is_partially_or_fully_submerged(level, player);

    if (up) {
        if (!submerged) {
            if (!thing_overlaps(level, player, player->x, player->y -0.5, 
                                thing_is_climbable) &&
                !thing_overlaps(level, player, player->x, player->y,      
                                thing_is_climbable)) {
                up = 0;
            }
        }
    }

    if (down) {
        if (!submerged) {
            if (!thing_overlaps(level, player, player->x, player->y + 0.5, 
                                thing_is_climbable)) {
                down = 0;
            }
        }
    }

    if (!time_have_x_hundredths_passed_since(2, level->last_moved)) {
        double x = player->x;
        double y = player->y;

        thing_move(level, player, x, y, false, false, false, false, fire);
        return (false);
    }

    level->last_moved = time_get_time_ms();

    double x = player->x;
    double y = player->y;

    double max_momentum = 0.5;
    double move_momentum = 0.012;
    double jump_speed = 0.15;
    double wall_friction = 0.95;

    /*
     * run?
     */
    if (sdl_shift_held) {
        max_momentum = 1.0;
        move_momentum = 0.020;
        jump_speed = 0.20;
    }

    /*
     * Don't move too fast either way as we build up speed
     */
    if (left) {
        player->momentum -= move_momentum;
        if (player->momentum <= -max_momentum) {
            player->momentum = -max_momentum;
        }
    }

    if (right) {
        player->momentum += move_momentum;
        if (player->momentum >= max_momentum) {
            player->momentum = max_momentum;
        }
    }

    /*
     * Don't allow too frequent jumps
     */
    if (jump) {
        if (player->is_submerged) {
            if (!time_have_x_hundredths_passed_since(50, level->last_jumped)) {
                jump = 0;
            }
        } else if (player->is_partially_submerged) {
            if (!time_have_x_hundredths_passed_since(25, level->last_jumped)) {
                jump = 0;
            }
        } else {
            if (!time_have_x_hundredths_passed_since(10, level->last_jumped)) {
                jump = 0;
            }
        }
    }

    if (jump) {
        if (level->last_hit_obstacle &&
            !time_have_x_hundredths_passed_since(15, level->last_hit_obstacle)) {

            /*
             * Allow the player to cling onto and jump when they hit a ledge 
             * as long as not falling too fast
             */
            if (player->fall_speed < THING_FALL_SPEED_CLING_ONTO_WALLS) {
                if (!player->jump_speed) {
                    player->jump_speed = jump_speed;
                }
            }
        }

        /*
         * Else if not falling, allow the jump
         */
        if (!player->fall_speed) {
            if (!player->jump_speed) {
                player->jump_speed = jump_speed;
            }
        }
    }

    if (submerged && jump) {
        jump = 0;
        up = 1;
    }

    if (bomb) {
        if (!time_have_x_hundredths_passed_since(15, level->last_bomb)) {
            bomb = 0;
        }

        if (bomb) {
            level->last_bomb = time_get_time_ms();

            thing_place_bomb(level, player, player->x, player->y);
        }
    }

    if (rope) {
        if (!time_have_x_hundredths_passed_since(15, level->last_rope)) {
            rope = 0;
        }

        if (rope) {
            level->last_rope = time_get_time_ms();

            thing_place_ropetop(level, player, player->x, player->y);
        }
    }

    if (torch) {
        if (!time_have_x_hundredths_passed_since(15, level->last_torch)) {
            torch = 0;
        }

        if (torch) {
            level->last_torch = time_get_time_ms();

            thing_place_torch(level, player, player->x, player->y);
        }
    }

    /*
     * If we hit a side wall when falling, slow the fall.
     */
    if (thing_hit_solid_obstacle(level, player, x + player->momentum, y)) {
        level->last_hit_obstacle = time_get_time_ms();

        if (player->fall_speed) {
            player->fall_speed *= wall_friction;
        }

        player->momentum = 0;
    }

    double lr_delta = player->momentum;
    double ud_delta = 0.1;

    x += lr_delta;
    y -= (double)up * ud_delta;
    y += (double)down * ud_delta;

    thing_move(level, player, x, y, up, down, left, right, fire);

    if (jump) {
        level->last_jumped = time_get_time_ms();
    }

    /*
     * If no key then we allow the console.
     */
    return (up || down || left || right || fire || jump || torch || bomb || rope);
}

uint8_t player_key (widp w, const SDL_KEYSYM *key)
{
    levelp level = &game.level;

    if (wid_menu_visible) {
        return (false);
    }

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

            if (player->in_shop_owned_by_thing_id) {
                shop_pay_for_items(level, player);
            } else {
                MSG_SHOUT_AT(INFO, player, 0, 0, "I'm not in a shop");
            }
            break;

        case 'q':
            if (level && level->is_test_level) {
                level_destroy(&level, false /* keep player */);
                wid_game_map_go_back_to_editor();
                return (true);
            }

            wid_game_quit_visible();
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
    if (wid_menu_visible) {
        return (false);
    }

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
                wid_set_tl_br_pct(game.wid_hp_icon, tl, br);

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
                wid_set_tl_br_pct(game.wid_hp_text, tl, br2);
                wid_set_no_shape(game.wid_hp_text);
                wid_set_do_not_lower(game.wid_hp_text, true);
                wid_move_to_pct_centered(game.wid_hp_text, x1 + dx, y2);

                wid_set_font(game.wid_hp_text, vlarge_font);
                snprintf(tmp, sizeof(tmp), "x %u", player->hp);
                wid_set_text(game.wid_hp_text, tmp);
                wid_set_text_outline(game.wid_hp_text, true);
            }
        }

        if (player->torches) {

            double x1 = 0.12;

            {
                game.wid_torches_icon = wid_new_window("icon-torches");
                wid_set_tl_br_pct(game.wid_torches_icon, tl, br);
                wid_set_tilename(game.wid_torches_icon, "icon-torches");
                wid_set_no_shape(game.wid_torches_icon);
                wid_set_do_not_lower(game.wid_torches_icon, true);
                wid_move_to_pct_centered(game.wid_torches_icon, x1, y1);
            }
            {
                game.wid_torches_text = wid_new_window("text-torches");
                wid_set_tl_br_pct(game.wid_torches_text, tl, br2);
                wid_set_no_shape(game.wid_torches_text);
                wid_set_do_not_lower(game.wid_torches_text, true);
                wid_move_to_pct_centered(game.wid_torches_text, x1 + dx, y2);

                wid_set_font(game.wid_torches_text, vlarge_font);
                snprintf(tmp, sizeof(tmp), "x %u", player->torches);
                wid_set_text(game.wid_torches_text, tmp);
                wid_set_text_outline(game.wid_torches_text, true);
            }
        }

        {
            double x1 = 0.19;

            {
                game.wid_ropes_icon = wid_new_window("icon-ropes");
                wid_set_tl_br_pct(game.wid_ropes_icon, tl, br);
                wid_set_tilename(game.wid_ropes_icon, "icon-ropes");
                wid_set_no_shape(game.wid_ropes_icon);
                wid_set_do_not_lower(game.wid_ropes_icon, true);
                wid_move_to_pct_centered(game.wid_ropes_icon, x1, y1);
            }
            {
                game.wid_ropes_text = wid_new_window("text-ropes");
                wid_set_tl_br_pct(game.wid_ropes_text, tl, br2);
                wid_set_no_shape(game.wid_ropes_text);
                wid_set_do_not_lower(game.wid_ropes_text, true);
                wid_move_to_pct_centered(game.wid_ropes_text, x1 + dx, y2);

                wid_set_font(game.wid_ropes_text, vlarge_font);
                snprintf(tmp, sizeof(tmp), "x %u", player->ropes);
                wid_set_text(game.wid_ropes_text, tmp);
                wid_set_text_outline(game.wid_ropes_text, true);
            }
        }
    }

    /*
     * Bombs and keys
     */
    {
        double y1 = 0.15;
        double y2 = 0.17;
        double dx = 0.02;
        fpoint tl = { 0.0, 0.0 };
        fpoint br = { 0.04, 0.07 };
        fpoint br2 = { 0.25, 0.18 };

        if (player->keys) {

            double x1 = 0.88;

            {
                game.wid_keys_icon = wid_new_window("icon-keys");
                wid_set_tl_br_pct(game.wid_keys_icon, tl, br);
                wid_set_tilename(game.wid_keys_icon, "icon-keys");
                wid_set_no_shape(game.wid_keys_icon);
                wid_set_do_not_lower(game.wid_keys_icon, true);
                wid_move_to_pct_centered(game.wid_keys_icon, x1, y1);
            }
            {
                game.wid_keys_text = wid_new_window("text-keys");
                wid_set_tl_br_pct(game.wid_keys_text, tl, br2);
                wid_set_no_shape(game.wid_keys_text);
                wid_set_do_not_lower(game.wid_keys_text, true);
                wid_move_to_pct_centered(game.wid_keys_text, x1 + dx, y2);

                wid_set_font(game.wid_keys_text, vlarge_font);
                snprintf(tmp, sizeof(tmp), "x %u", player->keys);
                wid_set_text(game.wid_keys_text, tmp);
                wid_set_text_outline(game.wid_keys_text, true);
            }
        }

        if (player->bombs) {

            double x1 = 0.95;

            {
                game.wid_bombs_icon = wid_new_window("icon-bombs");
                wid_set_tl_br_pct(game.wid_bombs_icon, tl, br);
                wid_set_tilename(game.wid_bombs_icon, "icon-bombs");
                wid_set_no_shape(game.wid_bombs_icon);
                wid_set_do_not_lower(game.wid_bombs_icon, true);
                wid_move_to_pct_centered(game.wid_bombs_icon, x1, y1);
            }
            {
                game.wid_bombs_text = wid_new_window("text-bombs");
                wid_set_tl_br_pct(game.wid_bombs_text, tl, br2);
                wid_set_no_shape(game.wid_bombs_text);
                wid_set_do_not_lower(game.wid_bombs_text, true);
                wid_move_to_pct_centered(game.wid_bombs_text, x1 + dx, y2);

                wid_set_font(game.wid_bombs_text, vlarge_font);
                snprintf(tmp, sizeof(tmp), "x %u", player->bombs);
                wid_set_text(game.wid_bombs_text, tmp);
                wid_set_text_outline(game.wid_bombs_text, true);
            }
        }
    }
    
    /*
     * Drowning and torches and ropes
     */
    if (player->breath) {
        double y1 = 0.15;
        fpoint tl = { 0.0, 0.0 };
        fpoint br = { 0.03, 0.05 };

        double x1 = 0.05;

        {
            int i;

            int max = ARRAY_SIZE(game.wid_drown_icon);
            int n = max - player->breath;
            if (n > max) {
                n = max;
            }

            for (i = 0; i < n; i++) {

                widp w = game.wid_drown_icon[i] = wid_new_window("icon-drown");
                wid_set_tl_br_pct(w, tl, br);

                wid_set_tilename(w, "icon-drown");

                wid_set_no_shape(w);
                wid_set_do_not_lower(w, true);
                wid_move_to_pct_centered(w, x1, y1);

                wid_effect_pulses(w);
                wid_bounce_to_pct_in(w, ((double) max - n) * 0.01, 0.8, 500, 999);

                x1 += 0.03;
            }

            wid_effect_pulses(player->wid);
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


    if (game.wid_torches_icon) {
        wid_destroy_nodelay(&game.wid_torches_icon);
        game.wid_torches_icon = 0;
    }

    if (game.wid_torches_text) {
        wid_destroy_nodelay(&game.wid_torches_text);
        game.wid_torches_text = 0;
    }


    if (game.wid_hp_icon) {
        wid_destroy_nodelay(&game.wid_hp_icon);
        game.wid_hp_icon = 0;
    }

    if (game.wid_hp_text) {
        wid_destroy_nodelay(&game.wid_hp_text);
        game.wid_hp_text = 0;
    }


    if (game.wid_bombs_icon) {
        wid_destroy_nodelay(&game.wid_bombs_icon);
        game.wid_bombs_icon = 0;
    }

    if (game.wid_bombs_text) {
        wid_destroy_nodelay(&game.wid_bombs_text);
        game.wid_bombs_text = 0;
    }


    if (game.wid_keys_icon) {
        wid_destroy_nodelay(&game.wid_keys_icon);
        game.wid_keys_icon = 0;
    }

    if (game.wid_keys_text) {
        wid_destroy_nodelay(&game.wid_keys_text);
        game.wid_keys_text = 0;
    }


    if (game.wid_ropes_icon) {
        wid_destroy_nodelay(&game.wid_ropes_icon);
        game.wid_ropes_icon = 0;
    }

    if (game.wid_ropes_text) {
        wid_destroy_nodelay(&game.wid_ropes_text);
        game.wid_ropes_text = 0;
    }

    {
        int max = ARRAY_SIZE(game.wid_drown_icon);

        int i;
        for (i = 0; i < max; i++) {
            if (game.wid_drown_icon[i]) {
                wid_destroy_nodelay(&game.wid_drown_icon[i]);
                game.wid_drown_icon[i] = 0;
            }
        }
    }
}
