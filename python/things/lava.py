import tp
import mm


def thing_init(t):
    return


def lava_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_lava(True)
    x.set_z_depth(mm.Z_DEPTH_LAVA)
    x.set_is_dungeon_item(True)
    x.set_is_animated(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=150)
    else:
        x.set_tile(tile=name, delay_ms=150)

    x.thing_init = thing_init

    return x


def init():
    x = lava_init(name="lava1",
                  short_name="Bubbling lava",
                  tiles=[
                         "lava1.1",
                         "lava1.2",
                         "lava1.3",
                         "lava1.4",
                         "lava1.5",
                         "lava1.6",
                         "lava1.7",
                         "lava1.8",
                         "lava1.9",
                         "lava1.10",
                         "lava1.11",
                         "lava1.12",
                         "lava1.13",
                         "lava1.14",
                         "lava1.15",
                         "lava1.16",
                         "lava1.17",
                         "lava1.18",
                         "lava1.19",
                         "lava1.20",
                         "lava1.21",
                         "lava1.22",
                         "lava1.23",
                         "lava1.24",
                         "lava1.25",
                         "lava1.26",
                         "lava1.27",
                         "lava1.28",
                         "lava1.29",
                         "lava1.30",
                         "lava1.31",
                         "lava1.32",
                         "lava1.33",
                         "lava1.34",
                         "lava1.35",
                         "lava1.36",
                         "lava1.37",
                         "lava1.38",
                         "lava1.39",
                         "lava1.40",
                         "lava1.41",
                         "lava1.42",
                         "lava1.43",
                         "lava1.44",
                         "lava1.45",
                         "lava1.46",
                         "lava1.47",
                         "lava1.48",
                         "lava1.49",
                         "lava1.50",
                         "lava1.51",
                         "lava1.52",
                         "lava1.53",
                         "lava1.54",
                         "lava1.55",
                         "lava1.56",
                         "lava1.57",
                         "lava1.58",
                         "lava1.59",
                         "lava1.60",
                         "lava1.61",
                         "lava1.62",
                         "lava1.63",
                         "lava1.64",
                     ])

    x.set_blit_bot_off(2)
    x.set_light_radius(1.5)
    x.set_light_tint("red")
    x.set_light_pulse_amount(0.5)
    x.set_is_candle_light(True)

init()
