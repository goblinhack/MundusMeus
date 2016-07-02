import pickle
import traceback
import level
import util
import mm

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
        return "{0}".format(self.world_id)

    def log (self, msg):
        mm.log("p-world: {0}: {1}".format(str(self), msg))

    def err (self, msg):
        mm.err("p-world: {0}: ERROR: {1}".format(str(self), msg))
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
                l = pickle.load(f)
        except:
            l = level.Level(self, xyz)

        return l

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
            p = util.Xyz(0,0,0)
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
