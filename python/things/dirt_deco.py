import tp


def thing_init(t):
    return


def dirt_deco1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("dirt")
    x.set_is_dirt_deco(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    dirt_deco1_init(name="dirt_deco",
                    short_name="dirt",
                    long_name="dirt",
                    tiles=[
                           "dirt-tl",
                           "dirt-top",
                           "dirt-tr",
                           "dirt-left",
                           "dirt-right",
                           "dirt-bl",
                           "dirt-bot",
                           "dirt-br",
                          ])

init()
