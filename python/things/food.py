import tp
import mm


def thing_init(t):
    return


def food_init(name, short_name, long_name):
    x = tp.Tp(name)
    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_is_food(True)
    x.set_z_depth(mm.Z_DEPTH_TREASURE)
    x.set_tile(tile=name)
    x.set_has_shadow(True)

    x.thing_init = thing_init


def init():
    food_init(name="carrot",
              short_name="Juicy carrot",
              long_name="Succulence of such orangeness. A fine carrot")

init()
