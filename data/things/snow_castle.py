from tp import tpp
import mm

x = tpp("snow_castle1");
x.set_short_name("snow_castle");
x.set_tooltip("snow castle");
x.set_is_land(True);
x.set_is_snow_castle(True);
x.set_world_depth("land");

mm.tp_set_tile(x, tile="snow_castle1.1");

mm.tp_set_tile(x, tile="snow_castle2.1");
