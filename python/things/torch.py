import tp
import mm


def thing_init(t):
    return


def torch1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_TREASURE)
    x.set_is_dungeon_item(True)
    x.set_is_animated(True)

    x.set_light_radius(4)
    x.set_light_tint("orange")
    x.set_is_candle_light(True)
    x.set_has_shadow(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    x = torch1_init(name="torch1",
                    short_name="Torch",
                    long_name="Flickering torch. " +
                    "Stare into the flames. See your doom. " +
                    "And flickery orange stuff.",
                    tiles=[
                           "torch1.1",
                           "torch1.2",
                           "torch1.3",
                           "torch1.4",
                           "torch1.5",
                           "torch1.6",
                           "torch1.7",
                           "torch1.8",
                           "torch1.9",
                           "torch1.10",
                           "torch1.11",
                           "torch1.12",
                           "torch1.13",
                           "torch1.14",
                           "torch1.15",
                          ])
    x.set_blit_top_off(1)

init()
