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

static char *py_obj_to_str (const PyObject *py_str)
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

static PyObject *hello (PyObject *obj, PyObject *args, PyObject *keywds)
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

static PyObject *con_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = 0;

    if (!PyArg_ParseTuple(args, "s", &a)) {
        return (0);
    }

    if (a) {
        CON("%s", a);
    }

    Py_INCREF(Py_None);

    return (Py_None);
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

    Py_INCREF(Py_None);

    return (Py_None);
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

    Py_INCREF(Py_None);

    return (Py_None);
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

    Py_INCREF(Py_None);

    return (Py_None);
}

static PyObject *tex_load_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = "unset file";
    char *b = "unset name";

    static char *kwlist[] = {"file", "name", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ss", kwlist, &a, &b)) {
        return (0);
    }

    if (!a) {
        ERR("tex_load, missing file attr");
        return (0);
    }

    if (!b) {
        ERR("tex_load, missing name attr");
        return (0);
    }

    LOG("tex_load(file=%s, name=%s)", a, b);
    tex_load(a, b);

    Py_INCREF(Py_None);

    return (Py_None);
}

static PyObject *tex_load_tiled_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = "unset file";
    char *b = "unset name";
    int c = 0;
    int d = 0;

    static char *kwlist[] = {"file", "name", "width", "height", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ssii", kwlist, &a, &b, &c, &d)) {
        return (0);
    }

    if (!a) {
        ERR("tex_load, missing file attr");
        return (0);
    }

    if (!b) {
        ERR("tex_load, missing name attr");
        return (0);
    }

    if (!c) {
        ERR("tex_load, missing width attr");
        return (0);
    }

    if (!d) {
        ERR("tex_load, missing height attr");
        return (0);
    }


    LOG("tex_load(file=%s, name=%s, width=%d, height=%d)", a, b, c, d);
    tex_load_tiled(a, b, c, d);

    Py_INCREF(Py_None);

    return (Py_None);
}

static PyObject *tile_load_arr_ (PyObject *obj, PyObject *args, PyObject *keywds)
{
    char *a = "unset tex_name";
    char *b = "unset tex_name_black_and_white";
    int c = 0;
    int d = 0;
    PyObject *e = 0;

    static char *kwlist[] = {"tex_name", "tex_name_black_and_white", "width", "height", "arr", 0};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ssiiO", kwlist, &a, &b, &c, &d, &e)) {
        return (0);
    }

    if (!a) {
        ERR("tile_load_arr, missing tex_name attr");
        return (0);
    }

    if (!b) {
        ERR("tile_load_arr, missing tex_name_black_and_white attr");
        return (0);
    }

    if (!c) {
        ERR("tile_load_arr, missing width attr");
        return (0);
    }

    if (!d) {
        ERR("tile_load_arr, missing height attr");
        return (0);
    }

    if (!e) {
        ERR("tile_load_arr, missing arr attr");
        return (0);
    }

    int numLines = PyList_Size(e);
    const char *arr[numLines + 1];
    int i;

    memset(arr, 0, sizeof(arr));

    LOG("tile_load_arr(tex_name=%s, tex_name_black_and_white=%s, width=%d, height=%d, ...)", a, b, c, d);

    for (i=0; i<numLines; i++){
        PyObject * strObj;
        strObj = PyList_GetItem(e, i); /* Can't fail */
        if (!strObj) {
            continue;
        }
        arr[i] = py_obj_to_str(strObj);

#if 0
        if (arr[i]) {
            LOG(" %s,", arr[i]);
        }
#endif
    }

    tile_load_arr(a, b, c, d, numLines, arr);

    for (i=0; i<numLines; i++){
        if (!arr[i]) {
            continue;
        }

        myfree((char*) arr[i]);
    }

    Py_INCREF(Py_None);

    return (Py_None);
}

static PyMethodDef python_c_METHODS[] =
{
    {"hello",           
        (PyCFunction)hello,             
        METH_VARARGS | METH_KEYWORDS,   
        "help text"},
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

    {"tex_load",        
        (PyCFunction)tex_load_,         
        METH_VARARGS | METH_KEYWORDS,   
        "load a texture"},

    {"tex_load_tiled",  
        (PyCFunction)tex_load_tiled_,   
        METH_VARARGS | METH_KEYWORDS,   
        "load a texture"},

    {"tile_load_arr",  
        (PyCFunction)tile_load_arr_,   
        METH_VARARGS | METH_KEYWORDS,   
        "load a tile array"},

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

static void py_err (void)
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
    string = PyUnicode_FromString("\n");
    ret = PyUnicode_Join(string, list);
    Py_DECREF(list);
    Py_DECREF(string);

    py_str = py_obj_to_str(ret);
    ERR("%s", py_str);
    myfree(py_str);

    Py_DECREF(ret);
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
    PyObject *mymod;

    PyImport_AppendInittab("mm", python_my_module_create);

    Py_Initialize();

    py_add_to_path(GFX_PATH);
    py_add_to_path(LEVELS_PATH);
    py_add_to_path(WORLD_PATH);
    py_add_to_path(DATA_PATH);

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
