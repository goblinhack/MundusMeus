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
#include "py_wid.h"
#include "string_ext.h"
#include "wid_tiles.h"

PyObject *wid_new_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    uintptr_t i_parent = 0;
    widp parent;
    char *name = 0;
    char *tiles = 0;
    widp w;

    static char *kwlist[] = {"wid", 
        "parent", 
        "name", 
        "tiles", 
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|Kss", kwlist, 
                                     &py_class,
                                     &i_parent,
                                     &name,
                                     &tiles)) {
        return (0);
    }

    if (!name) {
        ERR("wid_new, missing name");
        return (0);
    }

    parent = (widp) i_parent;

    if (parent) {
        w = wid_new_container(parent, name);
    } else {
        w = wid_new_window(name);
    }

    if (w) {
        if (tiles) {
            w->wid_tiles = wid_tiles_find(tiles);
            if (!w->wid_tiles) {
                ERR("did not find wid %s tiles %s", name, tiles);
            }

        }
    }

    return (Py_BuildValue("K", (uintptr_t) w));
}

PyObject *wid_destroy_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");

    wid_destroy(&w);

    Py_RETURN_NONE;
}

PyObject *wid_set_shape_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    int shape_none = 0;
    int shape_rounded_small = 0;
    int shape_rounded_large = 0;
    int shape_square = 0;
    int shape_square_outline = 0;

    static char *kwlist[] = {"wid", 
        "none",
        "rounded_small",
        "rounded_large",
        "square",
        "square_outline",
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iiiii", kwlist, 
                                     &py_class,
                                     &shape_none,
                                     &shape_rounded_small,
                                     &shape_rounded_large,
                                     &shape_square,
                                     &shape_square_outline)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    if (shape_none) {
        wid_set_no_shape(w);
    } else if (shape_rounded_small) {
        wid_set_rounded_small(w);
    } else if (shape_rounded_large) {
        wid_set_rounded_small(w);
    } else if (shape_square) {
        wid_set_square(w);
    } else if (shape_square_outline) {
        wid_set_square_outline(w);
    } else {
        DIE("no wid shape type set");
    }

    Py_RETURN_NONE;
}

PyObject *wid_set_tl_br_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;

    static char *kwlist[] = {"wid", 
        "x1",
        "y1",
        "x2",
        "y2",
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|dddd", kwlist, 
                                     &py_class,
                                     &x1,
                                     &y1,
                                     &x2,
                                     &y2)) {
        return (0);
    }

    fpoint tl;
    fpoint br;

    tl.x = x1;
    tl.y = y1;
    br.x = x2;
    br.y = y2;

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_tl_br(w, tl, br);

    Py_RETURN_NONE;
}

PyObject *wid_set_tl_br_pct_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;

    static char *kwlist[] = {"wid", 
        "x1",
        "y1",
        "x2",
        "y2",
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|dddd", kwlist, 
                                     &py_class,
                                     &x1,
                                     &y1,
                                     &x2,
                                     &y2)) {
        return (0);
    }

    fpoint tl;
    fpoint br;

    tl.x = x1;
    tl.y = y1;
    br.x = x2;
    br.y = y2;

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_tl_br_pct(w, tl, br);

    wid_update(w);
    wid_raise(w);

    Py_RETURN_NONE;
}

PyObject *wid_set_text_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    char *text = 0;
    const char *font = 0;
    int outline = 0;
    char *color_name = 0;

    static char *kwlist[] = {"wid", 
        "text",
        "font",
        "outline",
        "color",
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|ssis", kwlist, 
                                     &py_class,
                                     &text,
                                     &font,
                                     &outline,
                                     &color_name)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    if (font) {
        fontp f = string2font(&font);
        if (f) {
            wid_set_font(w, f);
        } else {
            ERR("font %s not found", font);
        }
    }

    if (color_name) {
        color c = color_find(color_name);
        wid_set_color(w, WID_COLOR_TEXT, c);
    }

    if (outline) {
        wid_set_text_outline(w, true);
    }

    wid_set_text(w, text);

    Py_RETURN_NONE;
}

PyObject *wid_set_color_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    char *color_name = 0;
    int bg = 0;
    int tl = 0;
    int br = 0;
    int text = 0;
    int blit = 0;
    double alpha = -1;

    static char *kwlist[] = {"wid", 
        "name",
        "bg",
        "tl",
        "br",
        "text",
        "blit",
        "alpha",
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|siiiiid", kwlist, 
                                     &py_class,
                                     &color_name,
                                     &bg,
                                     &tl,
                                     &br,
                                     &text,
                                     &blit,
                                     &alpha)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    if (!color_name) {
        ERR("no color set");
    }
        
    color c = color_find(color_name);
    if (bg) {
        wid_set_color(w, WID_COLOR_BG, c);
    }

    if (alpha != -1) {
        c.a = (int)(255.0 * alpha);
    }

    if (tl) {
        wid_set_color(w, WID_COLOR_TL, c);
    }

    if (bg) {
        wid_set_color(w, WID_COLOR_BG, c);
    }

    if (br) {
        wid_set_color(w, WID_COLOR_BR, c);
    }

    if (text) {
        wid_set_color(w, WID_COLOR_TEXT, c);
    }

    if (blit) {
        wid_set_color(w, WID_COLOR_BLIT, c);
    }

    Py_RETURN_NONE;
}
