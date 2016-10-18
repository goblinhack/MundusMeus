import tp


def thing_init(t):
    t.set_description("start")


def start_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_entrance(True)
    x.set_z_depth("floor2")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init

    return x


def init():
    x = start_init(name="start1",         short_name="start1",
                   tiles=[
                          "start1.1",
                         ])

    x.set_light_radius(3)
    x.set_light_tint("red")
    x.set_is_candle_light(True)
    x.set_blit_bot_off(0)
    x.set_blit_top_off(1)

init()
