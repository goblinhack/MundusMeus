import tp


def thing_init(t):
    return


def ice_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_ice(True)
    x.set_z_depth("ice1")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    ice_init(name="ice1",
              short_name="ice",
              long_name="ice",
              tiles=[
                     "ice1.1",
                     "ice1.2",
                     "ice1.3",
                     "ice1.4",
                     "ice1.5",
                     "ice1.6",
                     "ice1.7",
                     "ice1.8",
                     "ice1.9",
                     "ice1.10",
                     "ice1.11",
                     "ice1.12",
                     "ice1.13",
                     "ice1.14",
                     "ice1.15",
                     "ice1.16",
                     "ice1.17",
                     "ice1.18",
                     "ice1.19",
                     "ice1.20",
                     "ice1.21",
                     "ice1.22",
                     "ice1.23",
                     "ice1.24",
                    ])

init()
