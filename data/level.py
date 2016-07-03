import pickle
import traceback
import mm
import util

class Level:
    def __init__ (self, world, xyz):
        self.world = world
        self.xyz = xyz
        self.all_things = {}

    def __str__(self):
        return "l{0}".format(str(self.xyz))

    def destroy (self):
        self.log("Destroying level {")
        for key,value in self.all_things.items():
            value.destroy()

        self.all_things = {}
        self.log("} Destroyed level")
        del self

    def log (self, msg):
        mm.log("p-level: {0}: {1}".format(str(self), msg))

    def err (self, msg):
        mm.err("p-level: {0}: ERROR: {1}".format(str(self), msg))
        traceback.print_stack()

    def dump (self):
        for i in self.all_things:
            self.all_things[i].dump()

    def save (self):
        self.log("Save level")

        with open(str(self.world) + str(self), 'wb') as f:
            pickle.dump(self, f, pickle.HIGHEST_PROTOCOL)

    def set_dim (self, w, h):
        self.w = w
        self.h = h

        self.on_map = [[[] for x in range(w)] for y in range(h)] 
