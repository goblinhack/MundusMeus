import tp


def thing_init(t):
    return


def grass_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_grass(True)
    x.set_z_depth("grass1")

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
                     ])

init()
