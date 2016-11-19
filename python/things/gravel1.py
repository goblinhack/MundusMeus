import tp


def thing_init(t):
    return


def gravel_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_gravel(True)
    x.set_z_depth("gravel1")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    gravel_init(name="gravel1",
                short_name="gravel",
                long_name="gravel",
                tiles=[
                       "gravel1.1",
                       "gravel1.2",
                       "gravel1.3",
                       "gravel1.4",
                       "gravel1.5",
                       "gravel1.6",
                       "gravel1.7",
                       "gravel1.8",
                       "gravel1.9",
                       "gravel1.10",
                       "gravel1.11",
                       "gravel1.12",
                       "gravel1.13",
                       "gravel1.14",
                       "gravel1.15",
                       "gravel1.16",
                       "gravel1.17",
                       "gravel1.18",
                       "gravel1.19",
                       "gravel1.20",
                       "gravel1.21",
                       "gravel1.22",
                       "gravel1.23",
                       "gravel1.24",
                       "gravel1.25",
                       "gravel1.26",
                       "gravel1.27",
                       "gravel1.28",
                       "gravel1.29",
                       "gravel1.30",
                       "gravel1.31",
                       "gravel1.32",
                       "gravel1.33",
                       "gravel1.34",
                       "gravel1.35",
                       "gravel1.36",
                       "gravel1.37",
                       "gravel1.38",
                       "gravel1.39",
                       "gravel1.40",
                       "gravel1.41",
                       "gravel1.42",
                       "gravel1.43",
                       "gravel1.44",
                       "gravel1.45",
                       "gravel1.46",
                       "gravel1.47",
                       "gravel1.48",
                       "gravel1.49",
                       "gravel1.50",
                       "gravel1.51",
                       "gravel1.52",
                       "gravel1.53",
                       "gravel1.54",
                       "gravel1.55",
                       "gravel1.56",
                       "gravel1.57",
                       "gravel1.58",
                       "gravel1.59",
                       "gravel1.60",
                       "gravel1.61",
                       "gravel1.62",
                       "gravel1.63",
                       "gravel1.64",
                       "gravel1.65",
                       "gravel1.66",
                       "gravel1.67",
                       "gravel1.68",
                       "gravel1.69",
                       "gravel1.70",
                      ])

init()
