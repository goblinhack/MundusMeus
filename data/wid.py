import traceback
import mm


class Wid:

    def __init__(self, name, parent=0, **kw):
        self.parent = parent
        self.name = name
        self.wid_id = mm.wid_new(self, parent, name, **kw)
        self.name = "{0:x}:{1}".format(self.wid_id, self.name)
        self.log("Created wid")

    def __str__(self):
        return "{0}".format(self.name)

    def destroy(self):
        self.log("Destroying wid")
        mm.wid_destroy(self)
        del self

    def destroy_in(self, **kw):
        self.log("Destroying delay")
        mm.wid_destroy_in(self, **kw)
        del self

    def fade_in(self, **kw):
        mm.wid_fade_in(self, **kw)

    def fade_out(self, **kw):
        mm.wid_fade_out(self, **kw)

    def fade_in_out(self, **kw):
        mm.wid_fade_in_out(self, **kw)

    def visible(seld, **kw):
        mm.wid_visible(self, **kw)

    def hide(seld, **kw):
        mm.wid_hide(self, **kw)

    def toggle_hidden(seld, **kw):
        mm.wid_toggle_hidden(self, **kw)

    def mouse_hide(seld, **kw):
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

    def get_size(self):
        return mm.wid_get_size(self)

    def get_size_pct(self):
        return mm.wid_get_size_pct(self)

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

def mouse_hide(**kw):
    mm.wid_mouse_hide(**kw)

