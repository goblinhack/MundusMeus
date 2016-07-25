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

#define WID_SET_PROTO(__field__)                                              \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds);

#define WID_PROTO(__field__)                                                  \
PyObject *wid_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds);

#define WID_SET_DECL(__field__)                                               \
    {"wid_set_" #__field__,                                                   \
        (PyCFunction)wid_set_ ## __field__,                                   \
        METH_VARARGS | METH_KEYWORDS,                                           \
        "set a " #__field__ " in a wid"},                                     \

#define WID_DECL(__field__)                                                   \
    {"wid_" #__field__,                                                       \
        (PyCFunction)wid_ ## __field__,                                       \
        METH_VARARGS | METH_KEYWORDS,                                           \
        "call " #__field__ " in a wid"},                                      \

