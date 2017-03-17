/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"
#include "tile.h"
#include "py_tp.h"
#include "thing_tile.h"

PyObject *tp_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    PyObject *py_class = 0;

    static char *kwlist[] = {(char*) "tp", 0};

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

#define TP_BODY_SET_STRING(__field__)                                           \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *tp_name = 0;	                                                        \
    char *value = 0;	                                                        \
    tpp tp;                                                                  \
	                                                                        \
    static char *kwlist[] = {(char*) "class", (char*) "value", 0};	        \
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
    tp_name = py_obj_attr_str(py_class, "name");	                        \
    if (!tp_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    DBG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, tp_name, value);	        \
	                                                                        \
    tp = tp_find(tp_name);	                                                \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = dupstr(value, __FUNCTION__);	                        \
    value = 0;	                                                                \
	                                                                        \
done:	                                                                        \
    if (tp_name) {	                                                        \
        myfree(tp_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define TP_BODY_SET_STRING_FN(__field__, __fn__)                                \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *tp_name = 0;	                                                        \
    char *value = 0;	                                                        \
    tpp tp;                                                                     \
	                                                                        \
    static char *kwlist[] = {(char*) "class", (char*) "value", 0};	        \
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
    tp_name = py_obj_attr_str(py_class, "name");	                        \
    if (!tp_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    DBG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, tp_name, value);	        \
	                                                                        \
    tp = tp_find(tp_name);	                                                \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = dupstr(value, __FUNCTION__);	                        \
    value = 0;	                                                                \
    (__fn__)(tp);                                                               \
	                                                                        \
done:	                                                                        \
    if (tp_name) {	                                                        \
        myfree(tp_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define TP_BODY_SET_ENUM(__field__, __str2val__)                                \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *tp_name = 0;	                                                        \
    char *value = 0;	                                                        \
    tpp tp;                                                                     \
	                                                                        \
    static char *kwlist[] = {(char*) "class", (char*) "value", 0};	        \
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
    tp_name = py_obj_attr_str(py_class, "name");	                        \
    if (!tp_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp = tp_find(tp_name);	                                                \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = (__str2val__)(value);                                       \
    if (tp->__field__ == (TYPEOF(tp->__field__))-1) {                           \
        ERR("%s, cannot find enum %s", __FUNCTION__, value);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    DBG("python-to-c: %s(%s -> \"%s\"[%d])", __FUNCTION__, tp_name, value,      \
        tp->__field__);	                                                        \
	                                                                        \
    value = 0;	                                                                \
	                                                                        \
done:	                                                                        \
    if (tp_name) {	                                                        \
        myfree(tp_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define TP_BODY_SET_INT(__field__)                                              \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *tp_name = 0;	                                                        \
    int value = 0;	                                                        \
    tpp tp;                                                                     \
	                                                                        \
    static char *kwlist[] = {(char*) "class", (char*) "value", 0};	        \
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
    tp_name = py_obj_attr_str(py_class, "name");	                        \
    if (!tp_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    DBG("python-to-c: %s(%s -> %d)", __FUNCTION__, tp_name, value);	        \
	                                                                        \
    tp = tp_find(tp_name);	                                                \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = value;                                                      \
	                                                                        \
done:	                                                                        \
    if (tp_name) {	                                                        \
        myfree(tp_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define TP_BODY_SET_DOUBLE(__field__)                                           \
PyObject *tp_set_ ## __field__ (PyObject *obj, PyObject *args, PyObject *keywds)\
{	                                                                        \
    PyObject *py_class = 0;	                                                \
    char *tp_name = 0;	                                                        \
    double value = 0;	                                                        \
    tpp tp;                                                                     \
	                                                                        \
    static char *kwlist[] = {(char*) "class", (char*) "value", 0};	        \
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
    tp_name = py_obj_attr_str(py_class, (char*) "name");	                \
    if (!tp_name) {	                                                        \
        ERR("%s, missing tp name", __FUNCTION__);	                        \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    DBG("python-to-c: %s(%s -> %g)", __FUNCTION__, tp_name, value);	        \
	                                                                        \
    tp = tp_find(tp_name);	                                                \
    if (!tp) {	                                                                \
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	                \
        goto done;	                                                        \
    }	                                                                        \
	                                                                        \
    tp->__field__ = value;                                                      \
	                                                                        \
done:	                                                                        \
    if (tp_name) {	                                                        \
        myfree(tp_name);	                                                \
    }	                                                                        \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

PyObject *tp_set_tile (PyObject *obj, PyObject *args, PyObject *keywds)
{	
    PyObject *py_class = 0;	
    thing_tile *t;
    char *tp_name = 0;	
    char *tile = 0;	
    int delay_ms = 0;
    int is_moving = 0;
    int is_jumping = 0;
    int begin_jump = 0;
    int is_join_block = 0;
    int is_join_horiz = 0;
    int is_join_vert = 0;
    int is_join_node = 0;
    int is_join_left = 0;
    int is_join_right = 0;
    int is_join_top = 0;
    int is_join_bot = 0;
    int is_join_tl = 0;
    int is_join_tr = 0;
    int is_join_bl = 0;
    int is_join_br = 0;
    int is_join_t = 0;
    int is_join_t90 = 0;
    int is_join_t180 = 0;
    int is_join_t270 = 0;
    int is_join_x = 0;
    int is_join_tl2 = 0;
    int is_join_tr2 = 0;
    int is_join_bl2 = 0;
    int is_join_br2 = 0;
    int is_join_t_1 = 0;
    int is_join_t_2 = 0;
    int is_join_t_3 = 0;
    int is_join_t90_1 = 0;
    int is_join_t90_2 = 0;
    int is_join_t90_3 = 0;
    int is_join_t180_1 = 0;
    int is_join_t180_2 = 0;
    int is_join_t180_3 = 0;
    int is_join_t270_1 = 0;
    int is_join_t270_2 = 0;
    int is_join_t270_3 = 0;
    int is_join_x1 = 0;
    int is_join_x1_270 = 0;
    int is_join_x1_180 = 0;
    int is_join_x1_90 = 0;
    int is_join_x2 = 0;
    int is_join_x2_270 = 0;
    int is_join_x2_180 = 0;
    int is_join_x2_90 = 0;
    int is_join_x3 = 0;
    int is_join_x3_180 = 0;
    int is_join_x4 = 0;
    int is_join_x4_270 = 0;
    int is_join_x4_180 = 0;
    int is_join_x4_90 = 0;
    int is_join_horiz2 = 0;
    int is_join_vert2 = 0;
    int left = 0;
    int right = 0;
    int up = 0;
    int down = 0;
    int none = 0;
    int is_yyy5 = 0;
    int is_yyy6 = 0;
    int is_yyy7 = 0;
    int is_yyy8 = 0;
    int is_yyy9 = 0;
    int is_yyy10 = 0;
    int is_yyy11 = 0;
    int is_yyy12 = 0;
    int is_yyy13 = 0;
    int is_yyy14 = 0;
    int is_yyy15 = 0;
    int is_submerged = 0;
    int is_sleeping = 0;
    int is_open = 0;
    int is_dead = 0;
    int is_bloodied = 0;
    int is_end_of_anim = 0;
    int is_dead_on_end_of_anim = 0;
	
    static char *kwlist[] = {
        (char*) "class",
        (char*) "tile",
        (char*) "delay_ms",
        (char*) "is_moving",
        (char*) "is_jumping",
        (char*) "begin_jump",
        (char*) "is_join_block",
        (char*) "is_join_horiz",
        (char*) "is_join_vert",
        (char*) "is_join_node",
        (char*) "is_join_left",
        (char*) "is_join_right",
        (char*) "is_join_top",
        (char*) "is_join_bot",
        (char*) "is_join_tl",
        (char*) "is_join_tr",
        (char*) "is_join_bl",
        (char*) "is_join_br",
        (char*) "is_join_t",
        (char*) "is_join_t90",
        (char*) "is_join_t180",
        (char*) "is_join_t270",
        (char*) "is_join_x",
        (char*) "is_join_tl2",
        (char*) "is_join_tr2",
        (char*) "is_join_bl2",
        (char*) "is_join_br2",
        (char*) "is_join_t_1",
        (char*) "is_join_t_2",
        (char*) "is_join_t_3",
        (char*) "is_join_t90_1",
        (char*) "is_join_t90_2",
        (char*) "is_join_t90_3",
        (char*) "is_join_t180_1",
        (char*) "is_join_t180_2",
        (char*) "is_join_t180_3",
        (char*) "is_join_t270_1",
        (char*) "is_join_t270_2",
        (char*) "is_join_t270_3",
        (char*) "is_join_x1",
        (char*) "is_join_x1_270",
        (char*) "is_join_x1_180",
        (char*) "is_join_x1_90",
        (char*) "is_join_x2",
        (char*) "is_join_x2_270",
        (char*) "is_join_x2_180",
        (char*) "is_join_x2_90",
        (char*) "is_join_x3",
        (char*) "is_join_x3_180",
        (char*) "is_join_x4",
        (char*) "is_join_x4_270",
        (char*) "is_join_x4_180",
        (char*) "is_join_x4_90",
        (char*) "is_join_horiz2",
        (char*) "is_join_vert2",
        (char*) "is_dir_left",
        (char*) "is_dir_right",
        (char*) "is_dir_up",
        (char*) "is_dir_down",
        (char*) "is_dir_none",
        (char*) "is_yyy5",
        (char*) "is_yyy6",
        (char*) "is_yyy7",
        (char*) "is_yyy8",
        (char*) "is_yyy9",
        (char*) "is_yyy10",
        (char*) "is_yyy11",
        (char*) "is_yyy12",
        (char*) "is_yyy13",
        (char*) "is_yyy14",
        (char*) "is_yyy15",
        (char*) "is_submerged",
        (char*) "is_sleeping",
        (char*) "is_open",
        (char*) "is_dead",
        (char*) "is_bloodied",
        (char*) "is_end_of_anim",
        (char*) "is_dead_on_end_of_anim",
        0
    };
	
    if (!PyArg_ParseTupleAndKeywords(args, keywds,
                                     "O|siiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii",
                                     kwlist, &py_class,
                                     &tile,
                                     &delay_ms,
                                     &is_moving,
                                     &is_jumping,
                                     &begin_jump,
                                     &is_join_block,
                                     &is_join_horiz,
                                     &is_join_vert,
                                     &is_join_node,
                                     &is_join_left,
                                     &is_join_right,
                                     &is_join_top,
                                     &is_join_bot,
                                     &is_join_tl,
                                     &is_join_tr,
                                     &is_join_bl,
                                     &is_join_br,
                                     &is_join_t,
                                     &is_join_t90,
                                     &is_join_t180,
                                     &is_join_t270,
                                     &is_join_x,
                                     &is_join_tl2,
                                     &is_join_tr2,
                                     &is_join_bl2,
                                     &is_join_br2,
                                     &is_join_t_1,
                                     &is_join_t_2,
                                     &is_join_t_3,
                                     &is_join_t90_1,
                                     &is_join_t90_2,
                                     &is_join_t90_3,
                                     &is_join_t180_1,
                                     &is_join_t180_2,
                                     &is_join_t180_3,
                                     &is_join_t270_1,
                                     &is_join_t270_2,
                                     &is_join_t270_3,
                                     &is_join_x1,
                                     &is_join_x1_270,
                                     &is_join_x1_180,
                                     &is_join_x1_90,
                                     &is_join_x2,
                                     &is_join_x2_270,
                                     &is_join_x2_180,
                                     &is_join_x2_90,
                                     &is_join_x3,
                                     &is_join_x3_180,
                                     &is_join_x4,
                                     &is_join_x4_270,
                                     &is_join_x4_180,
                                     &is_join_x4_90,
                                     &is_join_horiz2,
                                     &is_join_vert2,
                                     &left,
                                     &right,
                                     &up,
                                     &down,
                                     &none,
                                     &is_yyy5,
                                     &is_yyy6,
                                     &is_yyy7,
                                     &is_yyy8,
                                     &is_yyy9,
                                     &is_yyy10,
                                     &is_yyy11,
                                     &is_yyy12,
                                     &is_yyy13,
                                     &is_yyy14,
                                     &is_yyy15,
                                     &is_submerged,
                                     &is_sleeping,
                                     &is_open,
                                     &is_dead,
                                     &is_bloodied,
                                     &is_end_of_anim,
                                     &is_dead_on_end_of_anim
                                     )) {	
        return (0);	
    }	
	
    if (!py_class) {	
        ERR("%s, missing class", __FUNCTION__);	
        return (0);	
    }	
	
    tpp tp;

    tp_name = py_obj_attr_str(py_class, "name");	
    if (!tp_name) {	
        ERR("%s, missing tp name", __FUNCTION__);	
        goto done;	
    }	
	
    DBG("python-to-c: %s(%s -> \"%s\")", __FUNCTION__, tp_name, tile);	
	
    tp = tp_find(tp_name);	
    if (!tp) {	
        ERR("%s, cannot find tp %s", __FUNCTION__, tp_name);	
        goto done;	
    }	
	
    if (!tp->tiles) {
        tp->tiles = tree_alloc(TREE_KEY_INTEGER, "TREE ROOT: thing tiles");
    }

    if (thing_tile_count >= TILES_ANIM_MAX) {
        DIE("out of anim tile space");
    }

    t = &thing_tile_arr[thing_tile_count];
    newptr(t, "tile");

    static uint32_t id;
    t->tree.key = id++;
    t->array_index = thing_tile_count++;

    if (!tree_insert_static(tp->tiles, &t->tree.node)) {
        ERR("insert thing tiles node fail");
    }

    if (t->has_dir_anim) {
        tp->has_dir_anim = true;
    }

    t->tile = tile_find(tile);
    if (!t->tile) {
        ERR("%s, cannot find tile %s for tp %s", __FUNCTION__, tile, tp_name);
    }
    t->tilename = dupstr(tile, "tilename");

    t->delay_ms = delay_ms;
    t->is_moving = is_moving;
    t->is_join_block = is_join_block;
    t->is_join_horiz = is_join_horiz;
    t->is_join_vert = is_join_vert;
    t->is_join_node = is_join_node;
    t->is_join_left = is_join_left;
    t->is_join_right = is_join_right;
    t->is_join_top = is_join_top;
    t->is_join_bot = is_join_bot;
    t->is_join_tl = is_join_tl;
    t->is_join_tr = is_join_tr;
    t->is_join_bl = is_join_bl;
    t->is_join_br = is_join_br;
    t->is_join_t = is_join_t;
    t->is_join_t90 = is_join_t90;
    t->is_join_t180 = is_join_t180;
    t->is_join_t270 = is_join_t270;
    t->is_join_x = is_join_x;
    t->is_join_tl2 = is_join_tl2;
    t->is_join_tr2 = is_join_tr2;
    t->is_join_bl2 = is_join_bl2;
    t->is_join_br2 = is_join_br2;
    t->is_join_t_1 = is_join_t_1;
    t->is_join_t_2 = is_join_t_2;
    t->is_join_t_3 = is_join_t_3;
    t->is_join_t90_1 = is_join_t90_1;
    t->is_join_t90_2 = is_join_t90_2;
    t->is_join_t90_3 = is_join_t90_3;
    t->is_join_t180_1 = is_join_t180_1;
    t->is_join_t180_2 = is_join_t180_2;
    t->is_join_t180_3 = is_join_t180_3;
    t->is_join_t270_1 = is_join_t270_1;
    t->is_join_t270_2 = is_join_t270_2;
    t->is_join_t270_3 = is_join_t270_3;
    t->is_join_x1 = is_join_x1;
    t->is_join_x1_270 = is_join_x1_270;
    t->is_join_x1_180 = is_join_x1_180;
    t->is_join_x1_90 = is_join_x1_90;
    t->is_join_x2 = is_join_x2;
    t->is_join_x2_270 = is_join_x2_270;
    t->is_join_x2_180 = is_join_x2_180;
    t->is_join_x2_90 = is_join_x2_90;
    t->is_join_x3 = is_join_x3;
    t->is_join_x3_180 = is_join_x3_180;
    t->is_join_x4 = is_join_x4;
    t->is_join_x4_270 = is_join_x4_270;
    t->is_join_x4_180 = is_join_x4_180;
    t->is_join_x4_90 = is_join_x4_90;
    t->is_join_horiz2 = is_join_horiz2;
    t->is_join_vert2 = is_join_vert2;
    t->is_yyy5 = is_yyy5;
    t->is_yyy6 = is_yyy6;
    t->is_yyy7 = is_yyy7;
    t->is_yyy8 = is_yyy8;
    t->is_yyy9 = is_yyy9;
    t->is_yyy10 = is_yyy10;
    t->is_yyy11 = is_yyy11;
    t->is_yyy12 = is_yyy12;
    t->is_yyy13 = is_yyy13;
    t->is_yyy14 = is_yyy14;
    t->is_yyy15 = is_yyy15;
    t->is_submerged = is_submerged;
    t->is_sleeping = is_sleeping;
    t->is_open = is_open;
    t->is_dead = is_dead;
    t->is_end_of_anim = is_end_of_anim;
    t->is_dead_on_end_of_anim = is_dead_on_end_of_anim;

    if (up) {
        if (left) {
            t->dir = THING_DIR_TL;
            t->has_dir_anim = true;
        } else if (right) {
            t->dir = THING_DIR_TR;
            t->has_dir_anim = true;
        } else {
            t->dir = THING_DIR_UP;
            t->has_dir_anim = true;
        }
    } else if (down) {
        if (left) {
            t->dir = THING_DIR_BL;
            t->has_dir_anim = true;
        } else if (right) {
            t->dir = THING_DIR_BR;
            t->has_dir_anim = true;
        } else {
            t->dir = THING_DIR_DOWN;
            t->has_dir_anim = true;
        }
    } else if (left) {
        t->dir = THING_DIR_LEFT;
        t->has_dir_anim = true;
    } else if (right) {
        t->dir = THING_DIR_RIGHT;
        t->has_dir_anim = true;
    } else if (none) {
        t->dir = THING_DIR_NONE;
        t->has_dir_anim = true;
    }
	
done:	
    if (tp_name) {	
        myfree(tp_name);	
    }	
	
    Py_RETURN_NONE;	
}	

TP_BODY_SET_DOUBLE(light_radius)
TP_BODY_SET_DOUBLE(scale)
TP_BODY_SET_INT(z_depth)
TP_BODY_SET_INT(z_order)
TP_BODY_SET_INT(speed)
TP_BODY_SET_INT(blit_top_off)
TP_BODY_SET_INT(blit_bot_off)
TP_BODY_SET_INT(blit_left_off)
TP_BODY_SET_INT(blit_right_off)
TP_BODY_SET_STRING(short_name)
TP_BODY_SET_STRING(raw_name)
static void light_tint_fixup (tpp t)
{
    if (t->light_tint) {
        t->light_color = color_find(t->light_tint);
    } else {
        t->light_color = WHITE;
    }
}
TP_BODY_SET_STRING_FN(light_tint, light_tint_fixup)
TP_BODY_SET_INT(is_animated)
TP_BODY_SET_INT(is_animated_no_dir)
TP_BODY_SET_INT(is_animation)
TP_BODY_SET_INT(is_candle_light)
TP_BODY_SET_INT(is_cats_eyes)
TP_BODY_SET_INT(is_shadow_caster)
TP_BODY_SET_INT(is_shadow_caster_soft)
TP_BODY_SET_INT(is_door)
TP_BODY_SET_INT(is_lava)
TP_BODY_SET_INT(is_water)
TP_BODY_SET_INT(is_effect_fade_in_out)
TP_BODY_SET_INT(is_effect_pulse)
TP_BODY_SET_INT(is_effect_rotate_2way)
TP_BODY_SET_INT(is_effect_sway)
TP_BODY_SET_INT(is_explosion)
TP_BODY_SET_INT(is_food)
TP_BODY_SET_INT(is_weapon)
TP_BODY_SET_INT(is_hidden)
TP_BODY_SET_INT(is_monst)
TP_BODY_SET_INT(is_player)
TP_BODY_SET_INT(is_rrr1)
TP_BODY_SET_INT(is_rrr2)
TP_BODY_SET_INT(is_rrr3)
TP_BODY_SET_INT(is_rrr4)
TP_BODY_SET_INT(is_rrr5)
TP_BODY_SET_INT(is_rrr6)
TP_BODY_SET_INT(is_rrr7)
TP_BODY_SET_INT(is_rrr8)
TP_BODY_SET_INT(is_rrr9)
TP_BODY_SET_INT(is_rrr10)
TP_BODY_SET_INT(is_rrr11)
TP_BODY_SET_INT(is_rrr12)
TP_BODY_SET_INT(is_rrr13)
TP_BODY_SET_INT(is_rrr14)
TP_BODY_SET_INT(is_rrr15)
TP_BODY_SET_INT(is_rrr16)
TP_BODY_SET_INT(is_rrr17)
TP_BODY_SET_INT(is_rrr18)
TP_BODY_SET_INT(is_rrr19)
TP_BODY_SET_INT(is_rrr20)
TP_BODY_SET_INT(is_rrr21)
TP_BODY_SET_INT(is_rrr22)
TP_BODY_SET_INT(is_rrr23)
TP_BODY_SET_INT(is_rrr24)
TP_BODY_SET_INT(is_rrr25)
TP_BODY_SET_INT(is_rrr26)
TP_BODY_SET_INT(is_rrr27)
TP_BODY_SET_INT(is_rrr28)
TP_BODY_SET_INT(is_rrr29)
TP_BODY_SET_INT(is_rrr30)
TP_BODY_SET_INT(is_rrr31)
TP_BODY_SET_INT(is_rrr32)
TP_BODY_SET_INT(is_rrr33)
TP_BODY_SET_INT(is_rrr34)
TP_BODY_SET_INT(is_rrr35)
TP_BODY_SET_INT(is_rrr36)
TP_BODY_SET_INT(is_rrr37)
TP_BODY_SET_INT(is_rrr38)
TP_BODY_SET_INT(is_rrr39)
TP_BODY_SET_INT(is_rrr40)
TP_BODY_SET_INT(is_rrr41)
TP_BODY_SET_INT(is_rrr42)
TP_BODY_SET_INT(is_rrr43)
TP_BODY_SET_INT(is_rrr44)
TP_BODY_SET_INT(is_rrr45)
TP_BODY_SET_INT(is_rrr46)
TP_BODY_SET_INT(has_shadow)
TP_BODY_SET_INT(is_chair)
TP_BODY_SET_INT(is_chest)
TP_BODY_SET_INT(is_bookcase)
TP_BODY_SET_INT(is_table)
TP_BODY_SET_INT(is_lawn)
TP_BODY_SET_INT(is_carpet_deco)
TP_BODY_SET_INT(is_carpet)
TP_BODY_SET_INT(is_house_item)
TP_BODY_SET_INT(is_cwall)
TP_BODY_SET_INT(is_animated_lr_flip)
TP_BODY_SET_INT(is_gravel_snow_deco)
TP_BODY_SET_INT(is_landrock_snow)
TP_BODY_SET_INT(is_hidden_from_editor)
TP_BODY_SET_INT(is_hwall)
TP_BODY_SET_INT(is_dungeon_item)
TP_BODY_SET_INT(is_difficulty_easy)
TP_BODY_SET_INT(is_difficulty_hard)
TP_BODY_SET_INT(is_difficulty_nightmare)
TP_BODY_SET_INT(is_world_item)
TP_BODY_SET_INT(is_solid_ground)
TP_BODY_SET_INT(is_dirt_deco)
TP_BODY_SET_INT(is_dirt_snow_deco)
TP_BODY_SET_INT(is_grass_deco)
TP_BODY_SET_INT(is_grass_snow_deco)
TP_BODY_SET_INT(is_gravel_deco)
TP_BODY_SET_INT(is_sand_deco)
TP_BODY_SET_INT(is_sand_snow_deco)
TP_BODY_SET_INT(is_snow_deco)
TP_BODY_SET_INT(is_sand_snow)
TP_BODY_SET_INT(is_dirt_snow)
TP_BODY_SET_INT(is_grass_snow)
TP_BODY_SET_INT(is_road_snow)
TP_BODY_SET_INT(is_road)
TP_BODY_SET_INT(is_dungeon_snow)
TP_BODY_SET_INT(is_dungeon)
TP_BODY_SET_INT(is_focus)
TP_BODY_SET_INT(is_gravel_snow)
TP_BODY_SET_INT(is_tree_conifer)
TP_BODY_SET_INT(is_snow_mound)
TP_BODY_SET_INT(is_rock)
TP_BODY_SET_INT(is_ice)
TP_BODY_SET_INT(is_gravel)
TP_BODY_SET_INT(is_small_rock)
TP_BODY_SET_INT(is_marsh_plant)
TP_BODY_SET_INT(is_landrock)
TP_BODY_SET_INT(is_snow)
TP_BODY_SET_INT(is_dirt)
TP_BODY_SET_INT(is_sand)
TP_BODY_SET_INT(is_plant)
TP_BODY_SET_INT(is_tree)
TP_BODY_SET_INT(is_grass)
TP_BODY_SET_INT(is_bridge)
TP_BODY_SET_INT(is_movement_blocking)
TP_BODY_SET_INT(is_treasure)
TP_BODY_SET_INT(is_dungeon_way_up)
TP_BODY_SET_INT(is_dungeon_way_down)
TP_BODY_SET_INT(is_chasm_smoke)
TP_BODY_SET_INT(is_key)
TP_BODY_SET_INT(is_dusty)
TP_BODY_SET_INT(is_corridor)
TP_BODY_SET_DOUBLE(light_pulse_amount)
TP_BODY_SET_INT(is_healing)
TP_BODY_SET_INT(is_armor)
TP_BODY_SET_INT(is_magical)
TP_BODY_SET_INT(is_sleeping)
TP_BODY_SET_INT(is_wall)
TP_BODY_SET_INT(is_floor)
