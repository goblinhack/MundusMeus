import tp
import mm


def thing_init(t):
    return


def water_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_water(True)
    x.set_z_depth(mm.Z_DEPTH_LAVA)
    x.set_is_dungeon_item(True)
    x.set_is_world_item(True)
    x.set_is_animated(True)

    x.set_light_radius(0.5)
    x.set_light_tint("cyan")
    x.set_light_pulse_amount(0.001)
    x.set_is_candle_light(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=100)
    else:
        x.set_tile(tile=name, delay_ms=100)

    x.thing_init = thing_init

    return x


def init():
    x = water_init(name="water2",
                   short_name="Water",
                   tiles=[
                          "water2.1",
                          "water2.2",
                          "water2.3",
                          "water2.4",
                          "water2.5",
                          "water2.6",
                          "water2.7",
                          "water2.8",
                          "water2.9",
                          "water2.10",
                          "water2.11",
                          "water2.12",
                          "water2.13",
                          "water2.14",
                          "water2.15",
                          "water2.16",
                          "water2.17",
                          "water2.18",
                          "water2.19",
                          "water2.20",
                          "water2.21",
                          "water2.22",
                          "water2.23",
                          "water2.24",
                          "water2.25",
                          "water2.26",
                          "water2.27",
                          "water2.28",
                          "water2.29",
                          "water2.30",
                          "water2.31",
                          "water2.32",
                          "water2.33",
                          "water2.34",
                          "water2.35",
                          "water2.36",
                          "water2.37",
                          "water2.38",
                          "water2.39",
                          "water2.40",
                          "water2.41",
                          "water2.42",
                          "water2.43",
                          "water2.44",
                          "water2.45",
                          "water2.46",
                          "water2.47",
                          "water2.48",
                          "water2.49",
                          "water2.50",
                          "water2.51",
                          "water2.52",
                          "water2.53",
                          "water2.54",
                          "water2.55",
                          "water2.56",
                          "water2.57",
                          "water2.58",
                          "water2.59",
                          "water2.60",
                          "water2.61",
                          "water2.62",
                          "water2.63",
                          "water2.64",
                     ])

    x.set_z_depth(mm.Z_DEPTH_LAVA)

init()
