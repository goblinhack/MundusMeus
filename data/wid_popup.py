import traceback
import mm
import wid
import wid_text


class WidPopup(wid.Wid):

    def __init__(self, name, 
                 x1, y1, x2, y2, 
                 text, 
                 font="small",
                 tiles=None,
                 parent=0, 
                 **kw):

        self.parent = parent
        self.name = name

        if tiles != None:
            super().__init__(name, tiles=tiles, parent=parent)
            tile_size = mm.tile_size_pct(tiles + "-tl")
            pad_w = tile_size[0] * 0.5
            pad_h = tile_size[1] * 0.5
        else:
            super().__init__(name, parent=parent)
            pad_w = 0
            pad_h = 0

        self.set_tl_br_pct(x1, y1, x2, y2)

        w = wid_text.WidText(name="textbox", 
                             text=text,
                             font=font,
                             parent=self.wid_id,
                             x1=pad_w, 
                             y1=pad_h, 
                             x2=1.0 - pad_w, 
                             y2=1.0 - pad_h)

        mm.wid_new_scrollbar(vert=True, parent=self.wid_id, owner=w.wid_id)

