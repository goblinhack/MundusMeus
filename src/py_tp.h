/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include <Python.h>

PyObject *tp_load_(PyObject *obj, PyObject *args, PyObject *keywds);

#define TP_SET_PROTO(__field__)                                                     \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds);

#define TP_SET_DECL(__field__)                                                      \
    {"tp_set_" #__field__,                                                      \
        (PyCFunction)tp_set_ ## __field__,                                      \
        METH_VARARGS | METH_KEYWORDS,                                           \
        "set a " #__field__ " in a thing template"},                            \

TP_SET_PROTO(tile)
TP_SET_PROTO(light_radius)
TP_SET_PROTO(scale)
TP_SET_PROTO(z_depth)
TP_SET_PROTO(z_order)
TP_SET_PROTO(z_depth)
TP_SET_PROTO(speed)
TP_SET_PROTO(blit_top_off)
TP_SET_PROTO(blit_bot_off)
TP_SET_PROTO(blit_left_off)
TP_SET_PROTO(blit_right_off)
TP_SET_PROTO(short_name)
TP_SET_PROTO(raw_name)
TP_SET_PROTO(light_tint)
TP_SET_PROTO(is_animated)
TP_SET_PROTO(is_animated_no_dir)
TP_SET_PROTO(is_animation)
TP_SET_PROTO(is_candle_light)
TP_SET_PROTO(is_cats_eyes)
TP_SET_PROTO(is_shadow_caster)
TP_SET_PROTO(is_shadow_caster_soft)
TP_SET_PROTO(is_door)
TP_SET_PROTO(is_water)
TP_SET_PROTO(is_lava)
TP_SET_PROTO(is_effect_fade_in_out)
TP_SET_PROTO(is_effect_pulse)
TP_SET_PROTO(is_effect_rotate_2way)
TP_SET_PROTO(is_effect_sway)
TP_SET_PROTO(is_explosion)
TP_SET_PROTO(is_food)
TP_SET_PROTO(is_weapon)
TP_SET_PROTO(is_hidden)
TP_SET_PROTO(is_monst)
TP_SET_PROTO(is_player)
TP_SET_PROTO(is_rock)
TP_SET_PROTO(is_rrr1)
TP_SET_PROTO(is_rrr2)
TP_SET_PROTO(is_rrr3)
TP_SET_PROTO(is_rrr4)
TP_SET_PROTO(is_rrr5)
TP_SET_PROTO(is_rrr6)
TP_SET_PROTO(is_rrr7)
TP_SET_PROTO(is_rrr8)
TP_SET_PROTO(is_rrr9)
TP_SET_PROTO(is_rrr10)
TP_SET_PROTO(is_rrr11)
TP_SET_PROTO(is_rrr12)
TP_SET_PROTO(is_rrr13)
TP_SET_PROTO(is_rrr14)
TP_SET_PROTO(is_rrr15)
TP_SET_PROTO(is_rrr16)
TP_SET_PROTO(is_rrr17)
TP_SET_PROTO(is_rrr18)
TP_SET_PROTO(is_rrr19)
TP_SET_PROTO(is_rrr20)
TP_SET_PROTO(is_rrr21)
TP_SET_PROTO(is_rrr22)
TP_SET_PROTO(is_rrr23)
TP_SET_PROTO(is_rrr24)
TP_SET_PROTO(is_rrr25)
TP_SET_PROTO(is_rrr26)
TP_SET_PROTO(is_rrr27)
TP_SET_PROTO(is_rrr28)
TP_SET_PROTO(is_rrr29)
TP_SET_PROTO(is_rrr30)
TP_SET_PROTO(is_rrr31)
TP_SET_PROTO(is_rrr32)
TP_SET_PROTO(is_rrr33)
TP_SET_PROTO(is_rrr34)
TP_SET_PROTO(is_rrr35)
TP_SET_PROTO(is_rrr36)
TP_SET_PROTO(is_rrr37)
TP_SET_PROTO(is_rrr38)
TP_SET_PROTO(is_rrr39)
TP_SET_PROTO(is_rrr40)
TP_SET_PROTO(is_rrr41)
TP_SET_PROTO(is_rrr42)
TP_SET_PROTO(is_rrr43)
TP_SET_PROTO(is_rrr44)
TP_SET_PROTO(is_rrr45)
TP_SET_PROTO(is_rrr46)
TP_SET_PROTO(is_rrr47)
TP_SET_PROTO(is_rrr48)
TP_SET_PROTO(is_rrr49)
TP_SET_PROTO(is_rrr50)
TP_SET_PROTO(is_rrr51)
TP_SET_PROTO(is_rrr52)
TP_SET_PROTO(is_rrr53)
TP_SET_PROTO(is_rrr54)
TP_SET_PROTO(is_rrr55)
TP_SET_PROTO(is_rrr56)
TP_SET_PROTO(is_rrr57)
TP_SET_PROTO(is_rrr58)
TP_SET_PROTO(is_rrr59)
TP_SET_PROTO(is_rrr60)
TP_SET_PROTO(is_rrr61)
TP_SET_PROTO(is_rrr62)
TP_SET_PROTO(is_rrr63)
TP_SET_PROTO(is_rrr64)
TP_SET_PROTO(is_rrr65)
TP_SET_PROTO(is_rrr66)
TP_SET_PROTO(is_rrr67)
TP_SET_PROTO(is_rrr68)
TP_SET_PROTO(is_rrr69)
TP_SET_PROTO(is_rrr70)
TP_SET_PROTO(is_rrr71)
TP_SET_PROTO(is_rrr72)
TP_SET_PROTO(is_rrr73)
TP_SET_PROTO(is_rrr74)
TP_SET_PROTO(is_rrr75)
TP_SET_PROTO(is_rrr76)
TP_SET_PROTO(is_rrr77)
TP_SET_PROTO(is_rrr78)
TP_SET_PROTO(is_rrr79)
TP_SET_PROTO(is_rrr80)
TP_SET_PROTO(is_rrr81)
TP_SET_PROTO(is_rrr82)
TP_SET_PROTO(is_rrr83)
TP_SET_PROTO(is_rrr84)
TP_SET_PROTO(is_rrr85)
TP_SET_PROTO(is_rrr86)
TP_SET_PROTO(is_rrr87)
TP_SET_PROTO(is_rrr88)
TP_SET_PROTO(is_rrr89)
TP_SET_PROTO(is_rrr90)
TP_SET_PROTO(is_rrr91)
TP_SET_PROTO(is_rrr92)
TP_SET_PROTO(is_rrr93)
TP_SET_PROTO(is_landrock)
TP_SET_PROTO(is_snow)
TP_SET_PROTO(is_hidden)
TP_SET_PROTO(is_dirt)
TP_SET_PROTO(is_sand)
TP_SET_PROTO(is_plant)
TP_SET_PROTO(is_tree)
TP_SET_PROTO(is_grass)
TP_SET_PROTO(is_bridge)
TP_SET_PROTO(is_movement_blocking)
TP_SET_PROTO(is_treasure)
TP_SET_PROTO(is_entrance)
TP_SET_PROTO(is_exit)
TP_SET_PROTO(is_chasm_smoke)
TP_SET_PROTO(is_key)
TP_SET_PROTO(is_dusty)
TP_SET_PROTO(is_corridor)
TP_SET_PROTO(light_pulse_amount)
TP_SET_PROTO(is_healing)
TP_SET_PROTO(is_armor)
TP_SET_PROTO(is_magical)
TP_SET_PROTO(is_shadow_caster)
TP_SET_PROTO(is_shadow_caster_soft)
TP_SET_PROTO(is_sleeping)
TP_SET_PROTO(is_wall)
TP_SET_PROTO(is_door)
TP_SET_PROTO(is_water)
TP_SET_PROTO(is_lava)
TP_SET_PROTO(is_floor)
