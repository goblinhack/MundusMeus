import pickle
import traceback
import mm
import game
import os
import biome_dungeon
import biome_land
import copy


class Chunk:

    def __init__(self, level, xyz, cx, cy):

        self.level = level
        self.xyz = copy.copy(xyz)
        self.chunk_name = str(self)

        self.debug("New chunk")

        self.thing_id_per_level = 10000

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
            if self.xyz.z < 0:
                self.biome_create(is_dungeon=True, seed=game.g.seed)
            else:
                self.biome_create(is_land=True, seed=game.g.seed)

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
        self.log("Save")

        with open(os.path.normcase(
                  os.path.join(os.environ["APPDATA"],
                               self.chunk_name)), 'wb') as f:
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
        else:
            self.log("Load from cache")

        #
        # recreate the widgets for this thing
        #
        for thing_id, t in self.all_things.items():
            t = self.all_things[thing_id]
            t.loaded(self, self.level)

    def scrolled_off(self):
        for thing_id, t in list(self.all_things.items()):
            t.scrolled_off()

        self.level.chunk_cache[str(self)] = self

    def tp_find(self, x, y, tp_name):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            return None

        for t in self.things_on_chunk[x][y]:
            if t.tp.name == tp_name:
                return t

        return None

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

    def thing_push(self, x, y, t):

        self.things_on_chunk[x][y].append(t)
        self.all_things[t.thing_id] = t

#        self.log("{0} push chunk {1}".format(t.thing_id, self))

    def thing_pop(self, x, y, t):

        self.things_on_chunk[x][y].remove(t)
        del self.all_things[t.thing_id]

#        self.log("{0} pop chunk {1}".format(t.thing_id, self))
