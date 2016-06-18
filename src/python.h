/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include <Python.h>

void python_init(void);
void python_fini(void);
void py_exec(const char *str);

char *py_obj_to_str(const PyObject *py_str);
char *py_obj_attr_str(const PyObject *py_obj, const char *attr);

PyObject *tex_load_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tex_load_tiled_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tile_load_arr_(PyObject *obj, PyObject *args, PyObject *keywds);
