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
                    ])

init()
