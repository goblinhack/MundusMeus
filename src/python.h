/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include <Python.h>

void python_init(char *argv[]);
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

PyObject *biome_set_is_land_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *biome_set_is_dungeon_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_sound_volume_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_music_volume_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_display_sync_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_full_screen_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_move_count_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_moves_per_day_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_map_fixup_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_map_cleanup_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_map_add_selection_buttons_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_map_remove_selection_buttons_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_map_clear_selection_buttons_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_map_set_selection_buttons_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_map_time_step_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz1_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz2_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz3_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz4_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz5_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz6_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz7_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz8_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz9_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz10_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz11_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz12_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz13_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz14_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz15_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz16_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz17_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz18_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz19_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz20_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz21_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz22_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz23_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz24_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz25_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz26_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz27_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz28_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz29_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz30_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz31_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz32_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz33_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz34_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz35_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz36_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz37_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz38_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz39_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz40_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz41_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz42_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz43_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz44_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz45_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz46_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz47_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz48_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz49_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz50_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz51_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz52_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz53_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz54_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz55_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz56_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz57_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz58_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz59_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz60_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz61_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz62_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz63_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz64_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz65_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz66_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz67_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz68_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz69_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz70_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz71_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz72_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz73_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz74_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz75_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz76_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz77_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz78_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz79_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz80_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz81_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz82_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz83_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz84_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz85_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz86_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz87_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz88_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz89_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz90_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz91_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz92_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_zzz93_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_sdl_delay_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_daylight_color_a_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_daylight_color_b_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_daylight_color_g_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_daylight_color_r_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_rain_amount_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_set_snow_amount_(PyObject *obj, PyObject *args, PyObject *keywds);
PyObject *game_scroll_chunk_(PyObject *obj, PyObject *args, PyObject *keywds);

#define PY_PROTO(__fn__)                                                    \
PyObject *__fn__ ## _ (PyObject *obj, PyObject *args, PyObject *keywds);

#define PY_DECL(__fn__)                                                     \
    {#__fn__,                                                               \
     (PyCFunction)__fn__ ## _,                                              \
     METH_VARARGS | METH_KEYWORDS,                                          \
     "call " #__fn__ " "},                                                  \

