import tp
import mm


def thing_init(t):
    return


def dirt_deco1_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_DIRT)
    x.set_is_dirt_snow_deco(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    dirt_deco1_init(name="dirt_snow_deco",
                    short_name="Dirty snow",
                    tiles=[
                           "dirt_snow_tl",
                           "dirt_snow_top",
                           "dirt_snow_tr",
                           "dirt_snow_left",
                           "dirt_snow_right",
                           "dirt_snow_bl",
                           "dirt_snow_bot",
                           "dirt_snow_br",
                          ])

init()
