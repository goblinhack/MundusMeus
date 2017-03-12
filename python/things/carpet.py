import tp
import mm


def thing_init(t):
    return


def carpet_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_carpet(True)
    x.set_is_house_item(True)
    x.set_z_depth(mm.Z_DEPTH_FLOOR2)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    carpet_init(name="carpet1",
                short_name="carpet",
                tiles=[
                       "carpet1.1",
                       "carpet1.2",
                       "carpet1.3",
                      ])
    carpet_init(name="carpet2",
                short_name="carpet",
                tiles=[
                       "carpet2.1",
                       "carpet2.2",
                       "carpet2.3",
                      ])
    carpet_init(name="carpet3",
                short_name="carpet",
                tiles=[
                       "carpet3.1",
                       "carpet3.2",
                       "carpet3.3",
                      ])

init()
