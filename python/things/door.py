import tp


def thing_init(t):
    t.set_description("door1")


def door1_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth("wall")
    x.set_is_movement_blocking(True)
    x.set_is_door(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    x = door1_init(name="door1",         short_name="door1",
                   tiles=[
                          "door1.1",
                          "door1.2",
                          "door1.3",
                         ])
    x.set_blit_top_off(1)

init()
