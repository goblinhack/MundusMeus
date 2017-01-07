import pickle
import jsonpickle
import traceback
import mm
import math
import game
import os
import biome_dungeon
import biome_land
import copy


class Chunk:

    def __init__(self, level, xyz, cx, cy):

        level.chunk[cx][cy] = self
        self.level = level
        self.xyz = copy.copy(xyz)
        self.debug("New chunk")

        self.thing_id_per_level = 10000
        self.max_thing_id = ((xyz.y * mm.WORLD_WIDTH) + xyz.x) * (xyz.z + 1)
        self.max_thing_id *= self.thing_id_per_level
        self.all_things = {}
        self.is_biome_land = False
        self.is_biome_dungeon = False

        self.is_snowy = False
        self.is_grassy = False
        self.is_watery = False
        self.cx = cx
        self.cy = cy
        self.base_x = cx * mm.CHUNK_WIDTH
        self.base_y = cy * mm.CHUNK_HEIGHT

        self.on_map = [[[] for x in range(mm.CHUNK_WIDTH)]
                       for y in range(mm.CHUNK_HEIGHT)]

        f = os.path.normcase(os.path.join(os.environ["APPDATA"], str(self)))
        if os.path.isfile(f):
            try:
                mm.con("Chunk {0}: Loading".format(self, f))
                self.load()
                need_new_chunk = False
            except Exception as inst:
                self.die("Chunk {0}: Loading failed, {1}, {2}".format(
                         self, f, str(inst)))
                need_new_chunk = True
        else:
            need_new_chunk = True

        if need_new_chunk:
            mm.con("Chunk {0}: Creating".format(str(self)))
            if self.xyz.z < 0:
                self.biome_create(is_dungeon=True, seed=game.g.seed)
            else:
                self.biome_create(is_land=True, seed=game.g.seed)

    #
    # Create a random biome
    #
    def biome_create(self, seed, is_land=False, is_dungeon=False):

        self.is_biome_land = is_land
        self.is_biome_dungeon = is_dungeon

        if self.is_biome_dungeon:
            self.biome_build = biome_dungeon.biome_build
            self.biome_populate = biome_dungeon.biome_populate

        if self.is_biome_land:
            self.biome_build = biome_land.biome_build
            self.biome_populate = biome_land.biome_populate

        mm.log("Biome build")
        self.biome_build(self, seed=game.g.seed)

        mm.log("Biome populate")
        self.biome_populate(self)

        mm.log("Biome create done")

    def __str__(self):
        return "l{0}".format(str(self.xyz))

    def destroy(self):
        self.debug("Destroying chunk {")

        #
        # to avoid dictionary changed size during iteration, walk the keys
        #
        for thing_id in list(self.all_things.keys()):

            #
            # Delete from the parent too
            #
            if thing_id in self.level.all_things:
                del self.level.all_things[thing_id]

            t = self.all_things[thing_id]
            t.destroy()

        self.all_things = {}
        self.debug("} Destroyed chunk")
        del self

    def tick(self):

        s = math.sin(game.g.move_count / (math.pi * 11))
        if self.is_snowy:
            if s > 0:
                mm.game_set_snow_amount(int(s * 100))
            else:
                mm.game_set_snow_amount(0)

        if self.is_grassy or self.is_watery:
            if s > 0:
                mm.game_set_rain_amount(int(s * 100))
            else:
                mm.game_set_rain_amount(0)

    def log(self, msg):
        mm.log("Chunk {0}: {1}".format(str(self), msg))

    def con(self, msg):
        mm.con("Chunk {0}: {1}".format(str(self), msg))

    def debug(self, msg):
        mm.log("Chunk {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.con("".join(traceback.format_stack()))
        mm.err("Chunk {0}: ERROR: {1}".format(str(self), msg))

    def die(self, msg):
        mm.con("".join(traceback.format_stack()))
        mm.die("Chunk {0}: ERROR: {1}".format(str(self), msg))

    def dump(self):
        for i in self.all_things:
            self.all_things[i].dump()

    def save(self):
        self.con("Save")

        if game.use_jsonpickle:
            with open(os.path.normcase(
                    os.path.join(os.environ["APPDATA"],
                                 str(self))), 'w') as f:

                print(jsonpickle.encode(self.xyz), file=f)

                print(jsonpickle.encode(self.max_thing_id), file=f)
                print(jsonpickle.encode(self.all_things), file=f)
                print(jsonpickle.encode(self.is_biome_land), file=f)
                print(jsonpickle.encode(self.is_biome_dungeon), file=f)
                print(jsonpickle.encode(mm.CHUNK_WIDTH), file=f)
                print(jsonpickle.encode(mm.CHUNK_HEIGHT), file=f)

                print(jsonpickle.encode(self.is_snowy), file=f)
                print(jsonpickle.encode(self.is_grassy), file=f)
                print(jsonpickle.encode(self.is_watery), file=f)
        else:
            with open(os.path.normcase(
                    os.path.join(os.environ["APPDATA"],
                                 str(self))), 'wb') as f:
                pickle.dump(self.xyz, f, pickle.HIGHEST_PROTOCOL)

                pickle.dump(self.max_thing_id, f, pickle.HIGHEST_PROTOCOL)
                pickle.dump(self.all_things, f, pickle.HIGHEST_PROTOCOL)
                pickle.dump(self.is_biome_land, f, pickle.HIGHEST_PROTOCOL)
                pickle.dump(self.is_biome_dungeon, f, pickle.HIGHEST_PROTOCOL)
                pickle.dump(mm.CHUNK_WIDTH, f, pickle.HIGHEST_PROTOCOL)
                pickle.dump(mm.CHUNK_HEIGHT, f, pickle.HIGHEST_PROTOCOL)

                pickle.dump(self.is_snowy, f, pickle.HIGHEST_PROTOCOL)
                pickle.dump(self.is_grassy, f, pickle.HIGHEST_PROTOCOL)
                pickle.dump(self.is_watery, f, pickle.HIGHEST_PROTOCOL)

    def load(self):
        self.con("Load")

        if game.use_jsonpickle:
            with open(os.path.normcase(
                      os.path.join(os.environ["APPDATA"],
                                   str(self))), 'r') as f:

                self.xyz = jsonpickle.decode(f.readline())

                self.max_thing_id = jsonpickle.decode(f.readline())
                self.all_things = jsonpickle.decode(f.readline())
                self.is_biome_land = jsonpickle.decode(f.readline())
                self.is_biome_dungeon = jsonpickle.decode(f.readline())
                mm.CHUNK_WIDTH = jsonpickle.decode(f.readline())
                mm.CHUNK_HEIGHT = jsonpickle.decode(f.readline())

                self.is_snowy = jsonpickle.decode(f.readline())
                self.is_grassy = jsonpickle.decode(f.readline())
                self.is_watery = jsonpickle.decode(f.readline())
        else:
            with open(os.path.normcase(
                      os.path.join(os.environ["APPDATA"],
                                   str(self))), 'rb') as f:
                self.xyz = pickle.load(f)

                self.max_thing_id = pickle.load(f)
                self.all_things = pickle.load(f)
                self.is_biome_land = pickle.load(f)
                self.is_biome_dungeon = pickle.load(f)
                mm.CHUNK_WIDTH = pickle.load(f)
                mm.CHUNK_HEIGHT = pickle.load(f)

                self.is_snowy = pickle.load(f)
                self.is_grassy = pickle.load(f)
                self.is_watery = pickle.load(f)

        #
        # jsonpickle converts dict int kers into strings...
        #
        tmp = {}
        for thing_id in self.all_things:
            t = self.all_things[thing_id]
            tmp[int(thing_id)] = t
        self.all_things = tmp

        #
        # recreate the widgets for this thing
        #
        for thing_id in self.all_things:
            t = self.all_things[thing_id]
            t.loaded(self, self.level)

    def set_biome(self, is_land=False, is_dungeon=False):
        self.is_biome_land = is_land
        self.is_biome_dungeon = is_dungeon

    def tp_find(self, x, y, tp_name):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return None

        for t in self.on_map[x][y]:
            if t.tp.name == tp_name:
                return t

        return None

    def tp_is(self, x, y, value):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return None

        for t in self.on_map[x][y]:
            v = getattr(t.tp, value)
            if v is not None:
                if v:
                    return (x, y)

        return None

    def tp_is_where(self, value):
        for y in range(mm.CHUNK_HEIGHT):
            for x in range(mm.CHUNK_WIDTH):
                for t in self.on_map[x][y]:
                    v = getattr(t.tp, value)
                    if v is not None:
                        if v:
                            return (x, y)

        return None

    def is_movement_blocking_at(self, x, y):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return False

        for t in self.on_map[x][y]:
            if t.tp.is_movement_blocking:
                return True

        return False

    def thing_push(self, x, y, t):

        self.on_map[x][y].append(t)
        self.all_things[t.thing_id] = t

#        self.log("{0} push chunk {1}".format(t.thing_id, self))

    def thing_pop(self, x, y, t):

        self.on_map[x][y].remove(t)
        del self.all_things[t.thing_id]

#        self.log("{0} pop chunk {1}".format(t.thing_id, self))
