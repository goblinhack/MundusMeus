import tp


def thing_init(t):
    t.set_description("ember1")


def ember1_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth("player")

    x.set_light_radius(1)
    x.set_light_tint("blue")
    x.set_is_candle_light(True)

    x.thing_init = thing_init

    return x


def init():
    ember1_init(name="ember1", short_name="ember1", tiles=None)

init()
