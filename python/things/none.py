import tp
import mm

#
# This tile is used to temporarily hide the mouse focus
#


def thing_init(t):
    return


def none_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_PLAYER)
    x.set_is_animated(True)
    x.set_is_hidden(True)
    x.set_is_focus(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=25)
    else:
        x.set_tile(tile=name, delay_ms=25)

    x.thing_init = thing_init

    return x


def init():
    none_init(name="none",
              short_name="None",
              tiles=[
                     "none",
                    ])

init()
