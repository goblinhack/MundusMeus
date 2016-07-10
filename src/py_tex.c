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
#include "pixel.h"
#include "thing_template.h"

PyObject *tex_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = "unset file";
    char *b = "unset name";

    static char *kwlist[] = {"file", "name", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ss", kwlist, &a, &b)) {
        Py_RETURN_NONE;
    }

    if (!a) {
        ERR("tex_load, missing file attr");
        Py_RETURN_NONE;
    }

    if (!b) {
        ERR("tex_load, missing name attr");
        Py_RETURN_NONE;
    }

    LOG("tex_load(file=%s, name=%s)", a, b);
    tex_load(a, b);

    Py_RETURN_NONE;
}

PyObject *tex_size_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *tex = "unset tex";

    static char *kwlist[] = {"tex", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist, &tex)) {
        Py_RETURN_NONE;
    }

    texp t = tex_find(tex);

    if (!tex) {
        ERR("tex_load, missing file attr");
        Py_RETURN_NONE;
    }

    return (Py_BuildValue("(ii)", tex_get_width(t), tex_get_height(t)));
}

PyObject *tex_pixel_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *tex = "unset tex";
    int x = 0;
    int y = 0;

    static char *kwlist[] = {"tex", "x", "y", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "sii", kwlist, &tex, &x, &y)) {
        Py_RETURN_NONE;
    }

    texp t = tex_find(tex);

    if (!tex) {
        ERR("tex_load, missing file attr");
        Py_RETURN_NONE;
    }

    color c;

    c = getPixel(tex_get_surface(t), x, y);

    return (Py_BuildValue("(iiii)", c.r, c.g, c.b, c.a));
}

PyObject *tex_load_tiled_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = "unset file";
    char *b = "unset name";
    int c = 0;
    int d = 0;

    static char *kwlist[] = {"file", "name", "width", "height", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ssii", kwlist, &a, &b, &c, &d)) {
        Py_RETURN_NONE;
    }

    if (!a) {
        ERR("tex_load, missing file attr");
        Py_RETURN_NONE;
    }

    if (!b) {
        ERR("tex_load, missing name attr");
        Py_RETURN_NONE;
    }

    if (!c) {
        ERR("tex_load, missing width attr");
        Py_RETURN_NONE;
    }

    if (!d) {
        ERR("tex_load, missing height attr");
        Py_RETURN_NONE;
    }


    LOG("tex_load(file=%s, name=%s, width=%d, height=%d)", a, b, c, d);
    tex_load_tiled(a, b, c, d);

    Py_RETURN_NONE;
}

PyObject *tile_load_arr_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *tex_name = "unset tex_name";
    char *tex_name_black_and_white = "unset tex_name_black_and_white";
    int width = 0;
    int height = 0;
    PyObject *e = 0;

    static char *kwlist[] = {"tex_name", "tex_name_black_and_white", "width", "height", "arr", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ssiiO", kwlist, &tex_name, &tex_name_black_and_white, &width, &height, &e)) {
        Py_RETURN_NONE;
    }

    if (!tex_name) {
        ERR("tile_load_arr, missing tex_name attr");
        Py_RETURN_NONE;
    }

    if (!tex_name_black_and_white) {
        ERR("tile_load_arr, missing tex_name_black_and_white attr");
        Py_RETURN_NONE;
    }

    if (!width) {
        ERR("tile_load_arr, missing width attr");
        Py_RETURN_NONE;
    }

    if (!height) {
        ERR("tile_load_arr, missing height attr");
        Py_RETURN_NONE;
    }

    if (!e) {
        ERR("tile_load_arr, missing arr attr");
        Py_RETURN_NONE;
    }

    int numLines = PyList_Size(e);
    const char *arr[numLines + 1];
    int i;

    memset(arr, 0, sizeof(arr));

    LOG("tile_load_arr(tex_name=%s, tex_name_black_and_white=%s, width=%d, height=%d, ...)", tex_name, tex_name_black_and_white, width, height);

    for (i=0; i<numLines; i++){
        PyObject * strObj;
        strObj = PyList_GetItem(e, i); /* Can't fail */
        if (!strObj) {
            continue;
        }

        arr[i] = py_obj_to_str(strObj);
    }

    tile_load_arr(tex_name, tex_name_black_and_white, width, height, numLines, arr);

    for (i=0; i<numLines; i++){
        if (!arr[i]) {
            continue;
        }

        myfree((char*) arr[i]);
    }

    Py_RETURN_NONE;
}
