import traceback
import mm

all_wids = {}

class Wid:

    def __init__(self, name, parent=0, is_scrollbar=False, **kw):
        self.parent = parent
        self.name = name

        if is_scrollbar is True:
            self.wid_id = mm.wid_new_scrollbar(self, parent, **kw)
        else:
            self.wid_id = mm.wid_new(self, parent, name, **kw)

        self.name = "{0:x}:{1}".format(self.wid_id, self.name)
        self.log("Created wid")

        all_wids[self.wid_id] = self

        self.callback_on_tooltip = None
        self.callback_on_key_down = None
        self.callback_on_key_up = None
        self.callback_on_joy_button = None
        self.callback_on_mouse_down = None
        self.callback_on_mouse_motion = None
        self.callback_on_mouse_focus_begin = None
        self.callback_on_mouse_focus_end = None
        self.callback_on_mouse_over_begin = None
        self.callback_on_mouse_over_end = None
        self.callback_on_mouse_up = None
        self.callback_on_destroy = None
        self.callback_on_destroy_begin = None
        self.callback_on_tick = None
        self.callback_on_display = None
        self.callback_on_display_top_level = None

    def __str__(self):
        return "{0}".format(self.name)

    def destroy(self):
        self.log("Destroying wid")
        mm.wid_destroy(self)
        all_wids[self.wid_id] = None
        del self

    def destroy_in(self, **kw):
        self.log("Destroying delay")
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
        mm.wid_visible(self, **kw)

    def hide(self, **kw):
        mm.wid_hide(self, **kw)

    def toggle_hidden(self, **kw):
        mm.wid_toggle_hidden(self, **kw)

    def mouse_hide(self, **kw):
        mm.wid_mouse_hide(self, **kw)

    def set_shape(self, **kw):
        mm.wid_set_shape(self, **kw)

    def set_tl_br(self, **kw):
        mm.wid_set_tl_br(self, **kw)

    def set_tl_br_pct(self, x1, y1, x2, y2):
        mm.wid_set_tl_br_pct(self, x1, y1, x2, y2)

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
            wid = all_wids[wid_id]
            if wid != None:
                return wid
        return None

    def get_parent(self):
        wid_id = mm.wid_get_parent(self)
        if wid_id is not None:
            wid = all_wids[wid_id]
            if wid != None:
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

    def set_focusable(self, **kw):
        mm.wid_set_focusable(self, **kw)

    def set_movable(self, **kw):
        mm.wid_set_movable(self, **kw)

    def set_movable_bounded(self, **kw):
        mm.wid_set_movable_bounded(self, **kw)

    def set_movable_horiz(self, **kw):
        mm.wid_set_movable_horiz(self, **kw)

    def set_movable_vert(self, **kw):
        mm.wid_set_movable_vert(self, **kw)

    def con(self, msg):
        mm.con("p-wid {0}: {1}".format(str(self), msg))

    def log(self, msg):
        mm.log("p-wid {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.err("p-wid {0}: ERROR: {1}".format(self.name, msg))
        traceback.print_stack()

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

    def set_on_mouse_down(self, callback):
        self.callback_on_mouse_down = callback
        if callback is None:
            return
        mm.wid_set_on_mouse_down(self)

    def set_on_mouse_motion(self, callback):
        self.callback_on_mouse_motion = callback
        if callback is None:
            return
        mm.wid_set_on_mouse_motion(self)

    def set_on_mouse_focus_begin(self, callback):
        self.callback_on_mouse_focus_begin = callback
        if callback is None:
            return
        mm.wid_set_on_mouse_focus_begin(self)

    def set_on_mouse_focus_end(self, callback):
        self.callback_on_mouse_focus_end = callback
        if callback is None:
            return
        mm.wid_set_on_mouse_focus_end(self)

    def set_on_mouse_over_begin(self, callback):
        self.callback_on_mouse_over_begin = callback
        if callback is None:
            return
        mm.wid_set_on_mouse_over_begin(self)

    def set_on_mouse_over_end(self, callback):
        self.callback_on_mouse_over_end = callback
        if callback is None:
            return
        mm.wid_set_on_mouse_over_end(self)

    def set_on_mouse_up(self, callback):
        self.callback_on_mouse_up = callback
        if callback is None:
            return
        mm.wid_set_on_mouse_up(self)

    def set_on_destroy(self, callback):
        self.callback_on_destroy = callback
        if callback is None:
            return
        mm.wid_set_on_destroy(self)

    def set_on_destroy_begin(self, callback):
        self.callback_on_destroy_begin = callback
        if callback is None:
            return
        mm.wid_set_on_destroy_begin(self)

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

    def set_on_display_top_level(self, callback):
        self.callback_on_display_top_level = callback
        mm.wid_set_on_display_top_level(self)

    def on_tooltip(self, tooltip):
        if self.callback_on_tooltip != None:
            self.callback_on_tooltip(self, tooltip)
            return
        mm.err("wid_set_on_tooltip not handled")

    def on_key_down(self, sym, mod):
        if self.callback_on_key_down != None:
            return self.callback_on_key_down(self, sym, mod)
        mm.err("wid_set_on_key_down not handled")

    def on_key_up(self, sym, mod):
        if self.callback_on_key_up != None:
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
        if self.callback_on_joy_button != None:
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

    def on_mouse_down(self, x, y, button):
        if self.callback_on_mouse_down != None:
            return self.callback_on_mouse_down(self, x, y, button)
        mm.err("wid_set_on_mouse_down not handled")

    def on_mouse_motion(self, x, y, relx, rely, wheelx, wheely):
        if self.callback_on_mouse_motion != None:
            return self.callback_on_mouse_motion(self, x, y, relx, rely, wheelx, wheely)
        self.err("wid_set_on_mouse_motion not handled".format(x,y,relx,rely,wheelx,wheely))
        return False

    def on_mouse_focus_begin(self):
        if self.callback_on_mouse_focus_begin != None:
            self.callback_on_mouse_focus_begin(self)
            return
        mm.err("wid_set_on_mouse_focus_begin not handled")

    def on_mouse_focus_end(self):
        if self.callback_on_mouse_focus_end != None:
            self.callback_on_mouse_focus_end(self)
            return
        mm.err("wid_set_on_mouse_focus_end not handled")

    def on_mouse_over_begin(self, relx, rely, wheelx, wheely):
        if self.callback_on_mouse_over_begin != None:
            return self.callback_on_mouse_over_begin(self, relx, rely, wheelx, wheely)
        self.err("wid_set_on_mouse_over_begin not handled")

    def on_mouse_over_end(self):
        if self.callback_on_mouse_over_end != None:
            self.callback_on_mouse_over_end(self)
            return
        mm.err("wid_set_on_mouse_over_end not handled")

    def on_mouse_up(self, x, y, button):
        if self.callback_on_mouse_up != None:
            return self.callback_on_mouse_up(self, x, y, button)
        mm.err("wid_set_on_mouse_up not handled")

    def on_destroy(self):
        if self.callback_on_destroy != None:
            self.callback_on_destroy(self)
            return
        mm.err("wid_set_on_destroy not handled")

    def on_destroy_begin(self):
        if self.callback_on_destroy_begin != None:
            self.callback_on_destroy_begin(self)
            return
        mm.err("wid_set_on_destroy_begin not handled")

    def on_tick(self):
        if self.callback_on_tick != None:
            self.callback_on_tick(self)
            return
        mm.err("wid_set_on_tick not handled")

    def on_display(self):
        if self.callback_on_display != None:
            self.callback_on_display(self)
            return
        mm.err("wid_set_on_display not handled")

    def on_display_top_level(self):
        if self.callback_on_display_top_level != None:
            self.callback_on_display_top_level(self)
            return
        mm.err("wid_set_on_display_top_level not handled")

def on_mouse_motion_callback(wid_id, x, y, relx, rely, wheelx, wheely):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_mouse_motion is None:
            return False
        return wid.on_mouse_motion(x, y, relx, rely, wheelx, wheely)
    mm.err("Widget not found for on_mouse_motion_callback, id {0:x}".format(wid_id))
    return False

def on_tooltip_callback(wid_id, tooltip):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_tooltip is None:
            return False
        wid.on_tooltip(tooltip)
        return
    mm.err("Widget not found for on_tooltip, id {0:x}".format(wid_id))

def on_key_down_callback(wid_id, sym, mod):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_key_down is None:
            return False
        return wid.on_key_down(sym, mod)
    mm.err("Widget not found for on_key_down, id {0:x}".format(wid_id))
    return False

def on_key_up_callback(wid_id, sym, mod):
    wid = all_wids[wid_id]
    if wid != None:
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
    if wid != None:
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
                        button_right_fire);
        return
    mm.err("Widget not found for on_joy_button, id {0:x}".format(wid_id))
    return False

def on_mouse_down_callback(wid_id, x, y, button):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_mouse_down is None:
            return False
        return wid.on_mouse_down(x, y, button)
    mm.err("Widget not found for on_mouse_down, id {0:x}".format(wid_id))
    return False

def on_mouse_focus_begin_callback(wid_id):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_focus_begin is None:
            return False
        wid.on_mouse_focus_begin()
        return
    mm.err("Widget not found for on_mouse_focus_begin, id {0:x}".format(wid_id))

def on_mouse_focus_end_callback(wid_id):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_focus_end is None:
            return False
        wid.on_mouse_focus_end()
        return
    mm.err("Widget not found for on_mouse_focus_end, id {0:x}".format(wid_id))

def on_mouse_over_begin_callback(wid_id, relx, rely, wheelx, wheely):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_mouse_over_begin is None:
            return False
        return wid.on_mouse_over_begin(relx, rely, wheelx, wheely)
    mm.err("Widget not found for on_mouse_over_begin, id {0:x}".format(wid_id))
    return False

def on_mouse_over_end_callback(wid_id):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_mouse_over_end is None:
            return False
        wid.on_mouse_over_end()
        return
    mm.err("Widget not found for on_mouse_over_end, id {0:x}".format(wid_id))

def on_mouse_up_callback(wid_id, x, y, button):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_mouse_up is None:
            return False
        return wid.on_mouse_up(x, y, button)
    mm.err("Widget not found for on_mouse_up, id {0:x}".format(wid_id))
    return False

def on_destroy_callback(wid_id):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_destroy is None:
            return
        wid.on_destroy()
        return
    mm.err("Widget not found for on_destroy, id {0:x}".format(wid_id))
    return

def on_destroy_begin_callback(wid_id):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_destroy_begin is None:
            return
        wid.on_destroy_begin()
        return
    mm.err("Widget not found for on_destroy_begin, id {0:x}".format(wid_id))
    return

def on_tick_callback(wid_id):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_tick is None:
            return
        wid.on_tick()
        return
    mm.err("Widget not found for on_tick, id {0:x}".format(wid_id))
    return 

def on_display_callback(wid_id):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_display is None:
            return
        wid.on_display()
        return 
    mm.err("Widget not found for on_display, id {0:x}".format(wid_id))
    return 

def on_display_top_level_callback(wid_id):
    wid = all_wids[wid_id]
    if wid != None:
        if wid.on_display_top_level is None:
            return
        wid.on_display_top_level()
        return 
    mm.err("Widget not found for on_display_top_level, id {0:x}".format(wid_id))
    return

def mouse_hide(**kw):
    mm.wid_mouse_hide(**kw)
