import tp
import mm


def init():
    x = tp.Tp("grass1")
    x.set_short_name("grass")
    x.set_is_land(True)
    x.set_is_grass(True)
    x.set_world_depth("land")

    x.set_tile(tile="grass1.1")

init()
