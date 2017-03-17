import tp
import mm


def thing_init(t):
    return


def thing_pushed(t):
    t.wid.bounce_to_pct_in(height=0.1,
                           fade=1.0, ms=500, count=1000)


def exit_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)

    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_is_dungeon_way_down(True)
    x.set_z_depth(mm.Z_DEPTH_FLOOR2)
    x.set_is_dungeon_item(True)
    x.set_has_shadow(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init

    return x


def init():
    x = exit_init(name="dungeon_way_down1",
                  short_name="Stairs",
                  long_name="Winding stairs leading down to certain demise.",
                  tiles=[
                         "dungeon_dungeon_way_down1.1",
                        ])

    x.set_light_radius(3)
    x.set_light_tint("red")
    x.set_is_candle_light(True)

    x = exit_init(name="dungeon_way_down1_deco",
                  short_name="Stairs",
                  long_name="Stairs leading down. " +
                  "And a sign saying, " +
                  "staff wanted, apply within.",
                  tiles=[
                         "dungeon_dungeon_way_down1_deco.1",
                        ])
    x.thing_pushed = thing_pushed
    x.set_z_depth(mm.Z_DEPTH_EXPLOSION)

init()
