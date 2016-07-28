import traceback
import mm
import wid
import wid_text


class WidPopup(wid.Wid):

    def __init__(self, name, 
                 x1, y1, 
                 width=0.0,
                 tiles=None,
                 parent=0, 
                 **kw):

        self.parent = parent
        self.name = name

        if tiles != None:
            super().__init__(name, tiles=tiles, parent=parent)
            tile_size = mm.tile_size_pct(tiles + "-tl")
            self.pad_w = tile_size[0] * 0.5
            self.pad_h = tile_size[1] * 0.5
        else:
            super().__init__(name, parent=parent)
            self.pad_w = 0
            self.pad_h = 0

        self.row_count = 0
        self.all_text = ""

        self.x1 = x1
        self.y1 = y1
        self.width = width
        self.row_text = []
        self.row_font = []
        self.row_color = []
        self.row_center = []
        self.row_rhs = []
        self.row_width = []

    def add_text(self, text, font="small", color="white", center=False, rhs=False):

        self.all_text += text
        self.row_text.append(text)
        self.row_font.append(font)
        self.row_color.append(color)
        self.row_center.append(center)
        self.row_rhs.append(rhs)
        self.row_count += 1

    def update(self):

        text_w, text_h, self.row_width = wid_text.text_size_pct(row_text=self.row_text,
                                                                row_font=self.row_font)

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

        textbox_x1 = inner_pad_w
        textbox_y1 = inner_pad_h * 0.8 # to account for the widget shadow
        textbox_x2 = 1.0 - inner_pad_w
        textbox_y2 = 1.0 - inner_pad_h * 0.9 # again, shadow padding

        self.text_box = wid_text.WidText(name="textbox", 
                                         row_text=self.row_text,
                                         row_font=self.row_font,
                                         row_color=self.row_color,
                                         row_center=self.row_center,
                                         row_rhs=self.row_rhs,
                                         row_width=self.row_width,
                                         parent=self.wid_id,
                                         x1=textbox_x1, y1=textbox_y1, 
                                         x2=textbox_x2, y2=textbox_y2)

        super().update()

        mm.wid_new_scrollbar(vert=True, parent=self.wid_id, owner=self.text_box.wid_id)

