import pickle
import datetime
import traceback
from curses import wrapper

def Timestamp():
    return "{:%Y-%m-%d %H:%M:%S}".format(datetime.datetime.now())

class Xyz:
    def __init__ (self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __str__ (self):
        return str(self.x) + "," + str(self.y) + "," + str(self.z)

class World:
    def __init__ (self, world):
        self.world_id = world

        #
        # Max thing ID in use in any level. This grows forever.
        #
        self.max_thing_id = 1

        #
        # Current playing level
        #
        self.level = None

        #
        # Stack of level co-ordinates so we can backtrack out of dungeons
        #
        self.level_stack = []

        self.log("Created")

    def destroy (self):
        self.log("Destroying world {")

        while self.level != None:
            self.pop_level()

        self.log("} Destroyed world")

        del self

    def __str__(self):
        return "world:{0}".format(self.world_id)

    def log (self, msg):
        print("{0: <19}: {1: <25}: WORLD: {2}".
                format(Timestamp(), self, msg))

    def err (self, msg):
        print("{0: <19}: {1: <25}: WORLD: ERROR: {2}".
                format(Timestamp(), self, msg))
        traceback.print_stack()

    def get_level (self):
        return self.level

    def get_level_name (self, xyz):
        return "world{0}@{1}".format(self.world_id, xyz)

    def load_level (self, xyz):
        level_name = self.get_level_name(xyz)
        self.log("Loading level {0}".format(level_name))

        try:
            with open(level_name, 'rb') as f:
                level = pickle.load(f)
        except:
            level = Level(self, xyz)

        return level

    def push_level (self, xyz):
        self.log("Push level {0}".format(xyz))

        #
        # Add a new level onto the stack. Destroy the current level first.
        #
        if self.level != None:
            self.level.save()
            self.level.destroy()
            self.level = None

        self.level_stack.append(xyz)
        self.level = self.load_level(xyz)
        self.save()

    def pop_level (self):
        self.save()

        #
        # Destory the current level and try and load the next in the stack.
        #
        if not len(self.level_stack):
            return

        xyz = self.level_stack[len(self.level_stack)-1]
        self.log("Pop level {0}".format(xyz))

        if self.level != None:
            self.level.destroy()
            self.level = None
            
        self.level_stack.pop()

        if not len(self.level_stack):
            return

        self.level = self.load_level(xyz)

    def save (self):
        self.log("Save world")
        with open(str(self), 'wb') as f:
            pickle.dump(self.world_id, f,           pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.max_thing_id, f,       pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.level_stack, f,        pickle.HIGHEST_PROTOCOL)

        self.level.save()

    def load (self):
        self.log("Load world")
        with open(str(self), 'rb') as f:
            self.world_id                           = pickle.load(f)
            self.max_thing_id                       = pickle.load(f)
            self.level_stack                        = pickle.load(f)

        #
        # Now load the last level we looked at
        #
        if len(self.level_stack) == 0:
            #
            # Just load the default level
            #
            self.err("Empty level stack")
            p = Xyz(0,0,0)
            self.push_level(p)
        else:
            xyz = self.level_stack[len(self.level_stack)-1]
            self.level = self.load_level(xyz)

    def dump (self):
        self.log("DUMP: {0: <19} {1}".format("world_id", 
            self.world_id))
        self.log("DUMP: {0: <19} {1}".format("max_thing_id", 
            self.max_thing_id))
        self.log("DUMP: {0: <19} {1}".format("level_stack", 
            self.level_stack))

        if self.level != None:
            self.level.dump()

class Level:
    def __init__ (self, world, xyz):
        self.world = world
        self.xyz = xyz
        self.all_things = {}

    def __str__(self):
        return "{0},level:{1}".format(self.world, self.xyz)

    def destroy (self):
        self.log("Destroying level {")
        for key,value in self.all_things.items():
            value.destroy()
            self.all_things.pop(key)
        self.log("} Destroyed level")
        del self

    def log (self, msg):
        print("{0: <19}: {1: <25}: LEVEL: {2}".format(Timestamp(), self, msg))

    def err (self, msg):
        print("{0: <19}: {1: <25}: LEVEL: ERROR: {2}".format(
            Timestamp(), self, msg))
        traceback.print_stack()

    def dump (self):
        for i in self.all_things:
            self.all_things[i].dump()

    def save (self):
        self.log("Save level")

        with open(str(self), 'wb') as f:
            pickle.dump(self, f, pickle.HIGHEST_PROTOCOL)

    def set_dim (self, w, h):
        self.w = w
        self.h = h

        self.on_map = [[[] for x in range(w)] for y in range(h)] 

class Thing:
    def __init__ (self, level, name):
        self.level = level
        self.name = name
        self.level = level

        level.world.max_thing_id += 1
        self.thing_id = level.world.max_thing_id

        self.x = -1
        self.y = -1
        self.on_map = False

        self.log("Created thing")

        if self.thing_id in self.level.all_things:
            self.err("Already in level list")
            return

        self.level.all_things[self.thing_id] = self

    def __str__(self):
        return "{0}: id[{1}]:{2}".format(self.level, self.thing_id, self.name)

    def destroy (self):
        if self.on_map:
            self.pop()

        if self.thing_id in self.level.all_things:
            self.level.all_things[self.thing_id] = []

        self.log("Destroyed thing")
        del self

    def log (self, msg):
        print("{0: <19}: {1: <25}: {2}".format(Timestamp(), self, msg))

    def err (self, msg):
        print("{0: <19}: {1: <25}: ERROR: {2}".format(
            Timestamp(), self.debug, msg))
        traceback.print_stack()

    def dump (self):
        self.log("@ {0},{1}".format(self.x, self.y))

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

def save_test():
    w = World(0)
    
    p = Xyz(0,0,0)
    w.push_level(p)
    l = w.get_level()
    l.set_dim(256, 256)
    
    p2 = Xyz(0,0,1)
    w.push_level(p2)
    l = w.get_level()
    l.set_dim(256, 256)
    
    for i in range(0,3):
        t = Thing(level=l, name="grass")
        t.push(10 + 1, 10 + 1)
        t.pop()

    w.save()

def load_test():
    w = World(0)
    w.load()
    w.dump()


save_test()
print("===============")
load_test()
