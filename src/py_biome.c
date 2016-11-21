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

PyObject *game_set_sound_volume_ (PyObject *obj, PyObject *args, 
                                 PyObject *keywds)
{
    static char *kwlist[] = {"value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist, 
                                     &game.sound_volume)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_music_volume_ (PyObject *obj, PyObject *args, 
                                 PyObject *keywds)
{
    static char *kwlist[] = {"value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist, 
                                     &game.music_volume)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_display_sync_ (PyObject *obj, PyObject *args, 
                                 PyObject *keywds)
{
    static char *kwlist[] = {"value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist, 
                                     &game.display_sync)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_full_screen_ (PyObject *obj, PyObject *args, 
                                 PyObject *keywds)
{
    static char *kwlist[] = {"value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist, 
                                     &game.full_screen)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_move_count_ (PyObject *obj, PyObject *args, 
                                 PyObject *keywds)
{
    static char *kwlist[] = {"value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist, 
                                     &game.move_count)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}
