/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"

PyObject *biome_set_is_land_ (PyObject *obj, PyObject *args, 
                              PyObject *keywds)
{
    static char *kwlist[] = {"value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist, 
                                     &game.biome_set_is_land)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *biome_set_is_dungeon_ (PyObject *obj, PyObject *args, 
                                 PyObject *keywds)
{
    static char *kwlist[] = {"value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist, 
                                     &game.biome_set_is_dungeon)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}
