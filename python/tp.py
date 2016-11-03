import mm
import random

all_tps = {}
all_treasure_tps = []
all_plant_tps = []
all_tree_tps = []


class Tp:
    """thing template"""

    def __init__(self,
                 name,
                 is_treasure=False,
                 is_plant=False,
                 is_tree=False,
                 d1000_appearing_roll=0):

        self.name = name
        self.is_treasure = is_treasure
        self.is_plant = is_plant
        self.is_tree = is_tree
        self.d1000_appearing_roll = d1000_appearing_roll

        #
        # Load it into the game engine
        #
        mm.tp_load(self)
        self.set_raw_name(name)

        all_tps[name] = self
        if is_treasure:
            all_treasure_tps.append(name)
        if is_plant:
            all_plant_tps.append(name)
        if is_tree:
            all_tree_tps.append(name)

        self.long_name = None
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
        self.is_rrr5 = False
        self.is_rrr6 = False
        self.is_rrr7 = False
        self.is_rrr8 = False
        self.is_rrr9 = False
        self.is_rrr10 = False
        self.is_rrr11 = False
        self.is_rrr12 = False
        self.is_rrr13 = False
        self.is_rrr14 = False
        self.is_rrr15 = False
        self.is_rrr16 = False
        self.is_rrr17 = False
        self.is_rrr18 = False
        self.is_rrr19 = False
        self.is_rrr20 = False
        self.is_rrr21 = False
        self.is_rrr22 = False
        self.is_rrr23 = False
        self.is_rrr24 = False
        self.is_rrr25 = False
        self.is_rrr26 = False
        self.is_rrr27 = False
        self.is_rrr28 = False
        self.is_rrr29 = False
        self.is_rrr30 = False
        self.is_rrr31 = False
        self.is_rrr32 = False
        self.is_rrr33 = False
        self.is_rrr34 = False
        self.is_rrr35 = False
        self.is_rrr36 = False
        self.is_rrr37 = False
        self.is_rrr38 = False
        self.is_rrr39 = False
        self.is_rrr40 = False
        self.is_rrr41 = False
        self.is_rrr42 = False
        self.is_rrr43 = False
        self.is_rrr44 = False
        self.is_rrr45 = False
        self.is_rrr46 = False
        self.is_rrr47 = False
        self.is_rrr48 = False
        self.is_rrr49 = False
        self.is_rrr50 = False
        self.is_rrr51 = False
        self.is_rrr52 = False
        self.is_rrr53 = False
        self.is_rrr54 = False
        self.is_rrr55 = False
        self.is_rrr56 = False
        self.is_rrr57 = False
        self.is_rrr58 = False
        self.is_rrr59 = False
        self.is_rrr60 = False
        self.is_rrr61 = False
        self.is_rrr62 = False
        self.is_rrr63 = False
        self.is_rrr64 = False
        self.is_rrr65 = False
        self.is_rrr66 = False
        self.is_rrr67 = False
        self.is_rrr68 = False
        self.is_rrr69 = False
        self.is_rrr70 = False
        self.is_rrr71 = False
        self.is_rrr72 = False
        self.is_rrr73 = False
        self.is_rrr74 = False
        self.is_rrr75 = False
        self.is_rrr76 = False
        self.is_rrr77 = False
        self.is_rrr78 = False
        self.is_rrr79 = False
        self.is_rrr80 = False
        self.is_rrr81 = False
        self.is_rrr82 = False
        self.is_rrr83 = False
        self.is_rrr84 = False
        self.is_rrr85 = False
        self.is_rrr86 = False
        self.is_rrr87 = False
        self.is_rrr88 = False
        self.is_rrr89 = False
        self.is_rrr90 = False
        self.is_rrr91 = False
        self.is_rrr92 = False
        self.is_rrr93 = False
        self.is_rrr94 = False
        self.is_rrr95 = False
        self.is_rrr96 = False
        self.is_dirt = False
        self.is_sand = False
        self.is_plant = False
        self.is_grass = False
        self.is_bridge = False
        self.is_movement_blocking = False
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

    def set_long_name(self, value):
        self.long_name = value

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

    def set_is_rrr14(self, value):
        self.is_rrr14 = value
        mm.tp_set_is_rrr14(self, value)

    def set_is_rrr15(self, value):
        self.is_rrr15 = value
        mm.tp_set_is_rrr15(self, value)

    def set_is_rrr16(self, value):
        self.is_rrr16 = value
        mm.tp_set_is_rrr16(self, value)

    def set_is_rrr17(self, value):
        self.is_rrr17 = value
        mm.tp_set_is_rrr17(self, value)

    def set_is_rrr18(self, value):
        self.is_rrr18 = value
        mm.tp_set_is_rrr18(self, value)

    def set_is_rrr19(self, value):
        self.is_rrr19 = value
        mm.tp_set_is_rrr19(self, value)

    def set_is_rrr20(self, value):
        self.is_rrr20 = value
        mm.tp_set_is_rrr20(self, value)

    def set_is_rrr21(self, value):
        self.is_rrr21 = value
        mm.tp_set_is_rrr21(self, value)

    def set_is_rrr22(self, value):
        self.is_rrr22 = value
        mm.tp_set_is_rrr22(self, value)

    def set_is_rrr23(self, value):
        self.is_rrr23 = value
        mm.tp_set_is_rrr23(self, value)

    def set_is_rrr24(self, value):
        self.is_rrr24 = value
        mm.tp_set_is_rrr24(self, value)

    def set_is_rrr25(self, value):
        self.is_rrr25 = value
        mm.tp_set_is_rrr25(self, value)

    def set_is_rrr26(self, value):
        self.is_rrr26 = value
        mm.tp_set_is_rrr26(self, value)

    def set_is_rrr27(self, value):
        self.is_rrr27 = value
        mm.tp_set_is_rrr27(self, value)

    def set_is_rrr28(self, value):
        self.is_rrr28 = value
        mm.tp_set_is_rrr28(self, value)

    def set_is_rrr29(self, value):
        self.is_rrr29 = value
        mm.tp_set_is_rrr29(self, value)

    def set_is_rrr30(self, value):
        self.is_rrr30 = value
        mm.tp_set_is_rrr30(self, value)

    def set_is_rrr31(self, value):
        self.is_rrr31 = value
        mm.tp_set_is_rrr31(self, value)

    def set_is_rrr32(self, value):
        self.is_rrr32 = value
        mm.tp_set_is_rrr32(self, value)

    def set_is_rrr33(self, value):
        self.is_rrr33 = value
        mm.tp_set_is_rrr33(self, value)

    def set_is_rrr34(self, value):
        self.is_rrr34 = value
        mm.tp_set_is_rrr34(self, value)

    def set_is_rrr35(self, value):
        self.is_rrr35 = value
        mm.tp_set_is_rrr35(self, value)

    def set_is_rrr36(self, value):
        self.is_rrr36 = value
        mm.tp_set_is_rrr36(self, value)

    def set_is_rrr37(self, value):
        self.is_rrr37 = value
        mm.tp_set_is_rrr37(self, value)

    def set_is_rrr38(self, value):
        self.is_rrr38 = value
        mm.tp_set_is_rrr38(self, value)

    def set_is_rrr39(self, value):
        self.is_rrr39 = value
        mm.tp_set_is_rrr39(self, value)

    def set_is_rrr40(self, value):
        self.is_rrr40 = value
        mm.tp_set_is_rrr40(self, value)

    def set_is_rrr41(self, value):
        self.is_rrr41 = value
        mm.tp_set_is_rrr41(self, value)

    def set_is_rrr42(self, value):
        self.is_rrr42 = value
        mm.tp_set_is_rrr42(self, value)

    def set_is_rrr43(self, value):
        self.is_rrr43 = value
        mm.tp_set_is_rrr43(self, value)

    def set_is_rrr44(self, value):
        self.is_rrr44 = value
        mm.tp_set_is_rrr44(self, value)

    def set_is_rrr45(self, value):
        self.is_rrr45 = value
        mm.tp_set_is_rrr45(self, value)

    def set_is_rrr46(self, value):
        self.is_rrr46 = value
        mm.tp_set_is_rrr46(self, value)

    def set_is_rrr47(self, value):
        self.is_rrr47 = value
        mm.tp_set_is_rrr47(self, value)

    def set_is_rrr48(self, value):
        self.is_rrr48 = value
        mm.tp_set_is_rrr48(self, value)

    def set_is_rrr49(self, value):
        self.is_rrr49 = value
        mm.tp_set_is_rrr49(self, value)

    def set_is_rrr50(self, value):
        self.is_rrr50 = value
        mm.tp_set_is_rrr50(self, value)

    def set_is_rrr51(self, value):
        self.is_rrr51 = value
        mm.tp_set_is_rrr51(self, value)

    def set_is_rrr52(self, value):
        self.is_rrr52 = value
        mm.tp_set_is_rrr52(self, value)

    def set_is_rrr53(self, value):
        self.is_rrr53 = value
        mm.tp_set_is_rrr53(self, value)

    def set_is_rrr54(self, value):
        self.is_rrr54 = value
        mm.tp_set_is_rrr54(self, value)

    def set_is_rrr55(self, value):
        self.is_rrr55 = value
        mm.tp_set_is_rrr55(self, value)

    def set_is_rrr56(self, value):
        self.is_rrr56 = value
        mm.tp_set_is_rrr56(self, value)

    def set_is_rrr57(self, value):
        self.is_rrr57 = value
        mm.tp_set_is_rrr57(self, value)

    def set_is_rrr58(self, value):
        self.is_rrr58 = value
        mm.tp_set_is_rrr58(self, value)

    def set_is_rrr59(self, value):
        self.is_rrr59 = value
        mm.tp_set_is_rrr59(self, value)

    def set_is_rrr60(self, value):
        self.is_rrr60 = value
        mm.tp_set_is_rrr60(self, value)

    def set_is_rrr61(self, value):
        self.is_rrr61 = value
        mm.tp_set_is_rrr61(self, value)

    def set_is_rrr62(self, value):
        self.is_rrr62 = value
        mm.tp_set_is_rrr62(self, value)

    def set_is_rrr63(self, value):
        self.is_rrr63 = value
        mm.tp_set_is_rrr63(self, value)

    def set_is_rrr64(self, value):
        self.is_rrr64 = value
        mm.tp_set_is_rrr64(self, value)

    def set_is_rrr65(self, value):
        self.is_rrr65 = value
        mm.tp_set_is_rrr65(self, value)

    def set_is_rrr66(self, value):
        self.is_rrr66 = value
        mm.tp_set_is_rrr66(self, value)

    def set_is_rrr67(self, value):
        self.is_rrr67 = value
        mm.tp_set_is_rrr67(self, value)

    def set_is_rrr68(self, value):
        self.is_rrr68 = value
        mm.tp_set_is_rrr68(self, value)

    def set_is_rrr69(self, value):
        self.is_rrr69 = value
        mm.tp_set_is_rrr69(self, value)

    def set_is_rrr70(self, value):
        self.is_rrr70 = value
        mm.tp_set_is_rrr70(self, value)

    def set_is_rrr71(self, value):
        self.is_rrr71 = value
        mm.tp_set_is_rrr71(self, value)

    def set_is_rrr72(self, value):
        self.is_rrr72 = value
        mm.tp_set_is_rrr72(self, value)

    def set_is_rrr73(self, value):
        self.is_rrr73 = value
        mm.tp_set_is_rrr73(self, value)

    def set_is_rrr74(self, value):
        self.is_rrr74 = value
        mm.tp_set_is_rrr74(self, value)

    def set_is_rrr75(self, value):
        self.is_rrr75 = value
        mm.tp_set_is_rrr75(self, value)

    def set_is_rrr76(self, value):
        self.is_rrr76 = value
        mm.tp_set_is_rrr76(self, value)

    def set_is_rrr77(self, value):
        self.is_rrr77 = value
        mm.tp_set_is_rrr77(self, value)

    def set_is_rrr78(self, value):
        self.is_rrr78 = value
        mm.tp_set_is_rrr78(self, value)

    def set_is_rrr79(self, value):
        self.is_rrr79 = value
        mm.tp_set_is_rrr79(self, value)

    def set_is_rrr80(self, value):
        self.is_rrr80 = value
        mm.tp_set_is_rrr80(self, value)

    def set_is_rrr81(self, value):
        self.is_rrr81 = value
        mm.tp_set_is_rrr81(self, value)

    def set_is_rrr82(self, value):
        self.is_rrr82 = value
        mm.tp_set_is_rrr82(self, value)

    def set_is_rrr83(self, value):
        self.is_rrr83 = value
        mm.tp_set_is_rrr83(self, value)

    def set_is_rrr84(self, value):
        self.is_rrr84 = value
        mm.tp_set_is_rrr84(self, value)

    def set_is_rrr85(self, value):
        self.is_rrr85 = value
        mm.tp_set_is_rrr85(self, value)

    def set_is_rrr86(self, value):
        self.is_rrr86 = value
        mm.tp_set_is_rrr86(self, value)

    def set_is_rrr87(self, value):
        self.is_rrr87 = value
        mm.tp_set_is_rrr87(self, value)

    def set_is_rrr88(self, value):
        self.is_rrr88 = value
        mm.tp_set_is_rrr88(self, value)

    def set_is_rrr89(self, value):
        self.is_rrr89 = value
        mm.tp_set_is_rrr89(self, value)

    def set_is_rrr90(self, value):
        self.is_rrr90 = value
        mm.tp_set_is_rrr90(self, value)

    def set_is_rrr91(self, value):
        self.is_rrr91 = value
        mm.tp_set_is_rrr91(self, value)

    def set_is_rrr92(self, value):
        self.is_rrr92 = value
        mm.tp_set_is_rrr92(self, value)

    def set_is_rrr93(self, value):
        self.is_rrr93 = value
        mm.tp_set_is_rrr93(self, value)

    def set_is_rrr94(self, value):
        self.is_rrr94 = value
        mm.tp_set_is_rrr94(self, value)

    def set_is_rrr95(self, value):
        self.is_rrr95 = value
        mm.tp_set_is_rrr95(self, value)

    def set_is_rrr96(self, value):
        self.is_rrr96 = value
        mm.tp_set_is_rrr96(self, value)

    def set_is_dirt(self, value):
        self.is_dirt = value
        mm.tp_set_is_dirt(self, value)

    def set_is_sand(self, value):
        self.is_sand = value
        mm.tp_set_is_sand(self, value)

    def set_is_plant(self, value):
        self.is_plant = value
        mm.tp_set_is_plant(self, value)

    def set_is_tree(self, value):
        self.is_tree = value
        mm.tp_set_is_tree(self, value)

    def set_is_grass(self, value):
        self.is_grass = value
        mm.tp_set_is_grass(self, value)

    def set_is_bridge(self, value):
        self.is_bridge = value
        mm.tp_set_is_bridge(self, value)

    def set_is_movement_blocking(self, value):
        self.is_movement_blocking = value
        mm.tp_set_is_movement_blocking(self, value)

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


def get_random_plant():
    while True:
        tp = all_tps[random.choice(all_plant_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_tree():
    while True:
        tp = all_tps[random.choice(all_tree_tps)]

        roll = random.randint(1, 1000)
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
