import tp


def thing_init(t):
    t.set_description("floor")


def food_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_floor(True)
    x.set_map_depth("floor")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    food_init(name="floor1",         short_name="floor1",
              tiles=[
                  "floor1.1",
                  "floor1.1",
                  "floor1.1",
                  "floor1.1",
                  "floor1.2",
                  "floor1.2",
                  "floor1.2",
                  "floor1.2",
                  "floor1.2",
                  "floor1.2",
                  "floor1.3",
                  "floor1.3",
                  "floor1.3",
                  "floor1.3",
                  "floor1.3",
                  "floor1.3",
                  "floor1.3",
                  "floor1.3",
                  "floor1.4",
                  "floor1.4",
                  "floor1.4",
                  "floor1.4",
                  "floor1.4",
                  "floor1.4",
                  "floor1.4",
                  "floor1.4",
                  "floor1.5",
                  "floor1.5",
                  "floor1.5",
                  "floor1.5",
                  "floor1.5",
                  "floor1.5",
                  "floor1.5",
                  "floor1.6",
                  "floor1.6",
                  "floor1.6",
                  "floor1.6",
                  "floor1.6",
                  "floor1.6",
                  "floor1.6",
                  "floor1.6",
                  "floor1.7",
                  "floor1.8",
                 ])
    food_init(name="chasm1",         short_name="chasm1")

init()
