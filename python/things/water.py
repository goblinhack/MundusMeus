import tp


def thing_init(t):
    t.set_description("water")


def water_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_water(True)
    x.set_map_depth("floor")
    x.set_is_animated(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=100)
    else:
        x.set_tile(tile=name, delay_ms=100)

    x.thing_init = thing_init

    return x


def init():
    x = water_init(name="water1",         short_name="water1",
                   tiles=[
                          "water1.1",
                          "water1.2",
                          "water1.3",
                          "water1.4",
                          "water1.5",
                          "water1.6",
                          "water1.7",
                          "water1.8",
                          "water1.9",
                          "water1.10",
                          "water1.11",
                          "water1.12",
                          "water1.13",
                          "water1.14",
                          "water1.15",
                          "water1.16",
                          "water1.17",
                          "water1.18",
                          "water1.19",
                          "water1.20",
                          "water1.21",
                          "water1.22",
                          "water1.23",
                          "water1.24",
                          "water1.25",
                          "water1.26",
                          "water1.27",
                          "water1.28",
                          "water1.29",
                          "water1.30",
                          "water1.31",
                          "water1.32",
                          "water1.33",
                          "water1.34",
                          "water1.35",
                          "water1.36",
                          "water1.37",
                          "water1.38",
                          "water1.39",
                          "water1.40",
                          "water1.41",
                          "water1.42",
                          "water1.43",
                          "water1.44",
                          "water1.45",
                          "water1.46",
                          "water1.47",
                          "water1.48",
                          "water1.49",
                          "water1.50",
                          "water1.51",
                          "water1.52",
                          "water1.53",
                          "water1.54",
                          "water1.55",
                          "water1.56",
                          "water1.57",
                          "water1.58",
                          "water1.59",
                          "water1.60",
                          "water1.61",
                          "water1.62",
                          "water1.63",
                          "water1.64",
                     ])
    x.set_blit_bot_off(1)

    x = water_init(name="water1-top",         short_name="water1-top",
                   tiles=[
                          "water1.1",
                          "water1.2",
                          "water1.3",
                          "water1.4",
                          "water1.5",
                          "water1.6",
                          "water1.7",
                          "water1.8",
                          "water1.9",
                          "water1.10",
                          "water1.11",
                          "water1.12",
                          "water1.13",
                          "water1.14",
                          "water1.15",
                          "water1.16",
                          "water1.17",
                          "water1.18",
                          "water1.19",
                          "water1.20",
                          "water1.21",
                          "water1.22",
                          "water1.23",
                          "water1.24",
                          "water1.25",
                          "water1.26",
                          "water1.27",
                          "water1.28",
                          "water1.29",
                          "water1.30",
                          "water1.31",
                          "water1.32",
                          "water1.33",
                          "water1.34",
                          "water1.35",
                          "water1.36",
                          "water1.37",
                          "water1.38",
                          "water1.39",
                          "water1.40",
                          "water1.41",
                          "water1.42",
                          "water1.43",
                          "water1.44",
                          "water1.45",
                          "water1.46",
                          "water1.47",
                          "water1.48",
                          "water1.49",
                          "water1.50",
                          "water1.51",
                          "water1.52",
                          "water1.53",
                          "water1.54",
                          "water1.55",
                          "water1.56",
                          "water1.57",
                          "water1.58",
                          "water1.59",
                          "water1.60",
                          "water1.61",
                          "water1.62",
                          "water1.63",
                          "water1.64",
                     ])
    x.set_blit_bot_off(1)
    x.set_blit_top_off(1)

init()
