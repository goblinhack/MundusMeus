import mm
import wid
import wid_text


class WidPopup(wid.Wid):

    def __init__(self, name,
                 x1=0.0,
                 y1=0.0,
                 width=0.5,
                 height=0.0,
                 tiles=None,
                 title_tiles=None,
                 body_tiles=None,
                 parent=0,
                 row_font=None,
                 row_color=None,
                 row_center=False,
                 row_rhs=False,
                 row_on_tooltip=None,
                 row_tooltip=None,
                 row_on_key_down=None,
                 row_on_key_sym=None,
                 row_on_key_mod=None,
                 row_on_key_up=None,
                 row_on_joy_button=None,
                 row_on_m_down=None,
                 row_on_m_up=None,
                 row_on_m_motion=None,
                 row_on_m_focus_b=None,
                 row_on_m_focus_e=None,
                 row_on_m_over_b=None,
                 row_on_m_over_e=None,
                 row_on_destroy=None,
                 row_on_destroy_b=None,
                 row_on_tick=None,
                 row_on_button_list=None,
                 row_on_display=None,
                 row_on_display_win=None,
                 title_font=None,
                 title_color=None,
                 title_center=False,
                 title_rhs=False,
                 title_on_tooltip=None,
                 title_tooltip=None,
                 title_on_key_down=None,
                 title_on_key_sym=None,
                 title_on_key_mod=None,
                 title_on_key_up=None,
                 title_on_joy_button=None,
                 title_on_m_down=None,
                 title_on_m_up=None,
                 title_on_m_motion=None,
                 title_on_m_focus_b=None,
                 title_on_m_focus_e=None,
                 title_on_m_over_b=None,
                 title_on_m_over_e=None,
                 title_on_destroy=None,
                 title_on_destroy_b=None,
                 title_on_tick=None,
                 title_on_button_list=None,
                 title_on_display=None,
                 title_on_display_win=None,
                 **kw):

        self.parent = parent
        self.name = name
        self.global_row_font = row_font
        self.global_row_color = row_color
        self.global_row_center = row_center
        self.global_row_rhs = row_rhs
        self.global_row_on_tooltip = row_on_tooltip
        self.global_row_tooltip = row_tooltip
        self.global_row_on_key_down = row_on_key_down
        self.global_row_on_key_sym = row_on_key_sym
        self.global_row_on_key_mod = row_on_key_mod
        self.global_row_on_key_up = row_on_key_up
        self.global_row_on_joy_button = row_on_joy_button
        self.global_row_on_m_down = row_on_m_down
        self.global_row_on_m_up = row_on_m_up
        self.global_row_on_m_motion = row_on_m_motion
        self.global_row_on_m_focus_b = row_on_m_focus_b
        self.global_row_on_m_focus_e = row_on_m_focus_e
        self.global_row_on_m_over_b = row_on_m_over_b
        self.global_row_on_m_over_e = row_on_m_over_e
        self.global_row_on_destroy = row_on_destroy
        self.global_row_on_destroy_b = row_on_destroy_b
        self.global_row_on_tick = row_on_tick
        self.global_row_on_button_list = row_on_button_list
        self.global_row_on_display = row_on_display
        self.global_row_on_display_win = row_on_display_win
        self.global_title_font = title_font
        self.global_title_color = title_color
        self.global_title_center = title_center
        self.global_title_rhs = title_rhs
        self.global_title_on_tooltip = title_on_tooltip
        self.global_title_tooltip = title_tooltip
        self.global_title_on_key_down = title_on_key_down
        self.global_title_on_key_sym = title_on_key_sym
        self.global_title_on_key_mod = title_on_key_mod
        self.global_title_on_key_up = title_on_key_up
        self.global_title_on_joy_button = title_on_joy_button
        self.global_title_on_m_down = title_on_m_down
        self.global_title_on_m_up = title_on_m_up
        self.global_title_on_m_motion = title_on_m_motion
        self.global_title_on_m_focus_b = title_on_m_focus_b
        self.global_title_on_m_focus_e = title_on_m_focus_e
        self.global_title_on_m_over_b = title_on_m_over_b
        self.global_title_on_m_over_e = title_on_m_over_e
        self.global_title_on_destroy = title_on_destroy
        self.global_title_on_destroy_b = title_on_destroy_b
        self.global_title_on_tick = title_on_tick
        self.global_title_on_button_list = title_on_button_list
        self.global_title_on_display = title_on_display
        self.global_title_on_display_win = title_on_display_win

        if tiles is not None:
            super().__init__(name, tiles=tiles, parent=parent)
            self.tile_size = mm.tile_size_pct(tiles + "_tl")
            self.pad_w = self.tile_size[0] * 0.5
            self.pad_h = self.tile_size[1] * 0.5
        else:
            super().__init__(name, parent=parent)
            self.tile_size = None
            self.pad_w = 0
            self.pad_h = 0

        self.x1 = x1
        self.y1 = y1
        self.width = width
        self.height = height
        self.inner_width = width - self.pad_w * 2
        self.inner_height = height - self.pad_h * 2
        self.debug = False

        self.row_count = 0
        self.row_text = []
        self.row_font = []
        self.row_color = []
        self.row_center = []
        self.row_rhs = []
        self.row_width = []
        self.line_width = []
        self.row_on_tooltip = []
        self.row_tooltip = []
        self.row_on_key_down = []
        self.row_on_key_sym = []
        self.row_on_key_mod = []
        self.row_on_key_up = []
        self.row_on_joy_button = []
        self.row_on_m_down = []
        self.row_on_m_up = []
        self.row_on_m_motion = []
        self.row_on_m_focus_b = []
        self.row_on_m_focus_e = []
        self.row_on_m_over_b = []
        self.row_on_m_over_e = []
        self.row_on_destroy = []
        self.row_on_destroy_b = []
        self.row_on_tick = []
        self.row_on_button_list = []
        self.row_on_display = []
        self.row_on_display_win = []

        self.title_count = 0
        self.title_text = []
        self.title_font = []
        self.title_color = []
        self.title_center = []
        self.title_rhs = []
        self.title_width = []
        self.title_line_width = []
        self.title_on_tooltip = []
        self.title_tooltip = []
        self.title_on_key_down = []
        self.title_on_key_sym = []
        self.title_on_key_mod = []
        self.title_on_key_up = []
        self.title_on_joy_button = []
        self.title_on_m_down = []
        self.title_on_m_up = []
        self.title_on_m_motion = []
        self.title_on_m_focus_b = []
        self.title_on_m_focus_e = []
        self.title_on_m_over_b = []
        self.title_on_m_over_e = []
        self.title_on_destroy = []
        self.title_on_destroy_b = []
        self.title_on_tick = []
        self.title_on_button_list = []
        self.title_on_display = []
        self.title_on_display_win = []

        self.title_tiles = title_tiles
        self.body_tiles = body_tiles

        self.scrollbar = None

    def add_text(self,
                 text,
                 on_tooltip=None,
                 tooltip=None,
                 on_key_down=None,
                 on_key_sym=None,
                 on_key_mod=None,
                 on_key_up=None,
                 on_joy_button=None,
                 on_m_down=None,
                 on_m_up=None,
                 on_m_motion=None,
                 on_m_focus_b=None,
                 on_m_focus_e=None,
                 on_m_over_b=None,
                 on_m_over_e=None,
                 on_destroy=None,
                 on_destroy_b=None,
                 on_tick=None,
                 on_button_list=None,
                 on_display=None,
                 on_display_win=None,
                 title=False,
                 font="small",
                 color="white",
                 center=False,
                 rhs=False,
                 debug=False):

        if debug:
            self.debug = debug

        if title is False:
            if font is None:
                font = self.global_row_font
            if color is None:
                color = self.global_row_color
            if center is False:
                center = self.global_row_center
            if rhs is None:
                rhs = self.global_row_rhs
            if on_tooltip is None:
                on_tooltip = self.global_row_on_tooltip
            if tooltip is None:
                tooltip = self.global_row_tooltip
            if on_key_down is None:
                on_key_down = self.global_row_on_key_down
            if on_key_sym is None:
                on_key_sym = self.global_row_on_key_sym
            if on_key_mod is None:
                on_key_mod = self.global_row_on_key_mod
            if on_key_up is None:
                on_key_up = self.global_row_on_key_up
            if on_joy_button is None:
                on_joy_button = self.global_row_on_joy_button
            if on_m_down is None:
                on_m_down = self.global_row_on_m_down
            if on_m_up is None:
                on_m_up = self.global_row_on_m_up
            if on_m_motion is None:
                on_m_motion = self.global_row_on_m_motion
            if on_m_focus_b is None:
                on_m_focus_b = self.global_row_on_m_focus_b
            if on_m_focus_e is None:
                on_m_focus_e = self.global_row_on_m_focus_e
            if on_m_over_b is None:
                on_m_over_b = self.global_row_on_m_over_b
            if on_m_over_e is None:
                on_m_over_e = self.global_row_on_m_over_e
            if on_destroy is None:
                on_destroy = self.global_row_on_destroy
            if on_destroy_b is None:
                on_destroy_b = self.global_row_on_destroy_b
            if on_tick is None:
                on_tick = self.global_row_on_tick
            if on_button_list is None:
                on_button_list = self.global_row_on_button_list
            if on_display is None:
                on_display = self.global_row_on_display
            if on_display_win is None:
                on_display_win = self.global_row_on_display_win

            self.row_text.append(text)
            self.row_font.append(font)
            self.row_color.append(color)
            self.row_center.append(center)
            self.row_rhs.append(rhs)
            self.row_on_tooltip.append(on_tooltip)
            self.row_tooltip.append(tooltip)
            self.row_on_key_down.append(on_key_down)
            self.row_on_key_sym.append(on_key_sym)
            self.row_on_key_mod.append(on_key_mod)
            self.row_on_key_up.append(on_key_up)
            self.row_on_joy_button.append(on_joy_button)
            self.row_on_m_down.append(on_m_down)
            self.row_on_m_up.append(on_m_up)
            self.row_on_m_motion.append(on_m_motion)
            self.row_on_m_focus_b.append(on_m_focus_b)
            self.row_on_m_focus_e.append(on_m_focus_e)
            self.row_on_m_over_b.append(on_m_over_b)
            self.row_on_m_over_e.append(on_m_over_e)
            self.row_on_destroy.append(on_destroy)
            self.row_on_destroy_b.append(on_destroy_b)
            self.row_on_tick.append(on_tick)
            self.row_on_button_list.append(on_button_list)
            self.row_on_display.append(on_display)
            self.row_on_display_win.append(on_display_win)
            self.row_count += 1

        else:

            if font is None:
                font = self.global_title_font
            if color is None:
                color = self.global_title_color
            if center is False:
                center = self.global_title_center
            if rhs is None:
                rhs = self.global_title_rhs
            if on_tooltip is None:
                on_tooltip = self.global_title_on_tooltip
            if tooltip is None:
                tooltip = self.global_title_tooltip
            if on_key_down is None:
                on_key_down = self.global_title_on_key_down
            if on_key_sym is None:
                on_key_sym = self.global_title_on_key_sym
            if on_key_mod is None:
                on_key_mod = self.global_title_on_key_mod
            if on_key_up is None:
                on_key_up = self.global_title_on_key_up
            if on_joy_button is None:
                on_joy_button = self.global_title_on_joy_button
            if on_m_down is None:
                on_m_down = self.global_title_on_m_down
            if on_m_up is None:
                on_m_up = self.global_title_on_m_up
            if on_m_motion is None:
                on_m_motion = self.global_title_on_m_motion
            if on_m_focus_b is None:
                on_m_focus_b = self.global_title_on_m_focus_b
            if on_m_focus_e is None:
                on_m_focus_e = self.global_title_on_m_focus_e
            if on_m_over_b is None:
                on_m_over_b = self.global_title_on_m_over_b
            if on_m_over_e is None:
                on_m_over_e = self.global_title_on_m_over_e
            if on_destroy is None:
                on_destroy = self.global_title_on_destroy
            if on_destroy_b is None:
                on_destroy_b = self.global_title_on_destroy_b
            if on_tick is None:
                on_tick = self.global_title_on_tick
            if on_button_list is None:
                on_button_list = self.global_title_on_button_list
            if on_display is None:
                on_display = self.global_title_on_display

            self.title_text.append(text)
            self.title_font.append(font)
            self.title_color.append(color)
            self.title_center.append(center)
            self.title_rhs.append(rhs)
            self.title_on_tooltip.append(on_tooltip)
            self.title_tooltip.append(tooltip)
            self.title_on_key_down.append(on_key_down)
            self.title_on_key_sym.append(on_key_sym)
            self.title_on_key_mod.append(on_key_mod)
            self.title_on_key_up.append(on_key_up)
            self.title_on_joy_button.append(on_joy_button)
            self.title_on_m_down.append(on_m_down)
            self.title_on_m_up.append(on_m_up)
            self.title_on_m_motion.append(on_m_motion)
            self.title_on_m_focus_b.append(on_m_focus_b)
            self.title_on_m_focus_e.append(on_m_focus_e)
            self.title_on_m_over_b.append(on_m_over_b)
            self.title_on_m_over_e.append(on_m_over_e)
            self.title_on_destroy.append(on_destroy)
            self.title_on_destroy_b.append(on_destroy_b)
            self.title_on_tick.append(on_tick)
            self.title_on_button_list.append(on_button_list)
            self.title_on_display.append(on_display)
            self.title_on_display_win.append(on_display_win)
            self.title_count += 1

    def update(self):

        if self.title_count > 0:
            title_w, title_h, self.title_width, self.title_line_width =  \
                wid_text.text_size_pct(row_text=self.title_text,
                                       row_font=self.title_font,
                                       width=self.inner_width)
        else:
            title_h, self.title_width, self.title_line_width =  \
                0, 0, 0

        if self.row_count > 0:
            body_w, body_h, self.row_width, self.line_width =  \
                wid_text.text_size_pct(row_text=self.row_text,
                                       row_font=self.row_font,
                                       width=self.inner_width)
        else:
            body_h, self.row_width, self.line_width =  \
                0, 0, 0

        if self.height == 0:
            self.height = title_h + body_h + self.pad_h * 2
            self.inner_height = title_h + body_h

        need_scrollbar = False
        if title_h + body_h > self.height - self.pad_h * 2:
            need_scrollbar = True

        self.set_pos_pct(self.x1, self.y1,
                         self.x1 + self.width,
                         self.y1 + self.height)

        inner_pad_w = self.pad_w
        inner_pad_h = self.pad_h
        inner_pad_w *= 1.0 / self.width
        inner_pad_h *= 1.0 / self.height

        title_h *= 1.0 / self.height
        body_h *= 1.0 / self.height

        if self.row_count > 0:

            if self.body_tiles is not None:
                textbox_x1 = inner_pad_w * 0.4
                textbox_x2 = 1.0 - inner_pad_w * 0.4

                # to account for the widget shadow
                textbox_y1 = inner_pad_h * 0.4

                textbox_y1 += title_h

                # again, shadow padding
                textbox_y2 = 1.0 - inner_pad_h * 0.4

                w = wid.Wid(name="textbox tiles",
                            tiles=self.body_tiles,
                            parent=self.wid_id)

                w.set_color(tl=True, bg=True, br=True, name="white")
                w.set_pos_pct(x1=textbox_x1, y1=textbox_y1,
                              x2=textbox_x2, y2=textbox_y2)

            textbox_x1 = inner_pad_w
            textbox_x2 = 1.0 - inner_pad_w

            # to account for the widget shadow
            textbox_y1 = inner_pad_h * 0.9

            textbox_y1 += title_h

            # again, shadow padding
            textbox_y2 = 1.0 - inner_pad_h * 0.9

            self.text_box = \
                wid_text.WidText(name="textbox",
                                 row_text=self.row_text,
                                 row_font=self.row_font,
                                 row_color=self.row_color,
                                 row_center=self.row_center,
                                 row_rhs=self.row_rhs,
                                 row_width=self.row_width,
                                 line_width=self.line_width,
                                 row_on_tooltip=self.row_on_tooltip,
                                 row_tooltip=self.row_tooltip,
                                 row_on_key_down=self.row_on_key_down,
                                 row_on_key_sym=self.row_on_key_sym,
                                 row_on_key_mod=self.row_on_key_mod,
                                 row_on_key_up=self.row_on_key_up,
                                 row_on_joy_button=self.row_on_joy_button,
                                 row_on_m_down=self.row_on_m_down,
                                 row_on_m_up=self.row_on_m_up,
                                 row_on_m_motion=self.row_on_m_motion,
                                 row_on_m_focus_b=self.row_on_m_focus_b,
                                 row_on_m_focus_e=self.row_on_m_focus_e,
                                 row_on_m_over_b=self.row_on_m_over_b,
                                 row_on_m_over_e=self.row_on_m_over_e,
                                 row_on_destroy=self.row_on_destroy,
                                 row_on_destroy_b=self.row_on_destroy_b,
                                 row_on_tick=self.row_on_tick,
                                 row_on_button_list=self.row_on_button_list,
                                 row_on_display=self.row_on_display,
                                 row_on_display_win=self.row_on_display_win,
                                 parent=self.wid_id,
                                 x1=textbox_x1, y1=textbox_y1,
                                 x2=textbox_x2, y2=textbox_y2,
                                 debug=self.debug)

        if need_scrollbar is True:
            self.scrollbar = wid.Wid(name="wid popup scroll",
                                     is_scrollbar=True,
                                     vert=True,
                                     parent=self.wid_id,
                                     owner=self.text_box.wid_id)
        if self.title_count > 0:

            if self.title_tiles is not None:
                textbox_x1 = inner_pad_w * 0.4
                textbox_x2 = 1.0 - inner_pad_w * 0.4

                # to account for the widget shadow
                textbox_y1 = inner_pad_h * 0.2
                textbox_y2 = inner_pad_h * 1.0 + title_h

                w = wid.Wid(name="textbox",
                            tiles=self.title_tiles,
                            parent=self.wid_id)

                w.set_color(tl=True, bg=True, br=True, name="white")
                w.set_pos_pct(x1=textbox_x1, y1=textbox_y1,
                              x2=textbox_x2, y2=textbox_y2)

            textbox_x1 = inner_pad_w
            textbox_x2 = 1.0 - inner_pad_w

            # to account for the widget shadow
            textbox_y1 = inner_pad_h * 0.6
            textbox_y2 = textbox_y1 + title_h

            self.text_box =\
                wid_text.WidText(name="textbox",
                                 row_text=self.title_text,
                                 row_font=self.title_font,
                                 row_color=self.title_color,
                                 row_center=self.title_center,
                                 row_rhs=self.title_rhs,
                                 row_width=self.title_width,
                                 line_width=self.title_line_width,
                                 row_on_tooltip=self.title_on_tooltip,
                                 row_tooltip=self.title_tooltip,
                                 row_on_key_down=self.title_on_key_down,
                                 row_on_key_sym=self.title_on_key_sym,
                                 row_on_key_mod=self.title_on_key_mod,
                                 row_on_key_up=self.title_on_key_up,
                                 row_on_joy_button=self.title_on_joy_button,
                                 row_on_m_down=self.title_on_m_down,
                                 row_on_m_up=self.title_on_m_up,
                                 row_on_m_motion=self.title_on_m_motion,
                                 row_on_m_focus_b=self.title_on_m_focus_b,
                                 row_on_m_focus_e=self.title_on_m_focus_e,
                                 row_on_m_over_b=self.title_on_m_over_b,
                                 row_on_m_over_e=self.title_on_m_over_e,
                                 row_on_destroy=self.title_on_destroy,
                                 row_on_destroy_b=self.title_on_destroy_b,
                                 row_on_tick=self.title_on_tick,
                                 row_on_button_list=self.title_on_button_list,
                                 row_on_display=self.title_on_display,
                                 row_on_display_win=self.title_on_display_win,
                                 parent=self.wid_id,
                                 x1=textbox_x1, y1=textbox_y1,
                                 x2=textbox_x2, y2=textbox_y2,
                                 debug=self.debug)
        self.to_front()

    def scroll_up(self):
        if self.scrollbar is not None:
            self.scrollbar.move_delta_pct(x=0, y=-0.05)

    def scroll_down(self):
        if self.scrollbar is not None:
            self.scrollbar.move_delta_pct(x=0, y=0.05)
