import tp


def thing_init(t):
    t.set_description("rock")


def rock_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_rock(True)
    x.set_map_depth("floor")
    x.set_blit_bot_off(1)
    x.set_blit_top_off(2)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=150)
    else:
        x.set_tile(tile=name, delay_ms=150)

    x.thing_init = thing_init


def init():
    rock_init(name="rock1",         short_name="rock1",
              tiles=[
                     "rock1.1",
                     "rock1.2",
                     "rock1.3",
                     "rock1.4",
                    ])

init()
