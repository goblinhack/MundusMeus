import tp
import mm


def init():
    x = tp.Tp("forest1")
    x.set_short_name("forest")
    x.set_is_land(True)
    x.set_is_forest(True)
    x.set_world_depth("land")

    mm.tp_set_tile(x, tile="forest1.1")

init()
