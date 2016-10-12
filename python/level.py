import pickle
import traceback
import mm


class Level:

    def __init__(self, world, xyz):
        self.world = world
        self.xyz = xyz
        self.all_things = {}

    def __str__(self):
        return "l{0}".format(str(self.xyz))

    def destroy(self):
        self.log("Destroying level {")
        for key, value in self.all_things.items():
            value.destroy()

        self.all_things = {}
        self.log("} Destroyed level")
        del self

    def log(self, msg):
        mm.log("p-level: {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.err("p-level: {0}: ERROR: {1}".format(str(self), msg))
        traceback.print_stack()

    def dump(self):
        for i in self.all_things:
            self.all_things[i].dump()

    def save(self):
        self.log("Save level")

        with open(str(self.world) + str(self), 'wb') as f:
            pickle.dump(self, f, pickle.HIGHEST_PROTOCOL)

    def set_dim(self, width, height):
        self.width = width
        self.height = height

        self.on_map = [[[] for x in range(width)] for y in range(height)]

    def tp_find(self, x, y, tp_name):
        if x >= self.width or y >= self.height or x < 0 or y < 0:
            return None

        for t in self.on_map[x][y]:
            if t.tp.name == tp_name:
                return t

        return None

    def thing_push(self, x, y, t):
        if x >= self.width or y >= self.height or x < 0 or y < 0:
            mm.err("thing_push: map oob {0} {1}".format(x, y))
            return

        self.on_map[x][y].append(t)
