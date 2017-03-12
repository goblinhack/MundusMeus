import tp
import mm


def thing_init(t):
    return


def thing_pushed(t):
    x = (t.x % 6) + 1
    y = (t.y % 6) + 1
    t.set_tilename("lawn{}.{}".format(x, y))
    return


def lawn_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_lawn(True)
    x.set_is_solid_ground(True)
    x.set_is_house_item(True)
    x.set_z_depth(mm.Z_DEPTH_LAWN)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init
    x.thing_pushed = thing_pushed


def init():
    lawn_init(name="lawn",
              short_name="lawn",
              tiles=[
                     "lawn1.1",
                     "lawn1.2",
                     "lawn1.3",
                     "lawn1.4",
                     "lawn1.5",
                     "lawn1.6",
                     "lawn2.1",
                     "lawn2.2",
                     "lawn2.3",
                     "lawn2.4",
                     "lawn2.5",
                     "lawn2.6",
                     "lawn3.1",
                     "lawn3.2",
                     "lawn3.3",
                     "lawn3.4",
                     "lawn3.5",
                     "lawn3.6",
                     "lawn4.1",
                     "lawn4.2",
                     "lawn4.3",
                     "lawn4.4",
                     "lawn4.5",
                     "lawn4.6",
                     "lawn5.1",
                     "lawn5.2",
                     "lawn5.3",
                     "lawn5.4",
                     "lawn5.5",
                     "lawn5.6",
                     "lawn6.1",
                     "lawn6.2",
                     "lawn6.3",
                     "lawn6.4",
                     "lawn6.5",
                     "lawn6.6",
                    ])

init()
