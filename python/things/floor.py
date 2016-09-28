import tp


def thing_init(t):
    t.set_description("floor")


def food_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_floor(True)
    x.set_map_depth("floor")
    x.set_blit_bot_off(1)
    x.set_blit_top_off(1)

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
                  "floor1.2",
                  "floor1.3",
                  "floor1.4",
                  "floor1.5",
                  "floor1.6",
                  "floor1.7",
                  "floor1.8",
                  "floor1.9",
                  "floor1.10",
                  "floor1.11",
                  "floor1.12",
                  "floor1.13",
                  "floor1.14",
                  "floor1.15",
                  "floor1.16",
                  "floor1.17",
                  "floor1.18",
                  "floor1.19",
                  "floor1.20",
                  "floor1.21",
                  "floor1.22",
                  "floor1.23",
                  "floor1.24",
                  "floor1.25",
                  "floor1.26",
                  "floor1.27",
                  "floor1.28",
                  "floor1.29",
                  "floor1.30",
                  "floor1.31",
                  "floor1.32",
                 ])

init()
