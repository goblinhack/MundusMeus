import pickle
import traceback
import mm
import os
import biome_dungeon
import biome_land
import copy
import random
import operator
import tp


class Chunk:

    class_version = 1

    def __init__(self, level, xyz, seed, cx, cy):

        self.version = self.__class__.class_version

        self.level = level
        self.where = copy.copy(xyz)
        self.seed = seed

        self.chunk_name = str(self)

        self.debug("New chunk")

        self.thing_id_per_level = 10000

        random.seed(self.seed)

        #
        # Unique IDs per chunk
        #
        self.max_thing_id = (xyz.y * mm.WORLD_WIDTH) + xyz.x
        self.max_thing_id *= xyz.z + mm.WORLD_DEPTH
        self.max_thing_id *= self.thing_id_per_level
        self.all_things = {}

        self.is_biome_land = False
        self.is_biome_dungeon = False

        self.is_snowy = False
        self.is_grassy = False
        self.is_watery = False
        self.update_pos(cx, cy)

        self.things_on_chunk = [[[] for x in range(mm.CHUNK_WIDTH)]
                                for y in range(mm.CHUNK_HEIGHT)]

        f = os.path.normcase(os.path.join(os.environ["APPDATA"], str(self)))
        if os.path.isfile(f):
            try:
                mm.log("Chunk {0}: Loading".format(self, f))
                self.load(cx, cy)
                need_new_chunk = False
            except Exception as inst:
                self.die("Chunk {0}: Loading failed, {1}, {2}".format(
                         self, f, str(inst)))
                need_new_chunk = True
        else:
            need_new_chunk = True

        if need_new_chunk:
            mm.log("Chunk {0}: Creating".format(str(self)))
            if self.where.z < 0:
                self.biome_create(is_dungeon=True, seed=seed)
            else:
                self.biome_create(is_land=True, seed=seed)

    #
    # We only keep a certain number of chunks active. This keeps track
    # of where the chunk is in the grid of chunks we display
    #
    def update_pos(self, cx, cy):
        self.cx = cx
        self.cy = cy
        self.base_x = cx * mm.CHUNK_WIDTH
        self.base_y = cy * mm.CHUNK_HEIGHT
        self.level.chunk[cx][cy] = self

    #
    # Set callbacks into biome code
    #
    def biome_set_vectors(self):
        self.biome_save = None
        self.biome_load = None

        if self.is_biome_dungeon:
            self.biome_save = biome_dungeon.biome_save
            self.biome_load = biome_dungeon.biome_load
            self.biome_build = biome_dungeon.biome_build
            self.biome_populate = biome_dungeon.biome_populate

        if self.is_biome_land:
            self.biome_build = biome_land.biome_build
            self.biome_populate = biome_land.biome_populate

    #
    # Create a random biome
    #
    def biome_create(self, seed, is_land=False, is_dungeon=False):

        self.is_biome_land = is_land
        self.is_biome_dungeon = is_dungeon

        self.biome_set_vectors()

        self.biome_seed = seed
        self.biome_name = "biome.{0}.{1}.seed.{2}".format(self.where.x,
                                                          self.where.y,
                                                          self.biome_seed)
        self.debug("Biome build")
        self.biome_build(self, seed)

        self.debug("Biome populate")
        self.biome_populate(self)

        self.debug("Biome create done")

    def __str__(self):
        return "level.{0}.seed.{1}".format(str(self.where), self.seed)

    def destroy(self):
        self.log("Destroying chunk {")

        #
        # to avoid dictionary changed size during iteration, walk the keys
        #
        for thing_id, t in self.all_things.items():

            #
            # Delete from the parent too
            #
            if thing_id in self.level.all_things:
                del self.level.all_things[thing_id]

            t.destroy()

        self.all_things = {}
        self.log("} Destroyed chunk")
        del self

    def log(self, msg):
        mm.log("Chunk {0}: {1}".format(str(self), msg))

    def con(self, msg):
        mm.con("Chunk {0}: {1}".format(str(self), msg))

    def debug(self, msg):
        return
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
        # self.log("Save")

        if self.biome_save:
            self.biome_save(self)

        with open(os.path.normcase(
                  os.path.join(os.environ["APPDATA"],
                               self.chunk_name)), 'wb') as f:
            pickle.dump(self.where, f, pickle.HIGHEST_PROTOCOL)

            pickle.dump(self.max_thing_id, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.all_things, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(mm.CHUNK_WIDTH, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(mm.CHUNK_HEIGHT, f, pickle.HIGHEST_PROTOCOL)

            pickle.dump(self.is_snowy, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.is_grassy, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.is_watery, f, pickle.HIGHEST_PROTOCOL)

            pickle.dump(self.seed, f, pickle.HIGHEST_PROTOCOL)

            pickle.dump(self.is_biome_land, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.is_biome_dungeon, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.biome_seed, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.biome_name, f, pickle.HIGHEST_PROTOCOL)

    def upgrade(self):

        if self.version < 2:
            self.v2_field = 2

#        self.debug("upgraded from ver {0} to {1}".format(
#                   self.version, self.__class__.class_version))

        self.version = self.__class__.class_version

    def load(self, cx, cy):
        #
        # Before we push any things on the map, make sure the parent
        # level knows where this chunk is
        #
        self.log("Load chunk at {0},{1}".format(cx, cy))
        self.update_pos(cx, cy)

        c = self.level.chunk_cache.get(self.chunk_name)
        if c is None:
            self.log("Load from disk")

            with open(os.path.normcase(
                        os.path.join(os.environ["APPDATA"],
                                     self.chunk_name)), 'rb') as f:
                self.where = pickle.load(f)

                self.max_thing_id = pickle.load(f)
                self.all_things = pickle.load(f)
                mm.CHUNK_WIDTH = pickle.load(f)
                mm.CHUNK_HEIGHT = pickle.load(f)

                self.is_snowy = pickle.load(f)
                self.is_grassy = pickle.load(f)
                self.is_watery = pickle.load(f)

                self.seed = pickle.load(f)

                self.is_biome_land = pickle.load(f)
                self.is_biome_dungeon = pickle.load(f)
                self.biome_seed = pickle.load(f)
                self.biome_name = pickle.load(f)
        else:
            self.log("Load from cache")

        if self.version != self.__class__.class_version:
            self.upgrade()

        self.biome_set_vectors()

        if self.biome_load:
            self.biome_load(self)

        #
        # recreate the widgets for this thing
        #
        for thing_id, t in self.all_things.items():
            t = self.all_things[thing_id]
            t.loaded(self, self.level)

    def scrolled_off(self):

        if self.biome_save:
            self.biome_save(self)

        for thing_id, t in list(self.all_things.items()):
            t.scrolled_off()

        self.level.chunk_cache[str(self)] = self

    def thing_find(self, x, y, tp_name):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return None

        for t in self.things_on_chunk[x][y]:
            if t.tp.name == tp_name:
                return t

        return None

    def thing_find_same_type(self, x, y, tp_name):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return None

        f = tp.all_tps[tp_name]
        for t in self.things_on_chunk[x][y]:
            if tp.same_type(f, t.tp):
                return t

        return None

    def thing_top(self, x, y):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return None

        for t in self.things_on_chunk[x][y]:
            tpp = t.tp

            if tpp.is_hidden:
                continue
            if tpp.is_hidden_from_editor:
                continue

            return t

        return None

    def things_at(self, x, y):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return []

        r = []
        for t in self.things_on_chunk[x][y]:
            tpp = t.tp

            if tpp.is_hidden:
                continue
            if tpp.is_hidden_from_editor:
                continue
            r.append(t)

        return r

    def tp_is(self, x, y, value):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return None

        for t in self.things_on_chunk[x][y]:
            v = getattr(t.tp, value)
            if v is not None:
                if v:
                    return (x, y)

        return None

    def tp_is_where(self, value):
        for y in range(mm.CHUNK_HEIGHT):
            for x in range(mm.CHUNK_WIDTH):
                for t in self.things_on_chunk[x][y]:
                    v = getattr(t.tp, value)
                    if v is not None:
                        if v:
                            return (x, y)

        return None

    def is_movement_blocking_at(self, x, y):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return False

        for t in self.things_on_chunk[x][y]:
            if t.tp.is_movement_blocking:
                return True

        return False

    def describe_position(self, x, y):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return ""

        #
        # If water then hide things like treasure.
        #
        hide_treasure = True
        for t in self.things_on_chunk[x][y]:
            if t.tp.is_floor or \
               t.tp.is_grass or \
               t.tp.is_lawn or \
               t.tp.is_carpet or \
               t.tp.is_dirt or \
               t.tp.is_snow or \
               t.tp.is_ice or \
               t.tp.is_gravel or \
               t.tp.is_gravel_snow or \
               t.tp.is_snow or \
               t.tp.is_sand or \
               t.tp.is_corridor or \
               t.tp.is_bridge or \
               t.tp.is_dusty:
                hide_treasure = False

        for t in self.things_on_chunk[x][y]:
            if t.tp.is_treasure:
                if hide_treasure:
                    continue

            if t.tp.long_name is not None:
                return t.tp.long_name.title()

        return ""

    def thing_push(self, x, y, t):

        #
        # When loading the game again, things think they are on
        # the chunk already and so we do a dup append. Just ignore
        # this.
        #
        l = self.things_on_chunk[x][y]
        if t in l:
            return

        l.append(t)

        # l.sort(key=lambda t: t.tp.z_depth, reverse=True)

        #
        # Allegedly this is faster:
        #
        func = operator.attrgetter("tp.z_depth")
        l.sort(key=func, reverse=True)

        self.all_things[t.thing_id] = t

    def thing_pop(self, x, y, t):

        self.things_on_chunk[x][y].remove(t)
        del self.all_things[t.thing_id]
