import tp


def thing_init(t):
    return


total_tree_snow = 0


def tree_snow_init(d1000_roll, tiles=[]):

    global total_tree_snow

    short_name = name = "tree_snow" + str(total_tree_snow + 1)
    total_tree_snow = total_tree_snow + 1

    x = tp.Tp(name=name,
              is_tree_snow=True,
              d1000_appearing_roll=d1000_roll)

    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_tree(True)
    x.set_z_depth("wall")
    x.set_is_shadow_caster_soft(True)
    x.set_is_shadow_caster(True)

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rare, low means common
    x = tree_snow_init(d1000_roll=1, tiles=["tree_snow"])
    x.set_blit_top_off(1)

    x = tree_snow_init(d1000_roll=1, tiles=["tree_snow2"])
    x.set_blit_top_off(1)

init()
