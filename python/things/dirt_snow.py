import tp
import mm


def thing_init(t):
    return


def dirt_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_dirt(True)
    x.set_is_dirt_snow(True)
    x.set_z_depth(mm.Z_DEPTH_DIRT)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    dirt_init(name="dirt_snow",
              short_name="Dirty snow",
              tiles=[
                     "dirt_snow.1",
                     "dirt_snow.2",
                     "dirt_snow.3",
                     "dirt_snow.4",
                     "dirt_snow.5",
                     "dirt_snow.6",
                     "dirt_snow.7",
                     "dirt_snow.8",
                     "dirt_snow.9",
                     "dirt_snow.10",
                     "dirt_snow.11",
                     "dirt_snow.12",
                     "dirt_snow.13",
                     "dirt_snow.14",
                     "dirt_snow.15",
                     "dirt_snow.16",
                     "dirt_snow.17",
                     "dirt_snow.18",
                     "dirt_snow.19",
                     "dirt_snow.20",
                     "dirt_snow.21",
                     "dirt_snow.22",
                     "dirt_snow.23",
                     "dirt_snow.24",
                    ])

init()
