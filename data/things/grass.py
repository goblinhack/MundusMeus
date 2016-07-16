from tp import tpp
import mm


def init():
    x = tpp("grass1")
    x.set_short_name("grass")
    x.set_is_land(True)
    x.set_is_grass(True)
    x.set_world_depth("land")

    mm.tp_set_tile(x, tile="grass1.1")
    mm.tp_set_tile(x, tile="grass1.2")
    mm.tp_set_tile(x, tile="grass1.3")
    mm.tp_set_tile(x, tile="grass1.4")
    mm.tp_set_tile(x, tile="grass1.5")
    mm.tp_set_tile(x, tile="grass1.6")
    mm.tp_set_tile(x, tile="grass1.7")
    mm.tp_set_tile(x, tile="grass1.8")
    mm.tp_set_tile(x, tile="grass1.9")
    mm.tp_set_tile(x, tile="grass1.10")
    mm.tp_set_tile(x, tile="grass1.11")
    mm.tp_set_tile(x, tile="grass1.12")
    mm.tp_set_tile(x, tile="grass1.13")
    mm.tp_set_tile(x, tile="grass1.14")
    mm.tp_set_tile(x, tile="grass1.15")
    mm.tp_set_tile(x, tile="grass1.16")
    mm.tp_set_tile(x, tile="grass1.17")
    mm.tp_set_tile(x, tile="grass1.18")
    mm.tp_set_tile(x, tile="grass1.19")
    mm.tp_set_tile(x, tile="grass1.20")

init()
