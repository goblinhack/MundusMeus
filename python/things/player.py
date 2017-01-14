import tp


def thing_init(t):
    return


def player_init(name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name("foolish explorer")
    x.set_short_name("very foolish explorer")
    x.set_is_player(True)
    x.set_z_depth("player")
    x.set_light_radius(10)
    x.set_light_tint("white")
    x.set_is_candle_light(True)
    x.set_is_animated(True)

    for t in tiles:
        x.set_tile(t, delay_ms=200)

    x.thing_init = thing_init


def init():
    player_init("player1", tiles=["player1.1", "player1.2"])

init()
