import tp


def thing_init(t):
    t.set_description("corridor")


def food_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_corridor(True)
    x.set_map_depth("floor")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    food_init(name="corridor1",         short_name="corridor1",
              tiles=[
                     "corridor1.1",
                     "corridor1.1",
                     "corridor1.1",
                     "corridor1.1",
                     "corridor1.2",
                     "corridor1.2",
                     "corridor1.2",
                     "corridor1.2",
                     "corridor1.2",
                     "corridor1.2",
                     "corridor1.3",
                     "corridor1.3",
                     "corridor1.3",
                     "corridor1.3",
                     "corridor1.3",
                     "corridor1.3",
                     "corridor1.3",
                     "corridor1.3",
                     "corridor1.4",
                     "corridor1.4",
                     "corridor1.4",
                     "corridor1.4",
                     "corridor1.4",
                     "corridor1.4",
                     "corridor1.4",
                     "corridor1.4",
                     "corridor1.5",
                     "corridor1.5",
                     "corridor1.5",
                     "corridor1.5",
                     "corridor1.5",
                     "corridor1.5",
                     "corridor1.5",
                     "corridor1.6",
                     "corridor1.6",
                     "corridor1.6",
                     "corridor1.6",
                     "corridor1.6",
                     "corridor1.6",
                     "corridor1.6",
                     "corridor1.6",
                     "corridor1.7",
                     "corridor1.8",
                    ])

init()
