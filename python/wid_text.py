import mm
import wid
import wid_focus
import shlex


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
                 row_on_m_down,
                 row_on_m_up,
                 row_on_m_motion,
                 row_on_m_focus_b,
                 row_on_m_focus_e,
                 row_on_m_over_b,
                 row_on_m_over_e,
                 row_on_destroy,
                 row_on_destroy_b,
                 row_on_tick,
                 row_on_button_list,
                 row_on_display,
                 row_on_display_win,
                 parent=0,
                 tiles=None,
                 debug=False,
                 **kw):

        self.parent = parent
        self.name = name

        if tiles is not None:
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
        h = 0

        for row in range(0, len(row_text)):
            text = row_text[row]
            font = row_font[row]
            color = row_color[row]
            width = row_width[row]
            center = row_center[row]
            rhs = row_rhs[row]

            #
            # This magic is to allow splitting of the string
            # into words but maintain a space char for display
            #
            text = text.replace(" ", "\\ ")
            lines = text.split("\n")

            begin_y = y
            l = 0

            button_count = 0
            col = 0

            for line in lines:

                lexer = shlex.shlex(line)
                lexer.wordchars += '.$%='
                words = list(lexer)

                x = 0

                if center is True:
                    w = width
                    x = (self.usable_w - w) / 2.0
                    x *= 1.0 / self.usable_w

                if rhs is True:
                    x = self.usable_w - width
                    x = x / self.usable_w

                max_h = 0
                for word in words:
                    w, h, c = mm.text_size_pct(font=font, text=word)
                    if c != "none":
                        color = c

                    w = w / self.usable_w
                    h = h / self.usable_h

                    if x + w > 1.0:
                        x = 0
                        y = y + max_h
                        max_h = 0

                    if h > max_h:
                        max_h = h

                    if word[:1] == "\'":

                        #
                        # Grab the next button event
                        #
                        button_event_list = row_on_button_list[row]
                        button_tiles = "button_plain"
                        tooltip = None
                        set_on_m_down = None
                        set_on_m_over_b = None
                        set_on_m_over_e = None
                        context = None

                        if button_event_list is not None:
                            if button_count < len(button_event_list):
                                #
                                # Settings per button
                                #
                                d = button_event_list[button_count]
                                if d is not None:
                                    try:
                                        button_tiles = d["tiles"]
                                    except KeyError:
                                        pass

                                    try:
                                        tooltip = d["tooltip"]
                                    except KeyError:
                                        pass

                                    try:
                                        event = d["on_m_down"]
                                        set_on_m_down = event
                                    except KeyError:
                                        pass

                                    try:
                                        event = d["on_m_over_b"]
                                        set_on_m_over_b = event
                                    except KeyError:
                                        pass

                                    try:
                                        event = d["on_m_over_e"]
                                        set_on_m_over_e = event
                                    except KeyError:
                                        pass

                                    try:
                                        val = d["context"]
                                        context = val
                                    except KeyError:
                                        pass
                            else:
                                mm.err("missing callback "
                                       "for button {0} text {1}".format(
                                           button_count, line))

                            button_count += 1

                        #
                        # Now make the button
                        #
                        new_word = word[1:-1]
                        word = new_word
                        child = wid.Wid(name="wid text child",
                                        tiles=button_tiles,
                                        parent=self.wid_id)
                        child.set_color(tl=True, bg=True, br=True, name="grey")

                        if set_on_m_down is not None:
                            child.set_on_m_down(set_on_m_down)

                        if tooltip is not None:
                            child.set_tooltip(text=tooltip)

                        child.context = context
                        child.row_on_m_over_b = set_on_m_over_b
                        child.row_on_m_over_e = set_on_m_over_e

                        child.set_on_m_over_b(
                                wid_text_button_on_m_over_b_callback)
                        child.set_on_m_over_e(
                                wid_text_button_on_m_over_e_callback)
                    else:
                        child = wid.Wid(name="wid text child",
                                        parent=self.wid_id)

                    child.set_pos_pct(x, y, x + w, y + h)

                    if color is not None:
                        child.set_text(text=word, centerx=True, font=font,
                                       color=color)
                    else:
                        child.set_text(text=word, centerx=True, font=font)

                    self.children.append(child)

                    x = x + w

                    child.row = row
                    child.button = button_count
                    child.col = col
                    col += 1

                y = y + max_h
                max_h = 0
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
            w.row_on_m_down = row_on_m_down[row]
            w.row_on_m_up = row_on_m_up[row]
            w.row_on_m_motion = row_on_m_motion[row]
            w.row_on_m_focus_b = row_on_m_focus_b[row]
            w.row_on_m_focus_e = row_on_m_focus_e[row]
            w.row_on_m_over_b = row_on_m_over_b[row]
            w.row_on_m_over_e = row_on_m_over_e[row]
            w.row_on_destroy = row_on_destroy[row]
            w.row_on_destroy_b = row_on_destroy_b[row]
            w.row_on_tick = row_on_tick[row]
            w.row_on_button_list = row_on_button_list[row]
            w.row_on_display = row_on_display[row]
            w.row_on_display_win = row_on_display_win[row]

            w.set_on_tooltip(row_on_tooltip[row])
            if row_tooltip[row] is not None:
                w.set_tooltip(text=row_tooltip[row])
            w.set_on_joy_button(row_on_joy_button[row])
            w.set_on_m_motion(row_on_m_motion[row])

            if row_on_key_down[row] is not None or \
               row_on_m_down[row] is not None or \
               row_on_m_over_b[row] is not None:
                wid_text_colorize_row(w)

                w.set_on_key_up(wid_text_on_key_up_callback)
                w.set_on_key_down(wid_text_on_key_down_callback)
                w.set_on_m_up(wid_text_on_m_up_callback)
                w.set_on_m_down(wid_text_on_m_down_callback)
                w.set_on_m_over_b(
                        wid_text_on_m_over_b_callback)
                w.set_on_m_over_e(wid_text_on_m_over_e_callback)
            else:
                w.set_on_key_up(row_on_key_up[row])
                w.set_on_key_down(row_on_key_down[row])
                w.set_on_m_up(row_on_m_up[row])
                w.set_on_m_down(row_on_m_down[row])
                w.set_on_m_over_b(row_on_m_over_b[row])
                w.set_on_m_over_e(row_on_m_over_e[row])

            w.set_on_destroy(row_on_destroy[row])
            w.set_on_destroy_b(wid_text_on_destroy)
            w.set_on_tick(row_on_tick[row])
            w.set_on_display(row_on_display[row])
            w.set_on_display_win(row_on_display_win[row])

            self.text_wids.append(w)

        self.update()


def wid_text_colorize_row(w):
    if w.row % 2 == 0:
        w.set_color(tl=True, bg=True, br=True, name="white", alpha=0.02)
    else:
        w.set_color(tl=True, bg=True, br=True, name="white", alpha=0.05)


def wid_text_on_destroy(w):
    wid_focus.clear_focus(w)


def wid_text_on_m_over_b_callback(w, relx, rely, wheelx, wheely):
    wid_focus.set_focus(w, auto_scroll=False)

    w.set_color(tl=True, bg=True, br=True, name="white", alpha=0.2)

    if w.row_on_m_over_b is not None:
        w.row_on_m_over_b(w, relx, rely, wheelx, wheely)


def wid_text_on_m_over_e_callback(w):
    wid_text_colorize_row(w)

    if w.row_on_m_over_e is not None:
        w.row_on_m_over_e(w)


def wid_text_on_m_up_callback(w, x, y, button):
    if w.row_on_m_up is not None:
        return w.row_on_m_up(w, x, y, button)
    return False


def wid_text_on_m_down_callback(w, x, y, button):
    wid_focus.set_focus(w, auto_scroll=False)

    w.set_active()
    w.set_color(tl=True, bg=True, br=True, name="red", alpha=0.5)

    if w.row_on_m_down is not None:
        return w.row_on_m_down(w, x, y, button)
    return False


def wid_text_on_key_up_callback(w, sym, mod):
    if w.row_on_key_up is not None:
        return w.row_on_key_up(w, sym, mod)
    return False


def wid_text_on_key_down_callback(w, sym, mod):
    rc = False
    if w.row_on_key_sym is not None:
        if w.row_on_key_sym == sym:
            if w.row_on_key_mod is not None:
                if w.row_on_key_mod == mod:
                    rc = w.row_on_key_down(w, sym, mod)
            else:
                rc = w.row_on_key_down(w, sym, mod)
    else:
        if w.row_on_key_down is not None:
            rc = w.row_on_key_down(w, sym, mod)

    if rc is True:
        w.set_active()
        w.set_color(tl=True, bg=True, br=True, name="red", alpha=0.5)
    else:
        parent = w.get_parent()
        for w in parent.text_wids:
            rc = False

            if w.row_on_key_sym is not None:
                if w.row_on_key_sym == sym:
                    if w.row_on_key_mod is not None:
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


def wid_text_button_on_m_over_b_callback(w, relx,
                                         rely, wheelx, wheely):
    w.set_color(tl=True, bg=True, br=True, name="white", alpha=1.0)
    wid_focus.set_focus(w, auto_scroll=False)

    if w.row_on_m_over_b is not None:
        w.row_on_m_over_b(w, relx, rely, wheelx, wheely)


def wid_text_button_on_m_over_e_callback(w):
    w.set_color(tl=True, bg=True, br=True, name="gray", alpha=1.0)

    if w.row_on_m_over_e is not None:
        w.row_on_m_over_e(w)


def text_size_pct(row_text, row_font, width):

    row_width = []
    line_width = []

    y = 0
    l = 0

    for row in range(0, len(row_text)):
        text = row_text[row]
        font = row_font[row]

        #
        # This magic is to allow splitting of the string
        # into words but maintain a space char for display
        #
        text = text.replace(" ", "\\ ")
        lines = text.split("\n")

        for line in lines:

            lexer = shlex.shlex(line)
            lexer.wordchars += '.$%='
            words = list(lexer)

            row_width.append(0)
            max_w = 0

            x = 0
            max_h = 0

            max_h = 0
            for word in words:
                w, h, c = mm.text_size_pct(font=font, text=word)

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
