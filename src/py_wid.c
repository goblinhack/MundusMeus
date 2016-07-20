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
#include "py_wid.h"
#include "thing.h"

PyObject *wid_new_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    uintptr_t i_parent = 0;
    widp parent;
    char *name = 0;
    widp wid;

    static char *kwlist[] = {"wid", "parent", "name", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|Ks", kwlist, 
                                     &py_class,
                                     &i_parent,
                                     &name)) {
        return (0);
    }

    if (!name) {
        ERR("wid_new, missing name");
        return (0);
    }

    parent = (widp) i_parent;

    wid = wid_new_container(parent, name);

    return (Py_BuildValue("K", (uintptr_t) wid));
}

PyObject *wid_destroy_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");

    wid_destroy(&w);

    Py_RETURN_NONE;	                                                        \
}

#define WID_BODY_SET_STRING(__field__)                                               \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
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
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, wid_name, value);	\
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = dupstr(value, __FUNCTION__);	                        \
    value = 0;	                                                                \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_SET_STRING_FN(__field__, __fn__)                                 \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
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
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, wid_name, value);	\
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = dupstr(value, __FUNCTION__);	                        \
    value = 0;	                                                                \
    (__fn__)(tp);                                                               \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_SET_ENUM(__field__, __str2val__)                                 \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
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
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = (__str2val__)(value);                                       \
    if (tp->__field__ == (typeof(tp->__field__))-1) {                           \
        ERR("%s, cannot find enum %s", __FUNCTION__, value);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> \"%s\"[%d])", __FUNCTION__, wid_name, value, tp->__field__);\
	                                                                        \
    value = 0;	                                                                \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_SET_INT(__field__)                                                  \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
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
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> %d)", __FUNCTION__, wid_name, value);	        \
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = value;                                                      \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_SET_DOUBLE(__field__)                                               \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
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
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> %g)", __FUNCTION__, wid_name, value);	        \
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = value;                                                      \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_STRING_FN(__field__, __fn__)                                 \
PyObject *wid_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
    const char *value = 0;	                                                \
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
    wid_name = py_obj_attr_str(py_class, "wid_id");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, wid_name, value);	\
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(tp, value);                                                        \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_DOUBLE_DOUBLE_FN(__field__, __fn__)                          \
PyObject *wid_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
    double d1 = 0;                                                              \
    double d2 = 0;                                                              \
	                                                                        \
    static char *kwlist[] = {"class", "x", "y", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|dd", kwlist, &py_class,   \
                                     &d1, &d2)) {	                        \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> %f, %f)", __FUNCTION__, wid_name, d1, d2);	\
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(tp, d1, d2);                                                       \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_VOID_FN(__field__, __fn__)                                   \
PyObject *wid_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
	                                                                        \
    static char *kwlist[] = {"class", 0};	                                \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, &py_class)) {   \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s)", __FUNCTION__, wid_name);	                \
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(tp);                                                               \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \
