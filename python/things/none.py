import tp


def thing_init(t):
    t.set_description("none")


def none_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth("floor3")
    x.set_is_animated(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=25)
    else:
        x.set_tile(tile=name, delay_ms=25)

    x.thing_init = thing_init

    return x


def init():
    none_init(name="none",         short_name="none",
              tiles=[
                     "none",
                    ])

init()