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
int py_call_ii(const char *name, int val1);

void py_call_void_module_int(const char *module, const char *name, int val1);
int py_call_int_module_int(const char *module, const char *name, int val1);
int py_call_int_module_void(const char *module, const char *name);
void py_call_void_module_void(const char *module, const char *name);

void py_call_void_module_ptr_iiiiiiiiiiiiiiiiiii(const char *module,
                                                 const char *name,
                                                 void *val1,
                                                 int val2,
                                                 int val3,
                                                 int val4,
                                                 int val5,
                                                 int val6,
                                                 int val7,
                                                 int val8,
                                                 int val9,
                                                 int val10,
                                                 int val11,
                                                 int val12,
                                                 int val13,
                                                 int val14,
                                                 int val15,
                                                 int val16,
                                                 int val17,
                                                 int val18,
                                                 int val19,
                                                 int val20);
int py_call_int_module_ptr_iiiiii(const char *module, const char *name, void *val1, int, int, int, int, int, int);
int py_call_int_module_ptr_iiiii(const char *module, const char *name, void *val1, int, int, int, int, int);
int py_call_int_module_ptr_iiii(const char *module, const char *name, void *val1, int, int, int, int);
int py_call_int_module_ptr_iii(const char *module, const char *name, void *val1, int, int, int);
int py_call_int_module_ptr_ii(const char *module, const char *name, void *val1, int, int);
int py_call_int_module_ptr_int(const char *module, const char *name, void *val1, int);
int py_call_int_module_ptr(const char *module, const char *name, void *val1);
int py_call_int_module_ptr_ptr(const char *module, const char *name, void *val1, void *val2);

void py_call_void_module_ptr_iiiiii(const char *module, const char *name, void *val1, int, int, int, int, int, int);
void py_call_void_module_ptr_iiiii(const char *module, const char *name, void *val1, int, int, int, int, int);
void py_call_void_module_ptr_iiii(const char *module, const char *name, void *val1, int, int, int, int);
void py_call_void_module_ptr_iii(const char *module, const char *name, void *val1, int, int, int);
void py_call_void_module_ptr_ii(const char *module, const char *name, void *val1, int, int);
void py_call_void_module_ptr_int(const char *module, const char *name, void *val1, int);
void py_call_void_module_ptr(const char *module, const char *name, void *val1);
void py_call_void_module_ptr_ptr(const char *module, const char *name, void *val1, void *val2);

void py_call_void(const char *name);

void py_err(void);
char *py_obj_to_str(const PyObject *py_str);
int py_obj_to_int(PyObject *py_obj);
uint64_t py_obj_to_uint64(PyObject *py_obj);

char *py_obj_attr_str(const PyObject *py_obj, const char *attr);
int py_obj_attr_int(const PyObject *py_obj, const char *attr);
uint64_t py_obj_attr_uint64(const PyObject *py_obj, const char *attr);

PyObject *ttf_text_size_ (PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *ttf_text_size_pct_ (PyObject *obj, PyObject *args, PyObject *keywds);

PyObject *tex_load_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tex_size_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tex_pixel_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tex_load_tiled_(PyObject *obj, PyObject *args, PyObject *keywds);

PyObject *tile_load_arr_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tile_get_size_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *tile_get_size_pct_(PyObject *obj, PyObject *args, PyObject *keywds);

PyObject *wid_tiles_load_(PyObject *obj, PyObject *args, PyObject *keywds);

PyObject *dmap_process_(PyObject *obj, PyObject *args, PyObject *keywds);

#define PY_PROTO(__fn__)                                                    \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds);

#define PY_DECL(__fn__)                                                     \
    {#__fn__,                                                               \
     (PyCFunction)__fn__ ## _,                                              \
     METH_VARARGS | METH_KEYWORDS,                                          \
     "call " #__fn__ " "},                                                  \

