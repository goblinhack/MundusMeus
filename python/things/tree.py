import tp


def thing_init(t):
    return


total_tree = 0


def tree_init(tiles=[]):

    global total_tree

    short_name = name = "tree" + str(total_tree + 1)
    total_tree = total_tree + 1

    x = tp.Tp(name=name, is_tree=True)

    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_tree(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    x = tree_init(tiles="tree1")
    x.set_blit_top_off(1)
    x = tree_init(tiles="tree2")
    x.set_blit_top_off(1)
    x = tree_init(tiles="tree3")
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

init()
