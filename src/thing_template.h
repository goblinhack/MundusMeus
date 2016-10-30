/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file.
 */

#pragma once

#include "tree.h"

#define IS_JOIN_ALT_MAX 20

enum {
    IS_JOIN_BLOCK,
    IS_JOIN_HORIZ,
    IS_JOIN_VERT,
    IS_JOIN_NODE,
    IS_JOIN_LEFT,
    IS_JOIN_RIGHT,
    IS_JOIN_TOP,
    IS_JOIN_BOT,
    IS_JOIN_TL,
    IS_JOIN_TR,
    IS_JOIN_BL,
    IS_JOIN_BR,
    IS_JOIN_T,
    IS_JOIN_T90,
    IS_JOIN_T180,
    IS_JOIN_T270,
    IS_JOIN_X,
    IS_JOIN_TL2,
    IS_JOIN_TR2,
    IS_JOIN_BL2,
    IS_JOIN_BR2,
    IS_JOIN_T_1,
    IS_JOIN_T_2,
    IS_JOIN_T_3,
    IS_JOIN_T90_1,
    IS_JOIN_T90_2,
    IS_JOIN_T90_3,
    IS_JOIN_T180_1,
    IS_JOIN_T180_2,
    IS_JOIN_T180_3,
    IS_JOIN_T270_1,
    IS_JOIN_T270_2,
    IS_JOIN_T270_3,
    IS_JOIN_X1,
    IS_JOIN_X1_270,
    IS_JOIN_X1_180,
    IS_JOIN_X1_90,
    IS_JOIN_X2,
    IS_JOIN_X2_270,
    IS_JOIN_X2_180,
    IS_JOIN_X2_90,
    IS_JOIN_X3,
    IS_JOIN_X3_180,
    IS_JOIN_X4,
    IS_JOIN_X4_270,
    IS_JOIN_X4_180,
    IS_JOIN_X4_90,
    IS_JOIN_VERT2,
    IS_JOIN_HORIZ2,
    IS_JOIN_MAX,
};

typedef struct tp_ {
    tree_key_string tree;
    tree_key_int tree2;

    int tp_id;

    /*
     * Internal description of the thing.
     */
    char *short_name;

    /*
     * Base filename
     */
    char *raw_name;

    /*
     * What shade of light does this thing emit
     */
    char *light_tint;
    color light_color;

    /*
     * In relation to other widgets, where are we.
     */
    uint8_t z_depth;
    uint8_t z_order;

    /*
     * Animation tiles.
     */
    tree_rootp tiles;

    /*
     * Speed in milliseconds it takes to move one tile.
     */
    int speed;

    int blit_top_off;
    int blit_bot_off;
    int blit_left_off;
    int blit_right_off;

    /*
     * How much light it gives off in tiles.
     */
    float light_radius;
    double light_pulse_amount;

    /*
     * How large or small the thing is blitted as.
     */
    float scale;

    uint8_t has_dir_anim:1;
    uint8_t is_animated:1;
    uint8_t is_animated_no_dir:1;
    uint8_t is_animation:1;
    uint8_t is_candle_light:1;
    uint8_t is_cats_eyes:1;
    uint8_t is_door:1;
    uint8_t is_water:1;
    uint8_t is_lava:1;
    uint8_t is_floor:1;
    uint8_t is_effect_fade_in_out:1;
    uint8_t is_effect_pulse:1;
    uint8_t is_effect_rotate_2way:1;
    uint8_t is_effect_sway:1;
    uint8_t is_food:1;
    uint8_t is_weapon:1;
    uint8_t is_joinable:1;
    uint8_t is_light_source:1;
    uint8_t is_monst:1;
    uint8_t is_not_light_blocking:1;
    uint8_t is_player:1;
    uint8_t is_shadow_caster:1;
    uint8_t is_shadow_caster_soft:1;
    uint8_t is_sleeping:1;
    uint8_t is_wall:1;
    uint8_t is_hidden:1;
    uint8_t is_explosion:1;
    uint8_t is_rrr1:1;
    uint8_t is_rrr2:1;
    uint8_t is_grass:1;
    uint8_t is_bridge:1;
    uint8_t is_movement_blocking:1;
    uint8_t is_treasure:1;
    uint8_t is_entrance:1;
    uint8_t is_exit:1;
    uint8_t is_chasm_smoke:1;
    uint8_t is_key:1;
    uint8_t is_dusty:1;
    uint8_t is_corridor:1;
    uint8_t is_healing:1;
    uint8_t is_armor:1;
    uint8_t is_magical:1;

    thing_tilep tilep_join[IS_JOIN_MAX][IS_JOIN_ALT_MAX];
    tilep tilep_join_tile[IS_JOIN_MAX][IS_JOIN_ALT_MAX];
    uint8_t tilep_join_count[IS_JOIN_MAX];
} thing_template;

uint8_t tp_init(void);
void tp_fini(void);
tpp tp_load(int id, const char *file);
tpp tp_find(const char *name);
tpp tp_find_short_name(const char *name);
uint8_t thing_test(int argc, char *argv[]);
tpp string2thing_template(const char **s);

const char *tp_name(tpp);
const char *tp_short_name(tpp);
const char *tp_raw_name(tpp);
tpp tp_fires(tpp);
const char *tp_light_tint(tpp);
color tp_light_color(tpp);
int tp_get_z_depth(tpp);
int tp_get_z_depth(tpp);
int tp_get_z_order(tpp);
int tp_get_speed(tpp);
int tp_get_blit_top_off(tpp);
int tp_get_blit_bot_off(tpp);
int tp_get_blit_left_off(tpp);
int tp_get_blit_right_off(tpp);
double tp_get_light_radius(tpp);
double tp_get_scale(tpp);

tree_rootp tp_get_tiles(tpp);

extern tree_rootp thing_templates;
extern tree_rootp thing_templates_create_order;

static inline int tp_to_id (tpp t)
{
    return (t->tp_id);
}

static inline tpp id_to_tp (int id)
{
    if (!id) {
        return (0);
    }

    extern thing_template thing_templates_chunk[TP_MAX];

    if (id >= TP_MAX) {
        DIE("overflow, bad template id %08X", id);
    }

    return (&thing_templates_chunk[id]);
}

static inline uint8_t tp_is_floor (tpp t)
{
    return (t->is_floor);
}

static inline uint8_t tp_is_wall (tpp t)
{
    return (t->is_wall);
}

static inline uint8_t tp_is_joinable (tpp t)
{
    return (t->is_joinable);
}

static inline uint8_t tp_is_door (tpp t)
{
    return (t->is_door);
}

static inline uint8_t tp_is_lava (tpp t)
{
    return (t->is_lava);
}

static inline uint8_t tp_is_water (tpp t)
{
    return (t->is_water);
}

static inline uint8_t tp_is_sleeping (tpp t)
{
    return (t->is_sleeping);
}

static inline uint8_t tp_is_food (tpp t)
{
    return (t->is_food);
}

static inline uint8_t tp_is_weapon (tpp t)
{
    return (t->is_weapon);
}

static inline uint8_t tp_is_monst (tpp t)
{
    return (t->is_monst);
}

static inline uint8_t tp_is_player (tpp t)
{
    return (t->is_player);
}

static inline uint8_t tp_is_shadow_caster (tpp t)
{
    return (t->is_shadow_caster);
}

static inline uint8_t tp_is_shadow_caster_soft (tpp t)
{
    return (t->is_shadow_caster_soft);
}

static inline uint8_t tp_is_light_source (tpp t)
{
    return (t->is_light_source);
}

static inline uint8_t tp_is_candle_light (tpp t)
{
    return (t->is_candle_light);
}

static inline uint8_t tp_is_cats_eyes (tpp t)
{
    return (t->is_cats_eyes);
}

static inline uint8_t tp_is_animation (tpp t)
{
    return (t->is_animation);
}

static inline uint8_t tp_is_animated (tpp t)
{
    return (t->is_animated);
}

static inline uint8_t tp_is_animated_no_dir (tpp t)
{
    return (t->is_animated_no_dir);
}

static inline uint8_t tp_is_rrr1 (tpp t)
{
    return (t->is_rrr1);
}

static inline uint8_t tp_is_rrr2 (tpp t)
{
    return (t->is_rrr2);
}

static inline uint8_t tp_is_grass (tpp t)
{
    return (t->is_grass);
}

static inline uint8_t tp_is_bridge (tpp t)
{
    return (t->is_bridge);
}

static inline uint8_t tp_is_movement_blocking (tpp t)
{
    return (t->is_movement_blocking);
}

static inline uint8_t tp_is_treasure (tpp t)
{
    return (t->is_treasure);
}

static inline uint8_t tp_is_entrance (tpp t)
{
    return (t->is_entrance);
}

static inline uint8_t tp_is_exit (tpp t)
{
    return (t->is_exit);
}

static inline uint8_t tp_is_chasm_smoke (tpp t)
{
    return (t->is_chasm_smoke);
}

static inline uint8_t tp_is_key (tpp t)
{
    return (t->is_key);
}

static inline uint8_t tp_is_dusty (tpp t)
{
    return (t->is_dusty);
}

static inline uint8_t tp_is_corridor (tpp t)
{
    return (t->is_corridor);
}

static inline double tp_light_pulse_amount (tpp t)
{
    return (t->light_pulse_amount);
}

static inline uint8_t tp_is_healing (tpp t)
{
    return (t->is_healing);
}

static inline uint8_t tp_is_armor (tpp t)
{
    return (t->is_armor);
}

static inline uint8_t tp_is_magical (tpp t)
{
    return (t->is_magical);
}

static inline uint8_t tp_is_not_light_blocking (tpp t)
{
    return (t->is_not_light_blocking);
}

static inline uint8_t tp_is_effect_sway (tpp t)
{
    return (t->is_effect_sway);
}

static inline uint8_t tp_is_effect_pulse (tpp t)
{
    return (t->is_effect_pulse);
}

static inline uint8_t tp_is_effect_rotate_2way (tpp t)
{
    return (t->is_effect_rotate_2way);
}

static inline uint8_t tp_is_hidden (tpp t)
{
    return (t->is_hidden);
}

static inline uint8_t tp_is_explosion (tpp t)
{
    return (t->is_explosion);
}

static inline uint8_t tp_is_effect_fade_in_out (tpp t)
{
    return (t->is_effect_fade_in_out);
}
