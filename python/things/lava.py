import tp


def thing_init(t):
    t.set_description("lava")


def lava_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_lava(True)
    x.set_z_depth("lava")
    x.set_is_animated(True)

    x.set_light_radius(0.7)
    x.set_light_tint("red")
    x.set_light_pulse_amount(0.5)
    x.set_is_candle_light(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=150)
    else:
        x.set_tile(tile=name, delay_ms=150)

    x.thing_init = thing_init

    return x


def init():
    x = lava_init(name="lava1",         short_name="lava1",
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
    x.set_blit_bot_off(1)

    x = lava_init(name="lava1-top",         short_name="lava1-top",
                  tiles=[
                         "toplava1.1",
                         "toplava1.2",
                         "toplava1.3",
                         "toplava1.4",
                         "toplava1.5",
                         "toplava1.6",
                         "toplava1.7",
                         "toplava1.8",
                         "toplava1.9",
                         "toplava1.10",
                         "toplava1.11",
                         "toplava1.12",
                         "toplava1.13",
                         "toplava1.14",
                         "toplava1.15",
                         "toplava1.16",
                         "toplava1.17",
                         "toplava1.18",
                         "toplava1.19",
                         "toplava1.20",
                         "toplava1.21",
                         "toplava1.22",
                         "toplava1.23",
                         "toplava1.24",
                         "toplava1.25",
                         "toplava1.26",
                         "toplava1.27",
                         "toplava1.28",
                         "toplava1.29",
                         "toplava1.30",
                         "toplava1.31",
                         "toplava1.32",
                         "toplava1.33",
                         "toplava1.34",
                         "toplava1.35",
                         "toplava1.36",
                         "toplava1.37",
                         "toplava1.38",
                         "toplava1.39",
                         "toplava1.40",
                         "toplava1.41",
                         "toplava1.42",
                         "toplava1.43",
                         "toplava1.44",
                         "toplava1.45",
                         "toplava1.46",
                         "toplava1.47",
                         "toplava1.48",
                         "toplava1.49",
                         "toplava1.50",
                         "toplava1.51",
                         "toplava1.52",
                         "toplava1.53",
                         "toplava1.54",
                         "toplava1.55",
                         "toplava1.56",
                         "toplava1.57",
                         "toplava1.58",
                         "toplava1.59",
                         "toplava1.60",
                         "toplava1.61",
                         "toplava1.62",
                         "toplava1.63",
                         "toplava1.64",
                     ])
    x.set_blit_bot_off(1)
    x.set_blit_top_off(1)

init()
