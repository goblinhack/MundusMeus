import tp
import mm


def thing_init(t):
    return


total_dungeon_snow = 0


def dungeon_snow_init(d1000_roll,
                      short_name,
                      long_name,
                      tiles=[]):

    global total_dungeon_snow

    name = "dungeon_snow" + str(total_dungeon_snow + 1)
    total_dungeon_snow = total_dungeon_snow + 1

    x = tp.Tp(name=name,
              is_dungeon_snow=True,
              d1000_appearing_roll=d1000_roll)
    x.set_long_name(long_name)
    x.set_short_name(short_name)
    x.set_is_dungeon(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_WALL)

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rare, low means common
    x = dungeon_snow_init(d1000_roll=1,
                          short_name="Ice Dungeon II",
                          long_name="Dungeon, perma-death and frost level",
                          tiles=["dungeon_snow"])
    x.set_blit_top_off(3)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)
    x.set_is_difficulty_hard(True)

    x = dungeon_snow_init(d1000_roll=1,
                          short_name="Ice Dungeon III",
                          long_name="Dungeon, cold as a tomb. level",
                          tiles=["dungeon_snow2"])
    x.set_blit_top_off(3)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)
    x.set_is_difficulty_nightmare(True)

init()
