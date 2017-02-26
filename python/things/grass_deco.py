import tp
import mm


def thing_init(t):
    return


def grass_deco1_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_GRASS)
    x.set_is_grass_deco(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    grass_deco1_init(name="grass_deco",
                     short_name="Grass",
                     tiles=[
                            "grass_tl",
                            "grass_top",
                            "grass_tr",
                            "grass_left",
                            "grass_right",
                            "grass_bl",
                            "grass_bot",
                            "grass_br",
                            ])

init()
