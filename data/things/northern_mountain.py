from tp import tpp
import mm

x = tpp("northern_mountain1");
x.set_short_name("northern_mountain");
x.set_is_land(True);
x.set_is_northern_mountain(True);
x.set_world_depth("land");

mm.tp_set_tile(x, tile="northern_mountain1.1");
mm.tp_set_tile(x, tile="northern_mountain1.2");
mm.tp_set_tile(x, tile="northern_mountain1.3");
mm.tp_set_tile(x, tile="northern_mountain1.4");
mm.tp_set_tile(x, tile="northern_mountain1.5");

mm.tp_set_tile(x, tile="northern_mountain2.1");
mm.tp_set_tile(x, tile="northern_mountain2.2");
mm.tp_set_tile(x, tile="northern_mountain2.3");
mm.tp_set_tile(x, tile="northern_mountain2.4");
mm.tp_set_tile(x, tile="northern_mountain2.5");
