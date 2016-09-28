import tp


def thing_init(t):
    t.set_description("water")


def water_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_water(True)
    x.set_map_depth("floor")
    x.set_blit_bot_off(1)
    x.set_is_animated(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=100)
    else:
        x.set_tile(tile=name, delay_ms=100)

    x.thing_init = thing_init


def init():
    water_init(name="water1",         short_name="water1",
               tiles=[
                      "water1.1",
                      "water1.2",
                      "water1.3",
                      "water1.4",
                      "water1.5",
                      "water1.6",
                      "water1.7",
                      "water1.8",
                     ])

init()
