/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"
#include "tex.h"
#include "pixel.h"

PyObject *tex_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = 0;
    char *b = 0;

    static char *kwlist[] = {(char*) "file", (char*) "name", 0};

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
    char *tex = 0;

    static char *kwlist[] = {(char*) "tex", 0};

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
    char *tex = 0;
    int x = 0;
    int y = 0;

    static char *kwlist[] = {(char*) "tex", (char*) "x", (char*) "y", 0};

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
    char *a = 0;
    char *b = 0;
    int c = 0;
    int d = 0;

    static char *kwlist[] = {(char*) "file", (char*) "name", (char*) "width", (char*) "height", 0};

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
