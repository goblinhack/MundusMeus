import tp


def thing_init(t):
    t.set_description("A grumbly little creature of dark intent")


def player_init(name, short_name):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_player(True)
    x.set_map_depth("player")
    x.set_light_radius(4)
    x.set_light_tint("yellow")
    x.set_is_candle_light(True)

    x.set_tile(tile=name)
    x.thing_init = thing_init


def init():
    player_init(name="player1",  short_name="Daniel-san")

init()
