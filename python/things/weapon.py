import tp
import mm

def thing_init(t):
    t.set_description("A weapon")

def weapon_init(name, short_name, damage, two_handed=False):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_weapon(True)
    x.set_world_depth("player")
    x.set_damage(damage)
    x.set_two_handed(two_handed)

    x.set_tile(tile=name)
    x.thing_init = thing_init

def init():
    weapon_init(name="axe1.1",
                short_name="Hand Axe",
                damage="1d4",
                two_handed=True
                )
    weapon_init(name="axe1.2",
                short_name="Battle Axe",
                damage="1d6"
                )
    weapon_init(name="axe1.3",
                short_name="Greataxe",
                damage="1d8+1",
                two_handed=True
                )
    weapon_init(name="axe1.4",
                short_name="Even Greater Axe",
                damage="1d10+2",
                two_handed=True
                )
    weapon_init(name="axe1.5",
                short_name="Masterwork Axe",
                damage="1d12"
                )
    weapon_init(name="axe1.6",
                short_name="Diamond Axe",
                damage="1d14"
                )
    weapon_init(name="axe1.7",
                short_name="Blood Axe",
                damage="2d6+2"
                )
    weapon_init(name="axe1.9",
                short_name="Cleaver Axe",
                damage="1d10",
                two_handed=True
                )
    weapon_init(name="ball_chain1.1",
                short_name="Flail",
                damage="1d4"
                )
    weapon_init(name="ball_chain1.2",
                short_name="Masterwork Flail",
                damage="1d6"
                )
    weapon_init(name="ball_chain1.3",
                short_name="Diamond Flail",
                damage="1d12"
                )
    weapon_init(name="bow1.1",
                short_name="Bow",
                damage="1d6",
                two_handed=True
                )
    weapon_init(name="bow1.2",
                short_name="Longishbow",
                damage="1d8",
                two_handed=True
                )
    weapon_init(name="bow1.3",
                short_name="Metal Longbow",
                damage="1d10",
                two_handed=True
                )
    weapon_init(name="bow1.4",
                short_name="Bowmaster",
                damage="1d10",
                two_handed=True
                )
    weapon_init(name="bow1.5",
                short_name="Masterwork Bow",
                damage="1d12",
                two_handed=True
                )
    weapon_init(name="bow1.6",
                short_name="Crossbow",
                damage="1d6",
                two_handed=True
                )
    weapon_init(name="bow1.7",
                short_name="Metal cross",
                damage="1d8",
                two_handed=True
                )
    weapon_init(name="bow1.8",
                short_name="Masterwork cross",
                damage="1d6",
                two_handed=True
                )
    weapon_init(name="mace1.1",
                short_name="Mace",
                damage="1d8"
                )
    weapon_init(name="mace1.2",
                short_name="War Mace",
                damage="1d10"
                )
    weapon_init(name="quiver1.1",
                short_name="Arrows",
                damage="1d6"
                )
    weapon_init(name="quiver1.2",
                short_name="Flame Arrows",
                damage="1d6"
                )
    weapon_init(name="quiver1.3",
                short_name="Energy Arrows",
                damage="1d6"
                )
    weapon_init(name="quiver1.4",
                short_name="Acid Arrows",
                damage="1d6"
                )
    weapon_init(name="stick1.1",
                short_name="Just a stick",
                damage="1d4"
                )
    weapon_init(name="stick1.2",
                short_name="Flame Stick",
                damage="1d4"
                )
    weapon_init(name="stick1.3",
                short_name="Magic Stick",
                damage="1d4"
                )
    weapon_init(name="stick2.1",
                short_name="Stick V2",
                damage="1d4+1"
                )
    weapon_init(name="stick2.2",
                short_name="Hooked Stick",
                damage="1d4+2"
                )
    weapon_init(name="stick2.3",
                short_name="Gnarly Stick",
                damage="1d4+3"
                )
    weapon_init(name="stick2.4",
                short_name="Battle Stick",
                damage="1d6"
                )
    weapon_init(name="sword1.1",
                short_name="Shortest Sword",
                damage="1d4"
                )
    weapon_init(name="sword1.2",
                short_name="Short Sword",
                damage="1d6"
                )
    weapon_init(name="sword1.3",
                short_name="Needle Sword",
                damage="1d4+2"
                )
    weapon_init(name="sword1.4",
                short_name="Meat Cleaver",
                damage="1d6"
                )
    weapon_init(name="sword1.5",
                short_name="Ice Shortsword",
                damage="1d6+1"
                )
    weapon_init(name="sword1.6",
                short_name="Platinum Shortsword",
                damage="1d8"
                )
    weapon_init(name="sword1.7",
                short_name="Flaming Shortsword",
                damage="1d6+2"
                )
    weapon_init(name="sword1.8",
                short_name="Gladius",
                damage="1d4+3"
                )
    weapon_init(name="sword1.9",
                short_name="Dao",
                damage="1d6"
                )
    weapon_init(name="sword1.10",
                short_name="Khopesh",
                damage="1d6+1"
                )
    weapon_init(name="sword1.11",
                short_name="Long Sword",
                damage="1d8",
                two_handed=True
                )
    weapon_init(name="sword1.12",
                short_name="Claymore",
                damage="1d8+2",
                two_handed=True
                )
    weapon_init(name="sword1.13",
                short_name="Greatsword",
                damage="1d10",
                two_handed=True
                )
    weapon_init(name="sword1.14",
                short_name="Masterwork Greatsword",
                damage="1d10+2",
                two_handed=True
                )
    weapon_init(name="sword1.15",
                short_name="Platinum Greatsword",
                damage="1d12",
                two_handed=True
                )
    weapon_init(name="sword1.16",
                short_name="Flaiming Greatsword",
                damage="1d10+3",
                two_handed=True
                )
    weapon_init(name="sword1.17",
                short_name="Serrated Sword",
                damage="1d6+4"
                )
    weapon_init(name="sword1.18",
                short_name="Ulfbehrt",
                damage="1d8+2",
                two_handed=True
                )
    weapon_init(name="sword1.19",
                short_name="Khanda",
                damage="1d10"
                )
    weapon_init(name="sword1.20",
                short_name="Ice Sword",
                damage="1d10+3"
                )
    weapon_init(name="sword1.22",
                short_name="Zweihander",
                damage="1d12",
                two_handed=True
                )
    weapon_init(name="sword_wooden1.1",
                short_name="Wooden Sword aka Stick",
                damage="1d4"
                )
    weapon_init(name="warhammer1.1",
                short_name="Maul",
                damage="1d8"
                )
    weapon_init(name="warhammer1.2",
                short_name="Warhammer",
                damage="1d10+1",
                two_handed=True
                )
    weapon_init(name="warhammer1.3",
                short_name="Masterwork Warhammer",
                damage="1d12+2",
                two_handed=True
                )

init()
