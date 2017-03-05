import traceback
import mm
import wid_focus

all_wids = {}


class Wid:

    def __init__(self, name,
                 wid_id=0,
                 parent=0,
                 is_grid=False,
                 is_scrollbar=False, **kw):

        self.parent = parent
        self.name = name

        if wid_id != 0:
            self.wid_id = wid_id
        elif is_grid is True:
            self.wid_id = mm.wid_new_grid(self, parent=parent, name=name, **kw)
        elif is_scrollbar is True:
            self.wid_id = mm.wid_new_scrollbar(self, parent, name=name, **kw)
        else:
            self.wid_id = mm.wid_new(self, parent, name, **kw)

#        self.name = "{0:x}:{1}".format(self.wid_id, self.name)
#        self.log("Created wid")

        all_wids[self.wid_id] = self

        self.callback_on_tooltip = None
        self.callback_on_key_down = None
        self.callback_on_key_up = None
        self.callback_on_joy_button = None
        self.callback_on_m_down = None
        self.callback_on_m_motion = None
        self.callback_on_m_focus_b = None
        self.callback_on_m_focus_e = None
        self.callback_on_m_over_b = None
        self.callback_on_m_over_e = None
        self.callback_on_m_up = None
        self.callback_on_destroy = None
        self.callback_on_destroy_b = None
        self.callback_on_tick = None
        self.callback_on_display = None
        self.callback_on_display_win = None
        self.is_visible = True

    def __str__(self):
        return "{0}".format(self.name)

    def destroy(self):
        mm.wid_destroy(self)
        all_wids[self.wid_id] = None
        del self

    def destroy_in(self, **kw):
        mm.wid_destroy_in(self, **kw)
        all_wids[self.wid_id] = None
        del self

    def fade_in(self, **kw):
        mm.wid_fade_in(self, **kw)

    def fade_out(self, **kw):
        mm.wid_fade_out(self, **kw)

    def fade_in_out(self, **kw):
        mm.wid_fade_in_out(self, **kw)

    def visible(self, **kw):
        self.is_visible = True
        mm.wid_visible(self, **kw)

    def hide(self, **kw):
        self.is_visible = False
        mm.wid_hide(self, **kw)
        wid_focus.clear_focus(self)

    def toggle_hidden(self, **kw):
        self.is_visible = not self.is_visible
        mm.wid_toggle_hidden(self, **kw)
        if not self.is_visible:
            wid_focus.clear_focus(self)

    def mouse_hide(self, **kw):
        mm.wid_mouse_hide(self, **kw)

    def set_shape(self, **kw):
        mm.wid_set_shape(self, **kw)

    def set_tooltip(self, **kw):
        mm.wid_set_tooltip(self, **kw)

    def set_tiles(self, **kw):
        mm.wid_set_tiles(self, **kw)

    def set_pos(self, **kw):
        mm.wid_set_pos(self, **kw)

    def set_pos_pct(self, x1, y1, x2, y2):
        mm.wid_set_pos_pct(self, x1, y1, x2, y2)

    def set_color(self, **kw):
        mm.wid_set_color(self, **kw)

    def set_text(self, **kw):
        mm.wid_set_text(self, **kw)

    def set_tex(self, **kw):
        mm.wid_set_tex(self, **kw)

    def to_back(self):
        mm.wid_lower(self)

    def to_front(self):
        mm.wid_raise(self)

    def update(self):
        mm.wid_update(self)

    def get_top_parent(self):
        wid_id = mm.wid_get_top_parent(self)
        if wid_id is not None:
            wid = all_wids.get(wid_id, None)
            if wid is not None:
                return wid
        return None

    def get_parent(self):
        wid_id = mm.wid_get_parent(self)
        if wid_id is not None:
            wid = all_wids.get(wid_id, None)
            if wid is not None:
                return wid
        return None

    def get_size(self):
        return mm.wid_get_size(self)

    def get_size_pct(self):
        return mm.wid_get_size_pct(self)

    def get_pos(self):
        return mm.wid_get_pos(self)

    def get_pos_pct(self):
        return mm.wid_get_pos_pct(self)

    def set_blit_y_offset(self, **kw):
        mm.wid_set_blit_y_offset(self, **kw)

    def set_bevel(self, **kw):
        mm.wid_set_bevel(self, **kw)

    def set_bevelled(self, **kw):
        mm.wid_set_bevelled(self, **kw)

    def set_cursor(self, **kw):
        mm.wid_set_cursor(self, **kw)

    def set_do_not_lower(self, **kw):
        mm.wid_set_do_not_lower(self, **kw)

    def set_do_not_raise(self, **kw):
        mm.wid_set_do_not_raise(self, **kw)

    def set_ignore_events(self, **kw):
        mm.wid_set_ignore_events(self, **kw)

    def set_focusable(self, **kw):
        mm.wid_set_focusable(self, **kw)

    def set_movable(self, **kw):
        mm.wid_set_movable(self, **kw)

    def set_movable_no_user_scroll(self, **kw):
        mm.wid_set_movable_no_user_scroll(self, **kw)

    def set_movable_bounded(self, **kw):
        mm.wid_set_movable_bounded(self, **kw)

    def set_movable_horiz(self, **kw):
        mm.wid_set_movable_horiz(self, **kw)

    def set_movable_vert(self, **kw):
        mm.wid_set_movable_vert(self, **kw)

    def con(self, msg):
        mm.con("p_wid {0}: {1}".format(str(self), msg))

    def log(self, msg):
        mm.log("p_wid {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.con("".join(traceback.format_stack()))
        mm.err("p_wid {0}: ERROR: {1}".format(self.name, msg))

    def die(self, msg):
        mm.con("".join(traceback.format_stack()))
        mm.die("p_wid {0}: ERROR: {1}".format(self.name, msg))

    def dump(self):
        self.log("@ {0},{1}".format(self.x, self.y))

    def move_to_horiz_vert_pct_in(self, **kw):
        mm.wid_move_to_horiz_vert_pct_in(self, **kw)

    def move_to_abs_centered_in(self, **kw):
        mm.wid_move_to_abs_centered_in(self, **kw)

    def move_to_centered_in(self, **kw):
        mm.wid_move_to_centered_in(self, **kw)

    def move_delta_pct_in(self, **kw):
        mm.wid_move_delta_pct_in(self, **kw)

    def move_to_abs_in(self, **kw):
        mm.wid_move_to_abs_in(self, **kw)

    def move_delta_in(self, **kw):
        mm.wid_move_delta_in(self, **kw)

    def move_to_pct_centered_in(self, **kw):
        mm.wid_move_to_pct_centered_in(self, **kw)

    def move_to_abs_poffset_in(self, **kw):
        mm.wid_move_to_abs_poffset_in(self, **kw)

    def move_to_pct_in(self, **kw):
        mm.wid_move_to_pct_in(self, **kw)

    def bounce_to_pct_in(self, **kw):
        mm.wid_bounce_to_pct_in(self, **kw)

    def move_delta(self, **kw):
        mm.wid_move_delta(self, **kw)

    def move_delta_pct(self, **kw):
        mm.wid_move_delta_pct(self, **kw)

    def move_to_abs(self, **kw):
        mm.wid_move_to_abs(self, **kw)

    def move_to_abs_centered(self, **kw):
        mm.wid_move_to_abs_centered(self, **kw)

    def move_to_pct(self, **kw):
        mm.wid_move_to_pct(self, **kw)

    def move_to_pct_centered(self, **kw):
        mm.wid_move_to_pct_centered(self, **kw)

    def move_to_vert_pct_in(self, **kw):
        mm.wid_move_to_vert_pct_in(self, **kw)

    def move_to_horiz_pct_in(self, **kw):
        mm.wid_move_to_horiz_pct_in(self, **kw)

    def move_to_vert_pct(self, **kw):
        mm.wid_move_to_vert_pct(self, **kw)

    def move_to_horiz_pct(self, **kw):
        mm.wid_move_to_horiz_pct(self, **kw)

    def move_to_bottom(self, **kw):
        mm.wid_move_to_bottom(self, **kw)

    def move_to_left(self, **kw):
        mm.wid_move_to_left(self, **kw)

    def move_to_right(self, **kw):
        mm.wid_move_to_right(self, **kw)

    def move_to_top(self, **kw):
        mm.wid_move_to_top(self, **kw)

    def move_end(self, **kw):
        mm.wid_move_end(self, **kw)

    def destroy_grid(self, **kw):
        mm.wid_destroy_grid(self, **kw)

    def detach_from_grid(self, **kw):
        mm.wid_detach_from_grid(self, **kw)

    def attach_to_grid(self, **kw):
        mm.wid_attach_to_grid(self, **kw)

    def empty_grid(self, **kw):
        mm.wid_empty_grid(self, **kw)

    def scroll_text(self, **kw):
        mm.wid_scroll_text(self, **kw)

    def set_focus(self, **kw):
        mm.wid_set_focus(self, **kw)

    def set_active(self, **kw):
        mm.wid_set_active(self, **kw)

    def focus_lock(self, **kw):
        mm.wid_focus_lock(self, **kw)

    def effect_pulses(self, **kw):
        mm.wid_effect_pulses(self, **kw)

    def effect_pulse_forever(self, **kw):
        mm.wid_effect_pulse_forever(self, **kw)

    def effect_pulse_stop(self, **kw):
        mm.wid_effect_pulse_stop(self, **kw)

    def blit_effect_pulses(self, **kw):
        mm.wid_blit_effect_pulses(self, **kw)

    def effect_sways(self, **kw):
        mm.wid_effect_sways(self, **kw)

    def animate(self, **kw):
        mm.wid_animate(self, **kw)

    def set_on_tooltip(self, callback):
        self.callback_on_tooltip = callback
        if callback is None:
            return
        mm.wid_set_on_tooltip(self)

    def set_on_key_down(self, callback):
        self.callback_on_key_down = callback
        if callback is None:
            return
        mm.wid_set_on_key_down(self)

    def set_on_key_up(self, callback):
        self.callback_on_key_up = callback
        if callback is None:
            return
        mm.wid_set_on_key_up(self)

    def set_on_joy_button(self, callback):
        self.callback_on_joy_button = callback
        if callback is None:
            return
        mm.wid_set_on_joy_button(self)

    def set_on_m_down(self, callback):
        self.callback_on_m_down = callback
        if callback is None:
            return
        mm.wid_set_on_m_down(self)

    def set_on_m_motion(self, callback):
        self.callback_on_m_motion = callback
        if callback is None:
            return
        mm.wid_set_on_m_motion(self)

    def set_on_m_focus_b(self, callback):
        self.callback_on_m_focus_b = callback
        if callback is None:
            return
        mm.wid_set_on_m_focus_b(self)

    def set_on_m_focus_e(self, callback):
        self.callback_on_m_focus_e = callback
        if callback is None:
            return
        mm.wid_set_on_m_focus_e(self)

    def set_on_m_over_b(self, callback):
        self.callback_on_m_over_b = callback
        if callback is None:
            return
        mm.wid_set_on_m_over_b(self)

    def set_on_m_over_e(self, callback):
        self.callback_on_m_over_e = callback
        if callback is None:
            return
        mm.wid_set_on_m_over_e(self)

    def set_on_m_up(self, callback):
        self.callback_on_m_up = callback
        if callback is None:
            return
        mm.wid_set_on_m_up(self)

    def set_on_destroy(self, callback):
        self.callback_on_destroy = callback
        if callback is None:
            return
        mm.wid_set_on_destroy(self)

    def set_on_destroy_b(self, callback):
        self.callback_on_destroy_b = callback
        if callback is None:
            return
        mm.wid_set_on_destroy_b(self)

    def set_on_tick(self, callback):
        self.callback_on_tick = callback
        if callback is None:
            return
        mm.wid_set_on_tick(self)

    def set_on_display(self, callback):
        self.callback_on_display = callback
        if callback is None:
            return
        mm.wid_set_on_display(self)

    def set_on_display_win(self, callback):
        self.callback_on_display_win = callback
        mm.wid_set_on_display_win(self)

    def on_tooltip(self, tooltip):
        if self.callback_on_tooltip is not None:
            self.callback_on_tooltip(self, tooltip)
            return
        mm.err("wid_set_on_tooltip not handled")

    def on_key_down(self, sym, mod):
        if self.callback_on_key_down is not None:
            return self.callback_on_key_down(self, sym, mod)
        mm.err("wid_set_on_key_down not handled")

    def on_key_up(self, sym, mod):
        if self.callback_on_key_up is not None:
            return self.callback_on_key_up(self, sym, mod)
        mm.err("wid_set_on_key_up not handled")

    def on_joy_button(self,
                      button_a,
                      button_b,
                      button_x,
                      button_y,
                      button_top_left,
                      button_top_right,
                      button_left_stick_down,
                      button_right_stick_down,
                      button_start,
                      button_xbox,
                      button_back,
                      button_up,
                      button_down,
                      button_left,
                      button_right,
                      button_left_fire,
                      button_right_fire):
        if self.callback_on_joy_button is not None:
            self.callback_on_joy_button(self,
                                        button_a,
                                        button_b,
                                        button_x,
                                        button_y,
                                        button_top_left,
                                        button_top_right,
                                        button_left_stick_down,
                                        button_right_stick_down,
                                        button_start,
                                        button_xbox,
                                        button_back,
                                        button_up,
                                        button_down,
                                        button_left,
                                        button_right,
                                        button_left_fire,
                                        button_right_fire)
        mm.err("wid_set_on_joy_button not handled")

    def on_m_down(self, x, y, button):
        if self.callback_on_m_down is not None:
            return self.callback_on_m_down(self, x, y, button)
        mm.err("wid_set_on_m_down not handled")

    def on_m_motion(self, x, y, relx, rely, wheelx, wheely):
        if self.callback_on_m_motion is not None:
            return self.callback_on_m_motion(self, x, y, relx, rely,
                                             wheelx,
                                             wheely)
        self.err("wid_set_on_m_motion not handled".format(x, y,
                                                          relx, rely,
                                                          wheelx, wheely))
        return False

    def on_m_focus_b(self):
        if self.callback_on_m_focus_b is not None:
            self.callback_on_m_focus_b(self)
            return
        mm.err("wid_set_on_m_focus_b not handled")

    def on_m_focus_e(self):
        if self.callback_on_m_focus_e is not None:
            self.callback_on_m_focus_e(self)
            return
        mm.err("wid_set_on_m_focus_e not handled")

    def on_m_over_b(self, relx, rely, wheelx, wheely):
        if self.callback_on_m_over_b is not None:
            return self.callback_on_m_over_b(self,
                                             relx, rely,
                                             wheelx, wheely)
        self.err("wid_set_on_m_over_b not handled")

    def on_m_over_e(self):
        if self.callback_on_m_over_e is not None:
            self.callback_on_m_over_e(self)
            return
        mm.err("wid_set_on_m_over_e not handled")

    def on_m_up(self, x, y, button):
        if self.callback_on_m_up is not None:
            return self.callback_on_m_up(self, x, y, button)
        mm.err("wid_set_on_m_up not handled")

    def on_destroy(self):
        if self.callback_on_destroy is not None:
            self.callback_on_destroy(self)
            return
        mm.err("wid_set_on_destroy not handled")

    def on_destroy_b(self):
        if self.callback_on_destroy_b is not None:
            self.callback_on_destroy_b(self)
            return
        mm.err("wid_set_on_destroy_b not handled")

    def on_tick(self):
        if self.callback_on_tick is not None:
            self.callback_on_tick(self)
            return
        mm.err("wid_set_on_tick not handled")

    def on_display(self):
        if self.callback_on_display is not None:
            self.callback_on_display(self)
            return
        mm.err("wid_set_on_display not handled")

    def on_display_win(self):
        if self.callback_on_display_win is not None:
            self.callback_on_display_win(self)
            return
        mm.err("wid_set_on_display_win not handled")


def on_m_motion_callback(wid_id, x, y, relx, rely, wheelx, wheely):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_m_motion is None:
            return False
        return wid.on_m_motion(x, y, relx, rely, wheelx, wheely)
    mm.err("Widget not found for" +
           "on_m_motion_callback, id {0:x}".format(wid_id))
    return False


def on_tooltip_callback(wid_id, tooltip):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_tooltip is None:
            return False
        wid.on_tooltip(tooltip)
        return
    mm.err("Widget not found for on_tooltip, id {0:x}".format(wid_id))


def on_key_down_callback(wid_id, sym, mod):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_key_down is None:
            return False
        return wid.on_key_down(sym, mod)
    mm.err("Widget not found for on_key_down, id {0:x}".format(wid_id))
    return False


def on_key_up_callback(wid_id, sym, mod):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_key_up is None:
            return False
        return wid.on_key_up(sym, mod)
    mm.err("Widget not found for on_key_up, id {0:x}".format(wid_id))
    return False


def on_joy_button_callback(wid_id,
                           button_a,
                           button_b,
                           button_x,
                           button_y,
                           button_top_left,
                           button_top_right,
                           button_left_stick_down,
                           button_right_stick_down,
                           button_start,
                           button_xbox,
                           button_back,
                           button_up,
                           button_down,
                           button_left,
                           button_right,
                           button_left_fire,
                           button_right_fire):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_joy_button is None:
            return False
        wid.on_joy_button(button_a,
                          button_b,
                          button_x,
                          button_y,
                          button_top_left,
                          button_top_right,
                          button_left_stick_down,
                          button_right_stick_down,
                          button_start,
                          button_xbox,
                          button_back,
                          button_up,
                          button_down,
                          button_left,
                          button_right,
                          button_left_fire,
                          button_right_fire)
        return
    mm.err("Widget not found for on_joy_button, id {0:x}".format(wid_id))
    return False


def on_m_down_callback(wid_id, x, y, button):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_m_down is None:
            return False
        return wid.on_m_down(x, y, button)
    mm.err("Widget not found for on_m_down, id {0:x}".format(wid_id))
    return False


def on_m_focus_b_callback(wid_id):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_focus_b is None:
            return False
        wid.on_m_focus_b()
        return
    mm.err("Widget not found for " +
           "on_m_focus_b, id {0:x}".format(wid_id))


def on_m_focus_e_callback(wid_id):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_focus_e is None:
            return False
        wid.on_m_focus_e()
        return
    mm.err("Widget not found for on_m_focus_e, id {0:x}".format(wid_id))


def on_m_over_b_callback(wid_id, relx, rely, wheelx, wheely):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_m_over_b is None:
            return False
        return wid.on_m_over_b(relx, rely, wheelx, wheely)
    mm.err("Widget not found for on_m_over_b, id {0:x}".format(wid_id))
    return False


def on_m_over_e_callback(wid_id):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_m_over_e is None:
            return False
        wid.on_m_over_e()
        return
    mm.err("Widget not found for on_m_over_e, id {0:x}".format(wid_id))


def on_m_up_callback(wid_id, x, y, button):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_m_up is None:
            return False
        return wid.on_m_up(x, y, button)
    mm.err("Widget not found for on_m_up, id {0:x}".format(wid_id))
    return False


def on_destroy_callback(wid_id):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_destroy is None:
            return
        wid.on_destroy()
        return
    mm.err("Widget not found for on_destroy, id {0:x}".format(wid_id))
    return


def on_destroy_b_callback(wid_id):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_destroy_b is None:
            return
        wid.on_destroy_b()
        return
    mm.err("Widget not found for on_destroy_b, id {0:x}".format(wid_id))
    return


def on_tick_callback(wid_id):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_tick is None:
            return
        wid.on_tick()
        return
    mm.err("Widget not found for on_tick, id {0:x}".format(wid_id))
    return


def on_display_callback(wid_id):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_display is None:
            return
        wid.on_display()
        return
    mm.err("Widget not found for on_display, id {0:x}".format(wid_id))
    return


def on_display_win_callback(wid_id):
    wid = all_wids[wid_id]
    if wid is not None:
        if wid.on_display_win is None:
            return
        wid.on_display_win()
        return
    mm.err("Widget not found for " +
           "on_display_win, id {0:x}".format(wid_id))
    return


def mouse_hide(**kw):
    mm.wid_mouse_hide(**kw)
