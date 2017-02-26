import tp
import mm


def thing_init(t):
    return


def gravel_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_gravel(True)
    x.set_is_solid_ground(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_GRAVEL)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    gravel_init(name="gravel",
                short_name="Gravel",
                tiles=[
                       "gravel.1",
                       "gravel.2",
                       "gravel.3",
                       "gravel.4",
                       "gravel.5",
                       "gravel.6",
                       "gravel.7",
                       "gravel.8",
                       "gravel.9",
                       "gravel.10",
                       "gravel.11",
                       "gravel.12",
                       "gravel.13",
                       "gravel.14",
                       "gravel.15",
                       "gravel.16",
                       "gravel.17",
                       "gravel.18",
                       "gravel.19",
                       "gravel.20",
                       "gravel.21",
                       "gravel.22",
                       "gravel.23",
                       "gravel.24",
                       "gravel.25",
                       "gravel.26",
                       "gravel.27",
                       "gravel.28",
                       "gravel.29",
                       "gravel.30",
                       "gravel.31",
                       "gravel.32",
                       "gravel.33",
                       "gravel.34",
                       "gravel.35",
                       "gravel.36",
                       "gravel.37",
                       "gravel.38",
                       "gravel.39",
                       "gravel.40",
                       "gravel.41",
                       "gravel.42",
                       "gravel.43",
                       "gravel.44",
                       "gravel.45",
                       "gravel.46",
                       "gravel.47",
                      ])

init()
