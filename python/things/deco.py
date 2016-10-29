import tp


def thing_init(t):
    return


def deco1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("player")
    x.set_blit_top_off(1)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    deco1_init(name="deco1",
               short_name="dungeon objet d'art",
               long_name="dungeon objet d'art",
               tiles=[
                      "deco1.1",
                      "deco1.2",
                      "deco1.3",
                      "deco1.4",
                      "deco1.5",
                      "deco1.6",
                      "deco1.7",
                      "deco1.8",
                     ])

init()
