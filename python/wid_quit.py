import mm
import wid_popup
import sys
import wid_focus
import game


global mywid


def wid_quit_common(w):
    hide()
    return
    destroy()


def wid_quit_yes(w):
    wid_quit_common(w)
    mm.die("exit game from {0}".format(sys._getframe().f_code.co_name))
    return True


def wid_quit_no(w):
    wid_quit_common(w)
    return True


def wid_quit_on_m_down_yes(w, x, y, button):
    wid_quit_yes(w)
    return True


def wid_quit_on_m_down_no(w, x, y, button):
    wid_quit_no(w)
    return True


def wid_quit_on_key_down_yes(w, sym, mod):
    wid_quit_yes(w)
    return True


def wid_quit_on_key_down_no(w, sym, mod):
    wid_quit_no(w)
    return True


def wid_quit_on_key_down(w, sym, mod):
    if sym == mm.SDLK_y:
        wid_quit_on_key_down_yes(w, sym, mod)
        return True

    if sym == mm.SDLK_n:
        wid_quit_on_key_down_no(w, sym, mod)
        return True

    hide()

    return True


def wid_quit_create():

    global mywid
    w = wid_popup.WidPopup(name="quit window",
                           tiles="wid1",
                           title_tiles="wid3",
                           body_tiles="wid2",
                           row_on_key_down=wid_quit_on_key_down,
                           title_on_key_down=wid_quit_on_key_down,
                           width=0.5,
                           height=0.0)
    mywid = w

    w.add_text(center=True,
               font="vlarge",
               color="white",
               title="true",
               text="%%tp=player1$ Quit the game?")

    w.add_text(font="vlarge",
               on_m_down=wid_quit_on_m_down_yes,
               tooltip="I can't let you do that, Dave...",
               text="%%fg=red$y) %%fg=white$Yep, quit")

    w.add_text(font="vlarge",
               on_m_down=wid_quit_on_m_down_no,
               text="%%fg=green$n) %%fg=white$Nope, persevere")

    w.update()
    w.move_to_pct_centered(x=0.5, y=0.5)
    w.set_focusable(value=-1)
    w.hide()

    return w


def destroy():
    global mywid
    if mywid is None:
        return

    mywid.destroy()
    mywid = None


def visible():
    global mywid
    if mywid is None:
        return

    mywid.toggle_hidden()
    mywid.set_focus()
    mywid.to_front()
    wid_focus.set_focus(mywid)

    mm.tip2("Press Escape to go back")


def hide():
    global mywid
    if mywid is None:
        return

    mywid.hide()

    game.g.map_help()

    return True
