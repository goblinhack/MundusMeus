import tp


def thing_init(t):
    return


def snow_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_snow(True)
    x.set_z_depth("snow1")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    snow_init(name="snow1",
              short_name="snow",
              long_name="snow",
              tiles=[
                     "snow1.1",
                     "snow1.2",
                     "snow1.3",
                     "snow1.4",
                     "snow1.5",
                     "snow1.6",
                     "snow1.7",
                     "snow1.8",
                     "snow1.9",
                     "snow1.10",
                     "snow1.11",
                     "snow1.12",
                     "snow1.13",
                     "snow1.14",
                     "snow1.15",
                     "snow1.16",
                     "snow1.17",
                     "snow1.18",
                     "snow1.19",
                     "snow1.20",
                     "snow1.21",
                     "snow1.22",
                     "snow1.23",
                     "snow1.24",
                    ])

init()
