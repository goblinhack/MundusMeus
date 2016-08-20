import tp
import mm


def init():
    x = tp.Tp("grass1")
    x.set_short_name("grass")
    x.set_is_land(True)
    x.set_is_grass(True)
    x.set_world_depth("land")

    mm.tp_set_tile(x, tile="grass1.1")

init()
