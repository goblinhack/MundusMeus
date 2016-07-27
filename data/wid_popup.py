import traceback
import mm
import wid
import wid_text


class WidPopup(wid.Wid):

    def __init__(self, name, 
                 x1, y1, 
                 width=0.0,
                 font="small",
                 tiles=None,
                 parent=0, 
                 **kw):

        self.parent = parent
        self.name = name
        self.font = font

        if tiles != None:
            super().__init__(name, tiles=tiles, parent=parent)
            tile_size = mm.tile_size_pct(tiles + "-tl")
            self.pad_w = tile_size[0] * 0.5
            self.pad_h = tile_size[1] * 0.5
        else:
            super().__init__(name, parent=parent)
            self.pad_w = 0
            self.pad_h = 0

        self.text_index = 0
        self.all_text = ""

        self.x1 = x1
        self.y1 = y1
        self.width = width

    def add_text(self, text):

        self.text_index += 1
        self.all_text += text

    def update(self):

        text_w, text_h = wid_text.text_size_pct(text=self.all_text, font=self.font)

        self.h = text_h + self.pad_h * 2

        if self.width != 0.0:
            self.w = self.width + self.pad_w * 2
        else:
            self.w = text_w + self.pad_w * 2

        self.set_tl_br_pct(self.x1, self.y1, 
                           self.x1 + self.w,
                           self.y1 + self.h)

        inner_pad_w = self.pad_w
        inner_pad_h = self.pad_h
        inner_pad_w *= 1.0 / self.w
        inner_pad_h *= 1.0 / self.h

        self.text_box = wid_text.WidText(name="textbox", 
                                         text=self.all_text,
                                         font=self.font,
                                         parent=self.wid_id,
                                         x1=inner_pad_w,
                                         y1=inner_pad_h,
                                         x2=1.0 - inner_pad_w,
                                         y2=1.0 - inner_pad_h)

        super().update()

        mm.wid_new_scrollbar(vert=True, parent=self.wid_id, owner=self.text_box.wid_id)

