/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"
#include "wid_game_map.h"
#include "map.h"

PyObject *biome_set_is_land_ (PyObject *obj, PyObject *args,
                              PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.biome_set_is_land)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *biome_set_is_dungeon_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.biome_set_is_dungeon)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_sound_volume_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.sound_volume)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_music_volume_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.music_volume)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_display_sync_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.display_sync)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_full_screen_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.full_screen)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_move_count_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.move_count)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_moves_per_day_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.moves_per_day)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_map_fixup_ (PyObject *obj, PyObject *args,
                           PyObject *keywds)
{
    map_fixup(&game.level);

    Py_RETURN_NONE;
}

PyObject *game_map_add_selection_buttons_ (PyObject *obj, PyObject *args,
                                           PyObject *keywds)
{
    map_add_selection_buttons(&game.level);

    Py_RETURN_NONE;
}

PyObject *game_map_remove_selection_buttons_ (PyObject *obj, PyObject *args,
                                              PyObject *keywds)
{
    map_remove_selection_buttons(&game.level);

    Py_RETURN_NONE;
}

PyObject *game_map_set_selection_buttons_ (PyObject *obj, PyObject *args,
                                           PyObject *keywds)
{
    const char *tp_name;
    int x = 0;
    int y = 0;

    static char *kwlist[] = {(char*) "x", (char*) "y", (char*) "what", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iis", kwlist,
                                     &x,
                                     &y,
                                     &tp_name)) {
        return (0);
    }

    map_set_selection_buttons(&game.level, x, y, tp_name);

    Py_RETURN_NONE;
}

PyObject *game_map_clear_selection_buttons_ (PyObject *obj, PyObject *args,
                                              PyObject *keywds)
{
    map_clear_selection_buttons(&game.level);

    Py_RETURN_NONE;
}

PyObject *game_map_cleanup_ (PyObject *obj, PyObject *args,
                             PyObject *keywds)
{
    map_cleanup(&game.level);

    Py_RETURN_NONE;
}

PyObject *game_map_time_step_ (PyObject *obj, PyObject *args,
                               PyObject *keywds)
{
    map_time_step(&game.level);

    Py_RETURN_NONE;
}

PyObject *game_set_zzz1_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz1)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz2_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz2)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz3_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz3)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz4_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz4)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz5_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz5)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz6_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz6)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz7_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz7)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz8_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz8)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz9_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz9)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz10_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz10)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz11_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz11)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz12_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz12)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz13_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz13)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz14_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz14)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz15_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz15)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz16_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz16)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz17_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz17)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz18_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz18)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz19_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz19)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz20_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz20)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz21_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz21)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz22_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz22)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz23_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz23)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz24_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz24)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz25_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz25)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz26_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz26)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz27_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz27)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz28_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz28)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz29_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz29)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz30_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz30)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz31_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz31)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz32_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz32)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz33_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz33)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz34_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz34)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz35_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz35)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz36_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz36)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz37_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz37)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz38_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz38)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz39_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz39)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz40_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz40)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz41_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz41)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz42_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz42)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz43_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz43)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz44_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz44)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz45_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz45)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz46_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz46)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz47_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz47)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz48_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz48)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz49_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz49)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz50_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz50)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz51_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz51)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz52_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz52)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz53_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz53)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz54_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz54)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz55_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz55)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz56_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz56)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz57_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz57)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz58_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz58)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz59_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz59)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz60_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz60)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz61_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz61)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz62_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz62)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz63_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz63)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz64_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz64)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz65_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz65)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz66_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz66)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz67_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz67)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz68_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz68)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz69_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz69)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz70_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz70)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz71_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz71)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz72_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz72)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz73_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz73)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz74_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz74)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz75_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz75)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz76_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz76)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz77_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz77)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz78_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz78)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz79_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz79)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz80_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz80)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz81_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz81)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz82_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz82)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz83_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz83)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz84_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz84)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz85_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz85)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz86_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz86)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz87_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz87)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz88_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz88)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz89_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz89)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz90_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz90)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz91_ (PyObject *obj, PyObject *args,
                                 PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz91)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz92_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz92)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_zzz93_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.zzz93)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_sdl_delay_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.sdl_delay)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_daylight_color_a_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.daylight_color_a)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_daylight_color_b_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.daylight_color_b)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_daylight_color_g_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.daylight_color_g)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_daylight_color_r_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.daylight_color_r)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_rain_amount_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.rain_amount)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_set_snow_amount_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    static char *kwlist[] = {(char*) "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &game.snow_amount)) {
        Py_RETURN_NONE;
    }

    Py_RETURN_NONE;
}

PyObject *game_scroll_chunk_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    static char *kwlist[] = {(char*) "dx", (char*) "dy", 0};
    int dx = 0;
    int dy = 0;

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii", kwlist,
                                     &dx, &dy)) {
        Py_RETURN_NONE;
    }

    wid_game_map_scroll_chunk(dx, dy);

    Py_RETURN_NONE;
}
