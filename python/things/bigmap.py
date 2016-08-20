import tp
import mm


def init():
    x = tp.Tp("bigmap")
    x.set_short_name("bigmap")
    x.set_world_depth("land")
    x.set_scale(4.0)

    mm.tp_set_tile(x, tile="bigmap")

init()
