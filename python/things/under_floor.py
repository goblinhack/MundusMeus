import tp


def thing_init(t):
    return


def under_floor_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_floor(True)
    x.set_z_depth("floor")
    x.set_blit_bot_off(1)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    under_floor_init(name="under_floor1",
                     short_name="floor bedrock",
                     long_name="floor bedrock",
                     tiles=[
                            "under_floor1.1",
                           ])

init()
