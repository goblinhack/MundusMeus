import wid_popup
import game


def wid_help_on_key_down(w, sym, mod):
    game.wid_help.hide()
    return True


def wid_help_on_m_down(w, x, y, button):
    game.wid_help.hide()
    return True


def wid_help_create():

    w = wid_popup.WidPopup(name="help window",
                           tiles="wid1",
                           title_tiles="wid3",
                           body_tiles="wid2",
                           row_on_key_down=wid_help_on_key_down,
                           title_on_key_down=wid_help_on_key_down,
                           row_on_m_down=wid_help_on_m_down,
                           title_on_m_down=wid_help_on_m_down,
                           width=0.5,
                           height=0.5)
    game.wid_help = w

    w.add_text(center=True,
               font="vlarge",
               color="white",
               title="true",
               text="%%tp=player1$Help of a Dubious Nature")

    w.add_text(font="vlarge",
               text="%%fg=green$q) %%fg=white$Quit the game")

    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="long message")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")
    w.add_text(font="vlarge", center=True, text="--")

    w.add_text(font="vlarge",
               text="%%fg=green$^e) %%fg=white$Editor mode")

    w.update()
    w.set_focusable(value=-1)
    w.move_to_pct_centered(x=0.5, y=0.5)
    w.hide()

    return w


def destroy():
    if game.wid_help is None:
        return

    game.wid_help.destroy()
    game.wid_help = None
