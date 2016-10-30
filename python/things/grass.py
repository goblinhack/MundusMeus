import tp


def thing_init(t):
    return


def grass_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_grass(True)
    x.set_z_depth("floor")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    grass_init(name="grass1",
               short_name="grass",
               long_name="grass",
               tiles=[
                      "grass1.1",
                      "grass1.2",
                      "grass1.3",
                      "grass1.4",
                      "grass1.5",
                      "grass1.6",
                      "grass1.7",
                      "grass1.8",
                      "grass1.9",
                      "grass1.10",
                      "grass1.11",
                      "grass1.12",
                      "grass1.13",
                      "grass1.14",
                      "grass1.15",
                      "grass1.16",
                      "grass1.17",
                      "grass1.18",
                      "grass1.19",
                      "grass1.20",
                      "grass1.21",
                      "grass1.22",
                      "grass1.23",
                      "grass1.24",
                      "grass1.25",
                      "grass1.26",
                      "grass1.27",
                      "grass1.28",
                      "grass1.29",
                      "grass1.30",
                     ])

init()
