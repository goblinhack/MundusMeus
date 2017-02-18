import tp


def thing_init(t):
    return


def door1_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name, is_door=True)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_z_depth("wall")
    x.set_is_dungeon_item(True)
    x.set_is_movement_blocking(True)
    x.set_is_shadow_caster(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    x = door1_init(name="door1",
                   short_name="portcullis",
                   long_name="portcullis",
                   tiles=["door1.1"])
    x.set_blit_top_off(1)

    x = door1_init(name="door2",
                   short_name="heavy secured door",
                   long_name="heavy secured door",
                   tiles=["door1.2"])
    x.set_blit_top_off(1)

    x = door1_init(name="door3",
                   short_name="oak door",
                   long_name="oak door",
                   tiles=["door1.3"])
    x.set_blit_top_off(1)

init()
