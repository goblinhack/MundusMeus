from tp import tpp
import mm


def init():
    x = tpp("player1")
    x.set_short_name("player1")
#    x.set_description("A grumbly little creature of dark intent")
#    x.set_damage("fists", hitdice=1, d=6)
#    x.set_damage("fists", hitdice=1, d=6)
    x.set_is_player(True)
    x.set_light_radius(6)
    x.set_world_depth("player")

    mm.tp_set_tile(x, tile="player1")

init()
