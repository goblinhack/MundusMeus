import tp


def thing_init(t):
    t.set_description("lava")


def lava_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_lava(True)
    x.set_map_depth("floor")
    x.set_blit_bot_off(1)
    x.set_is_animated(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=150)
    else:
        x.set_tile(tile=name, delay_ms=150)

    x.thing_init = thing_init


def init():
    lava_init(name="lava1",         short_name="lava1",
              tiles=[
                  "lava1.1",
                  "lava1.2",
                  "lava1.3",
                  "lava1.4",
                  "lava1.5",
                  "lava1.6",
                  "lava1.7",
                  "lava1.8",
                 ])

init()
