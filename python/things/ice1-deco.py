import tp


def thing_init(t):
    return


def ice_deco1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("ice1")
    x.set_is_ice(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    ice_deco1_init(name="ice1-deco",
                    short_name="ice",
                    long_name="ice",
                    tiles=[
                           "ice1-tl",
                           "ice1-top",
                           "ice1-tr",
                           "ice1-left",
                           "ice1-right",
                           "ice1-bl",
                           "ice1-bot",
                           "ice1-br",
                          ])

init()
