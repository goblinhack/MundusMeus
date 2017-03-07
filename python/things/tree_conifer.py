import tp
import mm


def thing_init(t):
    return


total_tree_conifer = 0


def tree_conifer1_init(d1000_roll, tiles=[]):

    global total_tree_conifer

    name = "tree_conifer" + str(total_tree_conifer + 1)
    total_tree_conifer = total_tree_conifer + 1

    x = tp.Tp(name=name,
              is_tree_conifer=True,
              d1000_appearing_roll=d1000_roll)

    x.set_short_name("Conifer")
    x.set_is_tree(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_shadow_caster(True)
    x.set_is_shadow_caster_soft(True)

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rare, low means common
    x = tree_conifer1_init(d1000_roll=1, tiles=["tree_conifer1"])
    x.set_blit_top_off(1)

    x = tree_conifer1_init(d1000_roll=1, tiles=["tree_conifer2"])
    x.set_blit_top_off(1)

    x = tree_conifer1_init(d1000_roll=1, tiles=["tree_conifer3"])
    x.set_blit_top_off(3)

    x = tree_conifer1_init(d1000_roll=1, tiles=["tree_conifer4"])
    x.set_blit_top_off(3)

init()
