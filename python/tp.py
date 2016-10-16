import mm
import random

all_tps = {}
all_treasure_tps = []


class Tp:
    """thing template"""

    def __init__(self,
                 name,
                 is_treasure=False,
                 d1000_appearing_roll=0):

        self.name = name
        self.is_treasure = is_treasure
        self.d1000_appearing_roll = d1000_appearing_roll

        #
        # Load it into the game engine
        #
        mm.tp_load(self)
        self.set_raw_name(name)

        all_tps[name] = self
        if is_treasure:
            all_treasure_tps.append(name)

        self.description = None
        self.short_name = None
        self.raw_name = None
        self.light_radius = None
        self.scale = None
        self.z_depth = None
        self.z_order = None
        self.speed = None
        self.blit_top_off = None
        self.blit_bot_off = None
        self.blit_left_off = None
        self.blit_right_off = None
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
        self.is_obstacle = False
        self.is_treasure = False
        self.is_entrance = False
        self.is_exit = False
        self.is_chasm_smoke = False
        self.is_key = False
        self.is_dusty = False
        self.is_corridor = False
        self.light_pulse_amount = False
        self.is_healing = False
        self.is_armor = False
        self.is_magical = False
        self.is_shadow_caster = False
        self.is_shadow_caster_soft = False
        self.is_sleeping = False
        self.is_wall = False
        self.is_cwall = False
        self.is_floor = False
        self.is_lava = False
        self.is_rock = False
        self.is_water = False
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

    def set_short_name(self, value):
        self.short_name = value
        mm.tp_set_short_name(self, value)

    def set_z_order(self, value):
        self.z_order = value
        mm.tp_set_z_order(self, value)

    def set_z_depth(self, value):
        self.z_depth = value
        mm.tp_set_z_depth(self, value)

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

    def set_blit_right_off(self, value):
        self.blit_right_off = value
        mm.tp_set_blit_right_off(self, value)

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

    def set_is_obstacle(self, value):
        self.is_obstacle = value
        mm.tp_set_is_obstacle(self, value)

    def set_is_treasure(self, value):
        self.is_treasure = value
        mm.tp_set_is_treasure(self, value)

    def set_is_entrance(self, value):
        self.is_entrance = value
        mm.tp_set_is_entrance(self, value)

    def set_is_exit(self, value):
        self.is_exit = value
        mm.tp_set_is_exit(self, value)

    def set_is_chasm_smoke(self, value):
        self.is_chasm_smoke = value
        mm.tp_set_is_chasm_smoke(self, value)

    def set_is_key(self, value):
        self.is_key = value
        mm.tp_set_is_key(self, value)

    def set_is_dusty(self, value):
        self.is_dusty = value
        mm.tp_set_is_dusty(self, value)

    def set_is_corridor(self, value):
        self.is_corridor = value
        mm.tp_set_is_corridor(self, value)

    def set_light_pulse_amount(self, value):
        self.light_pulse_amount = value
        mm.tp_set_light_pulse_amount(self, value)

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

    def set_is_cwall(self, value):
        self.is_cwall = value
        mm.tp_set_is_wall(self, value)

    def set_is_floor(self, value):
        self.is_floor = value
        mm.tp_set_is_floor(self, value)

    def set_is_door(self, value):
        self.is_door = value
        mm.tp_set_is_door(self, value)

    def set_is_lava(self, value):
        self.is_lava = value
        mm.tp_set_is_lava(self, value)

    def set_is_rock(self, value):
        self.is_rock = value
        mm.tp_set_is_wall(self, value)

    def set_is_water(self, value):
        self.is_water = value
        mm.tp_set_is_water(self, value)

    def set_is_minable(self, value):
        self.is_minable = value

def get_random_treasure(toughness=0):
    while True:
        tp = all_tps[random.choice(all_treasure_tps)]
        if tp.is_minable:
            continue

        roll = random.randint(1, 1000) + toughness * 10
        if roll >= tp.d1000_appearing_roll:
            return tp

def get_random_minable_treasure(toughness=0):
    while True:
        tp = all_tps[random.choice(all_treasure_tps)]
        if not tp.is_minable:
            continue

        roll = random.randint(1, 1000) + toughness * 10
        if roll >= tp.d1000_appearing_roll:
            return tp


