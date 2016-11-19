import tp


def thing_init(t):
    return


def under_corridor_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("bridge")
    x.set_is_bridge(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    under_corridor_init(name="under_corridor1",
                        short_name="corridor bedrock",
                        long_name="corridor bedrock",
                        tiles=[
                               "under_corridor1.1",
                               "under_corridor1.2",
                               "under_corridor1.3",
                               "under_corridor1.4",
                               "under_corridor1.5",
                               "under_corridor1.6",
                               "under_corridor1.7",
                               "under_corridor1.8",
                               "under_corridor1.9",
                               "under_corridor1.10",
                               "under_corridor1.11",
                               "under_corridor1.12",
                               "under_corridor1.13",
                               "under_corridor1.14",
                               "under_corridor1.15",
                               "under_corridor1.16",
                               "under_corridor1.17",
                               "under_corridor1.18",
                               "under_corridor1.19",
                               "under_corridor1.20",
                               "under_corridor1.21",
                               "under_corridor1.22",
                               "under_corridor1.23",
                               "under_corridor1.24",
                               "under_corridor1.25",
                               "under_corridor1.26",
                               "under_corridor1.27",
                               "under_corridor1.28",
                               "under_corridor1.29",
                               "under_corridor1.30",
                               "under_corridor1.31",
                               "under_corridor1.32",
                              ])

init()
