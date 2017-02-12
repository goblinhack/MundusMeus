/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"
#include "tile.h"

PyObject *tile_load_arr_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *tex_name = 0;
    char *tex_name_black_and_white = 0;
    int width = 0;
    int height = 0;
    PyObject *e = 0;

    static char *kwlist[] = {(char*) "tex_name", (char*) "tex_name_black_and_white", (char*) "width", (char*) "height", (char*) "arr", 0};

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

PyObject *tile_get_size_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *name = 0;
    tilep tile = 0;

    static char *kwlist[] = {
        (char*) "name",
        0
    };

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist,
                                     &name)) {
        Py_RETURN_NONE;
    }

    if (!name) {
        DIE("no tile name");
    }

    tile = tile_find(name);
    if (!tile) {
        DIE("no tile named %s", name);
    }

    double w = tile_get_width(tile);
    double h = tile_get_height(tile);

    return (Py_BuildValue("dd", w, h));
}

PyObject *tile_get_size_pct_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *name = 0;
    tilep tile = 0;

    static char *kwlist[] = {
        (char*) "name",
        0
    };

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist,
                                     &name)) {
        Py_RETURN_NONE;
    }

    if (!name) {
        DIE("no tile name");
    }

    tile = tile_find(name);
    if (!tile) {
        DIE("no tile named %s", name);
    }

    double w = tile_get_width(tile);
    double h = tile_get_height(tile);

    w = w / ((double) game.video_gl_width);
    h = h / ((double) game.video_gl_height);

    return (Py_BuildValue("dd", w, h));
}
