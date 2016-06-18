/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include <SDL.h>

#include "main.h"
#include "tree.h"
#include "thing_tile.h"
#include "tile.h"
#include "thing.h"
#include "string_util.h"

/*
 * Using static memory as these things never change once made.
 */
thing_template thing_templates_chunk[THING_MAX];

tree_root *thing_templates;
tree_root *thing_templates_create_order;

static uint8_t tp_init_done;
static void tp_destroy_internal(tpp t);

uint8_t tp_init (void)
{
    tp_init_done = true;

    return (true);
}

void tp_fini (void)
{
    if (tp_init_done) {
        tp_init_done = false;

        tree_destroy(&thing_templates,
            (tree_destroy_func)tp_destroy_internal);

        tree_destroy(&thing_templates_create_order,
            (tree_destroy_func)0);
    }
}

/*
 * Fill up the tile cache for future fast finds of tile types.
 */
static void tp_fill_cache (tpp t)
{
    thing_tilep thing_tile;
    uint32_t index;
    tilep tile;

    TREE_WALK_UNSAFE(t->tiles, thing_tile) {

        if (thing_tile->is_join_block) {
            index = IS_JOIN_BLOCK;
        } else if (thing_tile->is_join_t_1) {
            index = IS_JOIN_T_1;
        } else if (thing_tile->is_join_t_2) {
            index = IS_JOIN_T_2;
        } else if (thing_tile->is_join_t_3) {
            index = IS_JOIN_T_3;
        } else if (thing_tile->is_join_t90_1) {
            index = IS_JOIN_T90_1;
        } else if (thing_tile->is_join_t90_2) {
            index = IS_JOIN_T90_2;
        } else if (thing_tile->is_join_t90_3) {
            index = IS_JOIN_T90_3;
        } else if (thing_tile->is_join_t180_1) {
            index = IS_JOIN_T180_1;
        } else if (thing_tile->is_join_t180_2) {
            index = IS_JOIN_T180_2;
        } else if (thing_tile->is_join_t180_3) {
            index = IS_JOIN_T180_3;
        } else if (thing_tile->is_join_t270_1) {
            index = IS_JOIN_T270_1;
        } else if (thing_tile->is_join_t270_2) {
            index = IS_JOIN_T270_2;
        } else if (thing_tile->is_join_t270_3) {
            index = IS_JOIN_T270_3;
        } else if (thing_tile->is_join_block) {
            index = IS_JOIN_BLOCK;
        } else if (thing_tile->is_join_horiz) {
            index = IS_JOIN_HORIZ;
        } else if (thing_tile->is_join_vert) {
            index = IS_JOIN_VERT;
        } else if (thing_tile->is_join_node) {
            index = IS_JOIN_NODE;
        } else if (thing_tile->is_join_left) {
            index = IS_JOIN_LEFT;
        } else if (thing_tile->is_join_right) {
            index = IS_JOIN_RIGHT;
        } else if (thing_tile->is_join_top) {
            index = IS_JOIN_TOP;
        } else if (thing_tile->is_join_bot) {
            index = IS_JOIN_BOT;
        } else if (thing_tile->is_join_tl) {
            index = IS_JOIN_TL;
        } else if (thing_tile->is_join_tr) {
            index = IS_JOIN_TR;
        } else if (thing_tile->is_join_bl) {
            index = IS_JOIN_BL;
        } else if (thing_tile->is_join_br) {
            index = IS_JOIN_BR;
        } else if (thing_tile->is_join_t) {
            index = IS_JOIN_T;
        } else if (thing_tile->is_join_t90) {
            index = IS_JOIN_T90;
        } else if (thing_tile->is_join_t180) {
            index = IS_JOIN_T180;
        } else if (thing_tile->is_join_t270) {
            index = IS_JOIN_T270;
        } else if (thing_tile->is_join_x) {
            index = IS_JOIN_X;
        } else if (thing_tile->is_join_tl2) {
            index = IS_JOIN_TL2;
        } else if (thing_tile->is_join_tr2) {
            index = IS_JOIN_TR2;
        } else if (thing_tile->is_join_bl2) {
            index = IS_JOIN_BL2;
        } else if (thing_tile->is_join_br2) {
            index = IS_JOIN_BR2;
        } else if (thing_tile->is_join_x) {
            index = IS_JOIN_X;
        } else if (thing_tile->is_join_x1) {
            index = IS_JOIN_X1;
        } else if (thing_tile->is_join_x1_270) {
            index = IS_JOIN_X1_270;
        } else if (thing_tile->is_join_x1_180) {
            index = IS_JOIN_X1_180;
        } else if (thing_tile->is_join_x1_90) {
            index = IS_JOIN_X1_90;
        } else if (thing_tile->is_join_x2) {
            index = IS_JOIN_X2;
        } else if (thing_tile->is_join_x2_270) {
            index = IS_JOIN_X2_270;
        } else if (thing_tile->is_join_x2_180) {
            index = IS_JOIN_X2_180;
        } else if (thing_tile->is_join_x2_90) {
            index = IS_JOIN_X2_90;
        } else if (thing_tile->is_join_x3) {
            index = IS_JOIN_X3;
        } else if (thing_tile->is_join_x3_180) {
            index = IS_JOIN_X3_180;
        } else if (thing_tile->is_join_x4) {
            index = IS_JOIN_X4;
        } else if (thing_tile->is_join_x4_270) {
            index = IS_JOIN_X4_270;
        } else if (thing_tile->is_join_x4_180) {
            index = IS_JOIN_X4_180;
        } else if (thing_tile->is_join_x4_90) {
            index = IS_JOIN_X4_90;
        } else if (thing_tile->is_join_horiz2) {
            index = IS_JOIN_HORIZ2;
        } else if (thing_tile->is_join_vert2) {
            index = IS_JOIN_VERT2;
        } else {
            continue;
        }

        t->tilep_join[index][ t->tilep_join_count[index] ] = thing_tile;

        tile = tile_find(thing_tile_name(thing_tile));
        if (!tile) {
            ERR("no tile for join index %d for %s, tile name %s",
                index, t->short_name, thing_tile_name(thing_tile));
        }

        t->tilep_join_tile[index][ t->tilep_join_count[index] ] = tile;

        if (t->tilep_join_count[index]++ >= IS_JOIN_ALT_MAX) {
            ERR("too many tile join alternatives for %s", t->short_name);
        }
    }
}

tpp tp_load (uint16_t id, const char *name)
{
    tpp t;

    if (tp_find(name)) {
        ERR("thing template name [%s] already used", name);
    }

    if (!thing_templates) {
        thing_templates = tree_alloc(TREE_KEY_STRING, "TREE ROOT: thing");
    }

    if (!thing_templates_create_order) {
        thing_templates_create_order =
                        tree_alloc(TREE_KEY_INTEGER,
                                   "TREE ROOT: thing create order");
    }

    if (id >= THING_MAX - 1) {
        ERR("too many thing templates");
    }

    t = &thing_templates_chunk[id];
    newptr(t, "tpp");

    t->tree.key = dupstr(name, "TREE KEY: thing");

    t->tp_id = id;

    if (!tree_insert_static(thing_templates, &t->tree.node)) {
        ERR("thing template insert name [%s] failed", name);
    }

    static uint32_t create_order;

    t->tree2.key = create_order++;
    if (!tree_insert_static(thing_templates_create_order, &t->tree2.node)) {
        ERR("thing template insert create order [%s] failed", name);
    }

    tp_fill_cache(t);

    return (t);
}

static void tp_destroy_internal (tpp t)
{
    tree_destroy(&t->tiles, (tree_destroy_func)thing_tile_free);

    if (t->tooltip) {
        myfree(t->tooltip);
    }

    if (t->short_name) {
        myfree(t->short_name);
    }

    if (t->raw_name) {
        myfree(t->raw_name);
    }

    if (t->polymorph_on_death) {
        myfree(t->polymorph_on_death);
    }

    if (t->carried_as) {
        myfree(t->carried_as);
    }

    if (t->light_tint) {
        myfree(t->light_tint);
    }

    if (t->explodes_as) {
        myfree(t->explodes_as);
    }

    if (t->sound_on_creation) {
        myfree(t->sound_on_creation);
    }

    if (t->sound_on_hitting_something) {
        myfree(t->sound_on_hitting_something);
    }

    if (t->sound_on_death) {
        myfree(t->sound_on_death);
    }

    if (t->sound_on_hit) {
        myfree(t->sound_on_hit);
    }

    if (t->sound_on_collect) {
        myfree(t->sound_on_collect);
    }

    if (t->sound_random) {
        myfree(t->sound_random);
    }

    if (t->weapon_carry_anim) {
        myfree(t->weapon_carry_anim);
    }

    if (t->weapon_swing_anim) {
        myfree(t->weapon_swing_anim);
    }

    if (t->message_on_use) {
        myfree(t->message_on_use);
    }

    int i;
    for (i = 0; i < t->mob_spawn_count; i++) {
        if (t->mob_spawn_what[i]) {
            myfree(t->mob_spawn_what[i]);
        }
    }

    for (i = 0; i < t->spawn_on_death_count; i++) {
        if (t->spawn_on_death_what[i]) {
            myfree(t->spawn_on_death_what[i]);
        }
    }

    oldptr(t);
}

/*
 * Find an existing thing.
 */
tpp tp_find (const char *name)
{
    thing_template target;
    tpp result;

    if (!name) {
        ERR("no name for thing find");
    }

    // memset(&target, 0, sizeof(target)); intentional for speed
    target.tree.key = (char*) name;

    result = (typeof(result)) tree_find(thing_templates, &target.tree.node);
    if (!result) {
        return (0);
    }

    return (result);
}

tpp tp_find_short_name (const char *name)
{
    tree_root *tree;
    tpp t;

    tree = thing_templates;

    TREE_WALK(tree, t) {
        if (!strcasecmp(name, t->short_name)) {
            return (t);
        }
    }

    ERR("did not find short template name \"%s\"", name);

    return (0);
}

/*
 * Given a string name, map to a thing template.
 */
tpp string2thing_template (const char **s)
{
    static char tmp[MAXSTR];
    static const char *eo_tmp = tmp + MAXSTR;
    const char *c = *s;
    char *t = tmp;

    while (t < eo_tmp) {
        if ((*c == '\0') || (*c == '$')) {
            break;
        }

        *t++ = *c++;
    }

    if (c == eo_tmp) {
        return (0);
    }

    *t++ = '\0';
    *s += (t - tmp);

    thing_template find;
    tpp target;

    memset(&find, 0, sizeof(find));
    find.tree.key = tmp;

    target = (typeof(target)) tree_find(thing_templates, &find.tree.node);
    if (!target) {
        ERR("unknown thing [%s]", tmp);
    }

    return (target);
}

ENUM_DEF_C(MAP_DEPTH_ENUMS, map_depth)
ENUM_DEF_C(WORLD_DEPTH_ENUMS, world_depth)

const char *tp_name (tpp t)
{
    return (t->tree.key);
}

const char *tp_short_name (tpp t)
{
    return (t->short_name);
}

const char *tp_raw_name (tpp t)
{
    return (t->raw_name);
}

tpp tp_fires (tpp t)
{
    return (t->fires);
}

const char *tp_polymorph_on_death (tpp t)
{
    return (t->polymorph_on_death);
}

const char *tp_carried_as (tpp t)
{
    return (t->carried_as);
}

const char *tp_light_tint (tpp t)
{
    return (t->light_tint);
}

color tp_light_color (tpp t)
{
    return (t->light_color);
}

const char *tp_explodes_as (tpp t)
{
    return (t->explodes_as);
}

const char *tp_sound_on_creation (tpp t)
{
    return (t->sound_on_creation);
}

const char *tp_sound_on_hitting_something (tpp t)
{
    return (t->sound_on_hitting_something);
}

const char *tp_sound_on_death (tpp t)
{
    return (t->sound_on_death);
}

const char *tp_sound_on_hit (tpp t)
{
    return (t->sound_on_hit);
}

const char *tp_sound_on_collect (tpp t)
{
    return (t->sound_on_collect);
}

const char *tp_weapon_carry_anim (tpp t)
{
    return (t->weapon_carry_anim);
}

const char *tp_weapon_swing_anim (tpp t)
{
    return (t->weapon_swing_anim);
}

const char *tp_message_on_use (tpp t)
{
    return (t->message_on_use);
}

const char *tp_mob_spawn (tpp t)
{
    if (!t->mob_spawn_count) {
        return (0);
    }

    for (;;) {
        int i = myrand() % t->mob_spawn_count;
        int r = myrand() % 1000;

        if (r < t->mob_spawn_chance_d1000[i]) {
            if (!t->mob_spawn_what[i][0]) {
                return (0);
            }

            return (t->mob_spawn_what[i]);
        }
    }
}

const char *tp_spawn_on_death (tpp t)
{
    if (!t->spawn_on_death_count) {
        return (0);
    }

    for (;;) {
        int i = myrand() % t->spawn_on_death_count;
        int r = myrand() % 1000;

        if (r < t->spawn_on_death_chance_d1000[i]) {
            if (!t->spawn_on_death_what[i][0]) {
                return (0);
            }

            return (t->spawn_on_death_what[i]);
        }
    }
}

const char *tp_get_tooltip (tpp t)
{
    return (t->tooltip);
}

uint8_t tp_get_z_depth (tpp t)
{
    return (t->z_depth);
}

uint8_t tp_get_world_depth (tpp t)
{
    return (t->world_depth);
}

uint8_t tp_get_z_order (tpp t)
{
    return (t->z_order);
}

uint32_t tp_get_speed (tpp t)
{
    return (t->speed);
}

uint16_t tp_get_damage (tpp t)
{
    return (t->damage);
}

uint16_t tp_get_cost (tpp t)
{
    return (t->cost);
}

uint32_t tp_get_lifespan_ticks (tpp t)
{
    return (t->lifespan_ticks);
}

int32_t tp_get_bonus_score_on_death (tpp t)
{
    return (t->bonus_score_on_death);
}

uint32_t tp_get_vision_distance (tpp t)
{
    return (t->vision_distance);
}

uint32_t tp_get_approach_distance (tpp t)
{
    return (t->approach_distance);
}

int32_t tp_get_bonus_gold_on_collect (tpp t)
{
    return (t->bonus_gold_on_collect);
}

uint32_t tp_get_d10000_chance_of_appearing (tpp t)
{
    return (t->d10000_chance_of_appearing);
}

int32_t tp_get_blit_top_off (tpp t)
{
    return (t->blit_top_off);
}

int32_t tp_get_blit_bot_off (tpp t)
{
    return (t->blit_bot_off);
}

int32_t tp_get_blit_left_off (tpp t)
{
    return (t->blit_left_off);
}

int32_t tp_get_blit_right_off (tpp t)
{
    return (t->blit_right_off);
}

uint32_t tp_get_ppp6 (tpp t)
{
    return (t->ppp6);
}

uint32_t tp_get_drown_in_secs (tpp t)
{
    return (t->drown_in_secs);
}

uint32_t tp_get_min_appear_depth (tpp t)
{
    return (t->min_appear_depth);
}

uint32_t tp_get_max_appear_depth (tpp t)
{
    return (t->max_appear_depth);
}

uint32_t tp_get_jump_speed (tpp t)
{
    return (t->jump_speed);
}

uint32_t tp_get_hp_per_level (tpp t)
{
    return (t->hp_per_level);
}

double tp_get_light_radius (tpp t)
{
    return (t->light_radius);
}

double tp_get_weapon_density (tpp t)
{
    return (t->weapon_density);
}

double tp_get_weapon_spread (tpp t)
{
    return (t->weapon_spread);
}

double tp_get_scale (tpp t)
{
    if (!t->scale) {
        return (1.0);
    }

    return (t->scale);
}

double tp_get_explosion_radius (tpp t)
{
    if (!t->explosion_radius) {
        return (1.0);
    }

    return (t->explosion_radius);
}

double tp_get_collision_radius (tpp t)
{
    return (t->collision_radius);
}

uint32_t tp_get_hit_priority (tpp t)
{
    return (t->hit_priority);
}

uint32_t tp_get_weapon_fire_delay_hundredths (tpp t)
{
    return (t->weapon_fire_delay_hundredths);
}

uint32_t tp_get_sound_random_delay_secs (tpp t)
{
    return (t->sound_random_delay_secs);
}

uint32_t tp_get_swing_distance_from_player (tpp t)
{
    return (t->swing_distance_from_player);
}

int16_t tp_get_bonus_hp_on_collect (tpp t)
{
    return (t->bonus_hp_on_collect);
}

uint32_t tp_get_can_be_hit_chance (tpp t)
{
    return (t->can_be_hit_chance);
}

uint32_t tp_get_hit_delay_tenths (tpp t)
{
    return (t->hit_delay_tenths);
}

uint32_t tp_get_mob_spawn_delay_tenths (tpp t)
{
    return (t->mob_spawn_delay_tenths);
}

tree_rootp tp_get_tiles (tpp t)
{
    return (t->tiles);
}
