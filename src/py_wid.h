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
