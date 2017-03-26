import mm
import wid
import game


def wid_map_key_down(w, sym, mod):
    return game.map_key_down(w, sym, mod)


class WidMap:

    def __init__(self, width, height):

        self.wid_map = wid.Wid(name="wid_game_map")
        w = self.wid_map

        w.to_back()
        w.set_movable(value=False)
        w.set_do_not_raise(value=True)
        w.set_shape(none=True)
        w.set_pos_pct(x1=-0.0, y1=0.0, x2=0.85, y2=1.0)
        w.set_on_key_down(wid_map_key_down)
        w.set_color(tl=True, bg=True, br=True, name="white", alpha=0.0)

        #
        # Get the tile size for the screen
        #
        self.TILES_SCREEN_WIDTH = mm.TILES_SCREEN_WIDTH
        self.TILES_SCREEN_HEIGHT = mm.TILES_SCREEN_HEIGHT

        self.tile_width_pct, self.tile_height_pct = \
            mm.tile_size_pct(name="player1.1")

        self.wid_map_width, self.wid_map_height = w.get_size()

        self.tile_width = \
            (1.0 / self.TILES_SCREEN_WIDTH) * self.wid_map_width
        self.tile_height = \
            (1.0 / self.TILES_SCREEN_HEIGHT) * self.wid_map_height

        mm.game_tile_width = int(self.tile_width)
        mm.game_tile_height = int(self.tile_height)

        self.wid_grid = wid.Wid(is_grid=True,
                                parent=self.wid_map.wid_id,
                                name="wid_game_map_grid",
                                across=width,
                                down=height,
                                tile_width=self.tile_width,
                                tile_height=self.tile_height)

        self.wid_vert_scroll = wid.Wid(name="wid_game_vert_scroll",
                                       is_scrollbar=True,
                                       vert=True,
                                       parent=self.wid_map.wid_id,
                                       owner=self.wid_grid.wid_id)

        self.wid_horiz_scroll = wid.Wid(name="wid_game_horiz_scroll",
                                        is_scrollbar=True,
                                        horiz=True,
                                        parent=self.wid_map.wid_id,
                                        owner=self.wid_grid.wid_id)

        #
        # Stop the user scrolling around beyond where they can see
        #
        self.wid_horiz_scroll.set_movable_no_user_scroll(value=True)
        self.wid_vert_scroll.set_movable_no_user_scroll(value=True)

        self.wid_horiz_scroll.visible()
        self.wid_vert_scroll.visible()

        self.wid_grid.visible()

        self.wid_vert_scroll.update()
        self.wid_horiz_scroll.update()

    def destroy(self):

        mm.game_map_cleanup()

        if self.wid_map is not None:
            self.wid_map.destroy()
            self.wid_map = None

        if self.wid_grid is not None:
            self.wid_grid.destroy()
            self.wid_grid = None

        if self.wid_vert_scroll is not None:
            self.wid_vert_scroll.destroy()
            self.wid_vert_scroll = None

        if self.wid_horiz_scroll is not None:
            self.wid_horiz_scroll.destroy()
            self.wid_horiz_scroll = None
