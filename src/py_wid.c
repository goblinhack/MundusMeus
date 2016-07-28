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

PyObject *wid_new_scrollbar_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    uintptr_t i_parent = 0;
    uintptr_t i_owner = 0;
    widp parent;
    widp owner;
    int horiz = false;
    int vert = false;

    static char *kwlist[] = {
        "parent", 
        "owner", 
        "horiz", 
        "vert", 
        0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "KK|ii", kwlist, 
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

    if (horiz) {
        wid_new_horiz_scroll_bar(parent, owner);
    }

    if (vert) {
        wid_new_vert_scroll_bar(parent, owner);
    }

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
    double value = 0.0;

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
    int value = 0;

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
    int value = 0;

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
    int value = 0;

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
    int value = 0;

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
    int value = 0;

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
    int value = 0;

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
    int value = 0;

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
    int value = 0;

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
    int value = 0;

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

    int32_t tlx;
    int32_t tly;
    int32_t brx;
    int32_t bry;

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

    int32_t tlx;
    int32_t tly;
    int32_t brx;
    int32_t bry;

    wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

    double width = brx - tlx;
    double height = bry - tly;

    width = width / ((double) game.video_gl_width);
    height = height / ((double) game.video_gl_height);

    return (Py_BuildValue("dd", width, height));
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
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_abs, x, y)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_abs_centered, x, y)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_pct, x, y)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_pct_centered, x, y)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_vert_pct_in, pct, delay)
WID_BODY_DOUBLE_DOUBLE_FN(wid_move_to_horiz_pct_in, pct, delay)

WID_BODY_DOUBLE_FN(wid_move_to_vert_pct, pct)
WID_BODY_DOUBLE_FN(wid_move_to_horiz_pct, pct)

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
WID_BODY_VOID_FN(wid_set_focus)
WID_BODY_VOID_FN(wid_focus_lock)
WID_BODY_VOID_FN(wid_effect_pulses)
WID_BODY_VOID_FN(wid_effect_pulse_forever)
WID_BODY_VOID_FN(wid_effect_pulse_stop)
WID_BODY_VOID_FN(wid_blit_effect_pulses)
WID_BODY_VOID_FN(wid_effect_sways)
WID_BODY_VOID_FN(wid_animate)

