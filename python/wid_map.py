import mm
import wid


def wid_map_key_down(w, sym, mod):
    if sym == ord('`'):
        return False
    return True


class WidMap:

    def __init__(self, width, height):

        self.wid = wid.Wid(name="wid game map")
        w = self.wid

        w.to_back()
        w.set_movable(value=False)
        w.set_do_not_raise(value=True)
        w.set_do_not_raise(value=False)
        w.set_shape(none=True)
        w.set_pos_pct(x1=0.0, y1=0.0, x2=1.0, y2=1.0)
        w.set_on_key_down(wid_map_key_down)

        #
        # Get the tile size for the screen
        #
        self.TILES_SCREEN_WIDTH = 20
        self.TILES_SCREEN_HEIGHT = 10

        self.tile_width_pct, self.tile_height_pct = \
            mm.tile_size_pct(name="player1")

        self.wid_map_width, self.wid_map_height = w.get_size()

        self.tile_width = \
            (1.0 / self.TILES_SCREEN_WIDTH) * self.wid_map_width
        self.tile_height = \
            (1.0 / self.TILES_SCREEN_HEIGHT) * self.wid_map_height

        print(self.tile_width)
        print(self.tile_height)
