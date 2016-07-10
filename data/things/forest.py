from tp import tpp
import mm

def init():
    x = tpp("forest1")
    x.set_short_name("forest")
    x.set_is_land(True)
    x.set_is_forest(True)
    x.set_world_depth("land")

    mm.tp_set_tile(x, tile="forest1.1")
    mm.tp_set_tile(x, tile="forest1.2")
    mm.tp_set_tile(x, tile="forest1.3")
    mm.tp_set_tile(x, tile="forest1.4")
    mm.tp_set_tile(x, tile="forest1.5")
    mm.tp_set_tile(x, tile="forest1.6")

    mm.tp_set_tile(x, tile="forest2.1")
    mm.tp_set_tile(x, tile="forest2.2")
    mm.tp_set_tile(x, tile="forest2.3")
    mm.tp_set_tile(x, tile="forest2.4")
    mm.tp_set_tile(x, tile="forest2.5")
    mm.tp_set_tile(x, tile="forest2.6")

init()
