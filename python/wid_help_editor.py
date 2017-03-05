import wid_popup
import wid_focus
import game


global mywid


def wid_help_editor_on_key_down(w, sym, mod):
    hide()
    return True


def wid_help_editor_on_m_down(w, x, y, button):
    hide()
    return True


def wid_help_editor_create():

    global mywid
    w = wid_popup.WidPopup(name="help_editor window",
                           tiles="wid1",
                           title_tiles="wid3",
                           body_tiles="wid2",
                           row_on_key_down=wid_help_editor_on_key_down,
                           title_on_key_down=wid_help_editor_on_key_down,
                           row_on_m_down=wid_help_editor_on_m_down,
                           title_on_m_down=wid_help_editor_on_m_down,
                           width=0.5,
                           height=0.6)
    mywid = w

    w.add_text(center=True,
               font="vlarge",
               color="white",
               title="true",
               text="%%tp=player1$ Editor keys")

    w.add_text(font="vlarge",
               text="%%fg=green$d) %%fg=white$Draw mode")

    w.add_text(font="vlarge",
               text="%%fg=green$x) %%fg=white$Change to erase mode")

    w.add_text(font="vlarge",
               text="%%fg=green$y) %%fg=white$Yank tile under mouse")

    w.add_text(font="vlarge",
               text="%%fg=green$+) %%fg=white$Raise floor or corridor tile")

    w.add_text(font="vlarge",
               text="%%fg=green$-) %%fg=white$Lower floor or corridor tile")

    w.add_text(font="vlarge",
               text="%%fg=green$s) %%fg=white$Save current chunks")

    w.add_text(font="vlarge",
               text="%%fg=green$CTRL f) %%fg=white$Flood fill")

    w.add_text(font="vlarge",
               text="%%fg=green$CTRL z) %%fg=white$Erase whole level")

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


def hide():
    global mywid
    if mywid is None:
        return

    mywid.hide()

    game.g.map_help()

    return True
