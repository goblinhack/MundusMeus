import wid


wid_intro_title = None


def create():
    global wid_intro_title

    if wid_intro_title is None:
        w = wid.Wid(name="intro bg")
        wid_intro_title = w

        w.set_pos_pct(x1=0.0, y1=0.1, x2=1.0, y2=0.9)
        w.set_tex(name="main_title")
        w.set_color(tl=True, bg=True, br=True, name="white")
        w.to_back()
        w.update()
        w.set_do_not_raise(value=True)


def destroy():
    if wid_intro_title is not None:
        wid_intro_title.destroy()
