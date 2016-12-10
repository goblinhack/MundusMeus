/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"
#include "wid_tiles.h"

PyObject *wid_tiles_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = 0;
    double scale = 1.0;

    static char *kwlist[] = {(char*) "name", (char*) "scale", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "sd", kwlist, &a, &scale)) {
        Py_RETURN_NONE;
    }

    if (!a) {
        ERR("wid_tiles_load, missing file attr");
        Py_RETURN_NONE;
    }

    LOG("wid_tiles_load(name=%s, scale=%g)", a, scale);
    wid_tiles_load(a, scale);

    Py_RETURN_NONE;
}

