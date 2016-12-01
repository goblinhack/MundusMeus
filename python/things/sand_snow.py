import tp


def thing_init(t):
    return


def sand_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_sand(True)
    x.set_z_depth("sand")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    sand_init(name="sand_snow",
              short_name="sand",
              long_name="sand",
              tiles=[
                     "sand_snow.1",
                     "sand_snow.2",
                     "sand_snow.3",
                     "sand_snow.4",
                     "sand_snow.5",
                     "sand_snow.6",
                     "sand_snow.7",
                     "sand_snow.8",
                     "sand_snow.9",
                     "sand_snow.10",
                     "sand_snow.11",
                     "sand_snow.12",
                     "sand_snow.13",
                     "sand_snow.14",
                     "sand_snow.15",
                     "sand_snow.16",
                     "sand_snow.17",
                     "sand_snow.18",
                     "sand_snow.19",
                     "sand_snow.20",
                     "sand_snow.21",
                     "sand_snow.22",
                     "sand_snow.23",
                     "sand_snow.24",
                    ])

init()
