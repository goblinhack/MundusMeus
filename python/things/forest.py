import tp
import mm


def init():
    x = tp.Tp("forest1")
    x.set_short_name("forest")
    x.set_is_land(True)
    x.set_is_forest(True)
    x.set_world_depth("land")

    x.set_tile(tile="forest1.1")

init()
