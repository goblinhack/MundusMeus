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

    static char *kwlist[] = {"wid", "parent", "name", "tiles", 0};

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

#define WID_BODY_SET_STRING(__field__)                                               \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
    char *value = 0;	                                                        \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, &py_class,    \
                                     &value)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!value) {	                                                        \
        ERR("%s, missing value", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, wid_name, value);	\
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = dupstr(value, __FUNCTION__);	                        \
    value = 0;	                                                                \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_SET_STRING_FN(__field__, __fn__)                                 \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
    char *value = 0;	                                                        \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, &py_class,    \
                                     &value)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!value) {	                                                        \
        ERR("%s, missing value", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, wid_name, value);	\
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = dupstr(value, __FUNCTION__);	                        \
    value = 0;	                                                                \
    (__fn__)(tp);                                                               \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_SET_ENUM(__field__, __str2val__)                                 \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
    char *value = 0;	                                                        \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, &py_class,    \
                                     &value)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!value) {	                                                        \
        ERR("%s, missing value", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = (__str2val__)(value);                                       \
    if (tp->__field__ == (typeof(tp->__field__))-1) {                           \
        ERR("%s, cannot find enum %s", __FUNCTION__, value);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> \"%s\"[%d])", __FUNCTION__, wid_name, value, tp->__field__);\
	                                                                        \
    value = 0;	                                                                \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_SET_INT(__field__)                                                  \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
    int value = 0;	                                                        \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|i", kwlist, &py_class,    \
                                     &value)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> %d)", __FUNCTION__, wid_name, value);	        \
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = value;                                                      \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_SET_DOUBLE(__field__)                                               \
PyObject *wid_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
    double value = 0;	                                                        \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|d", kwlist, &py_class,    \
                                     &value)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> %g)", __FUNCTION__, wid_name, value);	        \
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = value;                                                      \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_STRING_FN(__field__, __fn__)                                 \
PyObject *wid_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
    const char *value = 0;	                                                \
	                                                                        \
    static char *kwlist[] = {"class", "value", 0};	                        \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|s", kwlist, &py_class,    \
                                     &value)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!value) {	                                                        \
        ERR("%s, missing value", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    wid_name = py_obj_attr_str(py_class, "wid_id");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, wid_name, value);	\
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(tp, value);                                                        \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_DOUBLE_DOUBLE_FN(__field__, __fn__)                          \
PyObject *wid_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
    double d1 = 0;                                                              \
    double d2 = 0;                                                              \
	                                                                        \
    static char *kwlist[] = {"class", "x", "y", 0};	                        \
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
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s -> %f, %f)", __FUNCTION__, wid_name, d1, d2);	\
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(tp, d1, d2);                                                       \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define WID_BODY_VOID_FN(__field__, __fn__)                                   \
PyObject *wid_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds) \
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *wid_name = 0;	                                                \
	                                                                        \
    static char *kwlist[] = {"class", 0};	                                \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist, &py_class)) {   \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    wid_name = py_obj_attr_str(py_class, "name");	                        \
    if (!wid_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    LOG("python-to-c: %s(%s)", __FUNCTION__, wid_name);	                \
	                                                                        \
    widp tp = wid_find(wid_name);	                                        \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, wid_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(tp);                                                               \
	                                                                        \
done:	                                                                        \
    if (wid_name) {	                                                        \
        myfree(wid_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \
