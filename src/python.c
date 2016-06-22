/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "python.h"
#include "py_tp.h"
#include "main.h"
#include "string_util.h"
#include "string_ext.h"
#include "frameobject.h"
#include "tex.h"
#include "tile.h"
#include "thing_template.h"

static PyObject *mymod;

void py_call_vmi (const char *module, const char *name, int val1)
{
    LOG("python: %s.%s(%d)", module, name, val1);

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

void py_call_vi (const char *name, int val1)
{
    LOG("python: %s(%d)", name, val1);

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

int py_call_imi (const char *module, const char *name, int val1)
{
    int ret = -1;

    LOG("python: %s.%s(%d)", module, name, val1);

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

int py_call_ii (const char *name, int val1)
{
    int ret = -1;

    LOG("python: %s(%d)", name, val1);

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

    {"tp_load",
        (PyCFunction)tp_load_,
        METH_VARARGS | METH_KEYWORDS,
        "load a thing template"},

    TP_DECL(tile)
    TP_DECL(light_radius)
    TP_DECL(weapon_density)
    TP_DECL(weapon_spread)
    TP_DECL(scale)
    TP_DECL(explosion_radius)
    TP_DECL(collision_radius)
    TP_DECL(d10000_chance_of_appearing)
    TP_DECL(z_depth)
    TP_DECL(z_order)
    TP_DECL(world_depth)
    TP_DECL(speed)
    TP_DECL(damage)
    TP_DECL(cost)
    TP_DECL(lifespan_ticks)
    TP_DECL(vision_distance)
    TP_DECL(approach_distance)
    TP_DECL(bonus_score_on_death)
    TP_DECL(bonus_gold_on_collect)
    TP_DECL(bonus_hp_on_collect)
    TP_DECL(blit_top_off)
    TP_DECL(blit_bot_off)
    TP_DECL(blit_left_off)
    TP_DECL(blit_right_off)
    TP_DECL(drown_in_secs)
    TP_DECL(min_appear_depth)
    TP_DECL(max_appear_depth)
    TP_DECL(jump_speed)
    TP_DECL(hp_per_level)
    TP_DECL(max_hp)
    TP_DECL(hit_priority)
    TP_DECL(weapon_fire_delay_hundredths)
    TP_DECL(sound_random_delay_secs)
    TP_DECL(swing_distance_from_player)
    TP_DECL(can_be_hit_chance)
    TP_DECL(hit_delay_tenths)
    TP_DECL(mob_spawn_delay_tenths)
    TP_DECL(short_name)
    TP_DECL(raw_name)
    TP_DECL(tooltip)
    TP_DECL(polymorph_on_death)
    TP_DECL(carried_as)
    TP_DECL(light_tint)
    TP_DECL(explodes_as)
    TP_DECL(sound_on_creation)
    TP_DECL(sound_on_hitting_something)
    TP_DECL(sound_on_death)
    TP_DECL(sound_on_hit)
    TP_DECL(sound_on_collect)
    TP_DECL(sound_random)
    TP_DECL(weapon_carry_anim)
    TP_DECL(weapon_swing_anim)
    TP_DECL(message_on_use)
    TP_DECL(is_acid)
    TP_DECL(is_acid_proof)
    TP_DECL(is_action)
    TP_DECL(is_action_down)
    TP_DECL(is_action_left)
    TP_DECL(is_action_right)
    TP_DECL(is_action_sleep)
    TP_DECL(is_action_trigger)
    TP_DECL(is_action_trigger_on_hero)
    TP_DECL(is_action_trigger_on_monst)
    TP_DECL(is_action_trigger_on_wall)
    TP_DECL(is_action_up)
    TP_DECL(is_action_zap)
    TP_DECL(is_animated)
    TP_DECL(is_animated_no_dir)
    TP_DECL(is_animated_only_when_moving)
    TP_DECL(is_animation)
    TP_DECL(is_bomb)
    TP_DECL(is_bullet)
    TP_DECL(is_candle_light)
    TP_DECL(is_carryable)
    TP_DECL(is_cats_eyes)
    TP_DECL(is_cloud_effect)
    TP_DECL(is_cobweb)
    TP_DECL(is_collision_map_large)
    TP_DECL(is_collision_map_medium)
    TP_DECL(is_collision_map_small)
    TP_DECL(is_collision_map_tiny)
    TP_DECL(is_combustable)
    TP_DECL(is_conical_breath_attack)
    TP_DECL(is_corpse)
    TP_DECL(is_corridor)
    TP_DECL(is_corridor_wall)
    TP_DECL(is_door)
    TP_DECL(is_dungeon_floor)
    TP_DECL(is_effect_fade_in_out)
    TP_DECL(is_effect_pulse)
    TP_DECL(is_effect_rotate_2way)
    TP_DECL(is_effect_sway)
    TP_DECL(is_entrance)
    TP_DECL(is_ethereal)
    TP_DECL(is_exit)
    TP_DECL(is_explosion)
    TP_DECL(is_fire)
    TP_DECL(is_fireball)
    TP_DECL(is_fires_when_angry)
    TP_DECL(is_food)
    TP_DECL(is_fragile)
    TP_DECL(is_given_randomly_at_start)
    TP_DECL(is_hard)
    TP_DECL(is_hidden)
    TP_DECL(is_hidden_from_editor)
    TP_DECL(is_inactive)
    TP_DECL(is_internal)
    TP_DECL(is_item_unusable)
    TP_DECL(is_joinable)
    TP_DECL(is_key)
    TP_DECL(is_ladder)
    TP_DECL(is_ladder_deco)
    TP_DECL(is_lava)
    TP_DECL(is_lava_proof)
    TP_DECL(is_levitating)
    TP_DECL(is_life_saving)
    TP_DECL(is_light_source)
    TP_DECL(is_magical_weapon)
    TP_DECL(is_melee_weapon)
    TP_DECL(is_mob_spawner)
    TP_DECL(is_monst)
    TP_DECL(is_non_explosive_gas_cloud)
    TP_DECL(is_not_light_blocking)
    TP_DECL(is_obstacle)
    TP_DECL(is_player)
    TP_DECL(is_projectile)
    TP_DECL(is_ranged_weapon)
    TP_DECL(is_rock)
    TP_DECL(is_rrr1)
    TP_DECL(is_rrr2)
    TP_DECL(is_rrr3)
    TP_DECL(is_rrr4)
    TP_DECL(is_rrr5)
    TP_DECL(is_rrr6)
    TP_DECL(is_rrr7)
    TP_DECL(is_rrr8)
    TP_DECL(is_rrr9)
    TP_DECL(is_rrr10)
    TP_DECL(is_rrr11)
    TP_DECL(is_rrr12)
    TP_DECL(is_rrr13)
    TP_DECL(is_rrr14)
    TP_DECL(is_rrr15)
    TP_DECL(is_rrr16)
    TP_DECL(is_northern_mountain)
    TP_DECL(is_snow_castle)
    TP_DECL(is_castle)
    TP_DECL(is_mountain)
    TP_DECL(is_forest)
    TP_DECL(is_sand)
    TP_DECL(is_grass)
    TP_DECL(is_land)
    TP_DECL(is_wanderer_lr)
    TP_DECL(is_snow_settlement)
    TP_DECL(is_northern_settlement)
    TP_DECL(is_snow)
    TP_DECL(is_settlement)
    TP_DECL(is_northern_rock)
    TP_DECL(is_sea)
    TP_DECL(is_rope)
    TP_DECL(is_throwable)
    TP_DECL(is_sawblade)
    TP_DECL(is_shadow_caster)
    TP_DECL(is_shadow_caster_soft)
    TP_DECL(is_shop_floor)
    TP_DECL(is_shopkeeper)
    TP_DECL(is_single_mob_spawner)
    TP_DECL(is_sleeping)
    TP_DECL(is_spawns_under)
    TP_DECL(is_spider_proof)
    TP_DECL(is_spikes)
    TP_DECL(is_stackable)
    TP_DECL(is_teleport)
    TP_DECL(is_torch)
    TP_DECL(is_trap)
    TP_DECL(is_treasure)
    TP_DECL(is_undead)
    TP_DECL(is_variable_size)
    TP_DECL(is_visible_on_debug_only)
    TP_DECL(is_wall)
    TP_DECL(is_deco)
    TP_DECL(is_wanderer)
    TP_DECL(is_warm_blooded)
    TP_DECL(is_water)
    TP_DECL(is_water_proof)
    TP_DECL(is_weapon)
    TP_DECL(is_weapon_carry_anim)

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


py_call_vmi("config", "set_game_video_pix_width", game.video_pix_width);
py_call_vmi("config", "set_game_video_pix_width", game.video_pix_width+100);
int x = py_call_imi("config", "get_game_video_pix_width", game.video_pix_width);
CON("x %d",x);
//py_call_int("set_game_video_pix_width", game.video_pix_width+1);
}

void python_fini (void)
{
   Py_Finalize();
}
