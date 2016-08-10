from tp import tpp
import mm

def thing_init(t):
    t.set_description("A grumbly little creature of dark intent")
#    t.set_attack(total=1)
#    t.add_damage(what="weapon", dice="1d4+1", chance=100, priority=1)
#    t.add_damage(what="fists",  dice="1d4+1", chance=50, priority=1)
#    t.add_damage(what="bite",   dice="1d4+2", chance=50, priority=2)

def init():
    x = tpp("player1")
    x.set_short_name("player1")
    x.set_is_player(True)
    x.set_light_radius(6)
    x.set_world_depth("player")

    mm.tp_set_tile(x, tile="player1")

    x.thing_init = thing_init

init()
