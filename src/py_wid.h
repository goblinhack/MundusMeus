/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include <Python.h>

PyObject *wid_new_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *wid_destroy_(PyObject *obj, PyObject *args, PyObject *keywds);
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
