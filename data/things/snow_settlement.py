from tp import tpp
import mm

x = tpp("snow_settlement1");
x.set_short_name("snow_settlement");
x.set_tooltip("snow settlement");
x.set_is_land(True);
x.set_is_snow_settlement(True);
x.set_world_depth("land");

mm.tp_set_tile(x, tile="snow_settlement1.1");
mm.tp_set_tile(x, tile="snow_settlement1.2");
mm.tp_set_tile(x, tile="snow_settlement1.3");
mm.tp_set_tile(x, tile="snow_settlement1.4");
mm.tp_set_tile(x, tile="snow_settlement1.5");

mm.tp_set_tile(x, tile="snow_settlement2.1");
mm.tp_set_tile(x, tile="snow_settlement2.2");
mm.tp_set_tile(x, tile="snow_settlement2.3");
mm.tp_set_tile(x, tile="snow_settlement2.4");
mm.tp_set_tile(x, tile="snow_settlement2.5");
