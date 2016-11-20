import tp


def thing_init(t):
    return


def player_init(name, short_name, long_name):
    x = tp.Tp(name)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_player(True)
    x.set_z_depth("player")
    x.set_light_radius(10)
    x.set_light_tint("white")

    x.set_tile(tile=name)
    x.thing_init = thing_init


def init():
    player_init(name="player1",
                short_name="foolish explorer",
                long_name="a very foolish explorer")

init()
