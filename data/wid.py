import traceback
import mm


class Wid:

    def __init__(self, name, parent=0, **kw):
        self.parent = parent
        self.name = name
        self.wid_id = mm.wid_new(self, parent, name, **kw)
        self.name = "{0:x}:{1}".format(self.wid_id, self.name)
        self.log("Created wid")

    def __str__(self):
        return "{0}".format(self.name)

    def destroy(self):
        self.log("Destroying wid")
        mm.wid_destroy(self)
        del self

    def set_shape(self, **kw):
        mm.wid_set_shape(self, **kw)

    def set_tl_br(self, **kw):
        mm.wid_set_tl_br(self, **kw)

    def set_tl_br_pct(self, x1, y1, x2, y2):
        mm.wid_set_tl_br_pct(self, x1, y1, x2, y2)

    def set_color(self, **kw):
        mm.wid_set_color(self, **kw)

    def set_text(self, **kw):
        mm.wid_set_text(self, **kw)
        
    def set_tex(self, **kw):
        mm.wid_set_tex(self, **kw)

    def to_back(self):
        mm.wid_lower(self)

    def to_front(self):
        mm.wid_raise(self)

    def update(self):
        mm.wid_update(self)

    def get_size(self):
        return mm.wid_get_size(self)

    def get_size_pct(self):
        return mm.wid_get_size_pct(self)

    def set_bevel(self, **kw):
        mm.wid_set_bevel(self, **kw)

    def set_bevelled(self, **kw):
        mm.wid_set_bevelled(self, **kw)

    def set_cursor(self, **kw):
        mm.wid_set_cursor(self, **kw)

    def set_do_not_lower(self, **kw):
        mm.wid_set_do_not_lower(self, **kw)

    def set_do_not_raise(self, **kw):
        mm.wid_set_do_not_raise(self, **kw)

    def set_focusable(self, **kw):
        mm.wid_set_focusable(self, **kw)

    def set_movable(self, **kw):
        mm.wid_set_movable(self, **kw)

    def set_movable_bounded(self, **kw):
        mm.wid_set_movable_bounded(self, **kw)

    def set_movable_horiz(self, **kw):
        mm.wid_set_movable_horiz(self, **kw)

    def set_movable_vert(self, **kw):
        mm.wid_set_movable_vert(self, **kw)

    def log(self, msg):
        mm.log("p-wid {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.err("p-wid {0}: ERROR: {1}".format(self.name, msg))
        traceback.print_stack()

    def dump(self):
        self.log("@ {0},{1}".format(self.x, self.y))
