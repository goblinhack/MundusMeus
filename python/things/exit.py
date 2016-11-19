import tp


def thing_init(t):
    return


def thing_pushed(t):
    t.wid.bounce_to_pct_in(height=0.1,
                           fade=1.0, ms=500, count=1000)


def exit_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)

    x.set_is_shadow_caster_soft(True)
    x.set_is_shadow_caster(True)
    x.set_long_name(short_name)
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
    x = exit_init(name="exit1",
                  short_name="stairs leading down",
                  long_name="stairs leading down",
                  tiles=[
                         "exit1.1",
                        ])

    x.set_light_radius(3)
    x.set_light_tint("red")
    x.set_is_candle_light(True)
    x.set_blit_bot_off(0)

    x = exit_init(name="exit1_deco",
                  short_name="a warning sign for fools",
                  long_name="a warning sign for fools",
                  tiles=[
                         "exit1_deco.1",
                        ])
    x.thing_pushed = thing_pushed
    x.set_z_depth("explosion")

init()
