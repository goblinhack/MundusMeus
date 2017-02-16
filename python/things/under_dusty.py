import tp


def thing_init(t):
    return


def under_dusty_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_long_name("dungeon bedrock")
    x.set_z_depth("bridge")
    x.set_is_bridge(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    under_dusty_init(name="under_dusty1",
                     short_name="aged bedrock",
                     long_name="aged bedrock",
                     tiles=[
                            "under_dusty1.1",
                           ])

init()
