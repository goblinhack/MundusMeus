import tp
import mm

def thing_init(t):
    t.set_description("Some food")

def food_init(name, short_name):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_food(True)
    x.set_z_depth("player")

    x.set_tile(tile=name)
    x.thing_init = thing_init

def init():
    food_init(name="carrot",         short_name="Juicy carrot")
    food_init(name="corn",           short_name="Corn on the cob")
    food_init(name="pepper",         short_name="Chilli pepper")
    food_init(name="brocolli",       short_name="Brocolli")
    food_init(name="cauliflower",    short_name="Cauliflower")
    food_init(name="tomato",         short_name="Tomato")
    food_init(name="cabbage",        short_name="Cabbage")
    food_init(name="cucumber",       short_name="Cucumber")
    food_init(name="squash",         short_name="Squashed")
    food_init(name="potato",         short_name="Potato")
    food_init(name="banana",         short_name="Banana")
    food_init(name="orange",         short_name="Orange")
    food_init(name="lemon",          short_name="Lemon")
    food_init(name="lime",           short_name="Lemon")
    food_init(name="plums",          short_name="Plums")
    food_init(name="white-grapes",   short_name="White grapes")
    food_init(name="red-grapes",     short_name="Red grapes")
    food_init(name="watermelon",     short_name="Watermelon")
    food_init(name="raspberries",    short_name="Raspberries")
    food_init(name="red-apple",      short_name="Red apple")
    food_init(name="green-apple",    short_name="Green apple")
    food_init(name="bread-and-wine", short_name="Bread and wine")
    food_init(name="cheese",         short_name="Cheese")

init()
