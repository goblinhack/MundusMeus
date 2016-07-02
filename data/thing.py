import pickle
import traceback
import mm
import tp
import util

class Thing:
    def __init__ (self, level, tp_name):
        self.level = level
        self.tp_name = tp_name

        level.world.max_thing_id += 1
        self.thing_id = level.world.max_thing_id
        self.name = "{0}:{1}".format(self.thing_id, self.tp_name)

        if not tp_name in tp.all_tps:
            self.err("Thing template {0} does not exist".format(tp_name))

        self.tp = tp.all_tps[tp_name]
        self.level = level

        self.x = -1
        self.y = -1
        self.on_map = False

        self.log("P created thing")

        if self.thing_id in self.level.all_things:
            self.err("Already in level list")
            return

        self.level.all_things[self.thing_id] = self

        mm.thing_load(self)

    def __str__(self):
        return "{0}: id[{1}]:{2}".format(self.level, self.thing_id, self.tp_name)

    def destroy (self):
        if self.on_map:
            self.pop()

        if self.thing_id in self.level.all_things:
            self.level.all_things[self.thing_id] = []

        self.log("Destroyed thing")
        del self

    def log (self, msg):
        mm.log("{0: <25}: {1}".format(str(self), msg))

    def err (self, msg):
        mm.err("{0: <25}: ERROR: {1}".format(self.name, msg))
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
