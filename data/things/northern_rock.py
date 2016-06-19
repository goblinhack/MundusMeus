from tp import tpp
import mm

x = tpp("northern_rock1");
x.set_short_name("northern_rock");
x.set_tooltip("northern rock");
x.set_is_land(True);
x.set_is_northern_rock(True);
x.set_world_depth("land");

mm.tp_set_tile(x, tile="northern_rock1.1");
mm.tp_set_tile(x, tile="northern_rock1.2");
mm.tp_set_tile(x, tile="northern_rock1.3");
mm.tp_set_tile(x, tile="northern_rock1.4");

mm.tp_set_tile(x, tile="northern_rock2.1");
mm.tp_set_tile(x, tile="northern_rock2.2");
mm.tp_set_tile(x, tile="northern_rock2.3");
mm.tp_set_tile(x, tile="northern_rock2.4");
