import tp


def thing_init(t):
    return


def grass_deco1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("grass1")
    x.set_is_grass(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    grass_deco1_init(name="grass1-deco",
                     short_name="grass",
                     long_name="grass",
                     tiles=[
                            "grass1-tl",
                            "grass1-top",
                            "grass1-tr",
                            "grass1-left",
                            "grass1-right",
                            "grass1-bl",
                            "grass1-bot",
                            "grass1-br",
                            ])

init()
