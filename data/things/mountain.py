from tp import tpp
import mm

x = tpp("mountain1");
x.set_short_name("mountain");
x.set_is_land(True);
x.set_is_mountain(True);
x.set_world_depth("land");

mm.tp_set_tile(x, tile="mountain1.1");
mm.tp_set_tile(x, tile="mountain1.2");
mm.tp_set_tile(x, tile="mountain1.3");
mm.tp_set_tile(x, tile="mountain1.4");
mm.tp_set_tile(x, tile="mountain1.5");

mm.tp_set_tile(x, tile="mountain2.1");
mm.tp_set_tile(x, tile="mountain2.2");
mm.tp_set_tile(x, tile="mountain2.3");
mm.tp_set_tile(x, tile="mountain2.4");
mm.tp_set_tile(x, tile="mountain2.5");

mm.tp_set_tile(x, tile="mountain3.1");
mm.tp_set_tile(x, tile="mountain3.2");
mm.tp_set_tile(x, tile="mountain3.3");
mm.tp_set_tile(x, tile="mountain3.4");
mm.tp_set_tile(x, tile="mountain3.5");
