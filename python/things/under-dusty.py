import tp


def thing_init(t):
    t.set_description("under-dusty")


def under_dusty_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth("lava")
    x.set_is_bridge(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    under_dusty_init(name="under-dusty1", short_name="under-dusty1",
                     tiles=[
                            "under-dusty1.1",
                            "under-dusty1.2",
                            "under-dusty1.3",
                            "under-dusty1.4",
                            "under-dusty1.5",
                            "under-dusty1.6",
                            "under-dusty1.7",
                            "under-dusty1.8",
                            "under-dusty1.9",
                            "under-dusty1.10",
                            "under-dusty1.11",
                            "under-dusty1.12",
                            "under-dusty1.13",
                            "under-dusty1.14",
                            "under-dusty1.15",
                            "under-dusty1.16",
                            "under-dusty1.17",
                            "under-dusty1.18",
                            "under-dusty1.19",
                            "under-dusty1.20",
                            "under-dusty1.21",
                            "under-dusty1.22",
                            "under-dusty1.23",
                            "under-dusty1.24",
                            "under-dusty1.25",
                            "under-dusty1.26",
                            "under-dusty1.27",
                            "under-dusty1.28",
                            "under-dusty1.29",
                            "under-dusty1.30",
                            "under-dusty1.31",
                            "under-dusty1.32",
                           ])

init()
