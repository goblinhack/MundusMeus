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

    container = wid.Wid(parent=w.wid_id, name="intro bg")
    container.set_pos_pct(x1=0.05, y1=0.05, x2=0.95, y2=0.95)
    container.set_color(tl=True, bg=True, br=True, name="white")

    wid_vert_scroll = wid.Wid(name="wid_mini_map_vert_scroll",
                              is_scrollbar=True,
                              vert=True,
                              parent=w.wid_id,
                              owner=container.wid_id)

    wid_horiz_scroll = wid.Wid(name="wid_mini_map_horiz_scroll",
                               is_scrollbar=True,
                               horiz=True,
                               parent=w.wid_id,
                               owner=container.wid_id)

    mini_map = wid.Wid(parent=container.wid_id, name="intro bg")
    mini_map.set_pos_pct(x1=0, y1=0, x2=10, y2=10)
    mini_map.set_tex(name="map")
    mini_map.set_color(tl=True, bg=True, br=True, name="white")

    wid_horiz_scroll.visible()
    wid_vert_scroll.visible()

    wid_vert_scroll.update()
    wid_horiz_scroll.update()


def destroy():
    global wid_mini_map_menu

    if wid_mini_map_menu is None:
        return

    wid_mini_map_menu.destroy()
    wid_mini_map_menu = None


def player_location_update(game):

    l = game.level
    g = game

    if g.wid_player_location:
        g.wid_player_location.destroy()

    g.wid_player_location = wid_popup.WidPopup(
            name="wid_player_location",
            width=0.2)
    w = g.wid_player_location

    text = ""

    text += "%%fg=white$Hour%%fg=green${0}%%fg=reset$\n".format(
            g.hour_str)

    text += "%%fg=white$Day%%fg=green${0}%%fg=reset$\n".format(
            g.day)

    text += "%%fg=white$Latitude %%fg=green${0}%%fg=reset$\n".format(
            l.where.x)

    text += "%%fg=white$Longitude %%fg=green${0}%%fg=reset$\n".format(
            l.where.y)

    text += "%%fg=white$Move\t\t%%fg=green${0}%%fg=reset$\n".format(
            g.move_count)

    if l.where.z < 0:
        text += "Depth %%fg=green${0} feet%%fg=reset\n".format(
                -l.where.z * 10)

#        player = g.player
#        text += "@ %%fg=green${0},{1}%%fg=reset ".format(player.x, player.y)
#        text += "offset %%fg=green${0},{1}%%fg=reset ".format(player.offset_x,
#                                                              player.offset_y)
#        text += "chunk %%fg=green${0}%%fg=reset ".format(player.chunk)
#
    w.add_text(font="fixed", text=text)

    w.set_color(bg=True, tl=True, br=True, name="blue", alpha=0.0)
    w.update()
    w.move_to_pct_centered(x=0.865, y=0.105)
    w.set_movable(value=False)
    w.set_ignore_events(value=True)
