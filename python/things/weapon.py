import tp
import mm


def thing_init(t):
    return


def weapon_init(name, short_name, long_name, damage, is_double_handed=False):

    x = tp.Tp(name)
    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_is_weapon(True)
    x.set_z_depth(mm.Z_DEPTH_TREASURE)
    x.set_damage(damage)
    x.set_is_double_handed(is_double_handed)

    x.set_tile(tile=name)
    x.thing_init = thing_init


def init():
    weapon_init(name="axe1.1",
                short_name="Hand Axe",
                long_name="Very handy axe. Useful for all axeing occasions.",
                damage="1d4",
                is_double_handed=True
                )
    weapon_init(name="axe1.2",
                short_name="Battle Axe",
                long_name="Dont battle without this axe.",
                damage="1d6"
                )
    weapon_init(name="axe1.3",
                short_name="Greataxe",
                long_name="This axe is great indeed. " +
                          "Not the greatest, but still pretty great.",
                damage="1d8+1",
                is_double_handed=True
                )
    weapon_init(name="axe1.4",
                short_name="Even Greater Axe",
                long_name="The greatest of great great axes.",
                damage="1d10+2",
                is_double_handed=True
                )
    weapon_init(name="axe1.5",
                short_name="Masterwork Axe",
                long_name="Finest craftwork axe. Definately not made by orcs.",
                damage="1d12"
                )
    weapon_init(name="axe1.6",
                short_name="Diamond Axe",
                long_name="Diamond encrusted bladed axe. " +
                          "Glistens in the dark.",
                damage="1d14"
                )
    weapon_init(name="axe1.7",
                short_name="Blood Axe",
                long_name="This axe yearns to be whetted with blood. "
                          "Hopefully not your own.",
                damage="2d6+2"
                )
    weapon_init(name="axe1.9",
                short_name="Cleaver Axe",
                long_name="An edge so sharp, " +
                          "you might lose your head over it.",
                damage="1d10",
                is_double_handed=True
                )
    weapon_init(name="ball_chain1.1",
                short_name="Flail",
                long_name="Don't flail around with this flail.",
                damage="1d4"
                )
    weapon_init(name="ball_chain1.2",
                short_name="Masterwork Flail",
                long_name="If you need to flail, this is the weapon for you.",
                damage="1d6"
                )
    weapon_init(name="ball_chain1.3",
                short_name="Diamond Flail",
                long_name="Flailing with a sharp edge.",
                damage="1d12"
                )
    weapon_init(name="bow1.1",
                short_name="Bow",
                long_name="Standard issue bow. Wooden. " +
                          "Bowish. What more can be said?",
                damage="1d6",
                is_double_handed=True
                )
    weapon_init(name="bow1.2",
                short_name="Longishbow",
                long_name="Not quite a long bow, but long enough.",
                damage="1d8",
                is_double_handed=True
                )
    weapon_init(name="bow1.3",
                short_name="Metal Longbow",
                long_name="A tough bow for a tough individual.",
                damage="1d10",
                is_double_handed=True
                )
    weapon_init(name="bow1.4",
                short_name="Bowmaster",
                long_name="The bow of masters. The bow master.",
                damage="1d10",
                is_double_handed=True
                )
    weapon_init(name="bow1.5",
                short_name="Masterwork Bow",
                long_name="Beautiful oaken bow with inlaid markings " +
                          "and a silver handle. Probably fires well too.",
                damage="1d12",
                is_double_handed=True
                )
    weapon_init(name="bow1.6",
                short_name="Crossbow",
                long_name="If your angry and have targets, " +
                          "this is the bow for you. " +
                          "No archery training required.",
                damage="1d6",
                is_double_handed=True
                )
    weapon_init(name="bow1.7",
                short_name="Metal cross",
                long_name="Resounding thuds will come from " +
                          "this device. And screams.",
                damage="1d8",
                is_double_handed=True
                )
    weapon_init(name="bow1.8",
                short_name="Masterwork cross",
                long_name="It's a weapon of pointy death, " +
                           "but it's beautifully made. Shiny.",
                damage="1d6",
                is_double_handed=True
                )
    weapon_init(name="mace1.1",
                short_name="Mace",
                long_name="No powder here, this is a serious mace, " +
                          "made for resounding head impacts.",
                damage="1d8"
                )
    weapon_init(name="mace1.2",
                short_name="War Mace",
                long_name="If you need to go to war, you need this mace.",
                damage="1d10"
                )
    weapon_init(name="quiver1.1",
                short_name="Arrows",
                long_name="Standard issue ACME arrows.",
                damage="1d6"
                )
    weapon_init(name="quiver1.2",
                short_name="Flame Arrows",
                long_name="Arrows that will ingite on use. No returns.",
                damage="1d6"
                )
    weapon_init(name="quiver1.3",
                short_name="Energy Arrows",
                long_name="Arrows that transform into beams of " +
                          "energy on use. No kidding.",
                damage="1d6"
                )
    weapon_init(name="quiver1.4",
                short_name="Acid Arrows",
                long_name="Don't touch the end of these arrows. " +
                          "And don't try and taste them either.",
                damage="1d6"
                )
    weapon_init(name="stick1.1",
                short_name="Just a stick",
                long_name="Sticky the stick.",
                damage="1d4"
                )
    weapon_init(name="stick1.2",
                short_name="Flame Stick",
                long_name="Sticky the stick, burning version.",
                damage="1d4"
                )
    weapon_init(name="stick1.3",
                short_name="Magic Stick",
                long_name="It's a magically enhanced stick... " +
                          "Who would believe that?",
                damage="1d4+1"
                )
    weapon_init(name="stick2.1",
                short_name="Stick V2",
                long_name="Sticky the stick, mildly improved version.",
                damage="1d4+2"
                )
    weapon_init(name="stick2.2",
                short_name="Hooked Stick",
                long_name="Great stick for inflicting a bit of extra " +
                          "damage than your common stick.",
                damage="1d4+3"
                )
    weapon_init(name="stick2.3",
                short_name="Gnarly Stick",
                long_name="An oaken stick with gnarly stuff on " +
                          "the business end. Good for hitting things with.",
                damage="1d4+4"
                )
    weapon_init(name="stick2.4",
                short_name="Battle Stick",
                long_name="The stick of the professional peasant.",
                damage="1d6"
                )
    weapon_init(name="sword1.1",
                short_name="Shortest Sword",
                long_name="The shortest of short swords.",
                damage="1d4"
                )
    weapon_init(name="sword1.2",
                short_name="Short Sword",
                long_name="The second shortest of short swords.",
                damage="1d6"
                )
    weapon_init(name="sword1.3",
                short_name="Needle Sword",
                long_name="A sword with a point so fine it will " +
                          "pierce you to the heart.",
                damage="1d4+2"
                )
    weapon_init(name="sword1.4",
                short_name="Meat Cleaver",
                long_name="Not exactly a skillful weapon, but it does " +
                          "the job. The job of a lunatic.",
                damage="1d6"
                )
    weapon_init(name="sword1.5",
                short_name="Ice Shortsword",
                long_name="It's short, blue and icy.",
                damage="1d6+1"
                )
    weapon_init(name="sword1.6",
                short_name="Platinum Shortsword",
                long_name="Of short swords, this is one of the best. " +
                          "Durable, short and shiny.",
                damage="1d8"
                )
    weapon_init(name="sword1.7",
                short_name="Flaming Shortsword",
                long_name="Mesmerizing blade. Flame ripples along its edges.",
                damage="1d6+2"
                )
    weapon_init(name="sword1.8",
                short_name="Gladius",
                long_name="Wide bladed Roman style sword. " +
                          "Great for leaving big wounds.",
                damage="1d4+3"
                )
    weapon_init(name="sword1.9",
                short_name="Dao",
                long_name="Wicked curved blade.",
                damage="1d6+2"
                )
    weapon_init(name="sword1.10",
                short_name="Khopesh",
                long_name="The oriental blade of the professional.",
                damage="1d6+4"
                )
    weapon_init(name="sword1.11",
                short_name="Long Sword",
                long_name="It's long. And a sword.",
                damage="1d8",
                is_double_handed=True
                )
    weapon_init(name="sword1.12",
                short_name="Claymore",
                long_name="The sword of the Highlander. This sword " +
                          "will give you your freedom. Or someone elses.",
                damage="1d8+2",
                is_double_handed=True
                )
    weapon_init(name="sword1.13",
                short_name="Greatsword",
                long_name="It's a sword and it's great.",
                damage="1d10",
                is_double_handed=True
                )
    weapon_init(name="sword1.14",
                short_name="Masterwork Greatsword",
                long_name="Don't mess around. Get this great sword.",
                damage="1d10+2",
                is_double_handed=True
                )
    weapon_init(name="sword1.15",
                short_name="Platinum Greatsword",
                long_name="They don't come much tougher than this.",
                damage="1d12+5",
                is_double_handed=True
                )
    weapon_init(name="sword1.16",
                short_name="Flaiming Greatsword",
                long_name="Dismember and cook your enemies.",
                damage="1d10+3",
                is_double_handed=True
                )
    weapon_init(name="sword1.17",
                short_name="Serrated Sword",
                long_name="Slice and dice with greatness.",
                damage="1d6+4"
                )
    weapon_init(name="sword1.18",
                short_name="Ulfbehrt",
                long_name="Quality hybrid of Viking and Knightly sword",
                damage="1d8+2",
                is_double_handed=True
                )
    weapon_init(name="sword1.19",
                short_name="Khanda",
                long_name="Double edged straight sword",
                damage="1d10"
                )
    weapon_init(name="sword1.20",
                short_name="Ice Sword",
                long_name="Ice ice sword.",
                damage="1d10+3"
                )
    weapon_init(name="sword1.22",
                short_name="Zweihander",
                long_name="Massive two handed ultra great sword.",
                damage="1d12+6",
                is_double_handed=True
                )
    weapon_init(name="sword_wooden1.1",
                short_name="Wooden Sword aka Stick",
                long_name="It's a stick",
                damage="1d4"
                )
    weapon_init(name="warhammer1.1",
                short_name="Maul",
                long_name="Long handled warhammer with metal head",
                damage="1d8"
                )
    weapon_init(name="warhammer1.2",
                short_name="Warhammer",
                long_name="It's a hammer. For war.",
                damage="1d10+1",
                is_double_handed=True
                )
    weapon_init(name="warhammer1.3",
                short_name="Masterwork Warhammer",
                long_name="A war hammer of distinction.",
                damage="1d12+2",
                is_double_handed=True
                )

init()
