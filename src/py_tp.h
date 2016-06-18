/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include <Python.h>

PyObject *tp_load_(PyObject *obj, PyObject *args, PyObject *keywds);

#define TP_PROTO_STRING(__field__)                                               \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds);

#define TP_DECL_STRING(__field__)                                               \
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
    CON("%s(%s -> \"%s\")", __FUNCTION__, tp_name, value);	                \
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

TP_PROTO_STRING(short_name)
TP_PROTO_STRING(raw_name)
TP_PROTO_STRING(tooltip)
TP_PROTO_STRING(polymorph_on_death)
TP_PROTO_STRING(carried_as)
TP_PROTO_STRING(light_tint)
TP_PROTO_STRING(explodes_as)
TP_PROTO_STRING(sound_on_creation)
TP_PROTO_STRING(sound_on_hitting_something)
TP_PROTO_STRING(sound_on_death)
TP_PROTO_STRING(sound_on_hit)
TP_PROTO_STRING(sound_on_collect)
TP_PROTO_STRING(sound_random)
TP_PROTO_STRING(weapon_carry_anim)
TP_PROTO_STRING(weapon_swing_anim)
TP_PROTO_STRING(message_on_use)
