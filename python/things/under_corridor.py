import tp
import mm


def thing_init(t):
    return


def under_corridor_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_BRIDGE)
    x.set_is_dungeon_item(True)
    x.set_is_bridge(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    under_corridor_init(name="under_corridor1",
                        short_name="corridor bedrock",
                        tiles=[
                               "under_corridor1.1",
                              ])

init()
