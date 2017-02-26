import tp
import mm


def thing_init(t):
    return


def ember1_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_PLAYER)

    x.set_light_radius(1.0)
    x.set_light_tint("orange")
    x.set_is_candle_light(True)
    x.set_is_hidden_from_editor(True)

    x.thing_init = thing_init

    return x


def init():
    ember1_init(name="ember1",
                short_name="Glowing ember",
                tiles=None)

init()
