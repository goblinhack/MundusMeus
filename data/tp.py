import mm

class tpp:
    """thing template"""
    def __init__(self, name):
        self.name = name;
        mm.tp_load(self);
        self.set_raw_name(name);

    def set_short_name(self, value):
        self.short_name = value;
        mm.tp_set_short_name(self, value);

    def set_raw_name (self, value):
        self.raw_name = value;
        mm.tp_set_raw_name(self, value);

    def set_light_radius (self, value):
        self.light_radius = value;
        mm.tp_set_light_radius(self, value);

    def set_weapon_density (self, value):
        self.weapon_density = value;
        mm.tp_set_weapon_density(self, value);

    def set_weapon_spread (self, value):
        self.weapon_spread = value;
        mm.tp_set_weapon_spread(self, value);

    def set_scale (self, value):
        self.scale = value;
        mm.tp_set_scale(self, value);

    def set_explosion_radius (self, value):
        self.explosion_radius = value;
        mm.tp_set_explosion_radius(self, value);

    def set_collision_radius (self, value):
        self.collision_radius = value;
        mm.tp_set_collision_radius(self, value);

    def set_d10000_chance_of_appearing (self, value):
        self.d10000_chance_of_appearing = value;
        mm.tp_set_d10000_chance_of_appearing(self, value);

    def set_z_depth (self, value):
        self.z_depth = value;
        mm.tp_set_z_depth(self, value);

    def set_z_order (self, value):
        self.z_order = value;
        mm.tp_set_z_order(self, value);

    def set_world_depth (self, value):
        self.world_depth = value;
        mm.tp_set_world_depth(self, value);

    def set_speed (self, value):
        self.speed = value;
        mm.tp_set_speed(self, value);

    def set_damage (self, value):
        self.damage = value;
        mm.tp_set_damage(self, value);

    def set_cost (self, value):
        self.cost = value;
        mm.tp_set_cost(self, value);

    def set_lifespan_ticks (self, value):
        self.lifespan_ticks = value;
        mm.tp_set_lifespan_ticks(self, value);

    def set_vision_distance (self, value):
        self.vision_distance = value;
        mm.tp_set_vision_distance(self, value);

    def set_approach_distance (self, value):
        self.approach_distance = value;
        mm.tp_set_approach_distance(self, value);

    def set_bonus_score_on_death (self, value):
        self.bonus_score_on_death = value;
        mm.tp_set_bonus_score_on_death(self, value);

    def set_bonus_gold_on_collect (self, value):
        self.bonus_gold_on_collect = value;
        mm.tp_set_bonus_gold_on_collect(self, value);

    def set_bonus_hp_on_collect (self, value):
        self.bonus_hp_on_collect = value;
        mm.tp_set_bonus_hp_on_collect(self, value);

    def set_blit_top_off (self, value):
        self.blit_top_off = value;
        mm.tp_set_blit_top_off(self, value);

    def set_blit_bot_off (self, value):
        self.blit_bot_off = value;
        mm.tp_set_blit_bot_off(self, value);

    def set_blit_left_off (self, value):
        self.blit_left_off = value;
        mm.tp_set_blit_left_off(self, value);

    def set_blit_right_off (self, value):
        self.blit_right_off = value;
        mm.tp_set_blit_right_off(self, value);

    def set_min_appear_depth (self, value):
        self.min_appear_depth = value;
        mm.tp_set_min_appear_depth(self, value);

    def set_max_appear_depth (self, value):
        self.max_appear_depth = value;
        mm.tp_set_max_appear_depth(self, value);

    def set_jump_speed (self, value):
        self.jump_speed = value;
        mm.tp_set_jump_speed(self, value);

    def set_hp_per_level (self, value):
        self.hp_per_level = value;
        mm.tp_set_hp_per_level(self, value);

    def set_max_hp (self, value):
        self.max_hp = value;
        mm.tp_set_max_hp(self, value);

    def set_hit_priority (self, value):
        self.hit_priority = value;
        mm.tp_set_hit_priority(self, value);

    def set_weapon_fire_delay_hundredths (self, value):
        self.weapon_fire_delay_hundredths = value;
        mm.tp_set_weapon_fire_delay_hundredths(self, value);

    def set_sound_random_delay_secs (self, value):
        self.sound_random_delay_secs = value;
        mm.tp_set_sound_random_delay_secs(self, value);

    def set_swing_distance_from_player (self, value):
        self.swing_distance_from_player = value;
        mm.tp_set_swing_distance_from_player(self, value);

    def set_can_be_hit_chance (self, value):
        self.can_be_hit_chance = value;
        mm.tp_set_can_be_hit_chance(self, value);

    def set_hit_delay_tenths (self, value):
        self.hit_delay_tenths = value;
        mm.tp_set_hit_delay_tenths(self, value);

    def set_mob_spawn_delay_tenths (self, value):
        self.mob_spawn_delay_tenths = value;
        mm.tp_set_mob_spawn_delay_tenths(self, value);

    def set_tooltip (self, value):
        self.tooltip = value;
        mm.tp_set_tooltip(self, value);

    def set_polymorph_on_death (self, value):
        self.polymorph_on_death = value;
        mm.tp_set_polymorph_on_death(self, value);

    def set_carried_as (self, value):
        self.carried_as = value;
        mm.tp_set_carried_as(self, value);

    def set_light_tint (self, value):
        self.light_tint = value;
        mm.tp_set_light_tint(self, value);

    def set_explodes_as (self, value):
        self.explodes_as = value;
        mm.tp_set_explodes_as(self, value);

    def set_sound_on_creation (self, value):
        self.sound_on_creation = value;
        mm.tp_set_sound_on_creation(self, value);

    def set_sound_on_hitting_something (self, value):
        self.sound_on_hitting_something = value;
        mm.tp_set_sound_on_hitting_something(self, value);

    def set_sound_on_death (self, value):
        self.sound_on_death = value;
        mm.tp_set_sound_on_death(self, value);

    def set_sound_on_hit (self, value):
        self.sound_on_hit = value;
        mm.tp_set_sound_on_hit(self, value);

    def set_sound_on_collect (self, value):
        self.sound_on_collect = value;
        mm.tp_set_sound_on_collect(self, value);

    def set_sound_random (self, value):
        self.sound_random = value;
        mm.tp_set_sound_random(self, value);

    def set_weapon_carry_anim (self, value):
        self.weapon_carry_anim = value;
        mm.tp_set_weapon_carry_anim(self, value);

    def set_weapon_swing_anim (self, value):
        self.weapon_swing_anim = value;
        mm.tp_set_weapon_swing_anim(self, value);

    def set_message_on_use (self, value):
        self.message_on_use = value;
        mm.tp_set_message_on_use(self, value);

    def set_is_acid (self, value):
        self.is_acid = value;
        mm.tp_set_is_acid(self, value);

    def set_is_acid_proof (self, value):
        self.is_acid_proof = value;
        mm.tp_set_is_acid_proof(self, value);

    def set_is_animated (self, value):
        self.is_animated = value;
        mm.tp_set_is_animated(self, value);

    def set_is_animated_no_dir (self, value):
        self.is_animated_no_dir = value;
        mm.tp_set_is_animated_no_dir(self, value);

    def set_is_animation (self, value):
        self.is_animation = value;
        mm.tp_set_is_animation(self, value);

    def set_is_bullet (self, value):
        self.is_bullet = value;
        mm.tp_set_is_bullet(self, value);

    def set_is_candle_light (self, value):
        self.is_candle_light = value;
        mm.tp_set_is_candle_light(self, value);

    def set_is_carryable (self, value):
        self.is_carryable = value;
        mm.tp_set_is_carryable(self, value);

    def set_is_cats_eyes (self, value):
        self.is_cats_eyes = value;
        mm.tp_set_is_cats_eyes(self, value);

    def set_is_cloud_effect (self, value):
        self.is_cloud_effect = value;
        mm.tp_set_is_cloud_effect(self, value);

    def set_is_collision_map_large (self, value):
        self.is_collision_map_large = value;
        mm.tp_set_is_collision_map_large(self, value);

    def set_is_collision_map_medium (self, value):
        self.is_collision_map_medium = value;
        mm.tp_set_is_collision_map_medium(self, value);

    def set_is_collision_map_small (self, value):
        self.is_collision_map_small = value;
        mm.tp_set_is_collision_map_small(self, value);

    def set_is_collision_map_tiny (self, value):
        self.is_collision_map_tiny = value;
        mm.tp_set_is_collision_map_tiny(self, value);

    def set_is_combustable (self, value):
        self.is_combustable = value;
        mm.tp_set_is_combustable(self, value);

    def set_is_conical_breath_attack (self, value):
        self.is_conical_breath_attack = value;
        mm.tp_set_is_conical_breath_attack(self, value);

    def set_is_corpse (self, value):
        self.is_corpse = value;
        mm.tp_set_is_corpse(self, value);

    def set_is_corridor (self, value):
        self.is_corridor = value;
        mm.tp_set_is_corridor(self, value);

    def set_is_corridor_wall (self, value):
        self.is_corridor_wall = value;
        mm.tp_set_is_corridor_wall(self, value);

    def set_is_door (self, value):
        self.is_door = value;
        mm.tp_set_is_door(self, value);

    def set_is_dungeon_floor (self, value):
        self.is_dungeon_floor = value;
        mm.tp_set_is_dungeon_floor(self, value);

    def set_is_effect_fade_in_out (self, value):
        self.is_effect_fade_in_out = value;
        mm.tp_set_is_effect_fade_in_out(self, value);

    def set_is_effect_pulse (self, value):
        self.is_effect_pulse = value;
        mm.tp_set_is_effect_pulse(self, value);

    def set_is_effect_rotate_2way (self, value):
        self.is_effect_rotate_2way = value;
        mm.tp_set_is_effect_rotate_2way(self, value);

    def set_is_effect_sway (self, value):
        self.is_effect_sway = value;
        mm.tp_set_is_effect_sway(self, value);

    def set_is_entrance (self, value):
        self.is_entrance = value;
        mm.tp_set_is_entrance(self, value);

    def set_is_ethereal (self, value):
        self.is_ethereal = value;
        mm.tp_set_is_ethereal(self, value);

    def set_is_exit (self, value):
        self.is_exit = value;
        mm.tp_set_is_exit(self, value);

    def set_is_explosion (self, value):
        self.is_explosion = value;
        mm.tp_set_is_explosion(self, value);

    def set_is_fire (self, value):
        self.is_fire = value;
        mm.tp_set_is_fire(self, value);

    def set_is_fireball (self, value):
        self.is_fireball = value;
        mm.tp_set_is_fireball(self, value);

    def set_is_food (self, value):
        self.is_food = value;
        mm.tp_set_is_food(self, value);

    def set_is_fragile (self, value):
        self.is_fragile = value;
        mm.tp_set_is_fragile(self, value);

    def set_is_given_randomly_at_start (self, value):
        self.is_given_randomly_at_start = value;
        mm.tp_set_is_given_randomly_at_start(self, value);

    def set_is_hard (self, value):
        self.is_hard = value;
        mm.tp_set_is_hard(self, value);

    def set_is_inactive (self, value):
        self.is_inactive = value;
        mm.tp_set_is_inactive(self, value);

    def set_is_internal (self, value):
        self.is_internal = value;
        mm.tp_set_is_internal(self, value);

    def set_is_joinable (self, value):
        self.is_joinable = value;
        mm.tp_set_is_joinable(self, value);

    def set_is_key (self, value):
        self.is_key = value;
        mm.tp_set_is_key(self, value);

    def set_is_lava (self, value):
        self.is_lava = value;
        mm.tp_set_is_lava(self, value);

    def set_is_lava_proof (self, value):
        self.is_lava_proof = value;
        mm.tp_set_is_lava_proof(self, value);

    def set_is_levitating (self, value):
        self.is_levitating = value;
        mm.tp_set_is_levitating(self, value);

    def set_is_life_saving (self, value):
        self.is_life_saving = value;
        mm.tp_set_is_life_saving(self, value);

    def set_is_light_source (self, value):
        self.is_light_source = value;
        mm.tp_set_is_light_source(self, value);

    def set_is_magical_weapon (self, value):
        self.is_magical_weapon = value;
        mm.tp_set_is_magical_weapon(self, value);

    def set_is_melee_weapon (self, value):
        self.is_melee_weapon = value;
        mm.tp_set_is_melee_weapon(self, value);

    def set_is_mob_spawner (self, value):
        self.is_mob_spawner = value;
        mm.tp_set_is_mob_spawner(self, value);

    def set_is_monst (self, value):
        self.is_monst = value;
        mm.tp_set_is_monst(self, value);

    def set_is_non_explosive_gas_cloud (self, value):
        self.is_non_explosive_gas_cloud = value;
        mm.tp_set_is_non_explosive_gas_cloud(self, value);

    def set_is_not_light_blocking (self, value):
        self.is_not_light_blocking = value;
        mm.tp_set_is_not_light_blocking(self, value);

    def set_is_obstacle (self, value):
        self.is_obstacle = value;
        mm.tp_set_is_obstacle(self, value);

    def set_is_player (self, value):
        self.is_player = value;
        mm.tp_set_is_player(self, value);

    def set_is_projectile (self, value):
        self.is_projectile = value;
        mm.tp_set_is_projectile(self, value);

    def set_is_ranged_weapon (self, value):
        self.is_ranged_weapon = value;
        mm.tp_set_is_ranged_weapon(self, value);

    def set_is_rock (self, value):
        self.is_rock = value;
        mm.tp_set_is_rock(self, value);

    def set_is_rrr1 (self, value):
        self.is_rrr1 = value;
        mm.tp_set_is_rrr1(self, value);

    def set_is_rrr2 (self, value):
        self.is_rrr2 = value;
        mm.tp_set_is_rrr2(self, value);

    def set_is_rrr3 (self, value):
        self.is_rrr3 = value;
        mm.tp_set_is_rrr3(self, value);

    def set_is_rrr4 (self, value):
        self.is_rrr4 = value;
        mm.tp_set_is_rrr4(self, value);

    def set_is_rrr5 (self, value):
        self.is_rrr5 = value;
        mm.tp_set_is_rrr5(self, value);

    def set_is_rrr6 (self, value):
        self.is_rrr6 = value;
        mm.tp_set_is_rrr6(self, value);

    def set_is_rrr7 (self, value):
        self.is_rrr7 = value;
        mm.tp_set_is_rrr7(self, value);

    def set_is_rrr8 (self, value):
        self.is_rrr8 = value;
        mm.tp_set_is_rrr8(self, value);

    def set_is_rrr9 (self, value):
        self.is_rrr9 = value;
        mm.tp_set_is_rrr9(self, value);

    def set_is_rrr10 (self, value):
        self.is_rrr10 = value;
        mm.tp_set_is_rrr10(self, value);

    def set_is_rrr11 (self, value):
        self.is_rrr11 = value;
        mm.tp_set_is_rrr11(self, value);

    def set_is_rrr12 (self, value):
        self.is_rrr12 = value;
        mm.tp_set_is_rrr12(self, value);

    def set_is_rrr13 (self, value):
        self.is_rrr13 = value;
        mm.tp_set_is_rrr13(self, value);

    def set_is_rrr14 (self, value):
        self.is_rrr14 = value;
        mm.tp_set_is_rrr14(self, value);

    def set_is_rrr15 (self, value):
        self.is_rrr15 = value;
        mm.tp_set_is_rrr15(self, value);

    def set_is_rrr16 (self, value):
        self.is_rrr16 = value;
        mm.tp_set_is_rrr16(self, value);

    def set_is_northern_mountain (self, value):
        self.is_northern_mountain = value;
        mm.tp_set_is_northern_mountain(self, value);

    def set_is_snow_castle (self, value):
        self.is_snow_castle = value;
        mm.tp_set_is_snow_castle(self, value);

    def set_is_castle (self, value):
        self.is_castle = value;
        mm.tp_set_is_castle(self, value);

    def set_is_mountain (self, value):
        self.is_mountain = value;
        mm.tp_set_is_mountain(self, value);

    def set_is_forest (self, value):
        self.is_forest = value;
        mm.tp_set_is_forest(self, value);

    def set_is_sand (self, value):
        self.is_sand = value;
        mm.tp_set_is_sand(self, value);

    def set_is_grass (self, value):
        self.is_grass = value;
        mm.tp_set_is_grass(self, value);

    def set_is_land (self, value):
        self.is_land = value;
        mm.tp_set_is_land(self, value);

    def set_is_wanderer_lr (self, value):
        self.is_wanderer_lr = value;
        mm.tp_set_is_wanderer_lr(self, value);

    def set_is_snow_settlement (self, value):
        self.is_snow_settlement = value;
        mm.tp_set_is_snow_settlement(self, value);

    def set_is_northern_settlement (self, value):
        self.is_northern_settlement = value;
        mm.tp_set_is_northern_settlement(self, value);

    def set_is_snow (self, value):
        self.is_snow = value;
        mm.tp_set_is_snow(self, value);

    def set_is_settlement (self, value):
        self.is_settlement = value;
        mm.tp_set_is_settlement(self, value);

    def set_is_northern_rock (self, value):
        self.is_northern_rock = value;
        mm.tp_set_is_northern_rock(self, value);

    def set_is_sea (self, value):
        self.is_sea = value;
        mm.tp_set_is_sea(self, value);

    def set_is_rope (self, value):
        self.is_rope = value;
        mm.tp_set_is_rope(self, value);

    def set_is_throwable (self, value):
        self.is_throwable = value;
        mm.tp_set_is_throwable(self, value);

    def set_is_shadow_caster (self, value):
        self.is_shadow_caster = value;
        mm.tp_set_is_shadow_caster(self, value);

    def set_is_shadow_caster_soft (self, value):
        self.is_shadow_caster_soft = value;
        mm.tp_set_is_shadow_caster_soft(self, value);

    def set_is_shop_floor (self, value):
        self.is_shop_floor = value;
        mm.tp_set_is_shop_floor(self, value);

    def set_is_shopkeeper (self, value):
        self.is_shopkeeper = value;
        mm.tp_set_is_shopkeeper(self, value);

    def set_is_sleeping (self, value):
        self.is_sleeping = value;
        mm.tp_set_is_sleeping(self, value);

    def set_is_stackable (self, value):
        self.is_stackable = value;
        mm.tp_set_is_stackable(self, value);

    def set_is_torch (self, value):
        self.is_torch = value;
        mm.tp_set_is_torch(self, value);

    def set_is_trap (self, value):
        self.is_trap = value;
        mm.tp_set_is_trap(self, value);

    def set_is_treasure (self, value):
        self.is_treasure = value;
        mm.tp_set_is_treasure(self, value);

    def set_is_undead (self, value):
        self.is_undead = value;
        mm.tp_set_is_undead(self, value);

    def set_is_variable_size (self, value):
        self.is_variable_size = value;
        mm.tp_set_is_variable_size(self, value);

    def set_is_wall (self, value):
        self.is_wall = value;
        mm.tp_set_is_wall(self, value);

    def set_is_wanderer (self, value):
        self.is_wanderer = value;
        mm.tp_set_is_wanderer(self, value);

    def set_is_water (self, value):
        self.is_water = value;
        mm.tp_set_is_water(self, value);

    def set_is_water_proof (self, value):
        self.is_water_proof = value;
        mm.tp_set_is_water_proof(self, value);

    def set_is_weapon (self, value):
        self.is_weapon = value;
        mm.tp_set_is_weapon(self, value);

    def set_is_weapon_carry_anim (self, value):
        self.is_weapon_carry_anim = value;
        mm.tp_set_is_weapon_carry_anim(self, value);

