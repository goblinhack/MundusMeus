import wid_popup


wid_console = None


def create():
    global wid_console

    if wid_console is None:
        w = wid_popup.WidPopup(name="test",
                               x1=0.0, y1=0.0)
        wid_console = w

        w.add_text(text="test1")
        w.add_text(text="test2")
        w.add_text(text="test3")
        w.set_color(bg=True, tl=True, br=True, name="red", alpha=200)
        w.update()


def destroy():
    if wid_console is not None:
        wid_console.destroy()
