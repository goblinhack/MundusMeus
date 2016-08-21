import traceback
import mm
import wid
import wid_focus


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
                 row_tooltip,
                 row_on_key_down,
                 row_on_key_sym,
                 row_on_key_mod,
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
                 row_on_button_list,
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

        self.set_pos_pct(x1, y1, x2, y2)
        self.to_front()

        self.children = []
        self.width, self.height = self.get_size_pct()

        self.usable_w = self.width
        self.usable_h = self.height
        self.text_wids = []

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

            button_count = 0

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
                    w, h, c = mm.text_size_pct(font=font, text=word)
                    if c != "none":
                        color = c

                    w = w / self.usable_w
                    h = h / self.usable_h

                    if x + w > 1.0:
                        x = 0
                        y = y + h

                    if word[:1] == "[":

                        #
                        # Grab the next button event
                        #
                        button_tiles="button_plain"
                        button_event_list = row_on_button_list[row]
                        set_on_mouse_down = None
                        tooltip = None

                        if button_event_list != None:
                            if button_count < len(button_event_list):
                                d = button_event_list[button_count]
                                if d != None:
                                    event = d["on_mouse_down"]
                                    if event != None:
                                        set_on_mouse_down = event

                                    if "tiles" in d:
                                        button_tiles = d["tiles"]

                                    tooltip = d["tooltip"]
                            else:
                                mm.err("missing callback "
                                    "for button {0} text {1}".format(
                                    button_count, line))

                            button_count += 1

                        #
                        # Now make the button 
                        #
                        new_word = " " + word[1:-1] + " "
                        word = new_word
                        child = wid.Wid(name="wid text child",
                                        tiles=button_tiles,
                                        parent=self.wid_id)
                        child.set_color(tl=True, bg=True, br=True, name="grey")

                        if set_on_mouse_down != None:
                            child.set_on_mouse_down(set_on_mouse_down)
                        if tooltip != None:
                            child.set_tooltip(text=tooltip)

                        child.set_on_mouse_over_begin(
                                wid_text_button_on_mouse_over_begin_callback)
                        child.set_on_mouse_over_end(
                                wid_text_button_on_mouse_over_end_callback)

                    else:
                        child = wid.Wid(name="wid text child",
                                        parent=self.wid_id)

                    child.set_pos_pct(x, y, x + w, y + h)

                    if color != None:
                        child.set_text(text=word, lhs=True, font=font,
                                       color=color)
                    else:
                        child.set_text(text=word, lhs=True, font=font)

                    self.children.append(child)

                    x = x + w

                    w, unused1, unused2 = mm.text_size_pct(font=font, text=" ")

                    x = x + w

                y = y + h
                l += 1

            w = wid.Wid(name="wid text multi line box", parent=self.wid_id)
            w.row = row
            w.set_pos_pct(0, begin_y, 1, y)
            w.to_back()
            w.set_do_not_raise()

            w.row_text = row_text[row]
            w.row_font = row_font[row]
            w.row_color = row_color[row]
            w.row_center = row_center[row]
            w.row_rhs = row_rhs[row]
            w.row_width = row_width[row]
            w.line_width = line_width[row]
            w.row_on_tooltip = row_on_tooltip
            w.row_tooltip = row_tooltip
            w.row_on_key_down = row_on_key_down[row]
            w.row_on_key_sym = row_on_key_sym[row]
            w.row_on_key_mod = row_on_key_mod[row]
            w.row_on_key_up = row_on_key_up[row]
            w.row_on_joy_button = row_on_joy_button[row]
            w.row_on_mouse_down = row_on_mouse_down[row]
            w.row_on_mouse_up = row_on_mouse_up[row]
            w.row_on_mouse_motion = row_on_mouse_motion[row]
            w.row_on_mouse_focus_begin = row_on_mouse_focus_begin[row]
            w.row_on_mouse_focus_end = row_on_mouse_focus_end[row]
            w.row_on_mouse_over_begin = row_on_mouse_over_begin[row]
            w.row_on_mouse_over_end = row_on_mouse_over_end[row]
            w.row_on_destroy = row_on_destroy[row]
            w.row_on_destroy_begin = row_on_destroy_begin[row]
            w.row_on_tick = row_on_tick[row]
            w.row_on_button_list = row_on_button_list[row]
            w.row_on_display = row_on_display[row]
            w.row_on_display_top_level = row_on_display_top_level[row]

            w.set_on_tooltip(row_on_tooltip[row])
            if row_tooltip[row] != None:
                w.set_tooltip(text=row_tooltip[row])
            w.set_on_joy_button(row_on_joy_button[row])
            w.set_on_mouse_motion(row_on_mouse_motion[row])

            if row_on_key_down[row] != None:
                wid_text_colorize_row(w)

                w.set_on_key_up(wid_text_on_key_up_callback)
                w.set_on_key_down(wid_text_on_key_down_callback)
                w.set_on_mouse_up(wid_text_on_mouse_up_callback)
                w.set_on_mouse_down(wid_text_on_mouse_down_callback)
                w.set_on_mouse_over_begin(wid_text_on_mouse_over_begin_callback)
                w.set_on_mouse_over_end(wid_text_on_mouse_over_end_callback)
            else:
                w.set_on_key_up(row_on_key_up[row])
                w.set_on_key_down(row_on_key_down[row])
                w.set_on_mouse_up(row_on_mouse_up[row])
                w.set_on_mouse_down(row_on_mouse_down[row])
                w.set_on_mouse_over_begin(row_on_mouse_over_begin[row])
                w.set_on_mouse_over_end(row_on_mouse_over_end[row])

            w.set_on_destroy(row_on_destroy[row])
            w.set_on_destroy_begin(wid_text_on_destroy)
            w.set_on_tick(row_on_tick[row])
            w.set_on_display(row_on_display[row])
            w.set_on_display_top_level(row_on_display_top_level[row])

            self.text_wids.append(w)

        self.update()

def wid_text_colorize_row(w):
    if w.row % 2 == 0:
        w.set_color(tl=True, bg=True, br=True, name="white", alpha=0.02)
    else:
        w.set_color(tl=True, bg=True, br=True, name="white", alpha=0.05)

def wid_text_on_destroy(w):
    wid_focus.clear_focus(w)

def wid_text_on_mouse_over_begin_callback(w, relx, rely, wheelx, wheely):
    wid_focus.set_focus(w, auto_scroll=False)

    w.set_color(tl=True, bg=True, br=True, name="white", alpha=0.2)

    if w.row_on_mouse_over_begin != None:
        w.row_on_mouse_over_begin(w, relx, rely, wheelx, wheely)

def wid_text_on_mouse_over_end_callback(w):
    wid_text_colorize_row(w)

    if w.row_on_mouse_over_end != None:
        w.row_on_mouse_over_end(w)

def wid_text_on_mouse_up_callback(w, x, y, button):
    if w.row_on_mouse_up != None:
        return w.row_on_mouse_up(w, x, y, button)
    return False

def wid_text_on_mouse_down_callback(w, x, y, button):
    wid_focus.set_focus(w, auto_scroll=False)

    w.set_active()
    w.set_color(tl=True, bg=True, br=True, name="red", alpha=0.5)

    if w.row_on_mouse_down != None:
        return w.row_on_mouse_down(w, x, y, button)
    return False

def wid_text_on_key_up_callback(w, sym, mod):
    if w.row_on_key_up != None:
        return w.row_on_key_up(w, sym, mod)
    return False

def wid_text_on_key_down_callback(w, sym, mod):
    rc = False
    if w.row_on_key_sym != None:
        if w.row_on_key_sym == sym:
            if w.row_on_key_mod != None:
                if w.row_on_key_mod == mod:
                    rc = w.row_on_key_down(w, sym, mod)
            else:
                rc = w.row_on_key_down(w, sym, mod)
    else:
        if w.row_on_key_down != None:
            rc = w.row_on_key_down(w, sym, mod)

    if rc is True:
        w.set_active()
        w.set_color(tl=True, bg=True, br=True, name="red", alpha=0.5)
    else:
        parent = w.get_parent()
        for w in parent.text_wids:
            rc = False

            if w.row_on_key_sym != None:
                if w.row_on_key_sym == sym:
                    if w.row_on_key_mod != None:
                        if w.row_on_key_mod == mod:
                            rc = w.row_on_key_down(w, sym, mod)
                    else:
                        rc = w.row_on_key_down(w, sym, mod)

            if rc is True:
                w.set_active()
                w.set_color(tl=True, bg=True, br=True, name="red", alpha=0.5)
                break

    if rc is True:
        wid_focus.set_focus(w, auto_scroll=True)

    return rc

def wid_text_button_on_mouse_over_begin_callback(w, relx, rely, wheelx, wheely):
    w.set_color(tl=True, bg=True, br=True, name="white", alpha=1.0)
    wid_focus.set_focus(w, auto_scroll=False)

def wid_text_button_on_mouse_over_end_callback(w):
    w.set_color(tl=True, bg=True, br=True, name="gray", alpha=1.0)

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

            count = len(words)
            cnt = 0
            for word in words:
                w, h, c = mm.text_size_pct(font=font, text=word)
                cnt += 1

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
