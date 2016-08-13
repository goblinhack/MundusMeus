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
    player_init("player1", "Daniel-san")
    player_init("player2", "Valkyrie")
    player_init("player3", "Odd Minion")
    player_init("player4", "V.Odd Minion")

    player_init("player5", "Gunslinger")
    player_init("player6", "Minion")
    player_init("player7", "Gunslinger")
    player_init("player8", "Minion")

    player_init("player9", "Gunslinger")
    player_init("player10", "Fair Maiden")
    player_init("player11", "Fair Maiden")
    player_init("player12", "Fair Maiden")

    player_init("player13", "Gunlady")
    player_init("player14", "Acolyte")
    player_init("player15", "Acolyte")
    player_init("player16", "Pirate")

    player_init("player17", "Berserker")
    player_init("player18", "Senior Knight")
    player_init("player19", "Crusader")
    player_init("player20", "Necromancer")

    player_init("player21", "Gunslinger")
    player_init("player22", "Monk")
    player_init("player23", "Gunslinger")
    player_init("player24", "Lady")

    player_init("player25", "Gunslinger")
    player_init("player26", "Lady")
    player_init("player27", "Death Knight")
    player_init("player28", "Gold Knight")

    player_init("player29", "Gunslinger")
    player_init("player30", "Crusader")
    player_init("player31", "Crusader")
    player_init("player32", "Crew Matey")

    player_init("player33", "Vigilante")
    player_init("player34", "Shiny Knight")
    player_init("player35", "Dwarf")
    player_init("player36", "Wizard")

    player_init("player37", "Gunslinger")
    player_init("player38", "Possessed Monk")
    player_init("player39", "He-ish-man")
    player_init("player40", "Snake Lady")

    player_init("player41", "Preacher")
    player_init("player42", "Long John")
    player_init("player43", "Pro Knight")
    player_init("player44", "Knight")

    player_init("player45", "Knight")
    player_init("player46", "Knight hero")
    player_init("player47", "Warlock")
    player_init("player48", "Pirate")

    player_init("player49", "Last Knight")
    player_init("player50", "Black Knight")
    player_init("player51", "Villager")
    player_init("player52", "Villager")
    player_init("player53", "Villager")
    player_init("player54", "Villager")
    player_init("player55", "Villager")
    player_init("player56", "Villager")
    player_init("player57", "Villager")
    player_init("player58", "Villager")
    player_init("player59", "Villager")
    player_init("player60", "Villager")
    player_init("player61", "Villager")
    player_init("player62", "Villager")
    player_init("player63", "Bard")
    player_init("player64", "Crewmate")

init()
