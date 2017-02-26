import tp
import mm


def thing_init(t):
    return


def grass_deco1_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_GRASS)
    x.set_is_grass_snow_deco(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    grass_deco1_init(name="grass_snow_deco",
                     short_name="Grass",
                     tiles=[
                            "grass_snow_tl",
                            "grass_snow_top",
                            "grass_snow_tr",
                            "grass_snow_left",
                            "grass_snow_right",
                            "grass_snow_bl",
                            "grass_snow_bot",
                            "grass_snow_br",
                            ])

init()
