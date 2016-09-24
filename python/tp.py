import mm

all_tps = {}


class Tp:
    """thing template"""

    def __init__(self, name):
        self.name = name
        mm.tp_load(self)
        self.set_raw_name(name)
        all_tps[name] = self

        self.description = None
        self.short_name = None
        self.raw_name = None
        self.light_radius = None
        self.scale = None
        self.z_depth = None
        self.z_order = None
        self.map_depth = None
        self.speed = None
        self.blit_top_off = None
        self.blit_bot_off = None
        self.blit_left_off = None
        self.light_tint = None
        self.is_animated = False
        self.is_animated_no_dir = False
        self.is_animation = False
        self.is_candle_light = False
        self.is_cats_eyes = False
        self.is_door = False
        self.is_effect_fade_in_out = False
        self.is_effect_pulse = False
        self.is_effect_rotate_2way = False
        self.is_effect_sway = False
        self.is_explosion = False
        self.is_joinable = False
        self.is_light_source = False
        self.is_monst = False
        self.is_player = False
        self.is_food = False
        self.is_weapon = False
        self.is_rrr1 = False
        self.is_rrr2 = False
        self.is_rrr3 = False
        self.is_rrr4 = False
        self.is_rrr5 = False
        self.is_rrr6 = False
        self.is_rrr7 = False
        self.is_rrr8 = False
        self.is_rrr9 = False
        self.is_rrr10 = False
        self.is_rrr11 = False
        self.is_rrr12 = False
        self.is_rrr13 = False
        self.is_healing = False
        self.is_armor = False
        self.is_magical = False
        self.is_shadow_caster = False
        self.is_shadow_caster_soft = False
        self.is_sleeping = False
        self.is_wall = False
        self.is_floor = False
        self.tiles = []
        self.damage = None
        self.is_double_handed = False

    def set_tile(self, tile=None, **kw):
        self.tiles.append(tile)
        mm.tp_set_tile(self, tile, **kw)

    def set_description(self, value):
        self.description = value

    def set_damage(self, value):
        self.damage = value

    def set_is_double_handed(self, value):
        self.is_double_handed = value

    def set_raw_name(self, value):
        self.raw_name = value
        mm.tp_set_raw_name(self, value)

    def set_light_radius(self, value):
        self.light_radius = value
        mm.tp_set_light_radius(self, value)

    def set_scale(self, value):
        self.scale = value
        mm.tp_set_scale(self, value)

    def set_z_depth(self, value):
        self.z_depth = value
        mm.tp_set_z_depth(self, value)

    def set_short_name(self, value):
        self.short_name = value
        mm.tp_set_short_name(self, value)

    def set_z_order(self, value):
        self.z_order = value
        mm.tp_set_z_order(self, value)

    def set_map_depth(self, value):
        self.map_depth = value
        mm.tp_set_map_depth(self, value)

    def set_speed(self, value):
        self.speed = value
        mm.tp_set_speed(self, value)

    def set_blit_top_off(self, value):
        self.blit_top_off = value
        mm.tp_set_blit_top_off(self, value)

    def set_blit_bot_off(self, value):
        self.blit_bot_off = value
        mm.tp_set_blit_bot_off(self, value)

    def set_blit_left_off(self, value):
        self.blit_left_off = value
        mm.tp_set_blit_left_off(self, value)

    def set_light_tint(self, value):
        self.light_tint = value
        mm.tp_set_light_tint(self, value)

    def set_is_animated(self, value):
        self.is_animated = value
        mm.tp_set_is_animated(self, value)

    def set_is_animated_no_dir(self, value):
        self.is_animated_no_dir = value
        mm.tp_set_is_animated_no_dir(self, value)

    def set_is_animation(self, value):
        self.is_animation = value
        mm.tp_set_is_animation(self, value)

    def set_is_candle_light(self, value):
        self.is_candle_light = value
        mm.tp_set_is_candle_light(self, value)

    def set_is_cats_eyes(self, value):
        self.is_cats_eyes = value
        mm.tp_set_is_cats_eyes(self, value)

    def set_is_door(self, value):
        self.is_door = value
        mm.tp_set_is_door(self, value)

    def set_is_effect_fade_in_out(self, value):
        self.is_effect_fade_in_out = value
        mm.tp_set_is_effect_fade_in_out(self, value)

    def set_is_effect_pulse(self, value):
        self.is_effect_pulse = value
        mm.tp_set_is_effect_pulse(self, value)

    def set_is_effect_rotate_2way(self, value):
        self.is_effect_rotate_2way = value
        mm.tp_set_is_effect_rotate_2way(self, value)

    def set_is_effect_sway(self, value):
        self.is_effect_sway = value
        mm.tp_set_is_effect_sway(self, value)

    def set_is_explosion(self, value):
        self.is_explosion = value
        mm.tp_set_is_explosion(self, value)

    def set_is_joinable(self, value):
        self.is_joinable = value
        mm.tp_set_is_joinable(self, value)

    def set_is_light_source(self, value):
        self.is_light_source = value
        mm.tp_set_is_light_source(self, value)

    def set_is_monst(self, value):
        self.is_monst = value
        mm.tp_set_is_monst(self, value)

    def set_is_player(self, value):
        self.is_player = value
        mm.tp_set_is_player(self, value)

    def set_is_food(self, value):
        self.is_food = value
        mm.tp_set_is_food(self, value)

    def set_is_weapon(self, value):
        self.is_weapon = value
        mm.tp_set_is_weapon(self, value)

    def set_is_rrr1(self, value):
        self.is_rrr1 = value
        mm.tp_set_is_rrr1(self, value)

    def set_is_rrr2(self, value):
        self.is_rrr2 = value
        mm.tp_set_is_rrr2(self, value)

    def set_is_rrr3(self, value):
        self.is_rrr3 = value
        mm.tp_set_is_rrr3(self, value)

    def set_is_rrr4(self, value):
        self.is_rrr4 = value
        mm.tp_set_is_rrr4(self, value)

    def set_is_rrr5(self, value):
        self.is_rrr5 = value
        mm.tp_set_is_rrr5(self, value)

    def set_is_rrr6(self, value):
        self.is_rrr6 = value
        mm.tp_set_is_rrr6(self, value)

    def set_is_rrr7(self, value):
        self.is_rrr7 = value
        mm.tp_set_is_rrr7(self, value)

    def set_is_rrr8(self, value):
        self.is_rrr8 = value
        mm.tp_set_is_rrr8(self, value)

    def set_is_rrr9(self, value):
        self.is_rrr9 = value
        mm.tp_set_is_rrr9(self, value)

    def set_is_rrr10(self, value):
        self.is_rrr10 = value
        mm.tp_set_is_rrr10(self, value)

    def set_is_rrr11(self, value):
        self.is_rrr11 = value
        mm.tp_set_is_rrr11(self, value)

    def set_is_rrr12(self, value):
        self.is_rrr12 = value
        mm.tp_set_is_rrr12(self, value)

    def set_is_rrr13(self, value):
        self.is_rrr13 = value
        mm.tp_set_is_rrr13(self, value)

    def set_is_healing(self, value):
        self.is_healing = value
        mm.tp_set_is_healing(self, value)

    def set_is_armor(self, value):
        self.is_armor = value
        mm.tp_set_is_armor(self, value)

    def set_is_magical(self, value):
        self.is_magical = value
        mm.tp_set_is_magical(self, value)

    def set_is_shadow_caster(self, value):
        self.is_shadow_caster = value
        mm.tp_set_is_shadow_caster(self, value)

    def set_is_shadow_caster_soft(self, value):
        self.is_shadow_caster_soft = value
        mm.tp_set_is_shadow_caster_soft(self, value)

    def set_is_sleeping(self, value):
        self.is_sleeping = value
        mm.tp_set_is_sleeping(self, value)

    def set_is_wall(self, value):
        self.is_wall = value
        mm.tp_set_is_wall(self, value)

    def set_is_floor(self, value):
        self.is_floor = value
