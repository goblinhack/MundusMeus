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
                      ])

init()
