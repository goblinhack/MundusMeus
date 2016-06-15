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

PyObject *tp_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *a = 0;

    static char *kwlist[] = {"tp", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, &a)) {
        return (0);
    }

    if (!a) {
        ERR("tp_load, missing name attr");
        return (0);
    }

    char *b = py_obj_attr_str(a, "name");
    if (!b) {
        ERR("tp_load, missing tp name");
        return (0);
    }

    static int id;
    id++;
    tp_load(id, b);

    myfree(b);

    Py_RETURN_NONE;
}

PyObject *tp_set_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *a = 0;
    char *b = 0;
    char *c = 0;

    char *tp_name = 0;
    char *tp_field = 0;
    char *tp_value = 0;

    static char *kwlist[] = {"tp", "field", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|ss", kwlist, &a, &b, &c)) {
        return (0);
    }

    if (!a) {
        ERR("tp_set, missing name attr");
        return (0);
    }

    if (!b) {
        ERR("tp_set, missing field");
        return (0);
    }

    if (!c) {
        ERR("tp_set, missing value");
        return (0);
    }

    tp_name = py_obj_attr_str(a, "name");
    if (!tp_name) {
        ERR("tp_set, missing tp name");
        goto done;
    }
CON("tp_name %s",tp_name);
CON("field %s",b);
CON("value %s",c);

    CON("tp_set(%s, %s -> %s", tp_name, b, c);

    tpp tp = tp_find(tp_name);
    if (!tp) {
        myfree(tp_name);
        myfree(tp_field);
        myfree(tp_value);
        ERR("tp_set, cannot find tp %s", tp_name);
        goto done;
    }

done:
    myfree(tp_name);
    myfree(tp_field);
    myfree(tp_value);

    Py_RETURN_NONE;
}
