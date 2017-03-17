import mm
import random

all_tps = {}
all_treasure_tps = []
all_plant_tps = []
all_small_rock_tps = []
all_snow_mound_tps = []
all_marsh_plant_tps = []
all_tree_tps = []
all_tree_snow_tps = []
all_tree_conifer_tps = []
all_floor_tps = []
all_corridor_tps = []
all_wall_tps = []
all_cwall_tps = []
all_hwall_tps = []
all_door_tps = []
all_dungeon_tps = []
all_dungeon_snow_tps = []


class Tp:
    """thing template"""

    def __init__(self,
                 name,
                 is_treasure=False,
                 is_plant=False,
                 is_small_rock=False,
                 is_snow_mound=False,
                 is_marsh_plant=False,
                 is_tree=False,
                 is_tree_snow=False,
                 is_tree_conifer=False,
                 is_floor=False,
                 is_corridor=False,
                 is_wall=False,
                 is_cwall=False,
                 is_hwall=False,
                 is_door=False,
                 is_dungeon=False,
                 is_dungeon_snow=False,
                 d1000_appearing_roll=0):

        self.name = name
        self.d1000_appearing_roll = d1000_appearing_roll

        self.is_treasure = is_treasure
        if is_treasure:
            all_treasure_tps.append(name)

        self.is_small_rock = is_small_rock
        if is_small_rock:
            all_small_rock_tps.append(name)

        self.is_snow_mound = is_snow_mound
        if is_snow_mound:
            all_snow_mound_tps.append(name)

        self.is_plant = is_plant
        if is_plant:
            all_plant_tps.append(name)

        self.is_marsh_plant = is_marsh_plant
        if is_marsh_plant:
            all_marsh_plant_tps.append(name)

        self.is_tree = is_tree
        if is_tree:
            all_tree_tps.append(name)

        self.is_tree_snow = is_tree_snow
        if is_tree_snow:
            all_tree_snow_tps.append(name)

        self.is_tree_conifer = is_tree_conifer
        if is_tree_conifer:
            all_tree_conifer_tps.append(name)

        self.is_wall = is_wall
        if is_wall:
            all_wall_tps.append(name)

        self.is_cwall = is_cwall
        if is_cwall:
            all_cwall_tps.append(name)

        self.is_hwall = is_hwall
        if is_hwall:
            all_hwall_tps.append(name)

        self.is_door = is_door
        if is_door:
            all_door_tps.append(name)

        self.is_floor = is_floor
        if is_floor:
            all_floor_tps.append(name)

        self.is_corridor = is_corridor
        if is_corridor:
            all_corridor_tps.append(name)

        self.is_dungeon = is_dungeon
        if is_dungeon:
            all_dungeon_tps.append(name)

        self.is_dungeon_snow = is_dungeon_snow
        if is_dungeon_snow:
            all_dungeon_snow_tps.append(name)

        #
        # Load it into the game engine
        #
        mm.tp_load(self)
        self.set_raw_name(name)

        all_tps[name] = self
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
        self.has_shadow = False
        self.is_chair = False
        self.is_chest = False
        self.is_bookcase = False
        self.is_table = False
        self.is_lawn = False
        self.is_carpet_deco = False
        self.is_carpet = False
        self.is_house_item = False
        self.is_cwall = False
        self.is_animated_lr_flip = False
        self.is_gravel_snow_deco = False
        self.is_landrock_snow = False
        self.is_hidden_from_editor = False
        self.is_dungeon_item = False
        self.is_difficulty_easy = False
        self.is_difficulty_hard = False
        self.is_difficulty_nightmare = False
        self.is_world_item = False
        self.is_solid_ground = False
        self.is_dirt_deco = False
        self.is_dirt_snow_deco = False
        self.is_grass_deco = False
        self.is_grass_snow_deco = False
        self.is_gravel_deco = False
        self.is_sand_deco = False
        self.is_sand_snow_deco = False
        self.is_snow_deco = False
        self.is_sand_snow = False
        self.is_dirt_snow = False
        self.is_grass_snow = False
        self.is_road_snow = False
        self.is_road = False
        self.is_dungeon_snow = False
        self.is_dungeon = False
        self.is_focus = False
        self.is_gravel_snow = False
        self.is_tree_conifer = False
        self.is_snow_mound = False
        self.is_rock = False
        self.is_ice = False
        self.is_ice = False
        self.is_gravel = False
        self.is_small_rock = False
        self.is_marsh_plant = False
        self.is_landrock = False
        self.is_snow = False
        self.is_hidden = False
        self.is_dirt = False
        self.is_sand = False
        self.is_plant = False
        self.is_grass = False
        self.is_bridge = False
        self.is_movement_blocking = False
        self.is_dungeon_way_up = False
        self.is_dungeon_way_down = False
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
        self.is_hwall = False
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

    def set_has_shadow(self, value):
        self.has_shadow = value
        mm.tp_set_has_shadow(self, value)

    def set_is_chair(self, value):
        self.is_chair = value
        mm.tp_set_is_chair(self, value)

    def set_is_chest(self, value):
        self.is_chest = value
        mm.tp_set_is_chest(self, value)

    def set_is_bookcase(self, value):
        self.is_bookcase = value
        mm.tp_set_is_bookcase(self, value)

    def set_is_table(self, value):
        self.is_table = value
        mm.tp_set_is_table(self, value)

    def set_is_lawn(self, value):
        self.is_lawn = value
        mm.tp_set_is_lawn(self, value)

    def set_is_carpet_deco(self, value):
        self.is_carpet_deco = value
        mm.tp_set_is_carpet_deco(self, value)

    def set_is_carpet(self, value):
        self.is_carpet = value
        mm.tp_set_is_carpet(self, value)

    def set_is_house_item(self, value):
        self.is_house_item = value
        mm.tp_set_is_house_item(self, value)

    def set_is_cwall(self, value):
        self.is_cwall = value
        mm.tp_set_is_cwall(self, value)

    def set_is_hwall(self, value):
        self.is_hwall = value
        mm.tp_set_is_hwall(self, value)

    def set_is_animated_lr_flip(self, value):
        self.is_animated_lr_flip = value
        mm.tp_set_is_animated_lr_flip(self, value)

    def set_is_gravel_snow_deco(self, value):
        self.is_gravel_snow_deco = value
        mm.tp_set_is_gravel_snow_deco(self, value)

    def set_is_landrock_snow(self, value):
        self.is_landrock_snow = value
        mm.tp_set_is_landrock_snow(self, value)

    def set_is_hidden_from_editor(self, value):
        self.is_hidden_from_editor = value
        mm.tp_set_is_hidden_from_editor(self, value)

    def set_is_dungeon_item(self, value):
        self.is_dungeon_item = value
        mm.tp_set_is_dungeon_item(self, value)

    def set_is_difficulty_easy(self, value):
        self.is_difficulty_easy = value
        mm.tp_set_is_difficulty_easy(self, value)

    def set_is_difficulty_hard(self, value):
        self.is_difficulty_hard = value
        mm.tp_set_is_difficulty_hard(self, value)

    def set_is_difficulty_nightmare(self, value):
        self.is_difficulty_nightmare = value
        mm.tp_set_is_difficulty_nightmare(self, value)

    def set_is_world_item(self, value):
        self.is_world_item = value
        mm.tp_set_is_world_item(self, value)

    def set_is_solid_ground(self, value):
        self.is_solid_ground = value
        mm.tp_set_is_solid_ground(self, value)

    def set_is_dirt_deco(self, value):
        self.is_dirt_deco = value
        mm.tp_set_is_dirt_deco(self, value)

    def set_is_dirt_snow_deco(self, value):
        self.is_dirt_snow_deco = value
        mm.tp_set_is_dirt_snow_deco(self, value)

    def set_is_grass_deco(self, value):
        self.is_grass_deco = value
        mm.tp_set_is_grass_deco(self, value)

    def set_is_grass_snow_deco(self, value):
        self.is_grass_snow_deco = value
        mm.tp_set_is_grass_snow_deco(self, value)

    def set_is_gravel_deco(self, value):
        self.is_gravel_deco = value
        mm.tp_set_is_gravel_deco(self, value)

    def set_is_sand_deco(self, value):
        self.is_sand_deco = value
        mm.tp_set_is_sand_deco(self, value)

    def set_is_sand_snow_deco(self, value):
        self.is_sand_snow_deco = value
        mm.tp_set_is_sand_snow_deco(self, value)

    def set_is_snow_deco(self, value):
        self.is_snow_deco = value
        mm.tp_set_is_snow_deco(self, value)

    def set_is_sand_snow(self, value):
        self.is_sand_snow = value
        mm.tp_set_is_sand_snow(self, value)

    def set_is_dirt_snow(self, value):
        self.is_dirt_snow = value
        mm.tp_set_is_dirt_snow(self, value)

    def set_is_grass_snow(self, value):
        self.is_grass_snow = value
        mm.tp_set_is_grass_snow(self, value)

    def set_is_road_snow(self, value):
        self.is_road_snow = value
        mm.tp_set_is_road_snow(self, value)

    def set_is_road(self, value):
        self.is_road = value
        mm.tp_set_is_road(self, value)

    def set_is_dungeon_snow(self, value):
        self.is_dungeon_snow = value
        mm.tp_set_is_dungeon_snow(self, value)

    def set_is_dungeon(self, value):
        self.is_dungeon = value
        mm.tp_set_is_dungeon(self, value)

    def set_is_focus(self, value):
        self.is_focus = value
        mm.tp_set_is_focus(self, value)

    def set_is_gravel_snow(self, value):
        self.is_gravel_snow = value
        mm.tp_set_is_gravel_snow(self, value)

    def set_is_snow_mound(self, value):
        self.is_snow_mound = value
        mm.tp_set_is_snow_mound(self, value)

    def set_is_ice(self, value):
        self.is_ice = value
        mm.tp_set_is_ice(self, value)

    def set_is_gravel(self, value):
        self.is_gravel = value
        mm.tp_set_is_gravel(self, value)

    def set_is_small_rock(self, value):
        self.is_small_rock = value
        mm.tp_set_is_small_rock(self, value)

    def set_is_marsh_plant(self, value):
        self.is_marsh_plant = value
        mm.tp_set_is_marsh_plant(self, value)

    def set_is_landrock(self, value):
        self.is_landrock = value
        mm.tp_set_is_landrock(self, value)

    def set_is_snow(self, value):
        self.is_snow = value
        mm.tp_set_is_snow(self, value)

    def set_is_hidden(self, value):
        self.is_hidden = value
        mm.tp_set_is_hidden(self, value)

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

    def set_is_tree_snow(self, value):
        self.is_tree_snow = value
        mm.tp_set_is_tree_snow(self, value)

    def set_is_tree_conifer(self, value):
        self.is_tree_conifer = value
        mm.tp_set_is_tree_conifer(self, value)

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

    def set_is_dungeon_way_up(self, value):
        self.is_dungeon_way_up = value
        mm.tp_set_is_dungeon_way_up(self, value)

    def set_is_dungeon_way_down(self, value):
        self.is_dungeon_way_down = value
        mm.tp_set_is_dungeon_way_down(self, value)

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
        mm.tp_set_is_rock(self, value)

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


def get_random_small_rock():
    while True:
        tp = all_tps[random.choice(all_small_rock_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_snow_mound():
    while True:
        tp = all_tps[random.choice(all_snow_mound_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_marsh_plant():
    while True:
        tp = all_tps[random.choice(all_marsh_plant_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_tree():
    while True:
        tp = all_tps[random.choice(all_tree_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_tree_snow():
    while True:
        tp = all_tps[random.choice(all_tree_snow_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_tree_conifer():
    while True:
        tp = all_tps[random.choice(all_tree_conifer_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_wall():
    while True:
        tp = all_tps[random.choice(all_wall_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_cwall():
    while True:
        tp = all_tps[random.choice(all_cwall_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_hwall():
    while True:
        tp = all_tps[random.choice(all_hwall_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_door():
    while True:
        tp = all_tps[random.choice(all_door_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_floor():
    while True:
        tp = all_tps[random.choice(all_floor_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_corridor():
    while True:
        tp = all_tps[random.choice(all_corridor_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_dungeon():
    while True:
        tp = all_tps[random.choice(all_dungeon_tps)]

        roll = random.randint(1, 1000)
        if roll >= tp.d1000_appearing_roll:
            return tp


def get_random_dungeon_snow():
    while True:
        tp = all_tps[random.choice(all_dungeon_snow_tps)]

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


def same_type(tp1, tp2):
    if tp1.is_monst and tp2.is_monst:
        return True
    if tp1.is_player and tp2.is_player:
        return True
    if tp1.is_food and tp2.is_food:
        return True
    if tp1.is_weapon and tp2.is_weapon:
        return True
    if tp1.is_sand_snow and tp2.is_sand_snow:
        return True
    if tp1.is_dirt_snow and tp2.is_dirt_snow:
        return True
    if tp1.is_grass_snow and tp2.is_grass_snow:
        return True
    if tp1.is_road_snow and tp2.is_road_snow:
        return True
    if tp1.is_road and tp2.is_road:
        return True
    if tp1.is_dungeon_snow and tp2.is_dungeon_snow:
        return True
    if tp1.is_dungeon and tp2.is_dungeon:
        return True
    if tp1.is_gravel_snow and tp2.is_gravel_snow:
        return True
    if tp1.is_tree_conifer and tp2.is_tree_conifer:
        return True
    if tp1.is_snow_mound and tp2.is_snow_mound:
        return True
    if tp1.is_rock and tp2.is_rock:
        return True
    if tp1.is_ice and tp2.is_ice:
        return True
    if tp1.is_gravel and tp2.is_gravel:
        return True
    if tp1.is_small_rock and tp2.is_small_rock:
        return True
    if tp1.is_marsh_plant and tp2.is_marsh_plant:
        return True
    if tp1.is_landrock and tp2.is_landrock:
        return True
    if tp1.is_snow and tp2.is_snow:
        return True
    if tp1.is_dirt and tp2.is_dirt:
        return True
    if tp1.is_sand and tp2.is_sand:
        return True
    if tp1.is_plant and tp2.is_plant:
        return True
    if tp1.is_grass and tp2.is_grass:
        return True
    if tp1.is_bridge and tp2.is_bridge:
        return True
    if tp1.is_dungeon_way_up and tp2.is_dungeon_way_up:
        return True
    if tp1.is_dungeon_way_down and tp2.is_dungeon_way_down:
        return True
    if tp1.is_chasm_smoke and tp2.is_chasm_smoke:
        return True
    if tp1.is_key and tp2.is_key:
        return True
    if tp1.is_dusty and tp2.is_dusty:
        return True
    if tp1.is_corridor and tp2.is_corridor:
        return True
    if tp1.is_healing and tp2.is_healing:
        return True
    if tp1.is_armor and tp2.is_armor:
        return True
    if tp1.is_magical and tp2.is_magical:
        return True
    if tp1.is_wall and tp2.is_wall:
        return True
    if tp1.is_cwall and tp2.is_cwall:
        return True
    if tp1.is_hwall and tp2.is_hwall:
        return True
    if tp1.is_floor and tp2.is_floor:
        return True
    if tp1.is_lava and tp2.is_lava:
        return True
    if tp1.is_rock and tp2.is_rock:
        return True
    if tp1.is_water and tp2.is_water:
        return True
    if tp1.is_carpet and tp2.is_carpet:
        return True
    if tp1.is_lawn and tp2.is_lawn:
        return True
    return False
