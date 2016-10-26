/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */



void thing_effect_hit_miss (levelp level, thingp t)
{
    verify(t);
CON("todo miss");
return;

#if 0
    widp w = t->wid;
    if (w) {
        wid_set_mode(w, WID_MODE_ACTIVE);
        level_place_hit_miss(level,
                             0, // owner
                             t->x, t->y);
    }
#endif
}

void thing_effect_hit_success (levelp level, thingp t)
{
    verify(t);

CON("todo success");
return;
#if 0
    const char *sound = tp_sound_on_hit(thing_tp(t));
    if (sound) {
        MSG_SHOUT_AT(SOUND, t, t->x, t->y, "%s", sound);
    }
#endif

#if 0
    widp w = t->wid;
    if (w) {
        wid_set_mode(w, WID_MODE_ACTIVE);
        wid_set_color(w, WID_COLOR_BLIT, RED);

        /*
         * Shake the screen.
         */
        if (thing_is_player(t)) {
            sdl_joy_rumble(1.0, 2000);
        }

        /*
         * Shake the screen.
         */
        if (thing_is_player(t)) {
            wid_shake_to_pct_in(game.wid_grid,
                                0.01, 0.9, 100, 5);

            sdl_joy_rumble(1.0, 1000);
        } else if (thing_is_monst(t)) {
            wid_shake_to_pct_in(w, 0.1, 0.9, 100, 5);
        } else {
            wid_shake_to_pct_in(w, 0.2, 0.9, 100, 5);
        }

        if (thing_is_warm_blooded(t)) {
            level_place_blood(level,
                              0, // owner
                              t->x, t->y);
        } else {
            level_place_hit_success(level,
                                    0, // owner
                                    t->x, t->y);
        }
    }
#endif
}

void thing_effect_flames (levelp level, thingp t)
{
    verify(t);

    widp w = t->wid;
    if (w) {
        wid_set_mode(w, WID_MODE_ACTIVE);
        wid_set_color(w, WID_COLOR_BLIT, ORANGE);

        /*
         * Shake the screen.
         */
        if (thing_is_player(t)) {
            sdl_joy_rumble(1.0, 2000);
        }

        level_place_flames(level,
                          0, // owner
                          t->x, t->y);
    }
}

void thing_effect_hit_crit (levelp level, thingp t)
{
    verify(t);

    widp w = t->wid;
    if (w) {
        wid_set_mode(w, WID_MODE_ACTIVE);
        wid_set_color(w, WID_COLOR_BLIT, RED);

        /*
         * Shake the screen.
         */
        if (thing_is_player(t)) {
            wid_shake_to_pct_in(game.wid_grid,
                                0.01, 0.9, 100, 5);

            sdl_joy_rumble(1.0, 1000);
        } else if (thing_is_monst(t)) {
            wid_shake_to_pct_in(w, 0.1, 0.9, 100, 5);
        } else {
            wid_shake_to_pct_in(w, 0.2, 0.9, 100, 5);
        }

        level_place_hit_success(level,
                                0, // owner
                                t->x, t->y);
    }
}

void thing_effect_power_up (levelp level, thingp t)
{
    double rad;

    for (rad = 0.0; rad < RAD_360; rad += RAD_360 / 360.0) {
        widp w = t->wid;
        double px, py;

        wid_get_pct(w, &px, &py);

        fpoint tl = {0.0, 0.0};
        fpoint br = {0.1, 0.1};

        br.y =
            ((double)game.video_gl_width /
             (double)game.video_gl_height) / 10.0;

        widp power_up = wid_new_square_window("sparkle");
        wid_set_pos_pct(power_up, tl, br);
        wid_set_tilename(power_up, "powerup1.1");

        int delay = gaussrand(200, 500);
        if (delay < 100) {
            delay = 100;
        }

        double width = 0.05;
        px += fcos(rad) * width;
        py += (fsin(rad) * width) *
                        ((double)game.video_gl_width /
                         (double)game.video_gl_height);

        {
            double width = wid_get_width(w) /
                            (double)game.video_gl_width;
            px -= width / 8.0;

            double height = wid_get_height(w) /
                            (double)game.video_gl_height;
            py += height / 4.0;
        }

        wid_move_to_pct_centered(power_up, px, py);
        px += gaussrand(0.0, width/5.0);
        py += gaussrand(0.0, width/5.0);
        wid_move_to_pct_centered_in(power_up, px, py, delay);

        wid_set_no_shape(power_up);
        wid_set_mode(power_up, WID_MODE_NORMAL);

        wid_set_color(power_up, WID_COLOR_BLIT, WHITE);

        wid_fade_out(power_up, delay);
        wid_destroy_in(power_up, delay);
        wid_scaling_blit_to_pct_in(power_up, 0.1, 1.0, delay * 2, 0);
    }
}
