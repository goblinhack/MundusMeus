import wid_popup
import wid

wid_mini_map_menu = None


def wid_mini_map_create():
    global wid_mini_map_menu

    if wid_mini_map_menu is not None:
        return

    w = wid_popup.WidPopup(name="mini_map window",
                           tiles="wid1",
                           title_tiles="wid3",
                           body_tiles="wid2",
                           width=0.25,
                           height=0.3)
    wid_mini_map_menu = w

    w.update()
    w.set_focus()
    w.move_to_pct_centered(x=0.875, y=0.15)

    w.add_text(center=True,
               font="vlarge",
               color="white",
               title="true",
               text="Le map")

    mini_map = wid.Wid(parent=w.wid_id, name="intro bg")
    mini_map.set_pos_pct(x1=0.1, y1=0.1, x2=0.9, y2=0.9)
    mini_map.set_tex(name="map")
    mini_map.set_color(tl=True, bg=True, br=True, name="white")


def destroy():
    global wid_mini_map_menu

    if wid_mini_map_menu is None:
        return

    wid_mini_map_menu.destroy()
    wid_mini_map_menu = None
