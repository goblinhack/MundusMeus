from tp import tpp
import mm

def thing_init(t):
    t.set_description("A weapon")

def weapon_init(name, short_name):
    x = tpp(name)
    x.set_short_name(short_name)
    x.set_is_weapon(True)
    x.set_world_depth("player")

    mm.tp_set_tile(x, tile=name)
    x.thing_init = thing_init

def init():
    weapon_init(name="axe1.1",             short_name="x")
    weapon_init(name="axe1.10",            short_name="x")
    weapon_init(name="axe1.2",             short_name="x")
    weapon_init(name="axe1.3",             short_name="x")
    weapon_init(name="axe1.4",             short_name="x")
    weapon_init(name="axe1.5",             short_name="x")
    weapon_init(name="axe1.6",             short_name="x")
    weapon_init(name="axe1.7",             short_name="x")
    weapon_init(name="axe1.8",             short_name="x")
    weapon_init(name="axe1.9",             short_name="x")
    weapon_init(name="ball_chain1.1",      short_name="x")
    weapon_init(name="ball_chain1.2",      short_name="x")
    weapon_init(name="ball_chain1.3",      short_name="x")
    weapon_init(name="bow1.1",             short_name="x")
    weapon_init(name="bow1.2",             short_name="x")
    weapon_init(name="bow1.3",             short_name="x")
    weapon_init(name="bow1.4",             short_name="x")
    weapon_init(name="bow1.5",             short_name="x")
    weapon_init(name="bow1.6",             short_name="x")
    weapon_init(name="bow1.7",             short_name="x")
    weapon_init(name="bow1.8",             short_name="x")
    weapon_init(name="mace1.1",            short_name="x")
    weapon_init(name="mace1.2",            short_name="x")
    weapon_init(name="quiver1.1",          short_name="x")
    weapon_init(name="quiver1.2",          short_name="x")
    weapon_init(name="quiver1.3",          short_name="x")
    weapon_init(name="quiver1.4",          short_name="x")
    weapon_init(name="stick1.1",           short_name="x")
    weapon_init(name="stick1.2",           short_name="x")
    weapon_init(name="stick1.3",           short_name="x")
    weapon_init(name="stick2.1",           short_name="x")
    weapon_init(name="stick2.2",           short_name="x")
    weapon_init(name="stick2.3",           short_name="x")
    weapon_init(name="stick2.4",           short_name="x")
    weapon_init(name="sword1.1",           short_name="x")
    weapon_init(name="sword1.10",          short_name="x")
    weapon_init(name="sword1.11",          short_name="x")
    weapon_init(name="sword1.12",          short_name="x")
    weapon_init(name="sword1.13",          short_name="x")
    weapon_init(name="sword1.14",          short_name="x")
    weapon_init(name="sword1.15",          short_name="x")
    weapon_init(name="sword1.16",          short_name="x")
    weapon_init(name="sword1.17",          short_name="x")
    weapon_init(name="sword1.18",          short_name="x")
    weapon_init(name="sword1.19",          short_name="x")
    weapon_init(name="sword1.2",           short_name="x")
    weapon_init(name="sword1.20",          short_name="x")
    weapon_init(name="sword1.21",          short_name="x")
    weapon_init(name="sword1.22",          short_name="x")
    weapon_init(name="sword1.3",           short_name="x")
    weapon_init(name="sword1.4",           short_name="x")
    weapon_init(name="sword1.5",           short_name="x")
    weapon_init(name="sword1.6",           short_name="x")
    weapon_init(name="sword1.7",           short_name="x")
    weapon_init(name="sword1.8",           short_name="x")
    weapon_init(name="sword1.9",           short_name="x")
    weapon_init(name="sword_wooden1.1",    short_name="x")
    weapon_init(name="warhammer1.1",       short_name="x")
    weapon_init(name="warhammer1.2",       short_name="x")
    weapon_init(name="warhammer1.3",       short_name="x")

init()