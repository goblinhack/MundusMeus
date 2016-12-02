import tp


def thing_init(t):
    return


def grass_deco1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("grass")
    x.set_is_grass(True)
    x.set_is_grass_snow(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    grass_deco1_init(name="grass_snow_deco",
                     short_name="grass",
                     long_name="grass",
                     tiles=[
                            "grass_snow-tl",
                            "grass_snow-top",
                            "grass_snow-tr",
                            "grass_snow-left",
                            "grass_snow-right",
                            "grass_snow-bl",
                            "grass_snow-bot",
                            "grass_snow-br",
                            ])

init()
