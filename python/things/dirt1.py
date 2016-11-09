import tp


def thing_init(t):
    return


def dirt_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_dirt(True)
    x.set_z_depth("dirt1")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    dirt_init(name="dirt1",
              short_name="dirt",
              long_name="dirt",
              tiles=[
                     "dirt1.1",
                     "dirt1.2",
                     "dirt1.3",
                     "dirt1.4",
                     "dirt1.5",
                     "dirt1.6",
                     "dirt1.7",
                     "dirt1.8",
                     "dirt1.9",
                     "dirt1.10",
                     "dirt1.11",
                     "dirt1.12",
                     "dirt1.13",
                     "dirt1.14",
                     "dirt1.15",
                     "dirt1.16",
                     "dirt1.17",
                     "dirt1.18",
                     "dirt1.19",
                     "dirt1.20",
                     "dirt1.21",
                     "dirt1.22",
                     "dirt1.23",
                     "dirt1.24",
                    ])

init()
