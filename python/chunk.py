import pickle
import traceback
import mm
import math
import game
import tp
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
                mm.con("Chunk {0} loading".format(f))
                self.load()
                need_new_chunk = False
                mm.con("Loaded chunk @ {0} success".format(f))
            except Exception as inst:
                mm.con("Loading chunk failed, error [{0}]".format(inst))
                need_new_chunk = True
        else:
            need_new_chunk = True

        if need_new_chunk:
            mm.con("Chunk {0} creating".format(str(self)))
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
        mm.log("p-chunk: {0}: {1}".format(str(self), msg))

    def debug(self, msg):
        mm.log("p-chunk: {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.con("".join(traceback.format_stack()))
        mm.err("p-chunk: {0}: ERROR: {1}".format(str(self), msg))

    def die(self, msg):
        mm.con("".join(traceback.format_stack()))
        mm.die("p-chunk: {0}: ERROR: {1}".format(str(self), msg))

    def dump(self):
        for i in self.all_things:
            self.all_things[i].dump()

    def save(self):
        self.debug("Save chunk")

        with open(os.path.normcase(
                  os.path.join(os.environ["APPDATA"],
                               str(self))), 'wb') as f:
            pickle.dump(self.xyz, f, pickle.HIGHEST_PROTOCOL)

            pickle.dump(self.all_things, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.is_biome_land, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.is_biome_dungeon, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(mm.CHUNK_WIDTH, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(mm.CHUNK_HEIGHT, f, pickle.HIGHEST_PROTOCOL)

            pickle.dump(self.is_snowy, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.is_grassy, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.is_watery, f, pickle.HIGHEST_PROTOCOL)

    def load(self):
        self.debug("Load chunk")

        with open(os.path.normcase(
                  os.path.join(os.environ["APPDATA"],
                               str(self))), 'rb') as f:
            self.xyz = pickle.load(f)

            self.all_things = pickle.load(f)
            self.is_biome_land = pickle.load(f)
            self.is_biome_dungeon = pickle.load(f)
            mm.CHUNK_WIDTH = pickle.load(f)
            mm.CHUNK_HEIGHT = pickle.load(f)

            self.is_snowy = pickle.load(f)
            self.is_grassy = pickle.load(f)
            self.is_watery = pickle.load(f)

            for thing_id in self.all_things:
                t = self.all_things[thing_id]
                self.level.all_things[thing_id] = t
                mm.thing_new(t, thing_id, t.tp_name)

            mm.con("Pushing things on chunk @ {0}".format(str(self)))

            for thing_id in self.all_things:
                t = self.all_things[thing_id]
                t.chunk = self
                t.level = self.level
                t.tp = tp.all_tps[t.tp_name]

                if t.on_map:
                    t.on_map = False
                    t.push(t.x, t.y)
                    if t.tilename is not None:
                        t.set_tilename(t.tilename)

                if t.tp.is_player:
                    game.g.player = t

            if game.g.player is None:
                raise NameError("No player found on chunk")

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
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            mm.err("thing_push: map oob {0} {1}".format(x, y))
            return

        self.on_map[x][y].append(t)
        self.all_things[t.thing_id] = t

    def thing_pop(self, x, y, t):
        if x >= mm.CHUNK_WIDTH or y >= mm.CHUNK_HEIGHT or x < 0 or y < 0:
            mm.err("thing_push: map oob {0} {1}".format(x, y))
            return

        self.on_map[x][y].remove(t)
        if t.thing_id in self.all_things:
            del self.all_things[t.thing_id]
