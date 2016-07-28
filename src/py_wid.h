/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include <Python.h>

PyObject *wid_new_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_destroy_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_new_scrollbar_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_shape_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_tl_br_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_tl_br_pct_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_text_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_color_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_name_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_tex_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_raise_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_lower_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_update_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_bevel_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_bevelled_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_cursor_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_do_not_lower_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_do_not_raise_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_focusable_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_movable_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_movable_bounded_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_movable_horiz_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_set_movable_vert_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_get_size_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_get_size_pct_(PyObject *obj, PyObject *args, PyObject *keywds);

#define WID_PROTO(__fn__)                                                   \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds);

#define WID_DECL(__fn__)                                                    \
    {#__fn__,                                                               \
     (PyCFunction)__fn__ ## _,                                              \
     METH_VARARGS | METH_KEYWORDS,                                          \
     "call " #__fn__ " in a wid"},                                          \

WID_PROTO(wid_move_to_horiz_vert_pct_in)
WID_PROTO(wid_move_to_abs_centered_in)
WID_PROTO(wid_move_to_centered_in)
WID_PROTO(wid_move_delta_pct_in)
WID_PROTO(wid_move_to_abs_in)
WID_PROTO(wid_move_delta_in)
WID_PROTO(wid_move_to_pct_centered_in)
WID_PROTO(wid_move_to_abs_poffset_in)
WID_PROTO(wid_move_to_pct_in)

WID_PROTO(wid_move_delta)
WID_PROTO(wid_move_to_abs)
WID_PROTO(wid_move_to_abs_centered)
WID_PROTO(wid_move_to_pct)
WID_PROTO(wid_move_to_pct_centered)
WID_PROTO(wid_move_to_vert_pct_in)
WID_PROTO(wid_move_to_horiz_pct_in)

WID_PROTO(wid_move_to_vert_pct)
WID_PROTO(wid_move_to_horiz_pct)

WID_PROTO(wid_move_to_bottom)
WID_PROTO(wid_move_to_left)
WID_PROTO(wid_move_to_right)
WID_PROTO(wid_move_to_top)
WID_PROTO(wid_move_end)
