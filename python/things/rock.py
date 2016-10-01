import tp


def thing_init(t):
    t.set_description("rock")


def rock_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_rock(True)
    x.set_z_depth("floor")
    x.set_blit_top_off(1)
    x.set_blit_bot_off(1)

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
                     "rock1.5",
                     "rock1.6",
                     "rock1.7",
                     "rock1.8",
                     "rock1.9",
                     "rock1.10",
                     "rock1.11",
                     "rock1.12",
                     "rock1.13",
                     "rock1.14",
                     "rock1.15",
                     "rock1.16",
                     "rock1.17",
                     "rock1.18",
                     "rock1.19",
                     "rock1.20",
                     "rock1.21",
                     "rock1.22",
                     "rock1.23",
                     "rock1.24",
                     "rock1.25",
                     "rock1.26",
                     "rock1.27",
                     "rock1.28",
                     "rock1.29",
                     "rock1.30",
                     "rock1.31",
                     "rock1.32",
                    ])

init()
