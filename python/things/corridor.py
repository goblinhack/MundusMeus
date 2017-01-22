import tp


def thing_init(t):
    return


def corridor_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name, is_corridor=True)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_corridor(True)
    x.set_z_depth("floor")
    x.set_blit_bot_off(1)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    corridor_init(name="corridor1",
                  short_name="dusty corridor",
                  long_name="dusty corridor",
                  tiles=[
                         "corridor1.1",
                         "corridor1.2",
                         "corridor1.3",
                        ])
    corridor_init(name="corridor2",
                  short_name="dusty corridor",
                  long_name="dusty corridor",
                  tiles=[
                         "corridor2.1",
                         "corridor2.2",
                         "corridor2.3",
                        ])

init()
