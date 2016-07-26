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

        super().__init__(name, tiles=tiles, parent=parent)
        self.set_tl_br_pct(x1, y1, x2, y2)

        tile_size = mm.tile_size_pct(tiles + "-tl")
        pad_w = tile_size[0] * 0.5
        pad_h = tile_size[1] * 0.5

        w = wid_text.WidText(name="textbox", 
                             text=text,
                             pad_w=pad_w,
                             pad_h=pad_w,
                             font=font,
                             parent=self.wid_id,
                             x1=0, y1=0, x2=1, y2=1)


