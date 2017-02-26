import tp
import mm


def thing_init(t):
    return


def grass_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_grass(True)
    x.set_is_grass_snow(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_GRASS)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    grass_init(name="grass_snow",
               short_name="Grass",
               tiles=[
                      "grass_snow.1",
                      "grass_snow.2",
                      "grass_snow.3",
                      "grass_snow.4",
                      "grass_snow.5",
                      "grass_snow.6",
                      "grass_snow.7",
                      "grass_snow.8",
                      "grass_snow.9",
                      "grass_snow.10",
                      "grass_snow.11",
                      "grass_snow.12",
                      "grass_snow.13",
                      "grass_snow.14",
                      "grass_snow.15",
                      "grass_snow.16",
                      "grass_snow.17",
                      "grass_snow.18",
                      "grass_snow.19",
                      "grass_snow.20",
                      "grass_snow.21",
                      "grass_snow.22",
                      "grass_snow.23",
                      "grass_snow.24",
                     ])

init()
