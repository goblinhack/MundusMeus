import tp


def thing_init(t):
    return


def entrance_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name)

    x.set_is_shadow_caster_soft(True)
    x.set_is_shadow_caster(True)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_dungeon_way_up(True)
    x.set_z_depth("floor2")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init

    return x


def init():
    x = entrance_init(name="dungeon_way_up1",
                      short_name="stairs leading upwards",
                      long_name="stairs leading upwards",
                      tiles=[
                             "dungeon_way_up1.1",
                            ])

    x.set_blit_bot_off(0)
    x.set_blit_top_off(1)

init()