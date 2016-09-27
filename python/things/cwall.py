import tp


def thing_init(t):
    t.set_description("cwall")


def cwall_init(name, short_name):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_cwall(True)
    x.set_map_depth("wall")
    x.set_blit_top_off(1)

    x.set_tile(tile=name)
    x.thing_init = thing_init


def init():
    cwall_init(name="cwall1",         short_name="cwall1")

init()
