import traceback
import mm
import wid
import tp
import game


class Thing:

    def __init__(self, tp_name, chunk=None, level=None, x=None, y=None):

        if x is not None:
            (chunk, offset_x, offset_y) = level.xy_to_chunk_xy(x, y)
            self.level = level
            do_push = True
        else:
            self.level = chunk.level
            do_push = False

        self.chunk = chunk
        self.tp_name = tp_name

        game.g.max_thing_id += 1
        self.thing_id = game.g.max_thing_id
        self.name = "{0}:{1}".format(self.thing_id, self.tp_name)

        if tp_name not in tp.all_tps:
            self.err("Thing template {0} does not exist".format(tp_name))

        self.tp = tp.all_tps[tp_name]

        self.x = -1
        self.y = -1
        self.on_map = False
        self.tilename = None

        #
        # Ordered list of locations to walk to
        #
        self.nexthops = []

#        self.debug("Created thing")

        #
        # Save on the level all things list. We can't save onto the chunk
        # yet as that depends on the thing co-ords which we do not have
        # until it is pushed.
        #
        if self.thing_id in self.level.all_things:
            self.err("Already in level list")
            return

        chunk.all_things[self.thing_id] = self
        self.level.all_things[self.thing_id] = self

        mm.thing_new(self, self.thing_id, tp_name)

        if self.tp.thing_init is not None:
            self.tp.thing_init(self)

        if do_push:
            self.push(x, y)

    def __getstate__(self):
        result = self.__dict__.copy()

        if "tp" not in result:
            self.dump()
            mm.die("Thing has no template")
        del result['tp']

        if "level" not in result:
            self.dump()
            mm.die("Thing has no level")
        del result['level']

        if "wid" in result:
            del result['wid']

        return result

    def __setstate__(self, dict):
        self.__dict__ = dict

    def __str__(self):
        return "{0}:{1}".format(self.thing_id, self.tp_name)

    def destroy(self, reason="no reason"):
        if self.on_map:
            self.pop()

#        self.debug("Destroying thing, {0}".format(reason) + " {")

        if self.thing_id in self.level.all_things:
            del self.level.all_things[self.thing_id]

        mm.thing_destroyed(self, reason)

#        self.debug("} " + "Destroyed thing, {0}".format(reason))
        del self

    def log(self, msg):
        mm.log("p-thing {0}: {1}".format(str(self), msg))

    def debug(self, msg):
        return
        mm.log("p-thing {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.err("p-thing {0}: ERROR: {1}".format(self.name, msg))
        traceback.print_stack()

    def die(self, msg):
        mm.die("p-thing {0}: ERROR: {1}".format(self.name, msg))

    def dump(self):
        self.log("@ {0},{1}".format(self.x, self.y))

    def move(self, x, y):
        if self.chunk is None:
            self.die("thing has no chunk when trying to move")

        self.chunk.thing_pop(self.offset_x, self.offset_y, self)

        self.x = x
        self.y = y

        (self.chunk, self.offset_x, self.offset_y) = \
            self.level.xy_to_chunk_xy(x, y)
        self.chunk.thing_push(self.offset_x, self.offset_y, self)

        mm.thing_move(self, x, y)

    def update_pos(self, x, y):
        if self.chunk is None:
            self.die("thing has no chunk when trying to move")

        self.x = x
        self.y = y

    def push(self, x, y):
        if self.on_map:
            self.err("Already on the map at {0},{1}".format(self.x, self.y))
            return
        self.on_map = True

        self.x = x
        self.y = y
        (self.chunk, self.offset_x, self.offset_y) = \
            self.level.xy_to_chunk_xy(x, y)

        self.chunk.thing_push(self.offset_x, self.offset_y, self)

        self.wid_id = mm.thing_push(self, x, y)
        self.wid = wid.Wid(name=self.tp_name, wid_id=self.wid_id)
        self.wid.thing = self

        if hasattr(self.tp, "thing_pushed"):
            if self.tp.thing_pushed is not None:
                self.tp.thing_pushed(self)

    def pop(self):
        if not self.on_map:
            self.err("Is not on the map")
            return
        self.on_map = False

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
        mm.thing_set_depth(self, value)
