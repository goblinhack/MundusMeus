import traceback
import mm
import tp


class Thing:

    def __init__(self, level, tp_name):
        self.level = level
        self.tp_name = tp_name

        level.world.max_thing_id += 1
        self.thing_id = level.world.max_thing_id
        self.name = "{0}:{1}".format(self.thing_id, self.tp_name)

        if tp_name not in tp.all_tps:
            self.err("Thing template {0} does not exist".format(tp_name))

        self.tp = tp.all_tps[tp_name]
        self.level = level

        self.x = -1
        self.y = -1
        self.on_map = False

        self.log("Created thing")

        if self.thing_id in self.level.all_things:
            self.err("Already in level list")
            return

        self.level.all_things[self.thing_id] = self

        mm.thing_new(self, tp_name)

        if self.tp.thing_init is not None:
            self.tp.thing_init(self)

    def __str__(self):
        return "{0}:{1}".format(self.thing_id, self.tp_name)

    def destroy(self, reason="no reason"):
        if self.on_map:
            self.pop()
        self.log("Destroying thing, {0}".format(reason) + " {")

        if self.thing_id in self.level.all_things:
            self.level.all_things[self.thing_id] = []

        mm.thing_destroyed(self, reason)

        self.log("} " + "Destroyed thing, {0}".format(reason))
        del self

    def log(self, msg):
        mm.log("p-thing {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.err("p-thing {0}: ERROR: {1}".format(self.name, msg))
        traceback.print_stack()

    def dump(self):
        self.log("@ {0},{1}".format(self.x, self.y))

    def push(self, x, y):
        self.x = x
        self.y = y

        if self.on_map:
            self.err("Already on the map at {0},{1}".format(self.x, self.y))
            return

        self.on_map = True
        self.level.on_map[x][y].append(self)
        mm.thing_push(self, x, y)

    def pop(self):
        if not self.on_map:
            self.err("Is not on the map")
            return

        self.level.on_map[self.x][self.y].remove(self)
        mm.thing_pop(self)
        self.on_map = False

    def set_description(self, value=""):
        self.description = value
