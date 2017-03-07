import tp
import mm


def thing_init(t):
    return


total_tree = 0


def tree1_init(d1000_roll, short_name, long_name, tiles=[]):

    global total_tree

    name = "tree" + str(total_tree + 1)
    total_tree = total_tree + 1

    x = tp.Tp(name=name,
              is_tree=True,
              d1000_appearing_roll=d1000_roll)

    x.set_short_name(short_name)
    x.set_long_name(long_name)
    x.set_is_tree(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_shadow_caster_soft(True)
    x.set_is_shadow_caster(True)

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rare, low means common
    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree1"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree2"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree3"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree4"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree5"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree6"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree7"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree8"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree9"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree10"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree11"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1,
                   short_name="A tree",
                   long_name="",
                   tiles=["tree12"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

init()
