/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "py_tp.h"
#include "py_thing.h"
#include "py_wid.h"
#include "main.h"
#include "string_util.h"
#include "string_ext.h"
#include "frameobject.h"
#include "tex.h"
#include "tile.h"
#include "thing_template.h"
#include "config.h"

static PyObject *mymod;

#define PY_BODY_DOUBLE_DOUBLE_INT_FN(__fn__, n1, n2, n3)                        \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    double d1 = 0;                                                              \
    double d2 = 0;                                                              \
    int i1 = 0;                                                                 \
	                                                                        \
    static char *kwlist[] = {#n1, #n2, #n3, 0};                                 \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ddi", kwlist,               \
                                     &d1, &d2, &i1)) {	                        \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(d1, d2, i1);                                                       \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define PY_BODY_DOUBLE_DOUBLE_FN(__fn__, n1, n2)                                \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    double d1 = 0;                                                              \
    double d2 = 0;                                                              \
	                                                                        \
    static char *kwlist[] = {#n1, #n2, 0};                                      \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "dd", kwlist,                \
                                     &d1, &d2)) {	                        \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    if (!py_class) {	                                                        \
        ERR("%s, missing class", __FUNCTION__);	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(d1, d2);                                                           \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define PY_BODY_DOUBLE_FN(__fn__, n1)                                           \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    double d1 = 0;                                                              \
	                                                                        \
    static char *kwlist[] = {"wid_id", #n1, 0};                                 \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "d", kwlist,                 \
                                     &d1)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(d1);                                                               \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define PY_BODY_INT_INT_INT_FN(__fn__, n1, n2, n3)                              \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    int n1 = 0;                                                                 \
    int n2 = 0;                                                                 \
    int n3 = 0;                                                                 \
	                                                                        \
    static char *kwlist[] = {#n1, #n2, #n3, 0};                                 \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iii", kwlist,               \
                                     &n1, &n2, &n3)) {	                        \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(n1, n2, n3);                                                       \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define PY_BODY_INT_FN(__fn__, n1)                                              \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    int n1 = 0;                                                                 \
	                                                                        \
    static char *kwlist[] = {#n1, 0};                                           \
	                                                                        \
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,                 \
                                     &n1)) {	                                \
        return (0);	                                                        \
    }	                                                                        \
	                                                                        \
    (__fn__)(n1);                                                               \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

#define PY_BODY_VOID_FN(__fn__)                                                 \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds)         \
{	                                                                        \
    (__fn__)();                                                                 \
	                                                                        \
    Py_RETURN_NONE;	                                                        \
}	                                                                        \

PY_PROTO(wid_mouse_hide)
PY_BODY_INT_FN(wid_mouse_hide, value)

void py_call_void_module_int (const char *module, const char *name, int val1)
{
    LOG("c-to-python: %s.%s(%d)", module, name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return;
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return;
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return;
    }

    if (PyCallable_Check(fn)) {
        PyObject *pArgs = Py_BuildValue("(i)", val1);
        PyObject *pValue = PyObject_CallObject(fn, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();
}

void py_call_void_int (const char *name, int val1)
{
    LOG("c-to-python: %s(%d)", name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *pFunc = PyObject_GetAttrString(mymod, name);
    if (PyCallable_Check(pFunc)) {
        PyObject *pArgs = Py_BuildValue("(i)", val1);
        PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python function %s(%d)", name, val1);
    }

    py_err();
}

void py_call_void (const char *name)
{
    LOG("c-to-python: %s()", name);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *pFunc = PyObject_GetAttrString(mymod, name);
    if (PyCallable_Check(pFunc)) {
        PyObject *pValue = PyObject_CallObject(pFunc, 0);
        if (pValue != NULL) {
            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python function %s", name);
    }

    py_err();
}

int py_call_int_module_ptr_iiiiii (const char *module, const char *name, void *val1, int val2, int val3, int val4, int val5, int val6, int val7)
{
    int ret = -1;

    LOG("c-to-python: %s.%s(%p)", module, name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return (ret);
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return (ret);
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return (ret);
    }

    if (PyCallable_Check(fn)) {
        PyObject *pArgs = Py_BuildValue("(Kiiiiii)", (uintptr_t) val1, val2, val3, val4, val5, val6, val7);
        PyObject *pValue = PyObject_CallObject(fn, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            ret = py_obj_to_int(pValue);

            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();

    return (ret);
}

int py_call_int_module_ptr_iiiii (const char *module, const char *name, void *val1, int val2, int val3, int val4, int val5, int val6)
{
    int ret = -1;

    LOG("c-to-python: %s.%s(%p)", module, name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return (ret);
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return (ret);
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return (ret);
    }

    if (PyCallable_Check(fn)) {
        PyObject *pArgs = Py_BuildValue("(Kiiiii)", (uintptr_t) val1, val2, val3, val4, val5, val6);
        PyObject *pValue = PyObject_CallObject(fn, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            ret = py_obj_to_int(pValue);

            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();

    return (ret);
}

int py_call_int_module_ptr_iiii (const char *module, const char *name, void *val1, int val2, int val3, int val4, int val5)
{
    int ret = -1;

    LOG("c-to-python: %s.%s(%p)", module, name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return (ret);
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return (ret);
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return (ret);
    }

    if (PyCallable_Check(fn)) {
        PyObject *pArgs = Py_BuildValue("(Kiiii)", (uintptr_t) val1, val2, val3, val4, val5);
        PyObject *pValue = PyObject_CallObject(fn, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            ret = py_obj_to_int(pValue);

            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();

    return (ret);
}

int py_call_int_module_ptr_iii (const char *module, const char *name, void *val1, int val2, int val3, int val4)
{
    int ret = -1;

    LOG("c-to-python: %s.%s(%p)", module, name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return (ret);
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return (ret);
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return (ret);
    }

    if (PyCallable_Check(fn)) {
        PyObject *pArgs = Py_BuildValue("(Kiii)", (uintptr_t) val1, val2, val3, val4);
        PyObject *pValue = PyObject_CallObject(fn, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            ret = py_obj_to_int(pValue);

            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();

    return (ret);
}

int py_call_int_module_ptr_ii (const char *module, const char *name, void *val1, int val2, int val3)
{
    int ret = -1;

    LOG("c-to-python: %s.%s(%p)", module, name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return (ret);
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return (ret);
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return (ret);
    }

    if (PyCallable_Check(fn)) {
        PyObject *pArgs = Py_BuildValue("(Kii)", (uintptr_t) val1, val2, val3);
        PyObject *pValue = PyObject_CallObject(fn, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            ret = py_obj_to_int(pValue);

            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();

    return (ret);
}

int py_call_int_module_ptr_int (const char *module, const char *name, void *val1, int val2)
{
    int ret = -1;

    LOG("c-to-python: %s.%s(%p)", module, name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return (ret);
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return (ret);
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return (ret);
    }

    if (PyCallable_Check(fn)) {
        PyObject *pArgs = Py_BuildValue("(Ki)", (uintptr_t) val1, val2);
        PyObject *pValue = PyObject_CallObject(fn, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            ret = py_obj_to_int(pValue);

            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();

    return (ret);
}

int py_call_int_module_ptr (const char *module, const char *name, void *val1)
{
    int ret = -1;

    LOG("c-to-python: %s.%s(%p)", module, name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return (ret);
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return (ret);
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return (ret);
    }

    if (PyCallable_Check(fn)) {
        PyObject *pArgs = Py_BuildValue("(K)", (uintptr_t) val1);
        PyObject *pValue = PyObject_CallObject(fn, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            ret = py_obj_to_int(pValue);

            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();

    return (ret);
}

int py_call_int_module_int (const char *module, const char *name, int val1)
{
    int ret = -1;

    LOG("c-to-python: %s.%s(%d)", module, name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return (ret);
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return (ret);
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return (ret);
    }

    if (PyCallable_Check(fn)) {
        PyObject *pArgs = Py_BuildValue("(i)", val1);
        PyObject *pValue = PyObject_CallObject(fn, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            ret = py_obj_to_int(pValue);

            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();

    return (ret);
}

int py_call_int_module_void (const char *module, const char *name)
{
    int ret = -1;

    LOG("c-to-python: %s.%s()", module, name);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return (ret);
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return (ret);
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return (ret);
    }

    if (PyCallable_Check(fn)) {
        PyObject *pValue = PyObject_CallObject(fn, 0);
        if (pValue != NULL) {
            ret = py_obj_to_int(pValue);

            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();

    return (ret);
}

void py_call_void_module_void (const char *module, const char *name)
{
    LOG("c-to-python: %s.%s()", module, name);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *v = PyObject_GetAttrString(mymod, module);
    if (!v) {
        ERR("cannot find python module %s", module);
        return;
    }

    PyObject *dict = PyModule_GetDict(v);
    if (!dict) {
        ERR("cannot find python module %s dict", module);
        return;
    }

    PyObject *fn = PyDict_GetItemString(dict, name);
    if (!fn) {
        ERR("cannot find python module %s fn %s", module, name);
        return;
    }

    if (PyCallable_Check(fn)) {
        PyObject *pValue = PyObject_CallObject(fn, 0);
        if (pValue != NULL) {
            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python module %s fn %s", module, name);
    }

    py_err();
}

int py_call_ii (const char *name, int val1)
{
    int ret = -1;

    LOG("c-to-python: %s(%d)", name, val1);
    if (!mymod) {
        DIE("python module not inited yet");
    }

    PyObject *pFunc = PyObject_GetAttrString(mymod, name);
    if (PyCallable_Check(pFunc)) {
        PyObject *pArgs = Py_BuildValue("(i)", val1);
        PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            ret = py_obj_to_int(pValue);
            Py_DECREF(pValue);
        }
    } else {
        ERR("cannot call python function %s(%d)", name, val1);
    }

    py_err();

    return (ret);
}

char *py_obj_to_str (const PyObject *py_str)
{
    PyObject *py_encstr;
    char *outstr = 0;
    char *str;

    py_encstr = 0;
    str = 0;

    if (!PyUnicode_Check((PyObject *)py_str)) {
        ERR("Object is a %s, not a string object.",
            Py_TYPE((PyObject *)py_str)->tp_name);
        goto err_out;
    }

    py_encstr = PyUnicode_AsEncodedString((PyObject *)py_str, "utf-8", 0);
    if (!py_encstr) {
        goto err_out;
    }

    str = PyBytes_AS_STRING(py_encstr);
    if (!str) {
        goto err_out;
    }

    outstr = dupstr(str, __FUNCTION__);

err_out:

    if (py_encstr) {
        Py_XDECREF(py_encstr);
    }

    if (PyErr_Occurred()) {
        ERR("string conversion failed");
    }

    return (outstr);
}

int py_obj_to_int (PyObject *py_obj)
{
    int val;

    val = 0;

    if (!PyLong_Check((PyObject *)py_obj)) {
        ERR("Object is a %s, not a int object.",
            Py_TYPE((PyObject *)py_obj)->tp_name);
        goto err_out;
    }

    val = PyLong_AsLong(py_obj);
    if (!val) {
        goto err_out;
    }

err_out:

    if (PyErr_Occurred()) {
        ERR("int conversion failed");
    }

    return (val);
}

uint64_t py_obj_to_uint64 (PyObject *py_obj)
{
    uint64_t val;

    val = 0;

    if (!PyLong_Check((PyObject *)py_obj)) {
        ERR("Object is a %s, not a uint64 object.",
            Py_TYPE((PyObject *)py_obj)->tp_name);
        goto err_out;
    }

    val = PyLong_AsUnsignedLongLong(py_obj);
    if (!val) {
        goto err_out;
    }

err_out:

    if (PyErr_Occurred()) {
        ERR("int conversion failed");
    }

    return (val);
}

int py_obj_attr_int (const PyObject *py_obj, const char *attr)
{
    PyObject *py_encstr;
    char *str;
    int i = 0;

    py_encstr = 0;
    str = 0;

    if (!PyObject_HasAttrString((PyObject *)py_obj, attr)) {
        ERR("Object is a %s, not a string object.",
            Py_TYPE((PyObject *)py_obj)->tp_name);
        goto err_out;
    }

    py_encstr = PyObject_GetAttrString((PyObject *)py_obj, attr);
    if (!py_encstr) {
        goto err_out;
    }

    i = py_obj_to_int(py_encstr);

err_out:
    if (py_encstr) {
        Py_XDECREF(py_encstr);
    }

    if (PyErr_Occurred()) {
        ERR("int conversion failed");
    }

    return (i);
}

uint64_t py_obj_attr_uint64 (const PyObject *py_obj, const char *attr)
{
    PyObject *py_encstr;
    char *str;
    uint64_t i = 0;

    py_encstr = 0;
    str = 0;

    if (!PyObject_HasAttrString((PyObject *)py_obj, attr)) {
        ERR("Object is a %s, not a string object.",
            Py_TYPE((PyObject *)py_obj)->tp_name);
        goto err_out;
    }

    py_encstr = PyObject_GetAttrString((PyObject *)py_obj, attr);
    if (!py_encstr) {
        goto err_out;
    }

    i = py_obj_to_uint64(py_encstr);

err_out:
    if (py_encstr) {
        Py_XDECREF(py_encstr);
    }

    if (PyErr_Occurred()) {
        ERR("int conversion failed");
    }

    return (i);
}

char *py_obj_attr_str (const PyObject *py_obj, const char *attr)
{
    PyObject *py_encstr;
    char *outstr = 0;
    char *str;

    py_encstr = 0;
    str = 0;

    if (!PyObject_HasAttrString((PyObject *)py_obj, attr)) {
        ERR("Object is a %s, not a string object.",
            Py_TYPE((PyObject *)py_obj)->tp_name);
        goto err_out;
    }

    py_encstr = PyObject_GetAttrString((PyObject *)py_obj, attr);
    if (!py_encstr) {
        goto err_out;
    }

    str = py_obj_to_str(py_encstr);
    if (!str) {
        goto err_out;
    }

    outstr = str;

err_out:

    if (py_encstr) {
        Py_XDECREF(py_encstr);
    }

    if (PyErr_Occurred()) {
        ERR("string conversion failed");
    }

    return (outstr);
}

static PyObject *con_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = 0;

    if (!PyArg_ParseTuple(args, "s", &a)) {
        return (0);
    }

    if (a) {
        CON("%s", a);
    }

    Py_RETURN_NONE;
}

static PyObject *log_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = 0;

    if (!PyArg_ParseTuple(args, "s", &a)) {
        return (0);
    }

    if (a) {
        LOG("%s", a);
    }

    Py_RETURN_NONE;
}

static PyObject *err_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = 0;

    if (!PyArg_ParseTuple(args, "s", &a)) {
        return (0);
    }

    if (a) {
        ERR("%s", a);
    }

    Py_RETURN_NONE;
}

static PyObject *die_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = 0;

    if (!PyArg_ParseTuple(args, "s", &a)) {
        return (0);
    }

    if (a) {
        DIE("%s", a);
    }

    Py_RETURN_NONE;
}

static PyMethodDef python_c_METHODS[] =
{
    /*
     * The cast of the function is necessary since PyCFunction values
     * only take two PyObject *parameters, and some take three.
     */
    {"con",
        (PyCFunction)con_,
        METH_VARARGS,
        "log to the console"},

    {"err",
        (PyCFunction)err_,
        METH_VARARGS,
        "error to the log file"},

    {"log",
        (PyCFunction)log_,
        METH_VARARGS,
        "log to the log file"},

    {"die",
        (PyCFunction)die_,
        METH_VARARGS,
        "exit game with error"},

    PY_DECL(wid_mouse_hide)

    {"wid_tiles_load",
        (PyCFunction)wid_tiles_load_,
        METH_VARARGS | METH_KEYWORDS,
        "load a block of wid tiles"},

    {"text_size",
        (PyCFunction)ttf_text_size_,
        METH_VARARGS | METH_KEYWORDS,
        "text size in pixels"},

    {"text_size_pct",
        (PyCFunction)ttf_text_size_pct_,
        METH_VARARGS | METH_KEYWORDS,
        "text size in percent"},

    {"tex_load",
        (PyCFunction)tex_load_,
        METH_VARARGS | METH_KEYWORDS,
        "load a texture"},

    {"tex_load_tiled",
        (PyCFunction)tex_load_tiled_,
        METH_VARARGS | METH_KEYWORDS,
        "load a texture"},

    {"tex_size",
        (PyCFunction)tex_size_,
        METH_VARARGS | METH_KEYWORDS,
        "tex info"},

    {"tex_pixel",
        (PyCFunction)tex_pixel_,
        METH_VARARGS | METH_KEYWORDS,
        "get pixels"},

    {"tile_load_arr",
        (PyCFunction)tile_load_arr_,
        METH_VARARGS | METH_KEYWORDS,
        "load a tile array"},

    {"tile_size",
        (PyCFunction)tile_get_size_,
        METH_VARARGS | METH_KEYWORDS,
        "tile size in pixels"},

    {"tile_size_pct",
        (PyCFunction)tile_get_size_pct_,
        METH_VARARGS | METH_KEYWORDS,
        "tile size in pct"},

    {"tp_load",
        (PyCFunction)tp_load_,
        METH_VARARGS | METH_KEYWORDS,
        "load a thing template"},

    {"thing_new",
        (PyCFunction)thing_new_,
        METH_VARARGS | METH_KEYWORDS,
        "create a thing template"},

    THING_DECL(destroyed)
    THING_DECL(move)
    THING_DECL(push)
    THING_DECL(pop)

    {"wid_new",
        (PyCFunction)wid_new_,
        METH_VARARGS | METH_KEYWORDS,
        "create a wid"},

    {"wid_destroy",
        (PyCFunction)wid_destroy_,
        METH_VARARGS | METH_KEYWORDS,
        "destroy a wid"},

    {"wid_new_scrollbar",
        (PyCFunction)wid_new_scrollbar_,
        METH_VARARGS | METH_KEYWORDS,
        "create scrollbars"},

    {"wid_set_shape",
        (PyCFunction)wid_set_shape_,
        METH_VARARGS | METH_KEYWORDS,
        "set wid shape"},

    {"wid_set_tl_br",
        (PyCFunction)wid_set_tl_br_,
        METH_VARARGS | METH_KEYWORDS,
        "set bounds"},

    {"wid_set_tl_br_pct",
        (PyCFunction)wid_set_tl_br_pct_,
        METH_VARARGS | METH_KEYWORDS,
        "set bounds in percent"},

    {"wid_set_text",
        (PyCFunction)wid_set_text_,
        METH_VARARGS | METH_KEYWORDS,
        "set text"},

    {"wid_set_color",
        (PyCFunction)wid_set_color_,
        METH_VARARGS | METH_KEYWORDS,
        "set color"},

    {"wid_set_tex",
        (PyCFunction)wid_set_tex_,
        METH_VARARGS | METH_KEYWORDS,
        "set texture"},

    {"wid_raise",
        (PyCFunction)wid_set_raise_,
        METH_VARARGS | METH_KEYWORDS,
        "raise wid"},

    {"wid_lower",
        (PyCFunction)wid_set_lower_,
        METH_VARARGS | METH_KEYWORDS,
        "lower wid"},

    {"wid_update",
        (PyCFunction)wid_set_update_,
        METH_VARARGS | METH_KEYWORDS,
        "update wid"},

    {"wid_set_bevel",
        (PyCFunction)wid_set_bevel_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_set_bevel"},

    {"wid_set_bevelled",
        (PyCFunction)wid_set_bevelled_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_set_bevelled"},

    {"wid_set_cursor",
        (PyCFunction)wid_set_cursor_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_set_cursor"},

    {"wid_set_do_not_lower",
        (PyCFunction)wid_set_do_not_lower_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_set_do_not_lower"},

    {"wid_set_do_not_raise",
        (PyCFunction)wid_set_do_not_raise_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_set_do_not_raise"},

    {"wid_set_focusable",
        (PyCFunction)wid_set_focusable_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_set_focusable"},

    {"wid_set_movable",
        (PyCFunction)wid_set_movable_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_set_movable"},

    {"wid_set_movable_bounded",
        (PyCFunction)wid_set_movable_bounded_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_set_movable_bounded"},

    {"wid_set_movable_horiz",
        (PyCFunction)wid_set_movable_horiz_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_set_movable_horiz"},

    {"wid_set_movable_vert",
        (PyCFunction)wid_set_movable_vert_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_set_movable_vert"},

    {"wid_get_size",
        (PyCFunction)wid_get_size_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_get_size"},

    {"wid_get_size_pct",
        (PyCFunction)wid_get_size_pct_,
        METH_VARARGS | METH_KEYWORDS,
        "wid_get_size_pct"},

    WID_DECL(wid_move_to_horiz_vert_pct_in)
    WID_DECL(wid_move_to_abs_centered_in)
    WID_DECL(wid_move_to_centered_in)
    WID_DECL(wid_move_delta_pct_in)
    WID_DECL(wid_move_to_abs_in)
    WID_DECL(wid_move_delta_in)
    WID_DECL(wid_move_to_pct_centered_in)
    WID_DECL(wid_move_to_abs_poffset_in)
    WID_DECL(wid_move_to_pct_in)

    WID_DECL(wid_move_delta)
    WID_DECL(wid_move_to_abs)
    WID_DECL(wid_move_to_abs_centered)
    WID_DECL(wid_move_to_pct)
    WID_DECL(wid_move_to_pct_centered)
    WID_DECL(wid_move_to_vert_pct_in)
    WID_DECL(wid_move_to_horiz_pct_in)

    WID_DECL(wid_move_to_vert_pct)
    WID_DECL(wid_move_to_horiz_pct)

    WID_DECL(wid_move_to_bottom)
    WID_DECL(wid_move_to_left)
    WID_DECL(wid_move_to_right)
    WID_DECL(wid_move_to_top)
    WID_DECL(wid_move_end)

    WID_DECL(wid_destroy_in)
    WID_DECL(wid_fade_in)
    WID_DECL(wid_fade_out)
    WID_DECL(wid_fade_in_out)
    WID_DECL(wid_visible)
    WID_DECL(wid_hide)
    WID_DECL(wid_toggle_hidden)
    WID_DECL(wid_mouse_hide)

    WID_DECL(wid_set_on_tooltip)
    WID_DECL(wid_set_on_key_down)
    WID_DECL(wid_set_on_key_up)
    WID_DECL(wid_set_on_joy_button)
    WID_DECL(wid_set_on_mouse_down)
    WID_DECL(wid_set_on_mouse_motion)
    WID_DECL(wid_set_on_mouse_focus_begin)
    WID_DECL(wid_set_on_mouse_focus_end)
    WID_DECL(wid_set_on_mouse_over_begin)
    WID_DECL(wid_set_on_mouse_over_end)
    WID_DECL(wid_set_on_mouse_up)
    WID_DECL(wid_set_on_destroy)
    WID_DECL(wid_set_on_destroy_begin)
    WID_DECL(wid_set_on_tick)
    WID_DECL(wid_set_on_display)
    WID_DECL(wid_set_on_display_top_level)

    WID_DECL(wid_destroy_grid)
    WID_DECL(wid_detach_from_grid)
    WID_DECL(wid_attach_to_grid)
    WID_DECL(wid_empty_grid)
    WID_DECL(wid_update)
    WID_DECL(wid_raise)
    WID_DECL(wid_lower)
    WID_DECL(wid_scroll_text)
    WID_DECL(wid_set_focus)
    WID_DECL(wid_focus_lock)
    WID_DECL(wid_effect_pulses)
    WID_DECL(wid_effect_pulse_forever)
    WID_DECL(wid_effect_pulse_stop)
    WID_DECL(wid_blit_effect_pulses)
    WID_DECL(wid_effect_sways)
    WID_DECL(wid_animate)

    TP_SET_DECL(tile)
    TP_SET_DECL(light_radius)
    TP_SET_DECL(z_depth)
    TP_SET_DECL(z_order)
    TP_SET_DECL(scale)
    TP_SET_DECL(world_depth)
    TP_SET_DECL(speed)
    TP_SET_DECL(blit_top_off)
    TP_SET_DECL(blit_bot_off)
    TP_SET_DECL(blit_left_off)
    TP_SET_DECL(blit_right_off)
    TP_SET_DECL(short_name)
    TP_SET_DECL(raw_name)
    TP_SET_DECL(light_tint)
    TP_SET_DECL(is_animated)
    TP_SET_DECL(is_animated_no_dir)
    TP_SET_DECL(is_animation)
    TP_SET_DECL(is_candle_light)
    TP_SET_DECL(is_door)
    TP_SET_DECL(is_effect_fade_in_out)
    TP_SET_DECL(is_effect_pulse)
    TP_SET_DECL(is_effect_rotate_2way)
    TP_SET_DECL(is_effect_sway)
    TP_SET_DECL(is_explosion)
    TP_SET_DECL(is_food)
    TP_SET_DECL(is_hidden)
    TP_SET_DECL(is_monst)
    TP_SET_DECL(is_player)
    TP_SET_DECL(is_rrr1)
    TP_SET_DECL(is_rrr2)
    TP_SET_DECL(is_rrr3)
    TP_SET_DECL(is_rrr4)
    TP_SET_DECL(is_rrr5)
    TP_SET_DECL(is_rrr6)
    TP_SET_DECL(is_rrr7)
    TP_SET_DECL(is_rrr8)
    TP_SET_DECL(is_rrr9)
    TP_SET_DECL(is_rrr10)
    TP_SET_DECL(is_rrr11)
    TP_SET_DECL(is_rrr12)
    TP_SET_DECL(is_rrr13)
    TP_SET_DECL(is_rrr14)
    TP_SET_DECL(is_rrr15)
    TP_SET_DECL(is_rrr16)
    TP_SET_DECL(is_shadow_caster)
    TP_SET_DECL(is_shadow_caster_soft)
    TP_SET_DECL(is_sleeping)
    TP_SET_DECL(is_wall)

    {0, 0, 0, 0}   /* sentinel */
};

static struct PyModuleDef python_c_MODULE = {
    PyModuleDef_HEAD_INIT,
    "mm",
    0,
    -1,
    python_c_METHODS,
    0, 0, 0, 0
};

static PyMODINIT_FUNC
python_my_module_create (void)
{
   PyObject *m = PyModule_Create(&python_c_MODULE);
   if (! m) {
        PyErr_Print();
        DIE("python init");
        return (0);
   }

   return (m);
}

void py_err (void)
{
    PyObject *err = PyErr_Occurred();
    if (!err) {
        return;
    }

    PyObject *ptype, *pvalue, *ptraceback, *pyobj_str;
    PyObject *ret, *list, *string;
    PyObject *mod;
    char *py_str;

    PyErr_Fetch(&ptype, &pvalue, &ptraceback);
    pyobj_str = PyObject_Str(pvalue);
    py_str = py_obj_to_str(pyobj_str);
    ERR("%s", py_str);
    myfree(py_str);

    mod = PyImport_ImportModule("traceback");
    list = PyObject_CallMethod(mod, "format_exception", "OOO", ptype, pvalue, ptraceback);
    if (list) {
        string = PyUnicode_FromString("\n");
        ret = PyUnicode_Join(string, list);
        Py_DECREF(list);
        Py_DECREF(string);

        py_str = py_obj_to_str(ret);
        ERR("%s", py_str);
        myfree(py_str);

        Py_DECREF(ret);
    }

    PyErr_Clear();

    PyThreadState *tstate = PyThreadState_GET();

    if (tstate && tstate->frame) {
        PyFrameObject *frame = tstate->frame;

        ERR("Python stack trace:\n");

        while (frame) {
            int line = frame->f_lineno;
            char *filename = py_obj_to_str(frame->f_code->co_filename);
            char *funcname = py_obj_to_str(frame->f_code->co_name);
            ERR("    %s(%d): %s\n", filename, line, funcname);
            frame = frame->f_back;
            myfree(filename);
            myfree(funcname);
        }
    }
}

void py_exec (const char *str)
{
    char *stdOutErr =
"import sys\n\
import mm\n\
class CatchOutErr:\n\
    def __init__(self):\n\
        self.value = ''\n\
    def write(self, txt):\n\
        self.value += txt\n\
catchOutErr = CatchOutErr()\n\
sys.stdout = catchOutErr\n\
sys.stderr = catchOutErr\n\
";

    PyObject *pModule = PyImport_AddModule("__main__");

    PyRun_SimpleString(stdOutErr);
    PyRun_SimpleString(str);

    PyObject *catcher = PyObject_GetAttrString(pModule, "catchOutErr");
    PyObject *output = PyObject_GetAttrString(catcher, "value");

    char *text = py_obj_to_str(output);
    if (text) {
        strchopc(text, '\n');
        CON("%s", text);
        myfree(text);
    }
    py_err();
}

static void py_add_to_path (const char *path)
{
    PyObject *py_cur_path, *py_item;
    char *new_path;
    int wc_len, i;
    wchar_t *wc_new_path;
    char *item;

    new_path = dupstr(path, __FUNCTION__);
    py_cur_path = PySys_GetObject("path");

    for (i = 0; i < PyList_Size(py_cur_path); i++) {
        char *tmp = strappend(new_path, ":");
        myfree(new_path);
        new_path = tmp;

        py_item = PyList_GetItem(py_cur_path, i);

        if (!PyUnicode_Check(py_item)) {
            continue;
        }

        item = py_obj_to_str(py_item);
        if (!item) {
            continue;
        }

        tmp = strappend(new_path, item);
        myfree(new_path);
        new_path = tmp;

        myfree(item);
    }

    /* Convert to wide chars. */
    wc_len = sizeof(wchar_t) * (strlen(new_path) + 1);

    wc_new_path = myzalloc(wc_len, "wchar str");
    if (!wc_new_path) {
        DIE("path alloc fail");
    }

    LOG("Set python path: %s", new_path);

    mbstowcs(wc_new_path, new_path, wc_len);
    PySys_SetPath(wc_new_path);
    myfree(new_path);
    myfree(wc_new_path);
}

void python_init (void)
{
    PyImport_AppendInittab("mm", python_my_module_create);

    Py_Initialize();

    py_add_to_path(GFX_PATH);
    py_add_to_path(LEVELS_PATH);
    py_add_to_path(WORLD_PATH);
    py_add_to_path(DATA_PATH);
    py_add_to_path(CLASSES_PATH);

    mymod = PyImport_ImportModule("init");
    if (!mymod) {
        py_err();
        DIE("module import failed");
    }
}

void python_fini (void)
{
   Py_Finalize();
}
