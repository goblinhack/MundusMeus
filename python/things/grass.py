import tp
import mm


def thing_init(t):
    return


def grass_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_grass(True)
    x.set_is_solid_ground(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_GRASS)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    grass_init(name="grass",
               short_name="Grass",
               tiles=[
                      "grass.1",
                      "grass.2",
                      "grass.3",
                      "grass.4",
                      "grass.5",
                      "grass.6",
                      "grass.7",
                      "grass.8",
                      "grass.9",
                      "grass.10",
                      "grass.11",
                      "grass.12",
                      "grass.13",
                      "grass.14",
                      "grass.15",
                      "grass.16",
                      "grass.17",
                      "grass.18",
                      "grass.19",
                      "grass.20",
                      "grass.21",
                      "grass.22",
                      "grass.23",
                      "grass.24",
                     ])

init()
