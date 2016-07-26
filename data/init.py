import mm
import os
import fnmatch
from os.path import dirname, basename
import imp


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

wid_intro_title = None


def wid_intro_bg_create():
    global wid_intro_title

    if wid_intro_title is None:
        wid_intro_title = w = wid.Wid(name="intro bg")
        w.set_tl_br_pct(x1=0.0, y1=0.1, x2=1.0, y2=0.9)
        w.set_tex(name="main_title");
        w.set_color(tl=True, bg=True, br=True, name="white")
        w.to_back()
        w.update();
        w.set_do_not_raise(value=True);


def init2():
    load_all_plugins()

    wid_intro_bg_create()

    w = wid_text.WidText(name="test", 
                         tiles="wid1", 
                         text="%%fg=green$hello there you%%fg=red$... a b c dddd ee fff gggg hh   _  iiiiiiiiiii jjjjjjjjjjjjjjjj kkkkkkk lllllll mmmmmmmmmmmm nnnnnnn ooooooooooooop            ppppppppppp qqqqqqqqq rrrrrr ssssss\nline2\nmmmmmmmmmmmmmmmm",
                         font="vsmall", 
                         x1=0.2, y1=0.2, x2=0.8, y2=0.8)

    w.set_text(text="hello", font="vsmall", color="red")

#    w = wid.Wid(name="test2", tiles="wid1")
#    w.set_tl_br_pct(x1=0.5, y1=0.2, x2=0.9, y2=0.3)

init1()
