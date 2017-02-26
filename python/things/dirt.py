import tp
import mm


def thing_init(t):
    return


def dirt_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_dirt(True)
    x.set_is_solid_ground(True)
    x.set_z_depth(mm.Z_DEPTH_DIRT)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    dirt_init(name="dirt",
              short_name="Dirt",
              tiles=[
                     "dirt.1",
                     "dirt.2",
                     "dirt.3",
                     "dirt.4",
                     "dirt.5",
                     "dirt.6",
                     "dirt.7",
                     "dirt.8",
                     "dirt.9",
                     "dirt.10",
                     "dirt.11",
                     "dirt.12",
                     "dirt.13",
                     "dirt.14",
                     "dirt.15",
                     "dirt.16",
                     "dirt.17",
                     "dirt.18",
                     "dirt.19",
                     "dirt.20",
                     "dirt.21",
                     "dirt.22",
                     "dirt.23",
                     "dirt.24",
                    ])

init()
