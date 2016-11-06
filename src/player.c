/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#include "main.h"
#include "player.h"
#include "time_util.h"
#include "wid_popup.h"

thingp player;

uint8_t player_init (void)
{
    return (true);
}

void player_fini (void)
{
    player = 0;
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
