import tp
import mm


def thing_init(t):
    return


total_dungeon = 0


def dungeon1_init(d1000_roll,
                  short_name,
                  long_name,
                  tiles=[]):

    global total_dungeon

    name = "dungeon" + str(total_dungeon + 1)
    total_dungeon = total_dungeon + 1

    x = tp.Tp(name=name,
              is_dungeon=True,
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
    x = dungeon1_init(d1000_roll=1,
                      short_name="Dungeon I",
                      long_name="Dungeon, novice level",
                      tiles=["dungeon1"])
    x.set_blit_top_off(3)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)
    x.set_is_difficulty_easy(True)

    # high numbers means rare, low means common
    x = dungeon1_init(d1000_roll=1,
                      short_name="Dungeon II",
                      long_name="Dungeon, moderate terror level",
                      tiles=["dungeon2"])
    x.set_blit_top_off(3)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)
    x.set_is_difficulty_hard(True)

    # high numbers means rare, low means common
    x = dungeon1_init(d1000_roll=1,
                      short_name="Dungeon III",
                      long_name="Dungeon, extreme nightmare level",
                      tiles=["dungeon3"])
    x.set_blit_top_off(3)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)
    x.set_is_difficulty_nightmare(True)

    # high numbers means rare, low means common
    x = dungeon1_init(d1000_roll=1,
                      short_name="Dungeon III",
                      long_name="Dungeon, extreme nightmare level",
                      tiles=["dungeon4"])
    x.set_blit_top_off(3)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)
    x.set_is_difficulty_nightmare(True)

init()
