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
#include "thing_tile.h"

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

#define TP_BODY_DOUBLE(__field__)                                               \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *tp_name = 0;	                                                        \
    double value = 0;	                                                        \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|d", kwlist, &py_class,    \
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
    LOG("%s(%s -> %g)", __FUNCTION__, tp_name, value);	                        \
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

TP_BODY_DOUBLE(light_radius)
TP_BODY_DOUBLE(weapon_density)
TP_BODY_DOUBLE(weapon_spread)
TP_BODY_DOUBLE(scale)
TP_BODY_DOUBLE(explosion_radius)
TP_BODY_DOUBLE(collision_radius)
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
static void light_tint_fixup (tpp t)
{
    if (t->light_tint) {
        t->light_color = color_find(t->light_tint);
    } else {
        t->light_color = WHITE;
    }
}
TP_BODY_STRING_FN(light_tint, light_tint_fixup)
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
TP_BODY_INT(is_animated)
TP_BODY_INT(is_animated_no_dir)
TP_BODY_INT(is_animation)
TP_BODY_INT(is_bullet)
TP_BODY_INT(is_candle_light)
TP_BODY_INT(is_carryable)
TP_BODY_INT(is_cats_eyes)
TP_BODY_INT(is_cloud_effect)
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
TP_BODY_INT(is_food)
TP_BODY_INT(is_fragile)
TP_BODY_INT(is_given_randomly_at_start)
TP_BODY_INT(is_hard)
TP_BODY_INT(is_hidden)
TP_BODY_INT(is_hidden_from_editor)
TP_BODY_INT(is_inactive)
TP_BODY_INT(is_internal)
TP_BODY_INT(is_joinable)
TP_BODY_INT(is_key)
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
TP_BODY_INT(is_northern_mountain)
TP_BODY_INT(is_snow_castle)
TP_BODY_INT(is_castle)
TP_BODY_INT(is_mountain)
TP_BODY_INT(is_forest)
TP_BODY_INT(is_sand)
TP_BODY_INT(is_grass)
TP_BODY_INT(is_land)
TP_BODY_INT(is_wanderer_lr)
TP_BODY_INT(is_snow_settlement)
TP_BODY_INT(is_northern_settlement)
TP_BODY_INT(is_snow)
TP_BODY_INT(is_settlement)
TP_BODY_INT(is_northern_rock)
TP_BODY_INT(is_sea)
TP_BODY_INT(is_rope)
TP_BODY_INT(is_throwable)
TP_BODY_INT(is_shadow_caster)
TP_BODY_INT(is_shadow_caster_soft)
TP_BODY_INT(is_shop_floor)
TP_BODY_INT(is_shopkeeper)
TP_BODY_INT(is_sleeping)
TP_BODY_INT(is_stackable)
TP_BODY_INT(is_torch)
TP_BODY_INT(is_trap)
TP_BODY_INT(is_treasure)
TP_BODY_INT(is_undead)
TP_BODY_INT(is_variable_size)
TP_BODY_INT(is_wall)
TP_BODY_INT(is_deco)
TP_BODY_INT(is_wanderer)
TP_BODY_INT(is_water)
TP_BODY_INT(is_water_proof)
TP_BODY_INT(is_weapon)
TP_BODY_INT(is_weapon_carry_anim)

PyObject *tp_set_tile (PyObject *obj, PyObject *args, PyObject *keywds)
{	
    PyObject *py_class = 0;	
    thing_tile *t;
    char *tp_name = 0;	
    char *tile = 0;	
    int delay_ms = 0;
    int is_moving = 0;
    int is_jumping = 0;
    int begin_jump = 0;
    int is_join_block = 0;
    int is_join_horiz = 0;
    int is_join_vert = 0;
    int is_join_node = 0;
    int is_join_left = 0;
    int is_join_right = 0;
    int is_join_top = 0;
    int is_join_bot = 0;
    int is_join_tl = 0;
    int is_join_tr = 0;
    int is_join_bl = 0;
    int is_join_br = 0;
    int is_join_t = 0;
    int is_join_t90 = 0;
    int is_join_t180 = 0;
    int is_join_t270 = 0;
    int is_join_x = 0;
    int is_join_tl2 = 0;
    int is_join_tr2 = 0;
    int is_join_bl2 = 0;
    int is_join_br2 = 0;
    int is_join_t_1 = 0;
    int is_join_t_2 = 0;
    int is_join_t_3 = 0;
    int is_join_t90_1 = 0;
    int is_join_t90_2 = 0;
    int is_join_t90_3 = 0;
    int is_join_t180_1 = 0;
    int is_join_t180_2 = 0;
    int is_join_t180_3 = 0;
    int is_join_t270_1 = 0;
    int is_join_t270_2 = 0;
    int is_join_t270_3 = 0;
    int is_join_x1 = 0;
    int is_join_x1_270 = 0;
    int is_join_x1_180 = 0;
    int is_join_x1_90 = 0;
    int is_join_x2 = 0;
    int is_join_x2_270 = 0;
    int is_join_x2_180 = 0;
    int is_join_x2_90 = 0;
    int is_join_x3 = 0;
    int is_join_x3_180 = 0;
    int is_join_x4 = 0;
    int is_join_x4_270 = 0;
    int is_join_x4_180 = 0;
    int is_join_x4_90 = 0;
    int is_join_horiz2 = 0;
    int is_join_vert2 = 0;
    int left = 0;
    int right = 0;
    int up = 0;
    int down = 0;
    int none = 0;
    int is_yyy5 = 0;
    int is_yyy6 = 0;
    int is_yyy7 = 0;
    int is_yyy8 = 0;
    int is_yyy9 = 0;
    int is_yyy10 = 0;
    int is_yyy11 = 0;
    int is_yyy12 = 0;
    int is_yyy13 = 0;
    int is_yyy14 = 0;
    int is_yyy15 = 0;
    int is_submerged = 0;
    int is_sleeping = 0;
    int is_open = 0;
    int is_dead = 0;
    int is_bloodied = 0;
    int is_end_of_anim = 0;
    int is_dead_on_end_of_anim = 0;
	
    static char *kwlist[] = {
        "class", 
        "tile",
        "delay_ms", 
        "is_moving", 
        "is_jumping", 
        "begin_jump", 
        "is_join_block", 
        "is_join_horiz", 
        "is_join_vert", 
        "is_join_node", 
        "is_join_left", 
        "is_join_right", 
        "is_join_top", 
        "is_join_bot", 
        "is_join_tl", 
        "is_join_tr", 
        "is_join_bl", 
        "is_join_br", 
        "is_join_t", 
        "is_join_t90", 
        "is_join_t180", 
        "is_join_t270", 
        "is_join_x", 
        "is_join_tl2", 
        "is_join_tr2", 
        "is_join_bl2", 
        "is_join_br2", 
        "is_join_t_1", 
        "is_join_t_2", 
        "is_join_t_3", 
        "is_join_t90_1", 
        "is_join_t90_2", 
        "is_join_t90_3", 
        "is_join_t180_1", 
        "is_join_t180_2", 
        "is_join_t180_3", 
        "is_join_t270_1", 
        "is_join_t270_2", 
        "is_join_t270_3", 
        "is_join_x1", 
        "is_join_x1_270", 
        "is_join_x1_180", 
        "is_join_x1_90", 
        "is_join_x2", 
        "is_join_x2_270", 
        "is_join_x2_180", 
        "is_join_x2_90", 
        "is_join_x3", 
        "is_join_x3_180", 
        "is_join_x4", 
        "is_join_x4_270", 
        "is_join_x4_180", 
        "is_join_x4_90", 
        "is_join_horiz2", 
        "is_join_vert2", 
        "is_dir_left", 
        "is_dir_right", 
        "is_dir_up", 
        "is_dir_down", 
        "is_dir_none", 
        "is_yyy5", 
        "is_yyy6", 
        "is_yyy7", 
        "is_yyy8", 
        "is_yyy9", 
        "is_yyy10", 
        "is_yyy11", 
        "is_yyy12", 
        "is_yyy13", 
        "is_yyy14", 
        "is_yyy15", 
        "is_submerged", 
        "is_sleeping", 
        "is_open", 
        "is_dead", 
        "is_bloodied", 
        "is_end_of_anim", 
        "is_dead_on_end_of_anim", 
        0
    };
	
    if (!PyArg_ParseTupleAndKeywords(args, keywds, 
                                     "O|siiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii", 
                                     kwlist, &py_class,
                                     &tile,
                                     &delay_ms,
                                     &is_moving,
                                     &is_jumping,
                                     &begin_jump,
                                     &is_join_block,
                                     &is_join_horiz,
                                     &is_join_vert,
                                     &is_join_node,
                                     &is_join_left,
                                     &is_join_right,
                                     &is_join_top,
                                     &is_join_bot,
                                     &is_join_tl,
                                     &is_join_tr,
                                     &is_join_bl,
                                     &is_join_br,
                                     &is_join_t,
                                     &is_join_t90,
                                     &is_join_t180,
                                     &is_join_t270,
                                     &is_join_x,
                                     &is_join_tl2,
                                     &is_join_tr2,
                                     &is_join_bl2,
                                     &is_join_br2,
                                     &is_join_t_1,
                                     &is_join_t_2,
                                     &is_join_t_3,
                                     &is_join_t90_1,
                                     &is_join_t90_2,
                                     &is_join_t90_3,
                                     &is_join_t180_1,
                                     &is_join_t180_2,
                                     &is_join_t180_3,
                                     &is_join_t270_1,
                                     &is_join_t270_2,
                                     &is_join_t270_3,
                                     &is_join_x1,
                                     &is_join_x1_270,
                                     &is_join_x1_180,
                                     &is_join_x1_90,
                                     &is_join_x2,
                                     &is_join_x2_270,
                                     &is_join_x2_180,
                                     &is_join_x2_90,
                                     &is_join_x3,
                                     &is_join_x3_180,
                                     &is_join_x4,
                                     &is_join_x4_270,
                                     &is_join_x4_180,
                                     &is_join_x4_90,
                                     &is_join_horiz2,
                                     &is_join_vert2,
                                     &left,
                                     &right,
                                     &up,
                                     &down,
                                     &none,
                                     &is_yyy5,
                                     &is_yyy6,
                                     &is_yyy7,
                                     &is_yyy8,
                                     &is_yyy9,
                                     &is_yyy10,
                                     &is_yyy11,
                                     &is_yyy12,
                                     &is_yyy13,
                                     &is_yyy14,
                                     &is_yyy15,
                                     &is_submerged,
                                     &is_sleeping,
                                     &is_open,
                                     &is_dead,
                                     &is_bloodied,
                                     &is_end_of_anim,
                                     &is_dead_on_end_of_anim
                                     )) {	
        return (0);	
    }	
	
    if (!py_class) {	
        ERR("%s, missing class", __FUNCTION__);	
        return (0);	
    }	
	
    tp_name = py_obj_attr_str(py_class, "name");	
    if (!tp_name) {	
        ERR("%s, missing tp name", __FUNCTION__);	
        goto done;	
    }	
	
    LOG("%s(%s -> \"%s\")", __FUNCTION__, tp_name, tile);	
	
    tpp tp = tp_find(tp_name);	
    if (!tp) {	
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	
        goto done;	
    }	
	
    if (!tp->tiles) {
        tp->tiles = tree_alloc(TREE_KEY_INTEGER, "TREE ROOT: thing tiles");
    }

    if (thing_tile_count >= TILES_ANIM_MAX) {
        DIE("out of anim tile space");
    }

    t = &thing_tile_arr[thing_tile_count];
    newptr(t, "tile");

    static uint32_t id;
    t->tree.key = id++;
    t->array_index = thing_tile_count++;

    if (!tree_insert_static(tp->tiles, &t->tree.node)) {
        ERR("insert thing tiles node fail");
    }

    if (t->has_dir_anim) {
        tp->has_dir_anim = true;
    }

    if (t->is_submerged) {
        tp->has_submerged_anim = true;
    }

    t->tile = tile_find(tile);
    if (!t->tile) {
        ERR("%s, cannot find tile %s for tp %s", __FUNCTION__, tile, tp_name);
    }
    t->tilename = dupstr(tile, "tilename");

    t->delay_ms = delay_ms;
    t->is_moving = is_moving;
    t->is_jumping = is_jumping;
    t->begin_jump = begin_jump;
    t->is_join_block = is_join_block;
    t->is_join_horiz = is_join_horiz;
    t->is_join_vert = is_join_vert;
    t->is_join_node = is_join_node;
    t->is_join_left = is_join_left;
    t->is_join_right = is_join_right;
    t->is_join_top = is_join_top;
    t->is_join_bot = is_join_bot;
    t->is_join_tl = is_join_tl;
    t->is_join_tr = is_join_tr;
    t->is_join_bl = is_join_bl;
    t->is_join_br = is_join_br;
    t->is_join_t = is_join_t;
    t->is_join_t90 = is_join_t90;
    t->is_join_t180 = is_join_t180;
    t->is_join_t270 = is_join_t270;
    t->is_join_x = is_join_x;
    t->is_join_tl2 = is_join_tl2;
    t->is_join_tr2 = is_join_tr2;
    t->is_join_bl2 = is_join_bl2;
    t->is_join_br2 = is_join_br2;
    t->is_join_t_1 = is_join_t_1;
    t->is_join_t_2 = is_join_t_2;
    t->is_join_t_3 = is_join_t_3;
    t->is_join_t90_1 = is_join_t90_1;
    t->is_join_t90_2 = is_join_t90_2;
    t->is_join_t90_3 = is_join_t90_3;
    t->is_join_t180_1 = is_join_t180_1;
    t->is_join_t180_2 = is_join_t180_2;
    t->is_join_t180_3 = is_join_t180_3;
    t->is_join_t270_1 = is_join_t270_1;
    t->is_join_t270_2 = is_join_t270_2;
    t->is_join_t270_3 = is_join_t270_3;
    t->is_join_x1 = is_join_x1;
    t->is_join_x1_270 = is_join_x1_270;
    t->is_join_x1_180 = is_join_x1_180;
    t->is_join_x1_90 = is_join_x1_90;
    t->is_join_x2 = is_join_x2;
    t->is_join_x2_270 = is_join_x2_270;
    t->is_join_x2_180 = is_join_x2_180;
    t->is_join_x2_90 = is_join_x2_90;
    t->is_join_x3 = is_join_x3;
    t->is_join_x3_180 = is_join_x3_180;
    t->is_join_x4 = is_join_x4;
    t->is_join_x4_270 = is_join_x4_270;
    t->is_join_x4_180 = is_join_x4_180;
    t->is_join_x4_90 = is_join_x4_90;
    t->is_join_horiz2 = is_join_horiz2;
    t->is_join_vert2 = is_join_vert2;
    t->is_yyy5 = is_yyy5;
    t->is_yyy6 = is_yyy6;
    t->is_yyy7 = is_yyy7;
    t->is_yyy8 = is_yyy8;
    t->is_yyy9 = is_yyy9;
    t->is_yyy10 = is_yyy10;
    t->is_yyy11 = is_yyy11;
    t->is_yyy12 = is_yyy12;
    t->is_yyy13 = is_yyy13;
    t->is_yyy14 = is_yyy14;
    t->is_yyy15 = is_yyy15;
    t->is_submerged = is_submerged;
    t->is_sleeping = is_sleeping;
    t->is_open = is_open;
    t->is_dead = is_dead;
    t->is_bloodied = is_bloodied;
    t->is_end_of_anim = is_end_of_anim;
    t->is_dead_on_end_of_anim = is_dead_on_end_of_anim;

    if (up) {
        if (left) {
            t->dir = THING_DIR_TL;
            t->has_dir_anim = true;
        } else if (right) {
            t->dir = THING_DIR_TR;
            t->has_dir_anim = true;
        } else {
            t->dir = THING_DIR_UP;
            t->has_dir_anim = true;
        }
    } else if (down) {
        if (left) {
            t->dir = THING_DIR_BL;
            t->has_dir_anim = true;
        } else if (right) {
            t->dir = THING_DIR_BR;
            t->has_dir_anim = true;
        } else {
            t->dir = THING_DIR_DOWN;
            t->has_dir_anim = true;
        }
    } else if (left) {
        t->dir = THING_DIR_LEFT;
        t->has_dir_anim = true;
    } else if (right) {
        t->dir = THING_DIR_RIGHT;
        t->has_dir_anim = true;
    } else if (none) {
        t->dir = THING_DIR_NONE;
        t->has_dir_anim = true;
    }
	
done:	
    if (tp_name) {	
        myfree(tp_name);	
    }	
	
    Py_RETURN_NONE;	
}	
