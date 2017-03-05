import tp
import mm


def thing_init(t):
    return


def sand_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_sand(True)
    x.set_is_solid_ground(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_SAND)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    sand_init(name="sand",
              short_name="Sand",
              tiles=[
                     "sand.1",
                     "sand.2",
                     "sand.3",
                     "sand.4",
                     "sand.5",
                     "sand.6",
                     "sand.7",
                     "sand.8",
                     "sand.9",
                     "sand.10",
                     "sand.11",
                     "sand.12",
                     "sand.13",
                     "sand.14",
                     "sand.15",
                     "sand.16",
                     "sand.17",
                     "sand.18",
                     "sand.19",
                     "sand.20",
                     "sand.21",
                     "sand.22",
                     "sand.23",
                     "sand.24",
                    ])

init()
