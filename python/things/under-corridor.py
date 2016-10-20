import tp


def thing_init(t):
    t.set_description("under-corridor")


def under_corridor_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
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
    under_corridor_init(name="under-corridor1", short_name="under-corridor1",
                        tiles=[
                               "under-corridor1.1",
                               "under-corridor1.2",
                               "under-corridor1.3",
                               "under-corridor1.4",
                               "under-corridor1.5",
                               "under-corridor1.6",
                               "under-corridor1.7",
                               "under-corridor1.8",
                               "under-corridor1.9",
                               "under-corridor1.10",
                               "under-corridor1.11",
                               "under-corridor1.12",
                               "under-corridor1.13",
                               "under-corridor1.14",
                               "under-corridor1.15",
                               "under-corridor1.16",
                               "under-corridor1.17",
                               "under-corridor1.18",
                               "under-corridor1.19",
                               "under-corridor1.20",
                               "under-corridor1.21",
                               "under-corridor1.22",
                               "under-corridor1.23",
                               "under-corridor1.24",
                               "under-corridor1.25",
                               "under-corridor1.26",
                               "under-corridor1.27",
                               "under-corridor1.28",
                               "under-corridor1.29",
                               "under-corridor1.30",
                               "under-corridor1.31",
                               "under-corridor1.32",
                              ])

init()
