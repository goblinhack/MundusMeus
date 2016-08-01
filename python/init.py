import mm
import os
import sys
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

    if basename(filepath) == "wid_popup.py":
        global wid_popup
        wid_popup = py_mod


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
wid_intro_menu = None
wid_quit_menu = None


def wid_intro_bg_create():
    global wid_intro_title

    if wid_intro_title is None:
        w = wid.Wid(name="intro bg")
        wid_intro_title = w

        w.set_tl_br_pct(x1=0.0, y1=0.1, x2=1.0, y2=0.9)
        w.set_tex(name="main_title");
        w.set_color(tl=True, bg=True, br=True, name="white")
        w.to_back()
        w.update();
        w.set_do_not_raise(value=True);


def wid_intro_menu_create():
    global wid_intro_menu

    if wid_intro_menu is None:
        w = wid_popup.WidPopup(name="test", 
                               x1=0.2, y1=0.5)
        wid_intro_menu = w

        w.add_text(text="%%fg=green$a) %%fg=white$new game\n")
        w.set_color(bg=True, tl=True, br=True, name="red", alpha=0)
        w.update()


def wid_on_mouse_motion_callback(wid, x, y, relx, rely, wheelx, wheely):
    wid.con("wid_set_on_mouse_motion {0} {1} {2} {3} {4} {5}".format(x,y,relx,rely,wheelx,wheely))
    return False

def wid_on_tooltip_callback(wid, tooltip):
    wid.con("wid_on_tooltip_callback")
    return

def wid_on_key_down_callback(wid, sym, mod):
    wid.con("wid_on_key_down_callback {0} {1}".format(sym, mod))
    return False

def wid_on_key_up_callback(wid, sym, mod):
    wid.con("wid_on_key_up_callback {0} {1}".format(sym, mod))
    return False

def wid_on_joy_button_callback(wid,
                             button_a,
                             button_b,
                             button_x,
                             button_y,
                             button_top_left,
                             button_top_right,
                             button_left_stick_down,
                             button_right_stick_down,
                             button_start,
                             button_xbox,
                             button_back,
                             button_up,
                             button_down,
                             button_left,
                             button_right,
                             button_left_fire,
                             button_right_fire):
    wid.con("wid_on_joy_button_callback")

def wid_on_mouse_down_callback(wid, x, y, button):
    wid.con("wid_on_mouse_down_callback {0} {1} {2}".format(x, y, button))
    return False

def wid_on_mouse_focus_begin_callback(wid):
    wid.con("wid_on_mouse_focus_begin_callback")
    return

def wid_on_mouse_focus_end_callback(wid):
    wid.con("wid_on_mouse_focus_end_callback")
    return

def wid_on_mouse_over_begin_callback(wid, relx, rely, wheelx, wheely):
    wid.con("wid_on_mouse_over_begin_callback {0} {1} {2} {3}".format(relx, rely, wheelx, wheely))
    return

def wid_on_mouse_over_end_callback(wid):
    wid.con("wid_on_mouse_over_end_callback")
    return

def wid_on_mouse_up_callback(wid, x, y, button):
    wid.con("wid_on_mouse_up_callback {0} {1} {2}".format(x, y, button))
    return False

def wid_on_destroy_callback(wid):
    wid.con("wid_on_destroy_callback")
    return

def wid_on_destroy_begin_callback(wid):
    wid.con("wid_on_destroy_begin_callback")
    return

def wid_on_tick_callback(wid):
    wid.con("wid_on_tick_callback")
    return

def wid_on_display_callback(wid):
    wid.con("wid_on_display_callback")
    return

def wid_on_display_top_level_callback(wid):
    wid.con("wid_on_display_top_level_callback")
    return

def wid_quit_on_key_down(wid, sym, mod):
    parent = wid.get_top_parent()
    if parent is None:
        return False

    if sym == SDLK_q:
        parent.destroy()
        mm.die("User exit request")
        return True

    if sym == SDLK_a:
        parent.destroy()
        return True

    return False

def wid_quit_create():
    global wid_quit_menu

    if wid_quit_menu is None:
        w = wid_popup.WidPopup(name="test",
                               tiles="wid1",
                               width=0.5,
                               height=0.0)
        wid_quit_menu = w

        w.add_text(center=True, 
                   font="vlarge", 
                   color="white", 
                   title="true",
                   text="%%tile=player4$ Quit the game?")
        w.add_text(center=True, 
                   font="vsmall", 
                   color="white", 
                   title="true",
                   text="---")

        w.add_text(font="vlarge", 
                   on_key_down=wid_quit_on_key_down, 
                   text="%%fg=green$a) %%fg=white$quit")

        w.add_text(font="small", 
                   on_key_down=wid_quit_on_key_down, 
                   text="%%fg=green$a) %%fg=white$quit")

        w.add_text(font="small", 
                   on_key_down=wid_quit_on_key_down, 
                   text="%%fg=green$a) %%fg=white$quit")

        w.add_text(font="small", 
                   on_key_down=wid_quit_on_key_down, 
                   text="%%fg=green$b) %%fg=white$nope, keep on going with lots of lots shshiohwo gowhgowhgowh oghwog wog owh gowh roghwoighowhgoiw h qhehghwhgoh odhgoshogshioghsoh iosos ob hsobh osfhboishboish oshf obis foihbsoih boishboishoibsh boshoib sfoshso ")
        w.update()
        w.set_focus()
        w.move_to_pct_centered(x=0.5, y=0.5)
#        w.set_on_mouse_motion(wid_on_mouse_motion_callback)
#        w.set_on_tooltip(wid_on_tooltip_callback)
#        w.set_on_key_down(wid_on_key_down_callback)
#        w.set_on_key_up(wid_on_key_up_callback)
#        w.set_on_joy_button(wid_on_joy_button_callback)
#        w.set_on_mouse_down(wid_on_mouse_down_callback)
#        w.set_on_mouse_focus_begin(wid_on_mouse_focus_begin_callback)
#        w.set_on_mouse_focus_end(wid_on_mouse_focus_end_callback)
#        w.set_on_mouse_over_begin(wid_on_mouse_over_begin_callback)
#        w.set_on_mouse_over_end(wid_on_mouse_over_end_callback)
#        w.set_on_mouse_up(wid_on_mouse_up_callback)
#        w.set_on_destroy(wid_on_destroy_callback)
#        w.set_on_destroy_begin(wid_on_destroy_begin_callback)
#        w.set_on_tick(wid_on_tick_callback)
#        w.set_on_display(wid_on_display_callback)
#        w.set_on_display_top_level(wid_on_display_top_level_callback)
#        w.destroy_in(delay=1000)


def init2():
    load_all_plugins()

    wid_intro_bg_create()
#    wid_intro_menu_create()
    wid_quit_create()

init1()
