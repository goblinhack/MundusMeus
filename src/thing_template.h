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
    uint8_t is_rrr3:1;
    uint8_t is_rrr4:1;
    uint8_t is_rrr5:1;
    uint8_t is_rrr6:1;
    uint8_t is_rrr7:1;
    uint8_t is_rrr8:1;
    uint8_t is_rrr9:1;
    uint8_t is_rrr10:1;
    uint8_t is_rrr11:1;
    uint8_t is_rrr12:1;
    uint8_t is_rrr13:1;
    uint8_t is_rrr14:1;
    uint8_t is_rrr15:1;
    uint8_t is_rrr16:1;
    uint8_t is_rrr17:1;
    uint8_t is_rrr18:1;
    uint8_t is_rrr19:1;
    uint8_t is_rrr20:1;
    uint8_t is_rrr21:1;
    uint8_t is_rrr22:1;
    uint8_t is_rrr23:1;
    uint8_t is_rrr24:1;
    uint8_t is_rrr25:1;
    uint8_t is_rrr26:1;
    uint8_t is_rrr27:1;
    uint8_t is_rrr28:1;
    uint8_t is_rrr29:1;
    uint8_t is_rrr30:1;
    uint8_t is_rrr31:1;
    uint8_t is_rrr32:1;
    uint8_t is_rrr33:1;
    uint8_t is_rrr34:1;
    uint8_t is_rrr35:1;
    uint8_t is_rrr36:1;
    uint8_t is_rrr37:1;
    uint8_t is_rrr38:1;
    uint8_t is_rrr39:1;
    uint8_t is_rrr40:1;
    uint8_t is_rrr41:1;
    uint8_t is_rrr42:1;
    uint8_t is_rrr43:1;
    uint8_t is_rrr44:1;
    uint8_t is_rrr45:1;
    uint8_t is_rrr46:1;
    uint8_t has_shadow:1;
    uint8_t is_chair:1;
    uint8_t is_chest:1;
    uint8_t is_bookcase:1;
    uint8_t is_table:1;
    uint8_t is_lawn:1;
    uint8_t is_carpet_deco:1;
    uint8_t is_carpet:1;
    uint8_t is_house_item:1;
    uint8_t is_cwall:1;
    uint8_t is_animated_lr_flip:1;
    uint8_t is_gravel_snow_deco:1;
    uint8_t is_landrock_snow:1;
    uint8_t is_hidden_from_editor:1;
    uint8_t is_hwall:1;
    uint8_t is_dungeon_item:1;
    uint8_t is_difficulty_easy:1;
    uint8_t is_difficulty_hard:1;
    uint8_t is_difficulty_nightmare:1;
    uint8_t is_world_item:1;
    uint8_t is_solid_ground:1;
    uint8_t is_dirt_deco:1;
    uint8_t is_dirt_snow_deco:1;
    uint8_t is_grass_deco:1;
    uint8_t is_grass_snow_deco:1;
    uint8_t is_gravel_deco:1;
    uint8_t is_sand_deco:1;
    uint8_t is_sand_snow_deco:1;
    uint8_t is_snow_deco:1;
    uint8_t is_sand_snow:1;
    uint8_t is_dirt_snow:1;
    uint8_t is_grass_snow:1;
    uint8_t is_road_snow:1;
    uint8_t is_road:1;
    uint8_t is_dungeon_snow:1;
    uint8_t is_dungeon:1;
    uint8_t is_focus:1;
    uint8_t is_gravel_snow:1;
    uint8_t is_tree_conifer:1;
    uint8_t is_snow_mound:1;
    uint8_t is_rock:1;
    uint8_t is_ice:1;
    uint8_t is_gravel:1;
    uint8_t is_small_rock:1;
    uint8_t is_marsh_plant:1;
    uint8_t is_landrock:1;
    uint8_t is_snow:1;
    uint8_t is_dirt:1;
    uint8_t is_sand:1;
    uint8_t is_plant:1;
    uint8_t is_tree:1;
    uint8_t is_grass:1;
    uint8_t is_bridge:1;
    uint8_t is_movement_blocking:1;
    uint8_t is_treasure:1;
    uint8_t is_dungeon_way_up:1;
    uint8_t is_dungeon_way_down:1;
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
double tp_get_light_radius(tpp);
double tp_get_scale(tpp);

static inline int32_t tp_get_blit_top_off (tpp t)
{
    return (t->blit_top_off);
}

static inline int32_t tp_get_blit_bot_off (tpp t)
{
    return (t->blit_bot_off);
}

static inline int32_t tp_get_blit_left_off (tpp t)
{
    return (t->blit_left_off);
}

static inline int32_t tp_get_blit_right_off (tpp t)
{
    return (t->blit_right_off);
}

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

static inline uint8_t tp_is_rrr3 (tpp t)
{
    return (t->is_rrr3);
}

static inline uint8_t tp_is_rrr4 (tpp t)
{
    return (t->is_rrr4);
}

static inline uint8_t tp_is_rrr5 (tpp t)
{
    return (t->is_rrr5);
}

static inline uint8_t tp_is_rrr6 (tpp t)
{
    return (t->is_rrr6);
}

static inline uint8_t tp_is_rrr7 (tpp t)
{
    return (t->is_rrr7);
}

static inline uint8_t tp_is_rrr8 (tpp t)
{
    return (t->is_rrr8);
}

static inline uint8_t tp_is_rrr9 (tpp t)
{
    return (t->is_rrr9);
}

static inline uint8_t tp_is_rrr10 (tpp t)
{
    return (t->is_rrr10);
}

static inline uint8_t tp_is_rrr11 (tpp t)
{
    return (t->is_rrr11);
}

static inline uint8_t tp_is_rrr12 (tpp t)
{
    return (t->is_rrr12);
}

static inline uint8_t tp_is_rrr13 (tpp t)
{
    return (t->is_rrr13);
}

static inline uint8_t tp_is_rrr14 (tpp t)
{
    return (t->is_rrr14);
}

static inline uint8_t tp_is_rrr15 (tpp t)
{
    return (t->is_rrr15);
}

static inline uint8_t tp_is_rrr16 (tpp t)
{
    return (t->is_rrr16);
}

static inline uint8_t tp_is_rrr17 (tpp t)
{
    return (t->is_rrr17);
}

static inline uint8_t tp_is_rrr18 (tpp t)
{
    return (t->is_rrr18);
}

static inline uint8_t tp_is_rrr19 (tpp t)
{
    return (t->is_rrr19);
}

static inline uint8_t tp_is_rrr20 (tpp t)
{
    return (t->is_rrr20);
}

static inline uint8_t tp_is_rrr21 (tpp t)
{
    return (t->is_rrr21);
}

static inline uint8_t tp_is_rrr22 (tpp t)
{
    return (t->is_rrr22);
}

static inline uint8_t tp_is_rrr23 (tpp t)
{
    return (t->is_rrr23);
}

static inline uint8_t tp_is_rrr24 (tpp t)
{
    return (t->is_rrr24);
}

static inline uint8_t tp_is_rrr25 (tpp t)
{
    return (t->is_rrr25);
}

static inline uint8_t tp_is_rrr26 (tpp t)
{
    return (t->is_rrr26);
}

static inline uint8_t tp_is_rrr27 (tpp t)
{
    return (t->is_rrr27);
}

static inline uint8_t tp_is_rrr28 (tpp t)
{
    return (t->is_rrr28);
}

static inline uint8_t tp_is_rrr29 (tpp t)
{
    return (t->is_rrr29);
}

static inline uint8_t tp_is_rrr30 (tpp t)
{
    return (t->is_rrr30);
}

static inline uint8_t tp_is_rrr31 (tpp t)
{
    return (t->is_rrr31);
}

static inline uint8_t tp_is_rrr32 (tpp t)
{
    return (t->is_rrr32);
}

static inline uint8_t tp_is_rrr33 (tpp t)
{
    return (t->is_rrr33);
}

static inline uint8_t tp_is_rrr34 (tpp t)
{
    return (t->is_rrr34);
}

static inline uint8_t tp_is_rrr35 (tpp t)
{
    return (t->is_rrr35);
}

static inline uint8_t tp_is_rrr36 (tpp t)
{
    return (t->is_rrr36);
}

static inline uint8_t tp_is_rrr37 (tpp t)
{
    return (t->is_rrr37);
}

static inline uint8_t tp_is_rrr38 (tpp t)
{
    return (t->is_rrr38);
}

static inline uint8_t tp_is_rrr39 (tpp t)
{
    return (t->is_rrr39);
}

static inline uint8_t tp_is_rrr40 (tpp t)
{
    return (t->is_rrr40);
}

static inline uint8_t tp_is_rrr41 (tpp t)
{
    return (t->is_rrr41);
}

static inline uint8_t tp_is_rrr42 (tpp t)
{
    return (t->is_rrr42);
}

static inline uint8_t tp_is_rrr43 (tpp t)
{
    return (t->is_rrr43);
}

static inline uint8_t tp_is_rrr44 (tpp t)
{
    return (t->is_rrr44);
}

static inline uint8_t tp_is_rrr45 (tpp t)
{
    return (t->is_rrr45);
}

static inline uint8_t tp_is_rrr46 (tpp t)
{
    return (t->is_rrr46);
}

static inline uint8_t tp_has_shadow (tpp t)
{
    return (t->has_shadow);
}

static inline uint8_t tp_is_chair (tpp t)
{
    return (t->is_chair);
}

static inline uint8_t tp_is_chest (tpp t)
{
    return (t->is_chest);
}

static inline uint8_t tp_is_bookcase (tpp t)
{
    return (t->is_bookcase);
}

static inline uint8_t tp_is_table (tpp t)
{
    return (t->is_table);
}

static inline uint8_t tp_is_lawn (tpp t)
{
    return (t->is_lawn);
}

static inline uint8_t tp_is_carpet_deco (tpp t)
{
    return (t->is_carpet_deco);
}

static inline uint8_t tp_is_carpet (tpp t)
{
    return (t->is_carpet);
}

static inline uint8_t tp_is_house_item (tpp t)
{
    return (t->is_house_item);
}

static inline uint8_t tp_is_hwall (tpp t)
{
    return (t->is_hwall);
}

static inline uint8_t tp_is_animated_lr_flip (tpp t)
{
    return (t->is_animated_lr_flip);
}

static inline uint8_t tp_is_gravel_snow_deco (tpp t)
{
    return (t->is_gravel_snow_deco);
}

static inline uint8_t tp_is_landrock_snow (tpp t)
{
    return (t->is_landrock_snow);
}

static inline uint8_t tp_is_hidden_from_editor (tpp t)
{
    return (t->is_hidden_from_editor);
}

static inline uint8_t tp_is_cwall (tpp t)
{
    return (t->is_cwall);
}

static inline uint8_t tp_is_dungeon_item (tpp t)
{
    return (t->is_dungeon_item);
}

static inline uint8_t tp_is_difficulty_easy (tpp t)
{
    return (t->is_difficulty_easy);
}

static inline uint8_t tp_is_difficulty_hard (tpp t)
{
    return (t->is_difficulty_hard);
}

static inline uint8_t tp_is_difficulty_nightmare (tpp t)
{
    return (t->is_difficulty_nightmare);
}

static inline uint8_t tp_is_world_item (tpp t)
{
    return (t->is_world_item);
}

static inline uint8_t tp_is_solid_ground (tpp t)
{
    return (t->is_solid_ground);
}

static inline uint8_t tp_is_dirt_deco (tpp t)
{
    return (t->is_dirt_deco);
}

static inline uint8_t tp_is_dirt_snow_deco (tpp t)
{
    return (t->is_dirt_snow_deco);
}

static inline uint8_t tp_is_grass_deco (tpp t)
{
    return (t->is_grass_deco);
}

static inline uint8_t tp_is_grass_snow_deco (tpp t)
{
    return (t->is_grass_snow_deco);
}

static inline uint8_t tp_is_gravel_deco (tpp t)
{
    return (t->is_gravel_deco);
}

static inline uint8_t tp_is_sand_deco (tpp t)
{
    return (t->is_sand_deco);
}

static inline uint8_t tp_is_sand_snow_deco (tpp t)
{
    return (t->is_sand_snow_deco);
}

static inline uint8_t tp_is_snow_deco (tpp t)
{
    return (t->is_snow_deco);
}

static inline uint8_t tp_is_sand_snow (tpp t)
{
    return (t->is_sand_snow);
}

static inline uint8_t tp_is_dirt_snow (tpp t)
{
    return (t->is_dirt_snow);
}

static inline uint8_t tp_is_grass_snow (tpp t)
{
    return (t->is_grass_snow);
}

static inline uint8_t tp_is_road_snow (tpp t)
{
    return (t->is_road_snow);
}

static inline uint8_t tp_is_road (tpp t)
{
    return (t->is_road);
}

static inline uint8_t tp_is_dungeon_snow (tpp t)
{
    return (t->is_dungeon_snow);
}

static inline uint8_t tp_is_dungeon (tpp t)
{
    return (t->is_dungeon);
}

static inline uint8_t tp_is_focus (tpp t)
{
    return (t->is_focus);
}

static inline uint8_t tp_is_gravel_snow (tpp t)
{
    return (t->is_gravel_snow);
}

static inline uint8_t tp_is_tree_conifer (tpp t)
{
    return (t->is_tree_conifer);
}

static inline uint8_t tp_is_snow_mound (tpp t)
{
    return (t->is_snow_mound);
}

static inline uint8_t tp_is_rock (tpp t)
{
    return (t->is_rock);
}

static inline uint8_t tp_is_ice (tpp t)
{
    return (t->is_ice);
}

static inline uint8_t tp_is_gravel (tpp t)
{
    return (t->is_gravel);
}

static inline uint8_t tp_is_small_rock (tpp t)
{
    return (t->is_small_rock);
}

static inline uint8_t tp_is_marsh_plant (tpp t)
{
    return (t->is_marsh_plant);
}

static inline uint8_t tp_is_landrock (tpp t)
{
    return (t->is_landrock);
}

static inline uint8_t tp_is_snow (tpp t)
{
    return (t->is_snow);
}

static inline uint8_t tp_is_hidden (tpp t)
{
    return (t->is_hidden);
}

static inline uint8_t tp_is_dirt (tpp t)
{
    return (t->is_dirt);
}

static inline uint8_t tp_is_sand (tpp t)
{
    return (t->is_sand);
}

static inline uint8_t tp_is_plant (tpp t)
{
    return (t->is_plant);
}

static inline uint8_t tp_is_tree (tpp t)
{
    return (t->is_tree);
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

static inline uint8_t tp_is_dungeon_way_up (tpp t)
{
    return (t->is_dungeon_way_up);
}

static inline uint8_t tp_is_dungeon_way_down (tpp t)
{
    return (t->is_dungeon_way_down);
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

static inline uint8_t tp_is_explosion (tpp t)
{
    return (t->is_explosion);
}

static inline uint8_t tp_is_effect_fade_in_out (tpp t)
{
    return (t->is_effect_fade_in_out);
}

tilep tp_first_tile(tpp tp);
