/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"
// REMOVED #include "string_util.h"
// REMOVED #include "string_ext.h"
// REMOVED #include "frameobject.h"
#include "wid_tiles.h"
// REMOVED #include "tile.h"
// REMOVED #include "pixel.h"
// REMOVED #include "thing_template.h"

PyObject *wid_tiles_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = "unset file";
    double scale = 1.0;

    static char *kwlist[] = {"name", "scale", 0};

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

