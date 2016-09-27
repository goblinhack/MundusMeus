import tp


def thing_init(t):
    t.set_description("wall")


def wall_init(name, short_name):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_wall(True)
    x.set_map_depth("wall")
    x.set_blit_top_off(1)

    x.set_tile(tile=name)
    x.thing_init = thing_init


def init():
    wall_init(name="wall1",         short_name="wall1")
    wall_init(name="dirt1",         short_name="dirt1")

init()
