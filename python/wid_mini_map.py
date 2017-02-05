import wid_popup
import wid
import mm

ctx = None


class MiniMap(object):

    def __init__(self):
        self.wid_mini_map_menu = None
        self.hscroll = None
        self.vscroll = None
        self.map_scale = 8
        self.wid_location = None

    def create(self):
        w = wid_popup.WidPopup(name="mini_map window",
                               tiles="wid1",
                               title_tiles="wid3",
                               body_tiles="wid2",
                               width=0.25,
                               height=0.3,
                               x1=0.75,
                               y1=0.00)

        self.wid_mini_map_menu = w

        w.update()
        w.set_focus()

        container = wid.Wid(parent=w.wid_id, name="intro bg")
        container.set_pos_pct(x1=0.05, y1=0.05, x2=0.95, y2=0.95)
        container.set_color(tl=True, bg=True, br=True, name="white")

        self.vscroll = wid.Wid(name="wid_mini_map_vert_scroll",
                               is_scrollbar=True,
                               vert=True,
                               parent=w.wid_id,
                               owner=container.wid_id)

        self.hscroll = wid.Wid(name="wid_mini_map_horiz_scroll",
                               is_scrollbar=True,
                               horiz=True,
                               parent=w.wid_id,
                               owner=container.wid_id)

        mini_map = wid.Wid(parent=container.wid_id, name="intro bg")
        mini_map.set_pos_pct(x1=0, y1=0,
                             x2=self.map_scale,
                             y2=self.map_scale)

        mini_map.set_tex(name="map")
        mini_map.set_color(tl=True, bg=True, br=True, name="white")

        self.hscroll.visible()
        self.vscroll.visible()

        self.hscroll.update()
        self.vscroll.update()

#    self.hscroll.move_to_pct_in(x=1.0, y=0.0, delay=4000)
#    self.vscroll.move_to_pct_in(x=0.0, y=1.0, delay=4000)

    def destroy(self):
        self.wid_mini_map_menu.destroy()

    def update(self, game):
        l = game.level
        g = game

        if self.wid_location:
            self.wid_location.destroy()

        self.wid_location = wid_popup.WidPopup(
                                        name="wid_location",
                                        width=0.2)
        w = self.wid_location

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

        w.add_text(font="fixed", text=text)

        w.set_color(bg=True, tl=True, br=True, name="blue", alpha=0.0)
        w.update()
        w.move_to_pct_centered(x=0.865, y=0.105)
        w.set_movable(value=False)
        w.set_ignore_events(value=True)

        x = l.where.x / mm.WORLD_WIDTH
        y = l.where.y / mm.WORLD_HEIGHT

        x -= (1.0 / self.map_scale) / 2
        y -= (1.0 / self.map_scale) / 2

        self.hscroll.move_to_pct_in(x=x, y=0.0, delay=1000)
        self.vscroll.move_to_pct_in(x=0.0, y=y, delay=1000)


def create():
    global ctx
    if ctx is not None:
        return
    ctx = MiniMap()
    ctx.create()


def destroy(self):
    global ctx
    if ctx is None:
        return
    ctx.destroy()
    ctx = None


def update(game):
    global ctx
    if ctx is None:
        return
    ctx.update(game)
