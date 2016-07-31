from tp import tpp
import mm


def init():
    x = tpp("bigmap")
    x.set_short_name("bigmap")
    x.set_world_depth("land")
    x.set_scale(4.0)

    mm.tp_set_tile(x, tile="bigmap")

init()
