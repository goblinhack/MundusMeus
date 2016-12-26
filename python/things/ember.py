import tp


def thing_init(t):
    return


def ember1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("player")

    x.set_light_radius(1.1)
    x.set_light_tint("gray30")
    x.set_light_pulse_amount(0.002)

    x.thing_init = thing_init

    return x


def init():
    ember1_init(name="ember1",
                short_name="glowing ember",
                long_name="glowing ember",
                tiles=None)

init()
