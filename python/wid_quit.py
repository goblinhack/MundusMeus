import mm
import wid_popup
import sys
import game


def wid_quit_common(w):
    game.wid_quit.hide()
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


def wid_quit_create():
    if game.wid_quit is not None:
        return

    w = wid_popup.WidPopup(name="quit window",
                           tiles="wid1",
                           title_tiles="wid3",
                           body_tiles="wid2",
                           width=0.5,
                           height=0.0)
    game.wid_quit = w

    w.add_text(center=True,
               font="vlarge",
               color="white",
               title="true",
               text="%%tp=player1$Quit the game?")

    w.add_text(font="vlarge",
               on_m_down=wid_quit_on_m_down_yes,
               on_key_down=wid_quit_on_key_down_yes,
               on_key_sym=mm.SDLK_y,
               tooltip="I can't let you do that, Dave...",
               text="%%fg=red$y) %%fg=white$Yep, quit")

    w.add_text(font="vlarge",
               on_m_down=wid_quit_on_m_down_no,
               on_key_down=wid_quit_on_key_down_no,
               on_key_sym=mm.SDLK_n,
               text="%%fg=green$n) %%fg=white$Nope, persevere")

    w.update()
    w.set_focusable(value=-1)
    w.move_to_pct_centered(x=0.5, y=0.5)
    w.hide()

    return w


def destroy():
    if game.wid_quit is None:
        return

    game.wid_quit.destroy()
    game.wid_quit = None
