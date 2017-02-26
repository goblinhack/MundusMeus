import tp
import mm


def thing_init(t):
    return


def snow_deco1_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_SNOW)
    x.set_is_snow_deco(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    snow_deco1_init(name="snow_deco",
                    short_name="Snow",
                    tiles=[
                           "snow_tl",
                           "snow_top",
                           "snow_tr",
                           "snow_left",
                           "snow_right",
                           "snow_bl",
                           "snow_bot",
                           "snow_br",
                          ])

init()
