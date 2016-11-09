import tp


def thing_init(t):
    return


def sand_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_sand(True)
    x.set_z_depth("sand1")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    sand_init(name="sand1",
              short_name="sand",
              long_name="sand",
              tiles=[
                     "sand1.1",
                     "sand1.2",
                     "sand1.3",
                     "sand1.4",
                     "sand1.5",
                     "sand1.6",
                     "sand1.7",
                     "sand1.8",
                     "sand1.9",
                     "sand1.10",
                     "sand1.11",
                     "sand1.12",
                     "sand1.13",
                     "sand1.14",
                     "sand1.15",
                     "sand1.16",
                     "sand1.17",
                     "sand1.18",
                     "sand1.19",
                     "sand1.20",
                     "sand1.21",
                     "sand1.22",
                     "sand1.23",
                     "sand1.24",
                    ])

init()
