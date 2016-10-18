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
