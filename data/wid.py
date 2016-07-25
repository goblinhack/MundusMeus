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

    def set_tl_br_pct(self, **kw):
        mm.wid_set_tl_br_pct(self, **kw)

    def set_color(self, **kw):
        mm.wid_set_color(self, **kw)

    def set_text(self, **kw):
        mm.wid_set_text(self, **kw)
        
    def set_tex(self, **kw):
        mm.wid_set_tex(self, **kw)

    def log(self, msg):
        mm.log("p-wid {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.err("p-wid {0}: ERROR: {1}".format(self.name, msg))
        traceback.print_stack()

    def dump(self):
        self.log("@ {0},{1}".format(self.x, self.y))
