from tp import tpp
import mm

x = tpp("rock1");
x.set_short_name("rock");
x.set_is_land(True);
x.set_is_rock(True);
x.set_world_depth("land");

mm.tp_set_tile(x, tile="rock1.1");
mm.tp_set_tile(x, tile="rock1.2");
mm.tp_set_tile(x, tile="rock1.3");
mm.tp_set_tile(x, tile="rock1.4");

mm.tp_set_tile(x, tile="rock2.1");
mm.tp_set_tile(x, tile="rock2.2");
mm.tp_set_tile(x, tile="rock2.3");
mm.tp_set_tile(x, tile="rock2.4");

mm.tp_set_tile(x, tile="rock3.1");
mm.tp_set_tile(x, tile="rock3.2");
mm.tp_set_tile(x, tile="rock3.3");
mm.tp_set_tile(x, tile="rock3.4");
