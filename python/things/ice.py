import tp
import mm


def thing_init(t):
    return


def ice_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_ice(True)
    x.set_z_depth(mm.Z_DEPTH_ICE)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    ice_init(name="ice",
             short_name="Ice",
             tiles=[
                    "ice.2",
                    "ice.3",
                    "ice.4",
                    "ice.5",
                    "ice.6",
                    "ice.7",
                    "ice.8",
                    "ice.9",
                    "ice.10",
                    "ice.11",
                    "ice.12",
                    "ice.13",
                    "ice.14",
                    "ice.15",
                    "ice.16",
                    "ice.17",
                    "ice.18",
                    "ice.19",
                    "ice.20",
                    "ice.21",
                    "ice.22",
                    "ice.23",
                    "ice.24",
                   ])

init()
