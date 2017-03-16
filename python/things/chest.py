import tp
import mm


def thing_init(t):
    return


def chest1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_house_item(True)
    x.set_is_movement_blocking(True)
    x.set_is_chest(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    chest1_init(name="chest1",
                short_name="chest",
                long_name="Chest for storing dark secrets.",
                tiles=["chest1"])

init()
