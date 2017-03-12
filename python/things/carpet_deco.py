import tp
import mm


def thing_init(t):
    return


def carpet_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_FLOOR2)
    x.set_is_carpet_deco(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    carpet_init(name="carpet1_deco",
                     short_name="carpet",
                     tiles=[
                            "carpet1_tl",
                            "carpet1_top",
                            "carpet1_tr",
                            "carpet1_left",
                            "carpet1_right",
                            "carpet1_bl",
                            "carpet1_bot",
                            "carpet1_br",
                            ])
    carpet_init(name="carpet2_deco",
                     short_name="carpet",
                     tiles=[
                            "carpet2_tl",
                            "carpet2_top",
                            "carpet2_tr",
                            "carpet2_left",
                            "carpet2_right",
                            "carpet2_bl",
                            "carpet2_bot",
                            "carpet2_br",
                            ])
    carpet_init(name="carpet3_deco",
                     short_name="carpet",
                     tiles=[
                            "carpet3_tl",
                            "carpet3_top",
                            "carpet3_tr",
                            "carpet3_left",
                            "carpet3_right",
                            "carpet3_bl",
                            "carpet3_bot",
                            "carpet3_br",
                            ])

init()
