/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "main.h"
#include "string_util.h"

static PyObject* hello (PyObject*obj, PyObject *args, PyObject *keywds)
{
    int a = 9;
    char *b = "def b";
    char *c = "def c";
    char *d = "def d";

    static char *kwlist[] = {"a", "b", "c", "d", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|sss", kwlist,
                                     &a, &b, &c, &d)) {
        return (0);
    }

    LOG("a %d",a);
    LOG("b %s",b);
    LOG("c %s",c);
    LOG("d %s",d);

    Py_INCREF(Py_None);

    char *s = "hello back";

    return (Py_BuildValue("s", s));
}

static PyMethodDef python_c_METHODS[] =
{
    /*
     * The cast of the function is necessary since PyCFunction values
     * only take two PyObject* parameters, and some take three.
     */
    {"hello", (PyCFunction)hello, METH_VARARGS | METH_KEYWORDS, "help text"},
    {0, 0, 0, 0}   /* sentinel */
};

static struct PyModuleDef python_c_MODULE = {
    PyModuleDef_HEAD_INIT,
    "my_c_mod",
    0,
    -1,
    python_c_METHODS,
    0, 0, 0, 0
};

static PyMODINIT_FUNC
python_my_module_create (void)
{
   PyObject* m = PyModule_Create(&python_c_MODULE);
   if (! m) {
        PyErr_Print();
        DIE("python init");
        return (0);
   }

   return (m);
}

static int py_obj_to_str (const PyObject *py_str, char **outstr)
{
    PyObject *py_encstr;
    int ret;
    char *str;

    py_encstr = 0;
    str = 0;
    ret = 0;

    if (!PyUnicode_Check((PyObject *)py_str)) {
        ERR("Object is a %s, not a string object.",
            Py_TYPE((PyObject *)py_str)->tp_name);
        ret = -1;
        goto err_out;
    }

    py_encstr = PyUnicode_AsEncodedString((PyObject *)py_str, "utf-8", 0);
    if (!py_encstr) {
        ret = -1;
        goto err_out;
    }

    str = PyBytes_AS_STRING(py_encstr);
    if (!str) {
        ret = -1;
        goto err_out;
    }

    *outstr = dupstr(str, __FUNCTION__);

err_out:

    if (py_encstr) {
        Py_XDECREF(py_encstr);
    }

    if (PyErr_Occurred()) {
        ERR("string conversion failed");
    }

    return (ret);
}

static void python_add_to_path (const char *path)
{
    PyObject *py_cur_path, *py_item;
    char *new_path;
    int wc_len, i;
    wchar_t *wc_new_path;
    char *item;

    new_path = dupstr(path, __FUNCTION__);
    py_cur_path = PySys_GetObject("path");

    for (i = 0; i < PyList_Size(py_cur_path); i++) {
        char *tmp = strappend(new_path, ";");
        myfree(new_path);
        new_path = tmp;

        py_item = PyList_GetItem(py_cur_path, i);

        if (!PyUnicode_Check(py_item)) {
            continue;
        }

        if (py_obj_to_str(py_item, &item) != 0) {
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

#if 0
   PyObject *mymod;
   PySys_SetPath(L".");
   mymod = PyImport_ImportModule("my_init");
   if (!mymod) {
        PyErr_Print();
        DIE("");
        fprintf(stderr,"\nZZZ %s %s %d ",__FILE__,__FUNCTION__,__LINE__);
        exit(1);
    }

    PyObject *strret, *strfunc, *strargs;
    char *cstrret;

    strfunc = PyObject_GetAttrString(mymod, "rstring");
    if (!strfunc) {
        PyErr_Print();
        fprintf(stderr,"\nZZZ %s %s %d ",__FILE__,__FUNCTION__,__LINE__);
        exit(1);
    }

    strargs = Py_BuildValue("(s)", "my_init");
    if (!strargs) {
        PyErr_Print();
        fprintf(stderr,"\nZZZ %s %s %d ",__FILE__,__FUNCTION__,__LINE__);
        exit(1);
    }

    strret = PyEval_CallObject(strfunc, strargs);
    if (!strret) {
        PyErr_Print();
        fprintf(stderr,"\nZZZ %s %s %d ",__FILE__,__FUNCTION__,__LINE__);
        exit(1);
    }
    PyArg_Parse(strret, "s", &cstrret);
}
#endif

void python_init (void)
{
   PyImport_AppendInittab("my_c_mod", python_my_module_create);

   Py_Initialize();

   python_add_to_path(GFX_PATH);
   python_add_to_path(LEVELS_PATH);
   python_add_to_path(WORLD_PATH);
   python_add_to_path(DATA_PATH);
}

void python_fini (void)
{
   Py_Finalize();
}
