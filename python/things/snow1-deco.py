import tp


def thing_init(t):
    return


def snow_deco1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("snow1")
    x.set_is_snow(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    snow_deco1_init(name="snow1-deco",
                    short_name="snow",
                    long_name="snow",
                    tiles=[
                           "snow1-tl",
                           "snow1-top",
                           "snow1-tr",
                           "snow1-left",
                           "snow1-right",
                           "snow1-bl",
                           "snow1-bot",
                           "snow1-br",
                          ])

init()
