from tp import tpp
import mm

x = tpp("swamp1");
x.set_short_name("some swamp");
x.set_tooltip("some swamp");
x.set_is_land(True);
x.set_world_depth("land");

mm.tp_set_tile(x, tile="swamp1.1");
mm.tp_set_tile(x, tile="swamp1.2");
mm.tp_set_tile(x, tile="swamp1.3");
mm.tp_set_tile(x, tile="swamp1.4");

mm.tp_set_tile(x, tile="swamp2.1");
mm.tp_set_tile(x, tile="swamp2.2");
mm.tp_set_tile(x, tile="swamp2.3");
mm.tp_set_tile(x, tile="swamp2.4");

mm.tp_set_tile(x, tile="swamp3.1");
mm.tp_set_tile(x, tile="swamp3.2");
mm.tp_set_tile(x, tile="swamp3.3");
mm.tp_set_tile(x, tile="swamp3.4");

mm.tp_set_tile(x, tile="swamp4.1");
mm.tp_set_tile(x, tile="swamp4.2");
mm.tp_set_tile(x, tile="swamp4.3");
mm.tp_set_tile(x, tile="swamp4.4");

mm.tp_set_tile(x, tile="swamp5.1");
mm.tp_set_tile(x, tile="swamp5.2");
mm.tp_set_tile(x, tile="swamp5.3");
mm.tp_set_tile(x, tile="swamp5.4");

