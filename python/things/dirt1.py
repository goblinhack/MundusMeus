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
                    ])

init()
