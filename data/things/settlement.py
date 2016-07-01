from tp import tpp
import mm

x = tpp("settlement1");
x.set_short_name("settlement");
x.set_is_land(True);
x.set_is_settlement(True);
x.set_world_depth("land");

mm.tp_set_tile(x, tile="settlement1.1");
mm.tp_set_tile(x, tile="settlement1.2");
mm.tp_set_tile(x, tile="settlement1.3");
mm.tp_set_tile(x, tile="settlement1.4");
mm.tp_set_tile(x, tile="settlement1.5");
mm.tp_set_tile(x, tile="settlement1.6");
mm.tp_set_tile(x, tile="settlement1.7");
mm.tp_set_tile(x, tile="settlement1.8");
mm.tp_set_tile(x, tile="settlement1.9");

mm.tp_set_tile(x, tile="settlement2.1");
mm.tp_set_tile(x, tile="settlement2.2");
mm.tp_set_tile(x, tile="settlement2.3");
mm.tp_set_tile(x, tile="settlement2.4");
mm.tp_set_tile(x, tile="settlement2.5");
mm.tp_set_tile(x, tile="settlement2.6");
mm.tp_set_tile(x, tile="settlement2.7");
mm.tp_set_tile(x, tile="settlement2.8");
mm.tp_set_tile(x, tile="settlement2.9");
