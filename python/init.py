import mm
import os
import sys
import fnmatch
from os.path import dirname, basename
import imp
import thing
import level
import world
import util


def find_plugins(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for f in files:
            if fnmatch.fnmatch(f, pattern):
                filename = os.path.join(root, f)
                yield filename


def load_one_plugin(filepath):
    if basename(filepath) == "init.py":
        return

    if basename(filepath) == "tp.py":
        return

    mod_name, file_ext = os.path.splitext(os.path.split(filepath)[-1])

    if file_ext.lower() == '.py':
        py_mod = imp.load_source(mod_name, filepath)

    elif file_ext.lower() == '.pyc':
        py_mod = imp.load_compiled(mod_name, filepath)

    #
    # Make aliases for imports so we can reference their functions
    #
    if basename(filepath) == "config.py":
        global config
        config = py_mod

    if basename(filepath) == "hooks.py":
        global hooks
        hooks = py_mod

    if basename(filepath) == "world.py":
        global world
        world = py_mod

    if basename(filepath) == "game.py":
        global game
        game = py_mod

    if basename(filepath) == "wid.py":
        global wid
        wid = py_mod

    if basename(filepath) == "wid_text.py":
        global wid_text
        wid_text = py_mod

    if basename(filepath) == "wid_popup.py":
        global wid_popup
        wid_popup = py_mod

    if basename(filepath) == "wid_focus.py":
        global wid_focus
        wid_focus = py_mod

    if basename(filepath) == "wid_quit.py":
        global wid_quit
        wid_quit = py_mod

    if basename(filepath) == "wid_tp.py":
        global wid_tp
        wid_tp = py_mod

    if basename(filepath) == "wid_intro_menu.py":
        global wid_intro_menu
        wid_intro_menu = py_mod

    if basename(filepath) == "wid_intro_bg.py":
        global wid_intro_bg
        wid_intro_bg = py_mod


def load_all_plugins():
    for filename in find_plugins(dirname(__file__), '*.py'):
        mm.con("Loading " + filename)
        load_one_plugin(filename)


def load_plugin(plugin):
    for filename in find_plugins(os.getcwd(), plugin):
        mm.con("Loading " + filename)
        load_one_plugin(filename)


def init1():
    """ game defaults """
    mm.game_video_pix_width = 0
    mm.game_video_pix_height = 0
    mm.game_sound_volume = 10
    mm.game_music_volume = 5
    mm.game_display_sync = 0
    mm.game_full_screen = 0
    mm.game_fps_counter = 0

    load_plugin('config.py')
    load_plugin('mundusmeus-config.py')
    config.save_game_config()


def init2():

    load_all_plugins()

    wid_intro_bg.create()
    wid_intro_menu.create()
    wid_quit.wid_quit_create()
    wid_tp.wid_tp_create()

    w = world.World(0)

    p = util.Xyz(0, 0, 0)
    w.push_level(p)

    l = w.get_level()
    l.set_dim(10, 10)

    t = thing.Thing(level=l, tp_name="player1")

init1()
