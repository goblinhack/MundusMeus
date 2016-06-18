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
#include "py_tp.h"

PyObject *tp_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;

    static char *kwlist[] = {"tp", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, &py_class)) {
        return (0);
    }

    if (!py_class) {
        ERR("tp_load, missing name attr");
        return (0);
    }

    char *tp_name = py_obj_attr_str(py_class, "name");
    if (!tp_name) {
        ERR("tp_load, missing tp name");
        return (0);
    }

    static int id;
    id++;
    tp_load(id, tp_name);

    myfree(tp_name);

    Py_RETURN_NONE;
}

TP_BODY_STRING(short_name)
TP_BODY_STRING(raw_name)
TP_BODY_STRING(tooltip)
TP_BODY_STRING(polymorph_on_death)
TP_BODY_STRING(carried_as)
TP_BODY_STRING(light_tint)
TP_BODY_STRING(explodes_as)
TP_BODY_STRING(sound_on_creation)
TP_BODY_STRING(sound_on_hitting_something)
TP_BODY_STRING(sound_on_death)
TP_BODY_STRING(sound_on_hit)
TP_BODY_STRING(sound_on_collect)
TP_BODY_STRING(sound_random)
TP_BODY_STRING(weapon_carry_anim)
TP_BODY_STRING(weapon_swing_anim)
TP_BODY_STRING(message_on_use)
