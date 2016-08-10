from tp import tpp
import mm

def thing_init(t):
    t.set_description("A grumbly little creature of dark intent")
#    x.add_damage("fists", hitdice=1, d=6)
#    x.add_damage("bite", hitdice=1, d=6)
    return

def init():
    x = tpp("player1")
    x.set_short_name("player1")
    x.set_is_player(True)
    x.set_light_radius(6)
    x.set_world_depth("player")

    mm.tp_set_tile(x, tile="player1")

    x.thing_init = thing_init

init()
