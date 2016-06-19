from tp import tpp
import mm

x = tpp("grass1");
x.set_short_name("some grass");
x.set_tooltip("some grass");
x.set_is_land(True);
x.set_world_depth("land");
x.set_light_tint("purple");
mm.tp_set_tile(x, tile="grass1.1", is_dead=10, delay_ms=100);

