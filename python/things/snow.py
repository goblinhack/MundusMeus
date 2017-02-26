import tp
import mm


def thing_init(t):
    return


def snow_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_snow(True)
    x.set_is_solid_ground(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_SNOW)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    snow_init(name="snow",
              short_name="Snow",
              tiles=[
                     "snow.1",
                     "snow.2",
                     "snow.3",
                     "snow.4",
                     "snow.5",
                     "snow.6",
                     "snow.7",
                     "snow.8",
                     "snow.9",
                     "snow.10",
                     "snow.11",
                     "snow.12",
                     "snow.13",
                     "snow.14",
                     "snow.15",
                     "snow.16",
                     "snow.17",
                     "snow.18",
                     "snow.19",
                     "snow.20",
                     "snow.21",
                     "snow.22",
                     "snow.23",
                     "snow.24",
                    ])

init()
