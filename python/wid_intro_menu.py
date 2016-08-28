import wid_popup


wid_intro_menu = None


def create():
    global wid_intro_menu

    if wid_intro_menu is None:
        w = wid_popup.WidPopup(name="test",
                               x1=0.2, y1=0.5)
        wid_intro_menu = w

        w.add_text(text="%%fg=green$a) %%fg=white$new game\n")
        w.set_color(bg=True, tl=True, br=True, name="red", alpha=0)
        w.update()


def destroy():
    if wid_intro_menu is not None:
        wid_intro_menu.destroy()
