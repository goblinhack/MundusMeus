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

void py_call_void_int(const char *name, int val1);
int py_call_int_int(const char *name, int val1);

void py_call_void_module_int(const char *module, const char *name, int val1);
int py_call_int_module_int(const char *module, const char *name, int val1);
int py_call_int_module_void(const char *module, const char *name);
void py_call_void_module_void(const char *module, const char *name);
void py_call_void(const char *name);

void py_err(void);
char *py_obj_to_str(const PyObject *py_str);
int py_obj_to_int(PyObject *py_obj);
char *py_obj_attr_str(const PyObject *py_obj, const char *attr);

PyObject *tex_load_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tex_size_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tex_pixel_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tex_load_tiled_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tile_load_arr_(PyObject *obj, PyObject *args, PyObject *keywds);
