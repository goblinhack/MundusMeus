/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"
#include "string_util.h"
#include "string_ext.h"
#include "frameobject.h"
#include "tex.h"
#include "tile.h"
#include "thing_template.h"
#include "py_tp.h"

PyObject *tp_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;

    static char *kwlist[] = {"tp", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, &py_class)) {
        return (0);
    }

    if (!py_class) {
        ERR("tp_load, missing name attr");
        return (0);
    }

    char *tp_name = py_obj_attr_str(py_class, "name");
    if (!tp_name) {
        ERR("tp_load, missing tp name");
        return (0);
    }

    static int id;
    id++;
    tp_load(id, tp_name);

    myfree(tp_name);

    Py_RETURN_NONE;
}

TP_BODY_INT(d10000_chance_of_appearing)
ENUM_DEF_C(MAP_DEPTH_ENUMS, map_depth)
TP_BODY_ENUM(z_depth, map_depth_str2val)
TP_BODY_INT(z_order)
ENUM_DEF_C(WORLD_DEPTH_ENUMS, world_depth)
TP_BODY_ENUM(world_depth, world_depth_str2val)
TP_BODY_INT(speed)
TP_BODY_INT(damage)
TP_BODY_INT(cost)
TP_BODY_INT(lifespan_ticks)
TP_BODY_INT(vision_distance)
TP_BODY_INT(approach_distance)
TP_BODY_INT(bonus_score_on_death)
TP_BODY_INT(bonus_gold_on_collect)
TP_BODY_INT(bonus_hp_on_collect)
TP_BODY_INT(blit_top_off)
TP_BODY_INT(blit_bot_off)
TP_BODY_INT(blit_left_off)
TP_BODY_INT(blit_right_off)
TP_BODY_INT(drown_in_secs)
TP_BODY_INT(min_appear_depth)
TP_BODY_INT(max_appear_depth)
TP_BODY_INT(jump_speed)
TP_BODY_INT(hp_per_level)
TP_BODY_INT(max_hp)
TP_BODY_INT(hit_priority)
TP_BODY_INT(weapon_fire_delay_hundredths)
TP_BODY_INT(sound_random_delay_secs)
TP_BODY_INT(swing_distance_from_player)
TP_BODY_INT(can_be_hit_chance)
TP_BODY_INT(hit_delay_tenths)
TP_BODY_INT(mob_spawn_delay_tenths)
TP_BODY_STRING(short_name)
TP_BODY_STRING(raw_name)
TP_BODY_STRING(tooltip)
TP_BODY_STRING(polymorph_on_death)
TP_BODY_STRING(carried_as)
TP_BODY_STRING(light_tint)
TP_BODY_STRING(explodes_as)
TP_BODY_STRING(sound_on_creation)
TP_BODY_STRING(sound_on_hitting_something)
TP_BODY_STRING(sound_on_death)
TP_BODY_STRING(sound_on_hit)
TP_BODY_STRING(sound_on_collect)
TP_BODY_STRING(sound_random)
TP_BODY_STRING(weapon_carry_anim)
TP_BODY_STRING(weapon_swing_anim)
TP_BODY_STRING(message_on_use)
TP_BODY_INT(is_acid)
TP_BODY_INT(is_acid_proof)
TP_BODY_INT(is_action)
TP_BODY_INT(is_action_down)
TP_BODY_INT(is_action_left)
TP_BODY_INT(is_action_right)
TP_BODY_INT(is_action_sleep)
TP_BODY_INT(is_action_trigger)
TP_BODY_INT(is_action_trigger_on_hero)
TP_BODY_INT(is_action_trigger_on_monst)
TP_BODY_INT(is_action_trigger_on_wall)
TP_BODY_INT(is_action_up)
TP_BODY_INT(is_action_zap)
TP_BODY_INT(is_animated)
TP_BODY_INT(is_animated_no_dir)
TP_BODY_INT(is_animated_only_when_moving)
TP_BODY_INT(is_animation)
TP_BODY_INT(is_bomb)
TP_BODY_INT(is_bullet)
TP_BODY_INT(is_candle_light)
TP_BODY_INT(is_carryable)
TP_BODY_INT(is_cats_eyes)
TP_BODY_INT(is_cloud_effect)
TP_BODY_INT(is_cobweb)
TP_BODY_INT(is_collision_map_large)
TP_BODY_INT(is_collision_map_medium)
TP_BODY_INT(is_collision_map_small)
TP_BODY_INT(is_collision_map_tiny)
TP_BODY_INT(is_combustable)
TP_BODY_INT(is_conical_breath_attack)
TP_BODY_INT(is_corpse)
TP_BODY_INT(is_corridor)
TP_BODY_INT(is_corridor_wall)
TP_BODY_INT(is_door)
TP_BODY_INT(is_dungeon_floor)
TP_BODY_INT(is_effect_fade_in_out)
TP_BODY_INT(is_effect_pulse)
TP_BODY_INT(is_effect_rotate_2way)
TP_BODY_INT(is_effect_sway)
TP_BODY_INT(is_entrance)
TP_BODY_INT(is_ethereal)
TP_BODY_INT(is_exit)
TP_BODY_INT(is_explosion)
TP_BODY_INT(is_fire)
TP_BODY_INT(is_fireball)
TP_BODY_INT(is_fires_when_angry)
TP_BODY_INT(is_food)
TP_BODY_INT(is_fragile)
TP_BODY_INT(is_given_randomly_at_start)
TP_BODY_INT(is_hard)
TP_BODY_INT(is_hidden)
TP_BODY_INT(is_hidden_from_editor)
TP_BODY_INT(is_inactive)
TP_BODY_INT(is_internal)
TP_BODY_INT(is_item_unusable)
TP_BODY_INT(is_joinable)
TP_BODY_INT(is_key)
TP_BODY_INT(is_ladder)
TP_BODY_INT(is_ladder_deco)
TP_BODY_INT(is_lava)
TP_BODY_INT(is_lava_proof)
TP_BODY_INT(is_levitating)
TP_BODY_INT(is_life_saving)
TP_BODY_INT(is_light_source)
TP_BODY_INT(is_magical_weapon)
TP_BODY_INT(is_melee_weapon)
TP_BODY_INT(is_mob_spawner)
TP_BODY_INT(is_monst)
TP_BODY_INT(is_non_explosive_gas_cloud)
TP_BODY_INT(is_not_light_blocking)
TP_BODY_INT(is_obstacle)
TP_BODY_INT(is_player)
TP_BODY_INT(is_projectile)
TP_BODY_INT(is_ranged_weapon)
TP_BODY_INT(is_rock)
TP_BODY_INT(is_rrr1)
TP_BODY_INT(is_rrr2)
TP_BODY_INT(is_rrr3)
TP_BODY_INT(is_rrr4)
TP_BODY_INT(is_rrr5)
TP_BODY_INT(is_rrr6)
TP_BODY_INT(is_rrr7)
TP_BODY_INT(is_rrr8)
TP_BODY_INT(is_rrr9)
TP_BODY_INT(is_rrr10)
TP_BODY_INT(is_rrr11)
TP_BODY_INT(is_rrr12)
TP_BODY_INT(is_rrr13)
TP_BODY_INT(is_rrr14)
TP_BODY_INT(is_rrr15)
TP_BODY_INT(is_rrr16)
TP_BODY_INT(is_rrr17)
TP_BODY_INT(is_rrr18)
TP_BODY_INT(is_rrr19)
TP_BODY_INT(is_rrr20)
TP_BODY_INT(is_rrr21)
TP_BODY_INT(is_rrr22)
TP_BODY_INT(is_grass)
TP_BODY_INT(is_land)
TP_BODY_INT(is_wanderer_lr)
TP_BODY_INT(is_boulder)
TP_BODY_INT(is_bubbles)
TP_BODY_INT(is_ropepile)
TP_BODY_INT(is_ropetop)
TP_BODY_INT(is_climbable)
TP_BODY_INT(is_smallrock)
TP_BODY_INT(is_rope)
TP_BODY_INT(is_throwable)
TP_BODY_INT(is_sawblade)
TP_BODY_INT(is_shadow_caster)
TP_BODY_INT(is_shadow_caster_soft)
TP_BODY_INT(is_shop_floor)
TP_BODY_INT(is_shopkeeper)
TP_BODY_INT(is_single_mob_spawner)
TP_BODY_INT(is_sleeping)
TP_BODY_INT(is_spawns_under)
TP_BODY_INT(is_spider_proof)
TP_BODY_INT(is_spikes)
TP_BODY_INT(is_stackable)
TP_BODY_INT(is_teleport)
TP_BODY_INT(is_torch)
TP_BODY_INT(is_trap)
TP_BODY_INT(is_treasure)
TP_BODY_INT(is_undead)
TP_BODY_INT(is_variable_size)
TP_BODY_INT(is_visible_on_debug_only)
TP_BODY_INT(is_wall)
TP_BODY_INT(is_deco)
TP_BODY_INT(is_wanderer)
TP_BODY_INT(is_warm_blooded)
TP_BODY_INT(is_water)
TP_BODY_INT(is_water_proof)
TP_BODY_INT(is_weapon)
TP_BODY_INT(is_weapon_carry_anim)

