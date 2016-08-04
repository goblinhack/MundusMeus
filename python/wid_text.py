import traceback
import mm
import wid


class WidText(wid.Wid):

    def __init__(self, name, 
                 x1, y1, x2, y2, 
                 row_text, 
                 row_font, 
                 row_color, 
                 row_center, 
                 row_rhs, 
                 row_width, 
                 line_width, 
                 row_on_tooltip,
                 row_on_key_down,
                 row_on_key_up,
                 row_on_joy_button,
                 row_on_mouse_down,
                 row_on_mouse_up,
                 row_on_mouse_motion,
                 row_on_mouse_focus_begin,
                 row_on_mouse_focus_end,
                 row_on_mouse_over_begin,
                 row_on_mouse_over_end,
                 row_on_destroy,
                 row_on_destroy_begin,
                 row_on_tick,
                 row_on_display,
                 row_on_display_top_level,
                 parent=0, 
                 tiles=None,
                 **kw):

        self.parent = parent
        self.name = name

        if tiles != None:
            super().__init__(name, parent=parent, tiles=tiles, **kw)
            self.set_color(tl=True, bg=True, br=True, name="white")
        else:
            super().__init__(name, parent=parent, **kw)

        self.set_tl_br_pct(x1, y1, x2, y2)

        self.children = []
        self.width, self.height = self.get_size_pct()

        self.usable_w = self.width
        self.usable_h = self.height

        y = 0
        for row in range(0, len(row_text)):
            text = row_text[row]
            font = row_font[row]
            color = row_color[row]
            width = row_width[row]
            center = row_center[row]
            rhs = row_rhs[row]

            lines = text.split("\n")

            begin_y = y
            l = 0

            for line in lines:

                width = line_width[l]
                words = line.split()
                x = 0

                if center is True:
                    x = self.usable_w - width
                    x = x / self.usable_w
                    x /= 2.0

                if rhs is True:
                    x = self.usable_w - width
                    x = x / self.usable_w

                for word in words:
                    w, h, c = mm.text_size_pct(font=font, text=word + " ")
                    if c != "none":
                        color = c

                    w = w / self.usable_w
                    h = h / self.usable_h

                    if x + w > 1.0:
                        x = 0
                        y = y + h

                    child = wid.Wid(name="wid text child", 
                                    parent=self.wid_id)

                    child.set_tl_br_pct(x, y, x + w, y + h)

                    if color != None:
                        child.set_text(text=word, lhs=True, font=font, 
                                       color=color)
                    else:
                        child.set_text(text=word, lhs=True, font=font)

                    self.children.append(child)

                    x = x + w

                y = y + h
                l += 1

            w = wid.Wid(name="wid text multi line box", parent=self.wid_id)
            w.row = row
            w.set_tl_br_pct(0, begin_y, 1, y)

            if row_on_key_down[row] != None:
                wid_text_colorize_row(w)
                w.set_on_mouse_over_begin(wid_text_on_mouse_over_begin_callback)
                w.set_on_mouse_over_end(wid_text_on_mouse_over_end_callback)

            w.set_on_mouse_down(row_on_mouse_down[row])
            w.set_on_tooltip(row_on_tooltip[row])
            w.set_on_key_down(row_on_key_down[row])
            w.set_on_key_up(row_on_key_up[row])
            w.set_on_joy_button(row_on_joy_button[row])
            w.set_on_mouse_up(row_on_mouse_up[row])
            w.set_on_mouse_motion(row_on_mouse_motion[row])
            w.set_on_mouse_focus_begin(row_on_mouse_focus_begin[row])
            w.set_on_mouse_focus_end(row_on_mouse_focus_end[row])

            if row_on_mouse_over_begin[row] != None:
                w.set_on_mouse_over_begin(row_on_mouse_over_begin[row])

            if row_on_mouse_over_end[row] != None:
                w.set_on_mouse_over_end(row_on_mouse_over_end[row])

            w.set_on_destroy(row_on_destroy[row])
            w.set_on_destroy_begin(row_on_destroy_begin[row])
            w.set_on_tick(row_on_tick[row])
            w.set_on_display(row_on_display[row])
            w.set_on_display_top_level(row_on_display_top_level[row])

            w.to_back()

        self.update()

def wid_text_colorize_row(w):
    if w.row % 2 == 0:
        w.set_color(tl=True, bg=True, br=True, name="white", alpha=0.0)
    else:
        w.set_color(tl=True, bg=True, br=True, name="white", alpha=0.05)

def wid_text_on_mouse_over_begin_callback(w, relx, rely, wheelx, wheely):
    w.set_color(tl=True, bg=True, br=True, name="white", alpha=0.2)

def wid_text_on_mouse_over_end_callback(w):
    wid_text_colorize_row(w)

def text_size_pct(row_text, row_font, width):

    row_width = []
    line_width = []

    y = 0
    l = 0

    for row in range(0, len(row_text)):
        text = row_text[row]
        font = row_font[row]
        row_width.append(0)

        lines = text.split("\n")
        max_w = 0

        for line in lines:

            words = line.split()
            x = 0
            max_h = 0

            for word in words:
                w, h, c = mm.text_size_pct(font=font, text=word + " ")

                if x + w > width:
                    line_width.append(0)
                    line_width[l] = x
                    l += 1
                    x = 0
                    y = y + max_h
                    max_h = 0
                    max_w = width 

                x = x + w

                if x > max_w:
                    max_w = x

                if h > max_h:
                    max_h = h

            if words != []:
                y = y + max_h

            line_width.append(0)
            line_width[l] = x
            l += 1

            row_width[row] = max_w

    return max_w, y, row_width, line_width
