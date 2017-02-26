import tp
import mm


def thing_init(t):
    return


def road_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_road(True)
    x.set_is_solid_ground(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_ROAD)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    road_init(name="road",
              short_name="Road",
              tiles=[
                     "road.1",
                     "road.2",
                     "road.3",
                     "road.4",
                     "road.5",
                     "road.6",
                     "road.7",
                     "road.8",
                     "road.9",
                     "road.10",
                     "road.11",
                     "road.12",
                     "road.13",
                     "road.14",
                     "road.15",
                     "road.16",
                     "road.17",
                     "road.18",
                     "road.19",
                     "road.20",
                     "road.21",
                     "road.22",
                     "road.23",
                     "road.24",
                     "road.25",
                     "road.26",
                     "road.27",
                     "road.28",
                     "road.29",
                     "road.30",
                     "road.31",
                     "road.32",
                     "road.33",
                     "road.34",
                     "road.35",
                     "road.36",
                     "road.37",
                     "road.38",
                     "road.39",
                     "road.40",
                     "road.41",
                     "road.42",
                     "road.43",
                     "road.44",
                     "road.45",
                     "road.46",
                    ])

init()
