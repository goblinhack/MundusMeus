/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"
#include "py_thing.h"

PyObject *thing_new_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    const char *tp_name;
    long int thing_id = 0;

    static char *kwlist[] = {(char*) "thing", (char*) "thing_id", (char*) "tp", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|ls", kwlist,
                                     &py_class,
                                     &thing_id,
                                     &tp_name)) {
        return (0);
    }

    if (!py_class) {
        ERR("thing_new, missing name attr");
        return (0);
    }

    char *thing_name = py_obj_attr_str(py_class, "name");
    if (!thing_name) {
        ERR("thing_new, missing tp name");
        return (0);
    }

    thing_new(thing_name, thing_id, tp_name);

    myfree(thing_name);

    Py_RETURN_NONE;
}

#define THING_BODY_SET_STRING(__field__)                                               \
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
    /* LOG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, thing_name, value); */	\
	                                                                        \
    thingp tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find thing %s", __FUNCTION__, thing_name);	        \
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

#define THING_BODY_SET_STRING_FN(__field__, __fn__)                                 \
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
    /* LOG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, thing_name, value); */	\
	                                                                        \
    thingp tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find thing %s", __FUNCTION__, thing_name);	                \
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

#define THING_BODY_SET_ENUM(__field__, __str2val__)                                 \
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
    thingp tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find thing %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = (__str2val__)(value);                                       \
    if (tp->__field__ == (TYPEOF(tp->__field__))-1) {                           \
        ERR("%s, cannot find enum %s", __FUNCTION__, value);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    /* LOG("python-to-c: %s(%s -> \"%s\"[%d])", __FUNCTION__, thing_name, value, tp->__field__); */ \
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

#define THING_BODY_SET_INT(__field__)                                           \
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
    /* LOG("python-to-c: %s(%s -> %d)", __FUNCTION__, thing_name, value); */	        \
	                                                                        \
    thingp tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find thing %s", __FUNCTION__, thing_name);	                \
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

#define THING_BODY_SET_DOUBLE(__field__)                                        \
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
    /* LOG("python-to-c: %s(%s -> %g)", __FUNCTION__, thing_name, value); */	        \
	                                                                        \
    thingp tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find thing %s", __FUNCTION__, thing_name);	                \
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

#define THING_BODY_STRING_FN(__field__, __fn__)                                 \
PyObject *thing_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *thing_name = 0;	                                                \
    const char *value = 0;	                                                \
    thingp tp;                                                                  \
	                                                                        \
    static char *kwlist[] = {(char*) "class", (char*) "value", 0};	        \
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
    /* LOG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, thing_name, value); */	\
	                                                                        \
    tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find thing %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(tp, value);                                                        \
	                                                                        \
done:	                                                                        \
    if (thing_name) {	                                                        \
        myfree(thing_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define THING_BODY_DOUBLE_FN(__field__, __fn__)                                 \
PyObject *thing_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *thing_name = 0;	                                                \
    double d1 = 0;                                                              \
    thingp tp;                                                                  \
	                                                                        \
    static char *kwlist[] = {(char*) "class", (char*) "value", 0};	        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|d", kwlist, &py_class,    \
                                     &d1)) {	                                \
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
    /* LOG("python-to-c: %s(%s -> %f)", __FUNCTION__, thing_name, d1);	 */     \
	                                                                        \
    tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find thing %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(tp, d1);                                                           \
	                                                                        \
done:	                                                                        \
    if (thing_name) {	                                                        \
        myfree(thing_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define THING_BODY_DOUBLE_DOUBLE_VOID_FN(__field__, __fn__)                     \
PyObject *thing_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *thing_name = 0;	                                                \
    double d1 = 0;                                                              \
    double d2 = 0;                                                              \
    thingp tp;                                                                  \
	                                                                        \
    static char *kwlist[] = {(char*) "class", (char*) "x", (char*) "y", 0};	                        \
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
    thing_name = py_obj_attr_str(py_class, "name");	                        \
    if (!thing_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    /* LOG("python-to-c: %s(%s -> %f, %f)", __FUNCTION__, thing_name, d1, d2); */	\
	                                                                        \
    tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find thing %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(tp, d1, d2);                                                       \
	                                                                        \
done:	                                                                        \
    if (thing_name) {	                                                        \
        myfree(thing_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define THING_BODY_DOUBLE_DOUBLE_FN(__field__, __fn__)                          \
PyObject *thing_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *thing_name = 0;	                                                \
    double d1 = 0;                                                              \
    double d2 = 0;                                                              \
    PyObject *o = 0;                                                            \
    thingp tp;                                                                  \
	                                                                        \
    static char *kwlist[] = {(char*) "class", (char*) "x", (char*) "y", 0};	                        \
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
    thing_name = py_obj_attr_str(py_class, "name");	                        \
    if (!thing_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    /* LOG("python-to-c: %s(%s -> %f, %f)", __FUNCTION__, thing_name, d1, d2);	 */ \
	                                                                        \
    tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find thing %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    o = (__fn__)(tp, d1, d2);                                                   \
	                                                                        \
done:	                                                                        \
    if (thing_name) {	                                                        \
        myfree(thing_name);	                                                \
    }	                                                                        \
	                                                                        \
    return (o);	                                                                \
}	                                                                        \

#define THING_BODY_VOID_FN(__field__, __fn__)                                   \
PyObject *thing_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *thing_name = 0;	                                                \
    thingp tp;                                                                  \
	                                                                        \
    static char *kwlist[] = {(char*) "class", 0};	                        \
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
    thing_name = py_obj_attr_str(py_class, "name");	                        \
    if (!thing_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    /* LOG("python-to-c: %s(%s)", __FUNCTION__, thing_name); */	                \
	                                                                        \
    tp = thing_find(thing_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find thing %s", __FUNCTION__, thing_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(tp);                                                               \
	                                                                        \
done:	                                                                        \
    if (thing_name) {	                                                        \
        myfree(thing_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

THING_BODY_STRING_FN(destroyed, thing_destroyed_)
THING_BODY_STRING_FN(set_tilename, thing_set_tilename_)
THING_BODY_STRING_FN(set_tp, thing_set_tp_)
THING_BODY_DOUBLE_FN(set_depth, thing_set_depth_)
THING_BODY_DOUBLE_DOUBLE_VOID_FN(move, thing_move_)
THING_BODY_DOUBLE_DOUBLE_FN(push, thing_push_)
THING_BODY_VOID_FN(pop, thing_pop_)
