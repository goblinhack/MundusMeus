import tp
import mm


def thing_init(t):
    return


def gravel_snow_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_gravel_snow(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_GRAVEL)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    gravel_snow_init(name="gravel_snow",
                     short_name="Snowy gravel",
                     tiles=[
                            "gravel_snow.1",
                            "gravel_snow.2",
                            "gravel_snow.3",
                            "gravel_snow.4",
                            "gravel_snow.5",
                            "gravel_snow.6",
                            "gravel_snow.7",
                            "gravel_snow.8",
                            "gravel_snow.9",
                            "gravel_snow.10",
                            "gravel_snow.11",
                            "gravel_snow.12",
                            "gravel_snow.13",
                            "gravel_snow.14",
                            "gravel_snow.15",
                            "gravel_snow.16",
                            "gravel_snow.17",
                            "gravel_snow.18",
                            "gravel_snow.19",
                            "gravel_snow.20",
                            "gravel_snow.21",
                            "gravel_snow.22",
                            "gravel_snow.23",
                            "gravel_snow.24",
                            ])

init()
