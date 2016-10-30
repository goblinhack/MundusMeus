import tp


def thing_init(t):
    return


total_plant = 0


def plant1_init(d1000_roll, tiles=[]):

    global total_plant

    short_name = name = "plant" + str(total_plant + 1)
    total_plant = total_plant + 1

    x = tp.Tp(name=name,
              is_plant=True,
              d1000_appearing_roll=d1000_roll)

    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_plant(True)
    x.set_z_depth("floor2")

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    x = plant1_init(d1000_roll=500, tiles=["plant1"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)
    x = plant1_init(d1000_roll=1, tiles=["plant2"])
    x = plant1_init(d1000_roll=10, tiles=["plant3"])
    x = plant1_init(d1000_roll=10, tiles=["plant4"])

init()
