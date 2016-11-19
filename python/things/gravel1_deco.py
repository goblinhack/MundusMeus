import tp


def thing_init(t):
    return


def gravel_deco1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("gravel1")
    x.set_is_gravel(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    gravel_deco1_init(name="gravel1_deco",
                      short_name="gravel",
                      long_name="gravel",
                      tiles=[
                             "gravel1-tl",
                             "gravel1-top",
                             "gravel1-tr",
                             "gravel1-left",
                             "gravel1-right",
                             "gravel1-bl",
                             "gravel1-bot",
                             "gravel1-br",
                            ])

init()
