import tp
import mm


def thing_init(t):
    return


def dungeon_way_up_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)

    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_is_dungeon_way_up(True)
    x.set_z_depth(mm.Z_DEPTH_FLOOR2)
    x.set_is_dungeon_item(True)
    x.set_has_shadow(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init

    return x


def init():
    x = dungeon_way_up_init(name="dungeon_way_up1",
                            short_name="Stairs",
                            long_name="Winding stairs leading upwards " +
                            "to a better life.",
                            tiles=[
                                   "dungeon_way_up1.1",
                                   ])

    x.set_blit_bot_off(0)
    x.set_blit_top_off(1)

init()
