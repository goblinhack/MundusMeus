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

PyObject *tex_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = "unset file";
    char *b = "unset name";

    static char *kwlist[] = {"file", "name", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ss", kwlist, &a, &b)) {
        return (0);
    }

    if (!a) {
        ERR("tex_load, missing file attr");
        return (0);
    }

    if (!b) {
        ERR("tex_load, missing name attr");
        return (0);
    }

    LOG("tex_load(file=%s, name=%s)", a, b);
    tex_load(a, b);

    Py_RETURN_NONE;
}

PyObject *tex_load_tiled_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = "unset file";
    char *b = "unset name";
    int c = 0;
    int d = 0;

    static char *kwlist[] = {"file", "name", "width", "height", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ssii", kwlist, &a, &b, &c, &d)) {
        return (0);
    }

    if (!a) {
        ERR("tex_load, missing file attr");
        return (0);
    }

    if (!b) {
        ERR("tex_load, missing name attr");
        return (0);
    }

    if (!c) {
        ERR("tex_load, missing width attr");
        return (0);
    }

    if (!d) {
        ERR("tex_load, missing height attr");
        return (0);
    }


    LOG("tex_load(file=%s, name=%s, width=%d, height=%d)", a, b, c, d);
    tex_load_tiled(a, b, c, d);

    Py_RETURN_NONE;
}

PyObject *tile_load_arr_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = "unset tex_name";
    char *b = "unset tex_name_black_and_white";
    int c = 0;
    int d = 0;
    PyObject *e = 0;

    static char *kwlist[] = {"tex_name", "tex_name_black_and_white", "width", "height", "arr", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ssiiO", kwlist, &a, &b, &c, &d, &e)) {
        return (0);
    }

    if (!a) {
        ERR("tile_load_arr, missing tex_name attr");
        return (0);
    }

    if (!b) {
        ERR("tile_load_arr, missing tex_name_black_and_white attr");
        return (0);
    }

    if (!c) {
        ERR("tile_load_arr, missing width attr");
        return (0);
    }

    if (!d) {
        ERR("tile_load_arr, missing height attr");
        return (0);
    }

    if (!e) {
        ERR("tile_load_arr, missing arr attr");
        return (0);
    }

    int numLines = PyList_Size(e);
    const char *arr[numLines + 1];
    int i;

    memset(arr, 0, sizeof(arr));

    LOG("tile_load_arr(tex_name=%s, tex_name_black_and_white=%s, width=%d, height=%d, ...)", a, b, c, d);

    for (i=0; i<numLines; i++){
        PyObject * strObj;
        strObj = PyList_GetItem(e, i); /* Can't fail */
        if (!strObj) {
            continue;
        }

        arr[i] = py_obj_to_str(strObj);
    }

    tile_load_arr(a, b, c, d, numLines, arr);

    for (i=0; i<numLines; i++){
        if (!arr[i]) {
            continue;
        }

        myfree((char*) arr[i]);
    }

    Py_RETURN_NONE;
}
