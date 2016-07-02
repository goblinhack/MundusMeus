import mm
import os
import fnmatch
from os.path import dirname, basename
import imp

def find_plugins(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.join(root, basename)
                yield filename

def load_one_plugin(filepath):
    if basename(filepath) == "init.py":
        return

    if basename(filepath) == "tp.py":
        return

    mod_name,file_ext = os.path.splitext(os.path.split(filepath)[-1])

    if file_ext.lower() == '.py':
        py_mod = imp.load_source(mod_name, filepath)

    elif file_ext.lower() == '.pyc':
        py_mod = imp.load_compiled(mod_name, filepath)

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

def load_all_plugins():
    for filename in find_plugins(dirname(__file__), '*.py'):
        mm.con("Loading " + filename);
        load_one_plugin(filename);

def load_plugin(plugin):
    for filename in find_plugins(os.getcwd(), plugin):
        mm.con("Loading " + filename);
        load_one_plugin(filename);

def init1():
    #
    # Defaults
    #
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

init1()
