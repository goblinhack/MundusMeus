import tp


def thing_init(t):
    return


def sand_deco1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("sand")
    x.set_is_sand(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    sand_deco1_init(name="sand_deco",
                    short_name="sand",
                    long_name="sand",
                    tiles=[
                           "sand-tl",
                           "sand-top",
                           "sand-tr",
                           "sand-left",
                           "sand-right",
                           "sand-bl",
                           "sand-bot",
                           "sand-br",
                          ])

init()
