import tp
import mm


def thing_init(t):
    return


def thing_pushed(t):
    t.wid.bounce_to_pct_in(height=0.1,
                           fade=1.0, ms=600, count=1000)


def key1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)
    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_dungeon_item(True)
    x.set_is_key(True)
    x.set_has_shadow(True)

    x.set_light_radius(1)
    x.set_light_tint("gray20")
    x.set_is_candle_light(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init
    x.thing_pushed = thing_pushed

    return x


def init():
    key1_init(name="key1",
              short_name="Key",
              long_name="A metal key",
              tiles=[
                     "key1.1",
                    ])

init()
