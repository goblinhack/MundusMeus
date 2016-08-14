from tp import tpp
import mm

def thing_init(t):
    t.set_description("A grumbly little creature of dark intent")

def player_init(name, short_name):
    x = tpp(name)
    x.set_short_name(short_name)
    x.set_is_player(True)
    x.set_light_radius(6)
    x.set_world_depth("player")

    mm.tp_set_tile(x, tile=name)
    x.thing_init = thing_init

def init():
    player_init(name="player1",  short_name="Daniel-san")
    player_init(name="player2",  short_name="Valkyrie")
    player_init(name="player3",  short_name="Odd Minion")
    player_init(name="player4",  short_name="V.Odd Minion")
    player_init(name="player5",  short_name="Gunslinger")
    player_init(name="player6",  short_name="Minion")
    player_init(name="player7",  short_name="Gunslinger")
    player_init(name="player8",  short_name="Minion")
    player_init(name="player9",  short_name="Gunslinger")
    player_init(name="player10", short_name="Fair Maiden")
    player_init(name="player11", short_name="Fair Maiden")
    player_init(name="player12", short_name="Fair Maiden")
    player_init(name="player13", short_name="Gunlady")
    player_init(name="player14", short_name="Acolyte")
    player_init(name="player15", short_name="Acolyte")
    player_init(name="player16", short_name="Pirate")
    player_init(name="player17", short_name="Berserker")
    player_init(name="player18", short_name="Senior Knight")
    player_init(name="player19", short_name="Crusader")
    player_init(name="player20", short_name="Necromancer")
    player_init(name="player21", short_name="Gunslinger")
    player_init(name="player22", short_name="Monk")
    player_init(name="player23", short_name="Gunslinger")
    player_init(name="player24", short_name="Lady")
    player_init(name="player25", short_name="Gunslinger")
    player_init(name="player26", short_name="Lady")
    player_init(name="player27", short_name="Death Knight")
    player_init(name="player28", short_name="Gold Knight")
    player_init(name="player29", short_name="Gunslinger")
    player_init(name="player30", short_name="Crusader")
    player_init(name="player31", short_name="Crusader")
    player_init(name="player32", short_name="Crew Matey")
    player_init(name="player33", short_name="Vigilante")
    player_init(name="player34", short_name="Shiny Knight")
    player_init(name="player35", short_name="Dwarf")
    player_init(name="player36", short_name="Wizard")
    player_init(name="player37", short_name="Gunslinger")
    player_init(name="player38", short_name="Possessed Monk")
    player_init(name="player39", short_name="He-ish-man")
    player_init(name="player40", short_name="Snake Lady")
    player_init(name="player41", short_name="Preacher")
    player_init(name="player42", short_name="Long John")
    player_init(name="player43", short_name="Pro Knight")
    player_init(name="player44", short_name="Knight")
    player_init(name="player45", short_name="Knight")
    player_init(name="player46", short_name="Knight hero")
    player_init(name="player47", short_name="Warlock")
    player_init(name="player48", short_name="Pirate")
    player_init(name="player49", short_name="Last Knight")
    player_init(name="player50", short_name="Black Knight")
    player_init(name="player51", short_name="Villager")
    player_init(name="player52", short_name="Villager")
    player_init(name="player53", short_name="Villager")
    player_init(name="player54", short_name="Villager")
    player_init(name="player55", short_name="Villager")
    player_init(name="player56", short_name="Villager")
    player_init(name="player57", short_name="Villager")
    player_init(name="player58", short_name="Villager")
    player_init(name="player59", short_name="Villager")
    player_init(name="player60", short_name="Villager")
    player_init(name="player61", short_name="Villager")
    player_init(name="player62", short_name="Villager")
    player_init(name="player63", short_name="Bard")
    player_init(name="player64", short_name="Crewmate")

init()
