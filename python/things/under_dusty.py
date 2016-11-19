import tp


def thing_init(t):
    return


def under_dusty_init(name, short_name, long_name, tiles=[]):
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
    under_dusty_init(name="under_dusty1",
                     short_name="aged bedrock",
                     long_name="aged bedrock",
                     tiles=[
                            "under_dusty1.1",
                            "under_dusty1.2",
                            "under_dusty1.3",
                            "under_dusty1.4",
                            "under_dusty1.5",
                            "under_dusty1.6",
                            "under_dusty1.7",
                            "under_dusty1.8",
                            "under_dusty1.9",
                            "under_dusty1.10",
                            "under_dusty1.11",
                            "under_dusty1.12",
                            "under_dusty1.13",
                            "under_dusty1.14",
                            "under_dusty1.15",
                            "under_dusty1.16",
                            "under_dusty1.17",
                            "under_dusty1.18",
                            "under_dusty1.19",
                            "under_dusty1.20",
                            "under_dusty1.21",
                            "under_dusty1.22",
                            "under_dusty1.23",
                            "under_dusty1.24",
                            "under_dusty1.25",
                            "under_dusty1.26",
                            "under_dusty1.27",
                            "under_dusty1.28",
                            "under_dusty1.29",
                            "under_dusty1.30",
                            "under_dusty1.31",
                            "under_dusty1.32",
                           ])

init()
