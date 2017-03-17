import tp
import mm


def thing_init(t):
    return


def chair1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_house_item(True)
    x.set_is_movement_blocking(True)
    x.set_is_chair(True)
    x.set_has_shadow(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    chair1_init(name="chair1",
                short_name="chair",
                long_name="Less comfy chair.",
                tiles=["chair1"])

    chair1_init(name="chair2",
                short_name="chair",
                long_name="Comfy chair.",
                tiles=["chair2"])

init()
