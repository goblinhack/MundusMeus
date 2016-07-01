from tp import tpp
import mm

x = tpp("castle1");
x.set_short_name("castle");
x.set_is_land(True);
x.set_is_castle(True);
x.set_world_depth("land");

mm.tp_set_tile(x, tile="castle1.1");
mm.tp_set_tile(x, tile="castle1.2");
mm.tp_set_tile(x, tile="castle1.3");
mm.tp_set_tile(x, tile="castle1.4");
mm.tp_set_tile(x, tile="castle1.5");
mm.tp_set_tile(x, tile="castle1.6");
mm.tp_set_tile(x, tile="castle1.7");

mm.tp_set_tile(x, tile="castle2.1");
mm.tp_set_tile(x, tile="castle2.2");
mm.tp_set_tile(x, tile="castle2.3");
mm.tp_set_tile(x, tile="castle2.4");
mm.tp_set_tile(x, tile="castle2.5");
mm.tp_set_tile(x, tile="castle2.6");
mm.tp_set_tile(x, tile="castle2.7");
