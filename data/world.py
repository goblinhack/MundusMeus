import pickle
import datetime
import traceback
from curses import wrapper

def timestamp():
    return "{:%Y-%m-%d %H:%M:%S}".format(datetime.datetime.now())

class World:
    def __init__ (self, world):
        self.world_id = world
        self.thing_id = 1

    def set_level (self, level):
        self.level = level

class Level:
    def __init__ (self, world, x, y, z, w, h):
        self.world = world
        self.x = x
        self.y = y
        self.z = z
        self.w = w
        self.h = h
        self.debug = "world{0}:x{1}:y{2}:z{3}".format(self.world.world_id, x, y, z)

        self.all_things = {}
        self.on_map = [[[] for x in range(w)] for y in range(h)] 

    def log (self, msg):
        print("{0: <20}: {1: <15}: {2}".format(timestamp(), self.debug, msg))

    def err (self, msg):
        print("{0: <20}: {1: <15}: ERROR: {2}".format(timestamp(), self.debug, msg))
        traceback.print_stack()

class Thing:
    def __init__ (self, level, name):
        self.level = level
        self.name = name
        self.level = level

        level.world.thing_id += 1
        self.thing_id = level.world.thing_id

        self.x = -1
        self.y = -1
        self.on_map = False

        self.debug = "id[{0}]:{1}".format(self.thing_id, self.name)
        self.log("created")

        if self.thing_id in self.level.all_things:
            self.err("Already in level list")
            return

        self.level.all_things[self.thing_id] = self

    def destroy (self):
        if self.on_map:
            self.pop()

        self.level.all_things[self.thing_id] = []
        self.log("detroyed")
        del self

    def log (self, msg):
        print("{0: <20}: {1: <15}: {2}".format(timestamp(), self.debug, msg))

    def err (self, msg):
        print("{0: <20}: {1: <15}: ERROR: {2}".format(timestamp(), self.debug, msg))
        traceback.print_stack()

    def push (self, x, y):
        self.x = x
        self.y = y

        if self.on_map:
            self.err("Already on the map at {0},{1}".format(self.x, self.y))
            return

        self.on_map = True
        self.level.on_map[x][y].append(self)

    def pop (self):
        if not self.on_map:
            self.err("Is not on the map")
            return

        self.level.on_map[self.x][self.y].remove(self)
        self.on_map = False


w = World(0)
l = Level(world=w, x=0, y=0, z=0, w=256, h=256)
w.set_level(l)

t = Thing(level=l, name="grass")
t.push(10, 10)
t.pop()
t.destroy()

import time
time.sleep(1)

#with open('company_data.pkl', 'wb') as output:
#    print("WRITING")
#
#    company1 = []
#    for i in range(0, 256*256*1):
#        obj = {}
#        obj['name'] = "grass"
#        obj['x'] = 1
#        obj['y'] = 1
#
#        company1.append(obj)
#    pickle.dump(company1, output, pickle.HIGHEST_PROTOCOL)
#
#    company2 = Level('spam', 42)
#    pickle.dump(company2, output, pickle.HIGHEST_PROTOCOL)
#    print("WROTE")
#
#del company1
#del company2
#
#with open('company_data.pkl', 'rb') as input:
#    print("READING")
#    company1 = pickle.load(input)
##    print(company1)  # -> banana
#
#    company2 = pickle.load(input)
#    print("READ")
#
#    print(company2.name) # -> spam
#    print(company2.value)  # -> 42
#
##def save_object(obj, filename):
##    with open(filename, 'wb') as output:
##        pickle.dump(obj, output, pickle.HIGHEST_PROTOCOL)
##
### sample usage
##save_object(company1, 'company1.pkl')
#
#def main(stdscr):
#    # Clear screen
#    stdscr.clear()
#
#    # This raises ZeroDivisionError when i == 10.
#    for i in range(0, 11):
#        stdscr.addstr(i, 0, 'xxx')
#
#    stdscr.refresh()
#    stdscr.getkey()
#
#wrapper(main)
