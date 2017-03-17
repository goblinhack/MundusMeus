import tp
import mm


def thing_init(t):
    return


def bookcase1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_house_item(True)
    x.set_is_movement_blocking(True)
    x.set_is_bookcase(True)
    x.set_has_shadow(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    x = bookcase1_init(name="bookcase1",
                       short_name="Bookcase",
                       long_name="Bookcase stuffed with arcane books.",
                       tiles=["bookcase1"])
    x.set_blit_top_off(1)

    x = bookcase1_init(name="bookcase2",
                       short_name="Bookcase",
                       long_name="Bookcase stuffed with arcane books.",
                       tiles=["bookcase2"])
    x.set_blit_top_off(1)

    x = bookcase1_init(name="bookcase3",
                       short_name="Bookcase",
                       long_name="Bookcase stuffed with arcane books.",
                       tiles=["bookcase3"])
    x.set_blit_top_off(1)

init()
