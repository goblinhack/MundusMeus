import tp


def thing_init(t):
    t.set_description("exit")


def thing_pushed(t):
    t.wid.bounce_to_pct_in(height=0.1,
                           fade=1.0, ms=500, count=1000)


def exit_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_exit(True)
    x.set_z_depth("floor2")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init

    return x


def init():
    x = exit_init(name="exit1",         short_name="exit1",
                  tiles=[
                         "exit1.1",
                        ])

    x.set_light_radius(3)
    x.set_light_tint("red")
    x.set_is_candle_light(True)
    x.set_blit_bot_off(1)

    x = exit_init(name="exit1-deco",         short_name="exit1-deco",
                  tiles=[
                         "exit1-deco.1",
                        ])
    x.thing_pushed = thing_pushed
    x.set_z_depth("explosion")

init()
