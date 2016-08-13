import mm
import wid
import wid_popup
import sys

wid_quit_menu = None

def wid_quit_common():
    destroy()

def wid_quit_yes():
    wid_quit_common()
    mm.die("exit game from {0}".format(sys._getframe().f_code.co_name))
    return False

def wid_quit_no():
    wid_quit_common()
    return False

def wid_quit_on_mouse_down_yes(w, x, y, button):
    wid_quit_yes()
    return False

def wid_quit_on_mouse_down_no(w, x, y, button):
    wid_quit_no()
    return False

def wid_quit_on_key_down_yes(w, sym, mod):
    wid_quit_yes()
    return True

def wid_quit_on_key_down_no(w, sym, mod):
    wid_quit_no()
    return False # so focus does not select us after we are dead!

def wid_quit_create():
    global wid_quit_menu

    if wid_quit_menu is not None:
        return

    w = wid_popup.WidPopup(name="quit window",
                           tiles="wid1",
                           title_tiles="wid3",
                           body_tiles="wid2",
                           width=0.5,
                           height=0.0)
    wid_quit_menu = w

    button_events=( 
            { 
                "on_mouse_down":wid_quit_on_mouse_down_yes, 
                "tiles":"button1", 
            },
            { 
                "on_mouse_down":wid_quit_on_mouse_down_yes, 
                "tiles":"button2", 
            },
        )

    w.add_text(center=True, 
               font="vlarge", 
               color="white", 
               title="true",
               text="%%tile=player4$  [a] [b] [c] [d] Quit the game?",
               on_button_list=button_events)

    w.add_text(center=True, 
               font="vlarge", 
               color="white", 
               title="true",
               text="%%tile=player4$Quit the game?")

    w.add_text(font="vlarge", 
               on_mouse_down=wid_quit_on_mouse_down_yes,
               on_key_down=wid_quit_on_key_down_yes,
               on_key_sym=mm.SDLK_y,
               text="%%fg=red$y) %%fg=white$Yep, quit")

    w.add_text(font="vlarge", 
               on_mouse_down=wid_quit_on_mouse_down_no,
               on_key_down=wid_quit_on_key_down_no,
               on_key_sym=mm.SDLK_n,
               text="%%fg=green$n) %%fg=white$Nope, persevere")

    w.update()
    w.set_focus()
    w.move_to_pct_centered(x=0.5, y=0.5)

def destroy():
    global wid_quit_menu

    if wid_quit_menu is None:
        return

    wid_quit_menu.destroy()
    wid_quit_menu = None
