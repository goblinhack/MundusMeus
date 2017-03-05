import tp
import mm


def thing_init(t):
    return


def gravel_snow_deco1_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_GRAVEL)
    x.set_is_gravel_snow_deco(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    gravel_snow_deco1_init(name="gravel_snow_deco",
                           short_name="gravel_snow",
                           tiles=[
                             "gravel_snow_tl",
                             "gravel_snow_top",
                             "gravel_snow_tr",
                             "gravel_snow_left",
                             "gravel_snow_right",
                             "gravel_snow_bl",
                             "gravel_snow_bot",
                             "gravel_snow_br",
                            ])

init()
