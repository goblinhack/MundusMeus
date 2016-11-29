import tp


def thing_init(t):
    return


total_dungeon = 0


def dungeon1_init(d1000_roll, tiles=[]):

    global total_dungeon

    short_name = name = "dungeon" + str(total_dungeon + 1)
    total_dungeon = total_dungeon + 1

    x = tp.Tp(name=name,
              is_dungeon=True,
              d1000_appearing_roll=d1000_roll)

    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_dungeon(True)
    x.set_z_depth("wall")
    x.set_is_shadow_caster_soft(True)
    x.set_is_shadow_caster(True)

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rare, low means common
    x = dungeon1_init(d1000_roll=1, tiles=["dungeon1"])
    x.set_blit_top_off(2)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    # high numbers means rare, low means common
    x = dungeon1_init(d1000_roll=1, tiles=["dungeon2"])
    x.set_blit_top_off(2)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    # high numbers means rare, low means common
    x = dungeon1_init(d1000_roll=1, tiles=["dungeon3"])
    x.set_blit_top_off(2)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    # high numbers means rare, low means common
    x = dungeon1_init(d1000_roll=1, tiles=["dungeon4"])
    x.set_blit_top_off(2)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    # high numbers means rare, low means common
    x = dungeon1_init(d1000_roll=1, tiles=["dungeon5"])
    x.set_blit_top_off(2)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

init()
