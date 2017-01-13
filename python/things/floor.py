import tp


def thing_init(t):
    return


def floor_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_floor(True)
    x.set_z_depth("floor")
#    x.set_blit_top_off(1)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    floor_init(name="floor1",
               short_name="dirty floor",
               long_name="dirty floor",
               tiles=[
                      "floor1.1",
                      "floor1.2",
                      "floor1.3",
                      "floor1.4",
                     ])

init()
