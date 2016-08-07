
import mm
import wid

wid_focus = None
wid_focus_parent = None

def set_focus(w, recurse=0):

    global wid_focus
    global wid_focus_parent

    if wid_focus == None:
        wid_focus = wid.Wid(name="wid focus")
        wid_focus.set_tl_br_pct(0, 0, 0.1, 0.05)
        wid_focus.set_color(tl=True, bg=True, br=True, name="white")
        wid_focus.set_tex(name="sword")

    p1 = w.get_parent()
    wid_focus_parent = p1

    p2 = p1.get_parent()
    if p2 != None:
        wid_focus_parent = p2

    ptlx, ptly, pbrx, pbry = p1.get_pos_pct()
    tlx, tly, brx, bry = w.get_pos_pct()
    y = (tly + bry) / 2
    x = tlx - 0.05

    wid_focus.move_to_pct_centered_in(x=x, y=y, delay=50)
    wid_focus.to_front()
    wid_focus.set_do_not_lower(value=True)

    if recurse > 100:
        return

    if p2 != None:
        if tly < ptly:
            p2.scroll_up()
            set_focus(w, recurse + 1)

        if bry > pbry:
            p2.scroll_down()
            set_focus(w, recurse + 1)

def clear_focus(w):

    global wid_focus
    global wid_focus_parent

    if wid_focus == None:
        return

    p = w.get_top_parent()

    if wid_focus_parent == p:
        wid_focus.hide()
        wid_focus_parent = None
