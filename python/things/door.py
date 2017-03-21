import tp
import mm


def thing_init(t):
    return


def door1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name, is_door=True)
    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_dungeon_item(True)
    x.set_is_movement_blocking(True)
    x.set_is_shadow_caster(True)
    x.set_has_shadow(True)

    x.set_light_radius(0.5)
    x.set_light_tint("yellow")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    x = door1_init(name="door1",
                   short_name="Portcullis",
                   long_name="Heavy metal portcullis. " +
                   "And a do not disturb sign.",
                   tiles=["door1.1"])
    x.set_blit_top_off(1)

    x = door1_init(name="door2",
                   short_name="Secured door",
                   long_name="Heavy metal chained and secured door.",
                   tiles=["door1.2"])
    x.set_blit_top_off(1)

    x = door1_init(name="door3",
                   short_name="Oak door",
                   long_name="Oak door. Who knows what lies beyond. " +
                   "Nothing probably.",
                   tiles=["door1.3"])
    x.set_blit_top_off(1)

init()
