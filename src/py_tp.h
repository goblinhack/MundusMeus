/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include <Python.h>

PyObject *tp_load_(PyObject *obj, PyObject *args, PyObject *keywds);

#define TP_PROTO(__field__)                                                     \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds);

#define TP_DECL(__field__)                                                      \
    {"tp_set_" #__field__,                                                      \
        (PyCFunction)tp_set_ ## __field__,                                      \
        METH_VARARGS | METH_KEYWORDS,                                           \
        "set a " #__field__ " in a thing template"},                            \

#define TP_BODY_STRING(__field__)                                               \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *tp_name = 0;	                                                        \
    char *value = 0;	                                                        \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, &py_class,    \
                                     &value)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!value) {	                                                        \
        ERR("%s, missing value", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    tp_name = py_obj_attr_str(py_class, "name");	                        \
    if (!tp_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("%s(%s -> \"%s\")", __FUNCTION__, tp_name, value);	                \
	                                                                        \
    tpp tp = tp_find(tp_name);	                                                \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = dupstr(value, __FUNCTION__);	                        \
    value = 0;	                                                                \
	                                                                        \
done:	                                                                        \
    if (tp_name) {	                                                        \
        myfree(tp_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define TP_BODY_STRING_FN(__field__, __fn__)                                    \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *tp_name = 0;	                                                        \
    char *value = 0;	                                                        \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, &py_class,    \
                                     &value)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!value) {	                                                        \
        ERR("%s, missing value", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    tp_name = py_obj_attr_str(py_class, "name");	                        \
    if (!tp_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("%s(%s -> \"%s\")", __FUNCTION__, tp_name, value);	                \
	                                                                        \
    tpp tp = tp_find(tp_name);	                                                \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = dupstr(value, __FUNCTION__);	                        \
    value = 0;	                                                                \
    (__fn__)(tp);                                                               \
	                                                                        \
done:	                                                                        \
    if (tp_name) {	                                                        \
        myfree(tp_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define TP_BODY_ENUM(__field__, __str2val__)                                    \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *tp_name = 0;	                                                        \
    char *value = 0;	                                                        \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, &py_class,    \
                                     &value)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!value) {	                                                        \
        ERR("%s, missing value", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    tp_name = py_obj_attr_str(py_class, "name");	                        \
    if (!tp_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tpp tp = tp_find(tp_name);	                                                \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = (__str2val__)(value);                                       \
    if (tp->__field__ == (typeof(tp->__field__))-1) {                           \
        ERR("%s, cannot find enum %s", __FUNCTION__, value);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("%s(%s -> \"%s\"[%d])", __FUNCTION__, tp_name, value, tp->__field__);	\
	                                                                        \
    value = 0;	                                                                \
	                                                                        \
done:	                                                                        \
    if (tp_name) {	                                                        \
        myfree(tp_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define TP_BODY_INT(__field__)                                                  \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *tp_name = 0;	                                                        \
    int value = 0;	                                                        \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, &py_class,    \
                                     &value)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    tp_name = py_obj_attr_str(py_class, "name");	                        \
    if (!tp_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("%s(%s -> %d)", __FUNCTION__, tp_name, value);	                        \
	                                                                        \
    tpp tp = tp_find(tp_name);	                                                \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = value;                                                      \
	                                                                        \
done:	                                                                        \
    if (tp_name) {	                                                        \
        myfree(tp_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

TP_PROTO(d10000_chance_of_appearing)
TP_PROTO(z_depth)
TP_PROTO(z_order)
TP_PROTO(world_depth)
TP_PROTO(speed)
TP_PROTO(damage)
TP_PROTO(cost)
TP_PROTO(lifespan_ticks)
TP_PROTO(vision_distance)
TP_PROTO(approach_distance)
TP_PROTO(bonus_score_on_death)
TP_PROTO(bonus_gold_on_collect)
TP_PROTO(bonus_hp_on_collect)
TP_PROTO(blit_top_off)
TP_PROTO(blit_bot_off)
TP_PROTO(blit_left_off)
TP_PROTO(blit_right_off)
TP_PROTO(drown_in_secs)
TP_PROTO(min_appear_depth)
TP_PROTO(max_appear_depth)
TP_PROTO(jump_speed)
TP_PROTO(hp_per_level)
TP_PROTO(max_hp)
TP_PROTO(hit_priority)
TP_PROTO(weapon_fire_delay_hundredths)
TP_PROTO(sound_random_delay_secs)
TP_PROTO(swing_distance_from_player)
TP_PROTO(can_be_hit_chance)
TP_PROTO(hit_delay_tenths)
TP_PROTO(mob_spawn_delay_tenths)
TP_PROTO(short_name)
TP_PROTO(raw_name)
TP_PROTO(tooltip)
TP_PROTO(polymorph_on_death)
TP_PROTO(carried_as)
TP_PROTO(light_tint)
TP_PROTO(explodes_as)
TP_PROTO(sound_on_creation)
TP_PROTO(sound_on_hitting_something)
TP_PROTO(sound_on_death)
TP_PROTO(sound_on_hit)
TP_PROTO(sound_on_collect)
TP_PROTO(sound_random)
TP_PROTO(weapon_carry_anim)
TP_PROTO(weapon_swing_anim)
TP_PROTO(message_on_use)
TP_PROTO(is_acid)
TP_PROTO(is_acid_proof)
TP_PROTO(is_action)
TP_PROTO(is_action_down)
TP_PROTO(is_action_left)
TP_PROTO(is_action_right)
TP_PROTO(is_action_sleep)
TP_PROTO(is_action_trigger)
TP_PROTO(is_action_trigger_on_hero)
TP_PROTO(is_action_trigger_on_monst)
TP_PROTO(is_action_trigger_on_wall)
TP_PROTO(is_action_up)
TP_PROTO(is_action_zap)
TP_PROTO(is_animated)
TP_PROTO(is_animated_no_dir)
TP_PROTO(is_animated_only_when_moving)
TP_PROTO(is_animation)
TP_PROTO(is_bomb)
TP_PROTO(is_bullet)
TP_PROTO(is_candle_light)
TP_PROTO(is_carryable)
TP_PROTO(is_cats_eyes)
TP_PROTO(is_cloud_effect)
TP_PROTO(is_cobweb)
TP_PROTO(is_collision_map_large)
TP_PROTO(is_collision_map_medium)
TP_PROTO(is_collision_map_small)
TP_PROTO(is_collision_map_tiny)
TP_PROTO(is_combustable)
TP_PROTO(is_conical_breath_attack)
TP_PROTO(is_corpse)
TP_PROTO(is_corridor)
TP_PROTO(is_corridor_wall)
TP_PROTO(is_door)
TP_PROTO(is_dungeon_floor)
TP_PROTO(is_effect_fade_in_out)
TP_PROTO(is_effect_pulse)
TP_PROTO(is_effect_rotate_2way)
TP_PROTO(is_effect_sway)
TP_PROTO(is_entrance)
TP_PROTO(is_ethereal)
TP_PROTO(is_exit)
TP_PROTO(is_explosion)
TP_PROTO(is_fire)
TP_PROTO(is_fireball)
TP_PROTO(is_fires_when_angry)
TP_PROTO(is_food)
TP_PROTO(is_fragile)
TP_PROTO(is_given_randomly_at_start)
TP_PROTO(is_hard)
TP_PROTO(is_hidden)
TP_PROTO(is_hidden_from_editor)
TP_PROTO(is_inactive)
TP_PROTO(is_internal)
TP_PROTO(is_item_unusable)
TP_PROTO(is_joinable)
TP_PROTO(is_key)
TP_PROTO(is_ladder)
TP_PROTO(is_ladder_deco)
TP_PROTO(is_lava)
TP_PROTO(is_lava_proof)
TP_PROTO(is_levitating)
TP_PROTO(is_life_saving)
TP_PROTO(is_light_source)
TP_PROTO(is_magical_weapon)
TP_PROTO(is_melee_weapon)
TP_PROTO(is_mob_spawner)
TP_PROTO(is_monst)
TP_PROTO(is_non_explosive_gas_cloud)
TP_PROTO(is_not_light_blocking)
TP_PROTO(is_obstacle)
TP_PROTO(is_player)
TP_PROTO(is_projectile)
TP_PROTO(is_ranged_weapon)
TP_PROTO(is_rock)
TP_PROTO(is_rrr1)
TP_PROTO(is_rrr2)
TP_PROTO(is_rrr3)
TP_PROTO(is_rrr4)
TP_PROTO(is_rrr5)
TP_PROTO(is_rrr6)
TP_PROTO(is_rrr7)
TP_PROTO(is_rrr8)
TP_PROTO(is_rrr9)
TP_PROTO(is_rrr10)
TP_PROTO(is_rrr11)
TP_PROTO(is_rrr12)
TP_PROTO(is_rrr13)
TP_PROTO(is_rrr14)
TP_PROTO(is_rrr15)
TP_PROTO(is_rrr16)
TP_PROTO(is_rrr17)
TP_PROTO(is_rrr18)
TP_PROTO(is_rrr19)
TP_PROTO(is_rrr20)
TP_PROTO(is_rrr21)
TP_PROTO(is_rrr22)
TP_PROTO(is_grass)
TP_PROTO(is_land)
TP_PROTO(is_wanderer_lr)
TP_PROTO(is_boulder)
TP_PROTO(is_bubbles)
TP_PROTO(is_ropepile)
TP_PROTO(is_ropetop)
TP_PROTO(is_climbable)
TP_PROTO(is_smallrock)
TP_PROTO(is_rope)
TP_PROTO(is_throwable)
TP_PROTO(is_sawblade)
TP_PROTO(is_shadow_caster)
TP_PROTO(is_shadow_caster_soft)
TP_PROTO(is_shop_floor)
TP_PROTO(is_shopkeeper)
TP_PROTO(is_single_mob_spawner)
TP_PROTO(is_sleeping)
TP_PROTO(is_spawns_under)
TP_PROTO(is_spider_proof)
TP_PROTO(is_spikes)
TP_PROTO(is_stackable)
TP_PROTO(is_teleport)
TP_PROTO(is_torch)
TP_PROTO(is_trap)
TP_PROTO(is_treasure)
TP_PROTO(is_undead)
TP_PROTO(is_variable_size)
TP_PROTO(is_visible_on_debug_only)
TP_PROTO(is_wall)
TP_PROTO(is_deco)
TP_PROTO(is_wanderer)
TP_PROTO(is_warm_blooded)
TP_PROTO(is_water)
TP_PROTO(is_water_proof)
TP_PROTO(is_weapon)
TP_PROTO(is_weapon_carry_anim)


