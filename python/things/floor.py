import tp


def thing_init(t):
    return


def floor_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_floor(True)
    x.set_z_depth("floor")
    x.set_blit_top_off(1)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    floor_init(name="floor1",
               short_name="dirty floor",
               long_name="dirty floor",
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
                     ])

init()
