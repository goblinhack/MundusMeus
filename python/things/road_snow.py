import tp
import mm


def thing_init(t):
    return


def road_snow_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_road_snow(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_ROAD)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    road_snow_init(name="road_snow",
                   short_name="Snowy road",
                   tiles=[
                          "road_snow.1",
                          "road_snow.2",
                          "road_snow.3",
                          "road_snow.4",
                          "road_snow.5",
                          "road_snow.6",
                          "road_snow.7",
                          "road_snow.8",
                          "road_snow.9",
                          "road_snow.10",
                          "road_snow.11",
                          "road_snow.12",
                          "road_snow.13",
                          "road_snow.14",
                          "road_snow.15",
                          "road_snow.16",
                          "road_snow.17",
                          "road_snow.18",
                          "road_snow.19",
                          "road_snow.20",
                          "road_snow.21",
                          "road_snow.22",
                          "road_snow.23",
                        ])

init()
