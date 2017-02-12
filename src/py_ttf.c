/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"
#include "font.h"
#include "ttf.h"

PyObject *ttf_text_size_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *name = 0;
    char *text = 0;
    double scale = 1.0;
    double advance = 1.0;
    int fixed_width = false;
    fontp f = 0;

    static char *kwlist[] = {
        (char*) "font",
        (char*) "text",
        (char*) "scale",
        (char*) "advance",
        (char*) "fixed_width",
        0
    };

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ss|ddi", kwlist,
                                     &name,
                                     &text,
                                     &scale,
                                     &advance,
                                     &fixed_width)) {
        Py_RETURN_NONE;
    }

    if (!name) {
        DIE("no font name");
    }

    if (!text) {
        DIE("no font text");
    }

    f = font_find(name);
    if (!f) {
        DIE("no font named %s", name);
    }

    double w, h;
    const char *c = 0;
    ttf_text_size(&f, text, &w, &h, 0, &c, scale, advance, fixed_width);

    return (Py_BuildValue("dds", w, h, c ? c : "none"));
}

PyObject *ttf_text_size_pct_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *name = 0;
    char *text = 0;
    double scale = 1.0;
    double advance = 1.0;
    int fixed_width = false;
    fontp f = 0;

    static char *kwlist[] = {
        (char*) "font",
        (char*) "text",
        (char*) "scale",
        (char*) "advance",
        (char*) "fixed_width",
        0
    };

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ss|ddi", kwlist,
                                     &name,
                                     &text,
                                     &scale,
                                     &advance,
                                     &fixed_width)) {
        Py_RETURN_NONE;
    }

    if (!name) {
        DIE("no font name");
    }

    if (!text) {
        DIE("no font text");
    }

    f = font_find(name);
    if (!f) {
        DIE("no font named %s", name);
    }

    double w, h;
    const char *c = 0;
    ttf_text_size(&f, text, &w, &h, 0, &c, scale, advance, fixed_width);

    w = w / ((double) game.video_gl_width);
    h = h / ((double) game.video_gl_height);

    return (Py_BuildValue("dds", w, h, c ? c : "none"));
}
