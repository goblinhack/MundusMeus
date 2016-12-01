import tp


def thing_init(t):
    return


def dirt_deco1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("dirt")
    x.set_is_dirt(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    dirt_deco1_init(name="dirt_snow_deco",
                    short_name="dirt",
                    long_name="dirt",
                    tiles=[
                           "dirt_snow-tl",
                           "dirt_snow-top",
                           "dirt_snow-tr",
                           "dirt_snow-left",
                           "dirt_snow-right",
                           "dirt_snow-bl",
                           "dirt_snow-bot",
                           "dirt_snow-br",
                          ])

init()
