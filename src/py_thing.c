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
#include "py_thing.h"
#include "thing_tile.h"

PyObject *thing_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;

    static char *kwlist[] = {"thing", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, &py_class)) {
        return (0);
    }

    if (!py_class) {
        ERR("thing_load, missing name attr");
        return (0);
    }

    char *thing_name = py_obj_attr_str(py_class, "name");
    if (!thing_name) {
        ERR("thing_load, missing tp name");
        return (0);
    }

    thing_load(thing_name);

    myfree(thing_name);

    Py_RETURN_NONE;
}

#define THING_BODY_STRING(__field__)                                               \
PyObject *thing_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *thing_name = 0;	                                                        \
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
    thing_name = py_obj_attr_str(py_class, "name");	                        \
    if (!thing_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("%s(%s -> \"%s\")", __FUNCTION__, thing_name, value);	                \
	                                                                        \
    thingp tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = dupstr(value, __FUNCTION__);	                        \
    value = 0;	                                                                \
	                                                                        \
done:	                                                                        \
    if (thing_name) {	                                                        \
        myfree(thing_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define THING_BODY_STRING_FN(__field__, __fn__)                                 \
PyObject *thing_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *thing_name = 0;	                                                \
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
    thing_name = py_obj_attr_str(py_class, "name");	                        \
    if (!thing_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("%s(%s -> \"%s\")", __FUNCTION__, thing_name, value);	                \
	                                                                        \
    thingp tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = dupstr(value, __FUNCTION__);	                        \
    value = 0;	                                                                \
    (__fn__)(tp);                                                               \
	                                                                        \
done:	                                                                        \
    if (thing_name) {	                                                        \
        myfree(thing_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define THING_BODY_ENUM(__field__, __str2val__)                                 \
PyObject *thing_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *thing_name = 0;	                                                \
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
    thing_name = py_obj_attr_str(py_class, "name");	                        \
    if (!thing_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    thingp tp = thing_find(thing_name);	                                                \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = (__str2val__)(value);                                       \
    if (tp->__field__ == (typeof(tp->__field__))-1) {                           \
        ERR("%s, cannot find enum %s", __FUNCTION__, value);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("%s(%s -> \"%s\"[%d])", __FUNCTION__, thing_name, value, tp->__field__);	\
	                                                                        \
    value = 0;	                                                                \
	                                                                        \
done:	                                                                        \
    if (thing_name) {	                                                        \
        myfree(thing_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define THING_BODY_INT(__field__)                                                  \
PyObject *thing_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *thing_name = 0;	                                                \
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
    thing_name = py_obj_attr_str(py_class, "name");	                        \
    if (!thing_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("%s(%s -> %d)", __FUNCTION__, thing_name, value);	                \
	                                                                        \
    thingp tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = value;                                                      \
	                                                                        \
done:	                                                                        \
    if (thing_name) {	                                                        \
        myfree(thing_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define THING_BODY_DOUBLE(__field__)                                               \
PyObject *thing_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *thing_name = 0;	                                                \
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
    thing_name = py_obj_attr_str(py_class, "name");	                        \
    if (!thing_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("%s(%s -> %g)", __FUNCTION__, thing_name, value);	                \
	                                                                        \
    thingp tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = value;                                                      \
	                                                                        \
done:	                                                                        \
    if (thing_name) {	                                                        \
        myfree(thing_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \
