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

TP_PROTO(tile)
TP_PROTO(light_radius)
TP_PROTO(scale)
TP_PROTO(z_depth)
TP_PROTO(z_order)
TP_PROTO(world_depth)
TP_PROTO(speed)
TP_PROTO(blit_top_off)
TP_PROTO(blit_bot_off)
TP_PROTO(blit_left_off)
TP_PROTO(blit_right_off)
TP_PROTO(short_name)
TP_PROTO(raw_name)
TP_PROTO(light_tint)
TP_PROTO(is_animated)
TP_PROTO(is_animated_no_dir)
TP_PROTO(is_animation)
TP_PROTO(is_candle_light)
TP_PROTO(is_cats_eyes)
TP_PROTO(is_shadow_caster)
TP_PROTO(is_shadow_caster_soft)
TP_PROTO(is_door)
TP_PROTO(is_effect_fade_in_out)
TP_PROTO(is_effect_pulse)
TP_PROTO(is_effect_rotate_2way)
TP_PROTO(is_effect_sway)
TP_PROTO(is_explosion)
TP_PROTO(is_food)
TP_PROTO(is_hidden)
TP_PROTO(is_monst)
TP_PROTO(is_player)
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
TP_PROTO(is_shadow_caster)
TP_PROTO(is_shadow_caster_soft)
TP_PROTO(is_sleeping)
TP_PROTO(is_wall)
