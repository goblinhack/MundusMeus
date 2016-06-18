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
