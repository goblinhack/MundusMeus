import mm
import os
import fnmatch
from os.path import dirname, basename
import imp
import sys


def find_plugins(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for f in files:
            if fnmatch.fnmatch(f, pattern):
                filename = os.path.normcase(os.path.join(root, f))
                yield filename


def load_one_plugin(filename):
    if "rollbackBackupDirectory" in filename:
        return

    if basename(filename) == "init.py":
        return

    if basename(filename) == "tp.py":
        return

    mm.log("Loading plugin: " + filename)

    mod_name, file_ext = os.path.splitext(os.path.split(filename)[-1])

    if file_ext.lower() == '.py':
        py_mod = imp.load_source(mod_name, filename)

    elif file_ext.lower() == '.pyc':
        py_mod = imp.load_compiled(mod_name, filename)

    #
    # Make aliases for imports so we can reference their functions
    #
    if basename(filename) == "config.py":
        global config
        config = py_mod

    if basename(filename) == "hooks.py":
        global hooks
        hooks = py_mod

    if basename(filename) == "world.py":
        global world
        world = py_mod

    if basename(filename) == "game.py":
        global game
        game = py_mod

    if basename(filename) == "wid_map.py":
        global wid_map
        wid_map = py_mod

    if basename(filename) == "wid.py":
        global wid
        wid = py_mod

    if basename(filename) == "wid_text.py":
        global wid_text
        wid_text = py_mod

    if basename(filename) == "wid_popup.py":
        global wid_popup
        wid_popup = py_mod

    if basename(filename) == "wid_focus.py":
        global wid_focus
        wid_focus = py_mod

    if basename(filename) == "wid_quit.py":
        global wid_quit
        wid_quit = py_mod

    if basename(filename) == "wid_tp_list.py":
        global wid_tp_list
        wid_tp_list = py_mod

    if basename(filename) == "wid_tp_grid.py":
        global wid_tp_grid
        wid_tp_grid = py_mod

    if basename(filename) == "wid_tp_detail.py":
        global wid_tp_detail
        wid_tp_detail = py_mod

    if basename(filename) == "wid_intro_menu.py":
        global wid_intro_menu
        wid_intro_menu = py_mod

    if basename(filename) == "wid_intro_bg.py":
        global wid_intro_bg
        wid_intro_bg = py_mod


def load_all_plugins():
    plug_path = os.path.normcase(os.path.join(dirname(__file__), ""))
    mm.log("Plugin path " + plug_path)
    for filename in find_plugins(plug_path, '*.py'):
        load_one_plugin(filename)


def load_plugin(plugin):
    for filename in find_plugins(os.getcwd(), plugin):
        load_one_plugin(filename)

    for filename in find_plugins(os.environ["APPDATA"], plugin):
        load_one_plugin(filename)


def init1():

    if "APPDATA" not in os.environ:
        os.environ['APPDATA'] = "appdata"

    if not os.path.isdir(os.environ['APPDATA']):
        os.mkdir(os.environ['APPDATA'])

    os.environ['APPDATA'] = os.path.normcase(
            os.path.join(os.environ["APPDATA"], "mundusmeus"))

    if not os.path.isdir(os.environ['APPDATA']):
        os.mkdir(os.environ['APPDATA'])

    sys.stdout = open(os.path.normcase(
            os.path.join(os.environ["APPDATA"], "stdout.txt")), "a")

    sys.stderr = open(os.path.normcase(
            os.path.join(os.environ["APPDATA"], "stderr.txt")), "a")

    """ game defaults """
    mm.game_video_pix_width = 0
    mm.game_video_pix_height = 0
    mm.game_sound_volume = 10
    mm.game_music_volume = 5
    mm.game_display_sync = 1
    mm.game_full_screen = 0
    mm.game_fps_counter = 1

    load_plugin('config.py')
    load_plugin('mundusmeus-config.py')
    config.save_game_config()


def init2():

    load_all_plugins()

#    wid_intro_bg.create()
#    wid_intro_menu.create()
#    wid_quit.wid_quit_create()

    w = wid_tp_list.WidTpList(name="tp window",
                              tiles="wid2",
                              body_tiles="wid1",
                              width=0.25,
                              height=0.5,
                              x=0.75,
                              y=0.00,
                              detail_width=0.40,
                              detail_height=0.3,
                              detail_x=0.35,
                              detail_y=0.00)
    print(w)

    w = wid_tp_grid.WidTpGrid(name="tp window",
                              tiles="wid2",
                              body_tiles="wid1",
                              width=0.25,
                              height=0.5,
                              x=0.75,
                              y=0.50,
                              detail_width=0.40,
                              detail_height=0.3,
                              detail_x=0.05,
                              detail_y=0.50)
    print(w)

    game.game_new()


init1()
