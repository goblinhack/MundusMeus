import tp
import mm


def thing_init(t):
    return


def gravel_deco1_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_GRAVEL)
    x.set_is_gravel_deco(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    gravel_deco1_init(name="gravel_deco",
                      short_name="Gravel",
                      tiles=[
                             "gravel_tl",
                             "gravel_top",
                             "gravel_tr",
                             "gravel_left",
                             "gravel_right",
                             "gravel_bl",
                             "gravel_bot",
                             "gravel_br",
                            ])

init()
