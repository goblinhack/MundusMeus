import traceback
import mm
import wid
import wid_text
import math


class WidPopup(wid.Wid):

    def __init__(self, name, 
                 x1=0.0,
                 y1=0.0, 
                 width=0.5,
                 height=0.0,
                 tiles=None,
                 parent=0, 
                 **kw):

        self.parent = parent
        self.name = name

        if tiles != None:
            super().__init__(name, tiles=tiles, parent=parent)
            self.tile_size = mm.tile_size_pct(tiles + "-tl")
            self.pad_w = self.tile_size[0] * 0.5
            self.pad_h = self.tile_size[1] * 0.5
        else:
            super().__init__(name, parent=parent)
            self.tile_size = None
            self.pad_w = 0
            self.pad_h = 0

        self.row_count = 0
        self.all_text = ""

        self.x1 = x1
        self.y1 = y1
        self.width = width
        self.height = height
        self.row_text = []
        self.row_font = []
        self.row_color = []
        self.row_center = []
        self.row_rhs = []
        self.row_width = []

        self.row_on_tooltip = []
        self.row_on_key_down = []
        self.row_on_key_up = []
        self.row_on_joy_button = []
        self.row_on_mouse_down = []
        self.row_on_mouse_up = []
        self.row_on_mouse_motion = []
        self.row_on_mouse_focus_begin = []
        self.row_on_mouse_focus_end = []
        self.row_on_mouse_over_begin = []
        self.row_on_mouse_over_end = []
        self.row_on_destroy = []
        self.row_on_destroy_begin = []
        self.row_on_tick = []
        self.row_on_display = []
        self.row_on_display_top_level = []

    def add_text(self, 
                 text, 
                 on_tooltip=None,
                 on_key_down=None,
                 on_key_up=None,
                 on_joy_button=None,
                 on_mouse_down=None,
                 on_mouse_up=None,
                 on_mouse_motion=None,
                 on_mouse_focus_begin=None,
                 on_mouse_focus_end=None,
                 on_mouse_over_begin=None,
                 on_mouse_over_end=None,
                 on_destroy=None,
                 on_destroy_begin=None,
                 on_tick=None,
                 on_display=None,
                 on_display_top_level=None,
                 font="small", 
                 color="white", 
                 center=False, 
                 rhs=False):

        self.all_text += text
        self.row_text.append(text)
        self.row_font.append(font)
        self.row_color.append(color)
        self.row_center.append(center)
        self.row_rhs.append(rhs)

        self.row_on_tooltip.append(on_tooltip)
        self.row_on_key_down.append(on_key_down)
        self.row_on_key_up.append(on_key_up)
        self.row_on_joy_button.append(on_joy_button)
        self.row_on_mouse_down.append(on_mouse_down)
        self.row_on_mouse_up.append(on_mouse_up)
        self.row_on_mouse_motion.append(on_mouse_motion)
        self.row_on_mouse_focus_begin.append(on_mouse_focus_begin)
        self.row_on_mouse_focus_end.append(on_mouse_focus_end)
        self.row_on_mouse_over_begin.append(on_mouse_over_begin)
        self.row_on_mouse_over_end.append(on_mouse_over_end)
        self.row_on_destroy.append(on_destroy)
        self.row_on_destroy_begin.append(on_destroy_begin)
        self.row_on_tick.append(on_tick)
        self.row_on_display.append(on_display)
        self.row_on_display_top_level.append(on_display_top_level)

        self.row_count += 1

    def update(self):

        text_w, text_h, self.row_width = wid_text.text_size_pct(row_text=self.row_text,
                                                                row_font=self.row_font,
                                                                width=self.width)
        self.width = self.width
        if self.height == 0:
            self.height = text_h

        self.width += self.pad_w * 2
        self.height += self.pad_h * 2

        self.set_tl_br_pct(self.x1, self.y1, 
                           self.x1 + self.width,
                           self.y1 + self.height)

        inner_pad_w = self.pad_w
        inner_pad_h = self.pad_h
        inner_pad_w *= 1.0 / self.width
        inner_pad_h *= 1.0 / self.height

        textbox_x1 = inner_pad_w
        textbox_y1 = inner_pad_h * 0.9 # to account for the widget shadow
        textbox_x2 = 1.0 - inner_pad_w
        textbox_y2 = 1.0 - inner_pad_h * 0.9 # again, shadow padding

        self.text_box = wid_text.WidText(name="textbox", 
                                         row_text=self.row_text,
                                         row_font=self.row_font,
                                         row_color=self.row_color,
                                         row_center=self.row_center,
                                         row_rhs=self.row_rhs,
                                         row_width=self.row_width,
                                         row_on_tooltip=self.row_on_tooltip,
                                         row_on_key_down=self.row_on_key_down,
                                         row_on_key_up=self.row_on_key_up,
                                         row_on_joy_button=self.row_on_joy_button,
                                         row_on_mouse_down=self.row_on_mouse_down,
                                         row_on_mouse_up=self.row_on_mouse_up,
                                         row_on_mouse_motion=self.row_on_mouse_motion,
                                         row_on_mouse_focus_begin=self.row_on_mouse_focus_begin,
                                         row_on_mouse_focus_end=self.row_on_mouse_focus_end,
                                         row_on_mouse_over_begin=self.row_on_mouse_over_begin,
                                         row_on_mouse_over_end=self.row_on_mouse_over_end,
                                         row_on_destroy=self.row_on_destroy,
                                         row_on_destroy_begin=self.row_on_destroy_begin,
                                         row_on_tick=self.row_on_tick,
                                         row_on_display=self.row_on_display,
                                         row_on_display_top_level=self.row_on_display_top_level,
                                         parent=self.wid_id,
                                         x1=textbox_x1, y1=textbox_y1, 
                                         x2=textbox_x2, y2=textbox_y2)

        super().update()

        mm.wid_new_scrollbar(vert=True, parent=self.wid_id, owner=self.text_box.wid_id)

