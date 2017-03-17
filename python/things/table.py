import tp
import mm


def thing_init(t):
    return


def table1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_house_item(True)
    x.set_is_movement_blocking(True)
    x.set_is_table(True)
    x.set_has_shadow(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    x = table1_init(name="table1",
                    short_name="Long oak table",
                    long_name="Long oak table. " +
                    "Covered in dishes.",
                    tiles=["table1"])
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = table1_init(name="table2",
                    short_name="Small table",
                    long_name="Delicate small table.",
                    tiles=["table2"])

    x = table1_init(name="table3",
                    short_name="Writing desk",
                    long_name="Heavily worn writing desk. ",
                    tiles=["table3"])

init()
