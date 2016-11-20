import tp


def thing_init(t):
    return


total_tree = 0


def tree1_init(d1000_roll, tiles=[]):

    global total_tree

    short_name = name = "tree" + str(total_tree + 1)
    total_tree = total_tree + 1

    x = tp.Tp(name=name,
              is_tree=True,
              d1000_appearing_roll=d1000_roll)

    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_tree(True)
    x.set_z_depth("wall")

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rare, low means common
    x = tree1_init(d1000_roll=1, tiles=["tree1"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=950, tiles=["tree2"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=950, tiles=["tree3"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1, tiles=["tree4"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1, tiles=["tree5"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1, tiles=["tree6"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    x = tree1_init(d1000_roll=1, tiles=["tree7"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

init()
