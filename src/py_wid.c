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
#include "wid.h"
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

    static char *kwlist[] = {
        "wid", 
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

PyObject *wid_new_scrollbar_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    uintptr_t i_parent = 0;
    uintptr_t i_owner = 0;
    widp parent;
    widp owner;
    int horiz = false;
    int vert = false;

    static char *kwlist[] = {
        "wid", 
        "parent", 
        "owner", 
        "horiz", 
        "vert", 
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "OKK|ii", kwlist, 
                                     &py_class,
                                     &i_parent,
                                     &i_owner,
                                     &horiz,
                                     &vert)) {
        return (0);
    }

    parent = (widp) i_parent;
    verify(parent);
    owner = (widp) i_owner;
    verify(owner);

    widp w = 0;

    if (horiz) {
        w = wid_new_horiz_scroll_bar(parent, owner);
    }

    if (vert) {
        w = wid_new_vert_scroll_bar(parent, owner);
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

PyObject *wid_set_tooltip_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    char *text = 0;

    static char *kwlist[] = {"wid", 
        "text",
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, 
                                     &py_class,
                                     &text)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_tooltip(w, text, small_font);

    Py_RETURN_NONE;
}

PyObject *wid_set_tiles_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    char *tiles = 0;

    static char *kwlist[] = {"wid", 
        "tiles",
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, 
                                     &py_class,
                                     &tiles)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    w->wid_tiles = wid_tiles_find(tiles);
    if (!w->wid_tiles) {
        ERR("did not find wid tiles %s", tiles);
    }

    Py_RETURN_NONE;
}

PyObject *wid_set_pos_ (PyObject *obj, PyObject *args, PyObject *keywds)
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

    wid_set_pos(w, tl, br);

    Py_RETURN_NONE;
}

PyObject *wid_set_pos_pct_ (PyObject *obj, PyObject *args, PyObject *keywds)
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

    wid_set_pos_pct(w, tl, br);

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
    double advance = -1;
    int bot = -1;
    int centerx = -1;
    int centery = -1;
    int fixed_width = -1;
    int lhs = -1;
    int rhs = -1;
    double scaling = -1;
    int top = -1;

    static char *kwlist[] = {"wid", 
        "text",
        "font",
        "outline",
        "color",
        "advance",
        "bot",
        "centerx",
        "centery",
        "fixed_width",
        "lhs",
        "rhs",
        "scaling",
        "top",
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|ssisdiiiiiidi", kwlist, 
                                     &py_class,
                                     &text,
                                     &font,
                                     &outline,
                                     &color_name,
                                     &advance,
                                     &bot,
                                     &centerx,
                                     &centery,
                                     &fixed_width,
                                     &lhs,
                                     &rhs,
                                     &scaling,
                                     &top
                                     )) {
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

    if (advance != -1) {
        wid_set_text_advance(w, advance);
    }

    if (bot != -1) {
        wid_set_text_bot(w, bot);
    }

    if (centerx != -1) {
        wid_set_text_centerx(w, centerx);
    }

    if (centery != -1) {
        wid_set_text_centery(w, centery);
    }

    if (fixed_width != -1) {
        wid_set_text_fixed_width(w, fixed_width);
    }

    if (lhs != -1) {
        wid_set_text_lhs(w, lhs);
    }

    if (rhs != -1) {
        wid_set_text_rhs(w, rhs);
    }

    if (scaling != -1) {
        wid_set_text_scaling(w, scaling);
    }

    if (top != -1) {
        wid_set_text_top(w, top);
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

PyObject *wid_set_tex_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    char *name = 0;

    static char *kwlist[] = {"wid_id", 
        "name",
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, 
                                     &py_class,
                                     &name)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_tex(w, 0, name);

    Py_RETURN_NONE;
}

PyObject *wid_set_raise_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid_id", 
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_raise(w);

    Py_RETURN_NONE;
}

PyObject *wid_set_lower_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid_id", 
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_lower(w);

    Py_RETURN_NONE;
}

PyObject *wid_set_update_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid_id", 
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_update(w);

    Py_RETURN_NONE;
}

PyObject *wid_set_bevel_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    double value = 1.0;

    static char *kwlist[] = {"wid_id", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|d", kwlist, 
                                     &py_class,
                                     &value)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_bevel(w, value);

    Py_RETURN_NONE;
}

PyObject *wid_set_bevelled_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    int value = 1;

    static char *kwlist[] = {"wid_id", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, 
                                     &py_class,
                                     &value)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_bevelled(w, value);

    Py_RETURN_NONE;
}

PyObject *wid_set_cursor_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    int value = 1;

    static char *kwlist[] = {"wid_id", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, 
                                     &py_class,
                                     &value)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_cursor(w, value);

    Py_RETURN_NONE;
}

PyObject *wid_set_do_not_lower_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    int value = 1;

    static char *kwlist[] = {"wid_id", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, 
                                     &py_class,
                                     &value)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_do_not_lower(w, value);

    Py_RETURN_NONE;
}

PyObject *wid_set_do_not_raise_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    int value = 1;

    static char *kwlist[] = {"wid_id", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, 
                                     &py_class,
                                     &value)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_do_not_raise(w, value);

    Py_RETURN_NONE;
}

PyObject *wid_set_focusable_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    int value = 1;

    static char *kwlist[] = {"wid_id", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, 
                                     &py_class,
                                     &value)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_focusable(w, value);

    Py_RETURN_NONE;
}

PyObject *wid_set_movable_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    int value = 1;

    static char *kwlist[] = {"wid_id", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, 
                                     &py_class,
                                     &value)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_movable(w, value);

    Py_RETURN_NONE;
}

PyObject *wid_set_movable_bounded_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    int value = 1;

    static char *kwlist[] = {"wid_id", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, 
                                     &py_class,
                                     &value)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_movable_bounded(w, value);

    Py_RETURN_NONE;
}

PyObject *wid_set_movable_horiz_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    int value = 1;

    static char *kwlist[] = {"wid_id", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, 
                                     &py_class,
                                     &value)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_movable_horiz(w, value);

    Py_RETURN_NONE;
}

PyObject *wid_set_movable_vert_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;
    int value = 1;

    static char *kwlist[] = {"wid_id", "value", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, 
                                     &py_class,
                                     &value)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    wid_set_movable_vert(w, value);

    Py_RETURN_NONE;
}

PyObject *wid_get_size_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid_id", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    int32_t tlx = 0;
    int32_t tly = 0;
    int32_t brx = 0;
    int32_t bry = 0;

    wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

    double width = brx - tlx;
    double height = bry - tly;

    return (Py_BuildValue("dd", width, height));
}

PyObject *wid_get_size_pct_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid_id", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    int32_t tlx = 0;
    int32_t tly = 0;
    int32_t brx = 0;
    int32_t bry = 0;

    wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

    double width = brx - tlx;
    double height = bry - tly;

    width = width / ((double) game.video_gl_width);
    height = height / ((double) game.video_gl_height);

    return (Py_BuildValue("dd", width, height));
}

PyObject *wid_get_pos_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid_id", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    int32_t tlx = 0;
    int32_t tly = 0;
    int32_t brx = 0;
    int32_t bry = 0;

    wid_get_abs_coords_unclipped(w, &tlx, &tly, &brx, &bry);

    return (Py_BuildValue("dddd", tlx, tly, brx, bry));
}

PyObject *wid_get_pos_pct_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid_id", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    int32_t tlx = 0;
    int32_t tly = 0;
    int32_t brx = 0;
    int32_t bry = 0;

    wid_get_abs_coords_unclipped(w, &tlx, &tly, &brx, &bry);

    double ptlx = ((double)tlx) / ((double) game.video_gl_width);
    double ptly = ((double)tly) / ((double) game.video_gl_height);
    double pbrx = ((double)brx) / ((double) game.video_gl_width);
    double pbry = ((double)bry) / ((double) game.video_gl_height);

    return (Py_BuildValue("dddd", ptlx, ptly, pbrx, pbry));
}

PyObject *wid_get_parent_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid_id", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    w = wid_get_parent(w);
    return (Py_BuildValue("K", (uintptr_t) w));
}

PyObject *wid_get_top_parent_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;
    widp w;

    static char *kwlist[] = {"wid_id", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, 
                                     &py_class)) {
        return (0);
    }

    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");
    verify(w);

    w = wid_get_top_parent(w);
    return (Py_BuildValue("K", (uintptr_t) w));
}

#define WID_BODY_DOUBLE_DOUBLE_INT_FN(__fn__, n1, n2, n3)                       \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    widp w;                                                                     \
    double d1 = 0;                                                              \
    double d2 = 0;                                                              \
    int i1 = 0;                                                                 \
	                                                                        \
    static char *kwlist[] = {"wid_id", #n1, #n2, #n3, 0};                       \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|ddi", kwlist, &py_class,  \
                                     &d1, &d2, &i1)) {	                        \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");              \
    verify(w);                                                                  \
	                                                                        \
    (__fn__)(w, d1, d2, i1);                                                    \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_DOUBLE_DOUBLE_FN(__fn__, n1, n2)                               \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    widp w;                                                                     \
    double d1 = 0;                                                              \
    double d2 = 0;                                                              \
	                                                                        \
    static char *kwlist[] = {"wid_id", #n1, #n2, 0};                            \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|dd", kwlist, &py_class,   \
                                     &d1, &d2)) {	                        \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");              \
    verify(w);                                                                  \
	                                                                        \
    (__fn__)(w, d1, d2);                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_DOUBLE_FN(__fn__, n1)                                          \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    widp w;                                                                     \
    double d1 = 0;                                                              \
	                                                                        \
    static char *kwlist[] = {"wid_id", #n1, 0};                                 \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|d", kwlist, &py_class,    \
                                     &d1)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");              \
    verify(w);                                                                  \
	                                                                        \
    (__fn__)(w, d1);                                                            \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_INT_INT_INT_FN(__fn__, n1, n2, n3)                             \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    widp w;                                                                     \
    int n1 = 0;                                                                 \
    int n2 = 0;                                                                 \
    int n3 = 0;                                                                 \
	                                                                        \
    static char *kwlist[] = {"wid_id", #n1, #n2, #n3, 0};                       \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iii", kwlist, &py_class,  \
                                     &n1, &n2, &n3)) {	                        \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");              \
    verify(w);                                                                  \
	                                                                        \
    (__fn__)(w, n1, n2, n3);                                                    \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_INT_FN(__fn__, n1)                                             \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    widp w;                                                                     \
    int n1 = 0;                                                                 \
	                                                                        \
    static char *kwlist[] = {"wid_id", #n1, 0};                                 \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, &py_class,    \
                                     &n1)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");              \
    verify(w);                                                                  \
	                                                                        \
    (__fn__)(w, n1);                                                            \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_VOID_FN(__fn__)                                                \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    widp w;                                                                     \
	                                                                        \
    static char *kwlist[] = {"wid_id", 0};                                      \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, &py_class)) {	\
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");              \
    verify(w);                                                                  \
	                                                                        \
    (__fn__)(w);                                                                \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

static void wid_set_on_tooltip_callback (widp w, widp tooltip)
{
    py_call_void_module_ptr_ptr("wid", 
                                "on_tooltip_callback", 
                                w,
                                tooltip);
}

static void wid_set_on_mouse_focus_b_callback (widp w)
{
    py_call_void_module_ptr("wid", 
                            "on_focus_b_callback", 
                            w);
}

static void wid_set_on_mouse_focus_e_callback (widp w)
{
    py_call_void_module_ptr("wid", 
                            "on_focus_e_callback", 
                            w);
}

static void wid_set_on_mouse_over_begin_callback (widp w, int32_t relx, int32_t rely, int32_t wheelx, int32_t wheely)
{
    py_call_void_module_ptr_iiii("wid", 
                                 "on_mouse_over_begin_callback", 
                                 w,
                                 relx, rely,
                                 wheelx, wheely);
}

static void wid_set_on_mouse_over_end_callback (widp w)
{
    py_call_void_module_ptr("wid", 
                            "on_mouse_over_end_callback", 
                            w);
}

static uint8_t wid_set_on_joy_button_callback (widp w, int32_t x, int32_t y)
{
    int button_a = sdl_joy_buttons[SDL_JOY_BUTTON_A] ? 1 : 0;
    int button_b = sdl_joy_buttons[SDL_JOY_BUTTON_B] ? 1 : 0;
    int button_x = sdl_joy_buttons[SDL_JOY_BUTTON_X] ? 1 : 0;
    int button_y = sdl_joy_buttons[SDL_JOY_BUTTON_Y] ? 1 : 0;
    int button_top_left = sdl_joy_buttons[SDL_JOY_BUTTON_TOP_LEFT] ? 1 : 0;
    int button_top_right = sdl_joy_buttons[SDL_JOY_BUTTON_TOP_RIGHT] ? 1 : 0;
    int button_left_stick_down = sdl_joy_buttons[SDL_JOY_BUTTON_LEFT_STICK_DOWN] ? 1 : 0;
    int button_right_stick_down = sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT_STICK_DOWN] ? 1 : 0;
    int button_start = sdl_joy_buttons[SDL_JOY_BUTTON_START] ? 1 : 0;
    int button_xbox = sdl_joy_buttons[SDL_JOY_BUTTON_XBOX] ? 1 : 0;
    int button_back = sdl_joy_buttons[SDL_JOY_BUTTON_BACK] ? 1 : 0;
    int button_up = sdl_joy_buttons[SDL_JOY_BUTTON_UP] ? 1 : 0;
    int button_down = sdl_joy_buttons[SDL_JOY_BUTTON_DOWN] ? 1 : 0;
    int button_left = sdl_joy_buttons[SDL_JOY_BUTTON_LEFT] ? 1 : 0;
    int button_right = sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT] ? 1 : 0;
    int button_left_fire = sdl_joy_buttons[SDL_JOY_BUTTON_LEFT_FIRE] ? 1 : 0;
    int button_right_fire = sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT_FIRE] ? 1 : 0;

    py_call_void_module_ptr_iiiiiiiiiiiiiiiiiii("wid", 
                                    "on_joy_button_callback", 
                                    w,
                                    x, y,
                                    button_a,
                                    button_b,
                                    button_x,
                                    button_y,
                                    button_top_left,
                                    button_top_right,
                                    button_left_stick_down,
                                    button_right_stick_down,
                                    button_start,
                                    button_xbox,
                                    button_back,
                                    button_up,
                                    button_down,
                                    button_left,
                                    button_right,
                                    button_left_fire,
                                    button_right_fire);
    return (true);
}

static uint8_t wid_set_on_mouse_down_callback (widp w, int32_t x, int32_t y, uint32_t button)
{
    int ret;

    ret = py_call_int_module_ptr_iii("wid", 
                                     "on_mouse_down_callback", 
                                     w,
                                     x, y, button);
    return (ret);
}

static uint8_t wid_set_on_mouse_up_callback (widp w, int32_t x, int32_t y, uint32_t button)
{
    int ret;

    ret = py_call_int_module_ptr_iii("wid", 
                                     "on_mouse_up_callback", 
                                     w,
                                     x, y, button);
    return (ret);
}

static uint8_t wid_set_on_mouse_motion_callback (widp w, int32_t x, int32_t y, int32_t relx, int32_t rely, int32_t wheelx, int32_t wheely)
{
    int ret = 
        py_call_int_module_ptr_iiiiii("wid", 
                                      "on_mouse_motion_callback", 
                                      w,
                                      x, y,
                                      relx, rely,
                                      wheelx, wheely);
    return (ret);
}

static uint8_t wid_set_on_key_down_callback (widp w, const struct SDL_KEYSYM *k)
{
    int ret;

    ret = py_call_int_module_ptr_ii("wid", 
                                    "on_key_down_callback", 
                                    w,
                                    k->sym, k->mod);
    return (ret);
}

static uint8_t wid_set_on_key_up_callback (widp w, const struct SDL_KEYSYM *k)
{
    int ret;

    ret = py_call_int_module_ptr_ii("wid", 
                                    "on_key_up_callback", 
                                    w,
                                    k->sym, k->mod);
    return (ret);
}

static void wid_set_on_destroy_callback (widp w)
{
    py_call_void_module_ptr("wid", 
                            "on_destroy_callback", 
                            w);
}

static void wid_set_on_destroy_begin_callback (widp w)
{
    py_call_void_module_ptr("wid", 
                            "on_destroy_begin_callback", 
                            w);
}

static void wid_set_on_tick_callback (widp w)
{
    py_call_void_module_ptr("wid", 
                            "on_tick_callback", 
                            w);
}

static void wid_set_on_display_top_level_callback (widp w)
{
    py_call_void_module_ptr("wid", 
                            "on_display_top_level_callback", 
                            w);
}

static void wid_set_on_display_callback (widp w, fpoint tl, fpoint br)
{
    py_call_void_module_ptr("wid", 
                            "on_display_callback", 
                            w);
}

#define WID_BODY_ON_CALLBACK_FN(__fn__)                                         \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    widp w;                                                                     \
	                                                                        \
    static char *kwlist[] = {"wid_id", 0};                                      \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, &py_class)) {	\
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    w = (widp) (uintptr_t) py_obj_attr_uint64(py_class, "wid_id");              \
    verify(w);                                                                  \
	                                                                        \
    (__fn__)(w, __fn__ ## _callback);                                           \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

WID_BODY_DOUBLE_DOUBLE_INT_FN(wid_move_to_horiz_vert_pct_in, x, y, delay)
WID_BODY_DOUBLE_DOUBLE_INT_FN(wid_move_to_abs_centered_in, x, y, delay)
WID_BODY_DOUBLE_DOUBLE_INT_FN(wid_move_to_centered_in, x, y, delay)
WID_BODY_DOUBLE_DOUBLE_INT_FN(wid_move_delta_pct_in, x, y, delay)
WID_BODY_DOUBLE_DOUBLE_INT_FN(wid_move_to_abs_in, x, y, delay)
WID_BODY_DOUBLE_DOUBLE_INT_FN(wid_move_delta_in, x, y, delay)
WID_BODY_DOUBLE_DOUBLE_INT_FN(wid_move_to_pct_centered_in, x, y, delay)
WID_BODY_DOUBLE_DOUBLE_INT_FN(wid_move_to_abs_poffset_in, x, y, delay)
WID_BODY_DOUBLE_DOUBLE_INT_FN(wid_move_to_pct_in, x, y, delay)

WID_BODY_DOUBLE_DOUBLE_FN(wid_move_delta, x, y)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_delta_pct, x, y)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_abs, x, y)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_abs_centered, x, y)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_pct, x, y)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_pct_centered, x, y)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_vert_pct_in, pct, delay)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_horiz_pct_in, pct, delay)

WID_BODY_DOUBLE_FN(wid_move_to_vert_pct, pct)
WID_BODY_DOUBLE_FN(wid_move_to_horiz_pct, pct)

WID_BODY_INT_FN(wid_destroy_in, delay)
WID_BODY_INT_FN(wid_fade_in, delay)
WID_BODY_INT_FN(wid_fade_out, delay)
WID_BODY_INT_FN(wid_visible, delay)
WID_BODY_INT_FN(wid_hide, delay)
WID_BODY_INT_FN(wid_toggle_hidden, delay)
WID_BODY_INT_INT_INT_FN(wid_fade_in_out, delay, repeat, fade_in_first)

WID_BODY_VOID_FN(wid_move_to_bottom)
WID_BODY_VOID_FN(wid_move_to_left)
WID_BODY_VOID_FN(wid_move_to_right)
WID_BODY_VOID_FN(wid_move_to_top)
WID_BODY_VOID_FN(wid_move_end)
WID_BODY_VOID_FN(wid_destroy_grid)
WID_BODY_VOID_FN(wid_detach_from_grid)
WID_BODY_VOID_FN(wid_attach_to_grid)
WID_BODY_VOID_FN(wid_empty_grid)
WID_BODY_VOID_FN(wid_update)
WID_BODY_VOID_FN(wid_raise)
WID_BODY_VOID_FN(wid_lower)
WID_BODY_VOID_FN(wid_scroll_text)
WID_BODY_VOID_FN(wid_set_active)
WID_BODY_VOID_FN(wid_set_focus)
WID_BODY_VOID_FN(wid_focus_lock)
WID_BODY_VOID_FN(wid_effect_pulses)
WID_BODY_VOID_FN(wid_effect_pulse_forever)
WID_BODY_VOID_FN(wid_effect_pulse_stop)
WID_BODY_VOID_FN(wid_blit_effect_pulses)
WID_BODY_VOID_FN(wid_effect_sways)
WID_BODY_VOID_FN(wid_animate)

WID_BODY_ON_CALLBACK_FN(wid_set_on_tooltip);
WID_BODY_ON_CALLBACK_FN(wid_set_on_key_down);
WID_BODY_ON_CALLBACK_FN(wid_set_on_key_up);
WID_BODY_ON_CALLBACK_FN(wid_set_on_joy_button);
WID_BODY_ON_CALLBACK_FN(wid_set_on_mouse_down);
WID_BODY_ON_CALLBACK_FN(wid_set_on_mouse_motion);
WID_BODY_ON_CALLBACK_FN(wid_set_on_mouse_focus_b);
WID_BODY_ON_CALLBACK_FN(wid_set_on_mouse_focus_e);
WID_BODY_ON_CALLBACK_FN(wid_set_on_mouse_over_begin);
WID_BODY_ON_CALLBACK_FN(wid_set_on_mouse_over_end);
WID_BODY_ON_CALLBACK_FN(wid_set_on_mouse_up);
WID_BODY_ON_CALLBACK_FN(wid_set_on_destroy);
WID_BODY_ON_CALLBACK_FN(wid_set_on_destroy_begin);
WID_BODY_ON_CALLBACK_FN(wid_set_on_tick);
WID_BODY_ON_CALLBACK_FN(wid_set_on_display);
WID_BODY_ON_CALLBACK_FN(wid_set_on_display_top_level);
