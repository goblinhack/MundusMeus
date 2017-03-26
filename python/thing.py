import traceback
import mm
import wid
import tp
import game


class Thing:

    class_version = 2

    def __init__(self, tp_name, chunk=None, level=None, x=None, y=None):

        self.version = self.__class__.class_version
        self.v1_field = 1

        if x is not None:
            if chunk is not None:
                self.level = chunk.level
            else:
                self.level = level

            (chunk, offset_x, offset_y) = self.level.xy_to_chunk_xy(x, y)
        else:
            self.level = chunk.level

        self.chunk = chunk
        self.tp_name = tp_name

        chunk.max_thing_id += 1
        self.thing_id = chunk.max_thing_id

        if tp_name not in tp.all_tps:
            self.err("Thing template {0} does not exist".format(tp_name))

        self.tp = tp.all_tps[tp_name]

        if x is None:
            self.x = -1
            self.y = -1
        else:
            self.x = x
            self.y = y

        self.depth = 0

        #
        # When saved to disk, or cached, the chunk co-ords are saved as an
        # offset from the chunk
        #
        self.coords_are_relative_to_chunk_base = True

        self.on_chunk = False
        self.tilename = None

        #
        # Ordered list of locations to walk to
        #
        self.nexthops = []

        # self.debug("Created thing")

        #
        # Save on the level all things list. We can't save onto the chunk
        # yet as that depends on the thing co-ords which we do not have
        # until it is pushed.
        #
        if self.thing_id in self.level.all_things:
            self.die("thing ID {0} is already in the level list".format(
                     self.thing_id))
            return

        chunk.all_things[self.thing_id] = self
        self.level.all_things[self.thing_id] = self

        self.name = str(self)
        mm.thing_new(self, self.thing_id, tp_name)

        if self.tp.thing_init is not None:
            self.tp.thing_init(self)

    def __getstate__(self):
        result = self.__dict__.copy()

        if "tp" not in result:
            self.dump()
            self.die("Trying to save thing that has no template")
        del result['tp']

        if "level" not in result:
            self.dump()
            self.die("Trying to save thing that has no level")
        del result['level']

        if "chunk" not in result:
            self.dump()
            self.die("Trying to save thing that has no chunk")
        del result['chunk']

        if "wid" in result:
            del result['wid']

        #
        # Thing co-ords are saved as offsets
        #
        if self.coords_are_relative_to_chunk_base:
            result["x"] -= self.chunk.base_x
            result["y"] -= self.chunk.base_y

        if self.tp.is_player:
            self.log("Save player on chunk {0}".format(self.chunk))
        return result

    def __setstate__(self, dict):
        self.__dict__ = dict

    def __str__(self):
        return "{0}:{1}".format(self.thing_id, self.tp_name)

    def log(self, msg):
        mm.log("Thing {0}: {1}".format(str(self), msg))

    def con(self, msg):
        mm.con("Thing {0}: {1}".format(str(self), msg))

    def debug(self, msg):
        mm.log("Thing {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.con("".join(traceback.format_stack()))
        mm.err("Thing {0}: ERROR: {1}".format(self, msg))

    def die(self, msg):
        mm.con("".join(traceback.format_stack()))
        mm.die("Thing {0}: FATAL ERROR: {1}".format(self, msg))

    def dump(self):
        self.log("@ {0},{1} on chunk {2}".format(self.x, self.y, self.chunk))

    def destroy(self, reason="no reason"):

        if hasattr(self.tp, "thing_destroyed"):
            if self.tp.thing_destroyed is not None:
                self.tp.thing_destroyed(self)

        if self.on_chunk:
            self.pop()

        # self.debug("Destroying thing, {0}".format(reason) + " {")

        if self.thing_id in self.level.all_things:
            del self.level.all_things[self.thing_id]

        mm.thing_destroyed(self, reason)

        # self.debug("} " + "Destroyed thing, {0}".format(reason))
        del self

    def upgrade(self):

        if self.version < 2:
            self.v2_field = 2

#        self.debug("upgraded from ver {0} to {1}".format(
#                   self.version, self.__class__.class_version))

        self.version = self.__class__.class_version

    #
    # Loaded from save file into a chunk
    #
    def loaded(self, chunk, level):

        if self.version != self.__class__.class_version:
            self.upgrade()

        level.all_things[self.thing_id] = self

        self.chunk = chunk
        self.level = chunk.level
        self.tp = tp.all_tps[self.tp_name]

        #
        # Thing co-ords are saved as offsets
        #
        self.x += chunk.base_x
        self.y += chunk.base_y
        self.coords_are_relative_to_chunk_base = True

        mm.thing_new(self, self.thing_id, self.tp_name)

        if self.on_chunk:
            self.on_chunk = False
            self.push()
            if self.tilename is not None:
                self.set_tilename(self.tilename)

        if self.tp.is_player:
            game.g.player = self

#        self.log("Loaded thing on chunk {0}".format(self.chunk))
#        self.con("loaded thing at {0} {1}".format(self.x, self.y))
        if self.depth:
            self.set_depth(self.depth)

    #
    # Still associated with a chunk but not currently being rendered
    #
    def scrolled_off(self):

        del self.chunk.level.all_things[self.thing_id]

        #
        # Acts like saved to disk. When loaded again the chunk will have a
        # new position.
        #
        self.x -= self.chunk.base_x
        self.y -= self.chunk.base_y
        self.coords_are_relative_to_chunk_base = False

        mm.thing_destroyed(self, "scroll off")

    #
    # Move a thing and see it move smoothly on the map
    #
    def move(self, x, y):
        if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or x < 0 or y < 0:
            return

        self.update_pos(x, y)

        mm.thing_move(self, x, y)

    #
    # The map move is done in bulk in C, just update the move here
    #
    def update_pos(self, x, y):
        if self.chunk is None:
            self.die("thing has no chunk when trying to move")

        self.chunk.thing_pop(self.offset_x, self.offset_y, self)

        self.x = x
        self.y = y

        old_chunk = self.chunk
        (self.chunk, self.offset_x, self.offset_y) = \
            self.level.xy_to_chunk_xy(x, y)
        if old_chunk != self.chunk:
            if self == game.g.player:
                self.log("Moved from {0} to chunk {1}".format(
                         old_chunk, self.chunk))

        if self.chunk is None:
            self.die("thing has no chunk at new position {0}, {1}",
                     self.x, self.y)

        self.chunk.thing_push(self.offset_x, self.offset_y, self)

    #
    # Associate the thing with a given chunk
    #
    def push(self, x=None, y=None):

        if x is None:
            x = self.x
            y = self.y

        if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or x < 0 or y < 0:
            self.die("Out of bounds at {0},{1}".format(self.x, self.y))
            return

        self.on_chunk = True
        # self.debug("pushed")

        self.x = x
        self.y = y
        old_chunk = self.chunk
        (self.chunk, self.offset_x, self.offset_y) = \
            self.level.xy_to_chunk_xy(x, y)

        if old_chunk != self.chunk:
            if self == game.g.player:
                self.log("Push, moved from {0} to chunk {1}".format(
                        old_chunk, self.chunk))

        if self.chunk is None:
            self.die("Has no chunk during push onto {0}, {1}",
                     self.x, self.y)

        self.chunk.thing_push(self.offset_x, self.offset_y, self)

        self.wid_id = mm.thing_push(self, x, y)
        self.wid = wid.Wid(name=self.tp_name, wid_id=self.wid_id)
        self.wid.thing = self

        if hasattr(self.tp, "thing_pushed"):
            if self.tp.thing_pushed is not None:
                self.tp.thing_pushed(self)

    #
    # De_associate the thing with its chunk
    #
    def pop(self):
        if not self.on_chunk:
            self.err("Is not on the map")
            return
        self.on_chunk = False
        # self.debug("pop")

        self.chunk.thing_pop(self.offset_x, self.offset_y, self)
        mm.thing_pop(self)

    def set_long_name(self, value=""):
        self.long_name = value

    def set_tilename(self, name):
        self.tilename = name
        mm.thing_set_tilename(self, name)

    def set_tp(self, tp_name):
        self.tp = tp.all_tps[tp_name]
        mm.thing_set_tp(self, tp_name)

    def set_depth(self, value=0.0):
        self.depth = value
        mm.thing_set_depth(self, value)
