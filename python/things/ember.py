import tp


def thing_init(t):
    return


def ember1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("player")

    x.set_light_radius(1.0)
    x.set_light_tint("orange")
    x.set_is_candle_light(True)

    x.thing_init = thing_init

    return x


def init():
    ember1_init(name="ember1",
                short_name="glowing ember",
                long_name="glowing ember",
                tiles=None)

init()
