import tp
import mm


def thing_init(t):
    return


total_plant = 0


def plant1_init(d1000_roll, tiles=[]):

    global total_plant

    name = "plant" + str(total_plant + 1)
    total_plant = total_plant + 1

    x = tp.Tp(name=name,
              is_plant=True,
              d1000_appearing_roll=d1000_roll)

    x.set_short_name("Plant")
    x.set_is_plant(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_FLOOR2)

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    x = plant1_init(d1000_roll=500, tiles=["plant1"])
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)
    # high numbers means rare, low means common
    x = plant1_init(d1000_roll=1, tiles=["plant2"])
    x = plant1_init(d1000_roll=10, tiles=["plant3"])
    x = plant1_init(d1000_roll=10, tiles=["plant4"])
    x = plant1_init(d1000_roll=100, tiles=["plant5"])
    x = plant1_init(d1000_roll=100, tiles=["plant5"])
    x = plant1_init(d1000_roll=100, tiles=["plant6"])
    x = plant1_init(d1000_roll=900, tiles=["plant7"])
    x = plant1_init(d1000_roll=900, tiles=["plant8"])
    x = plant1_init(d1000_roll=900, tiles=["plant9"])
    x = plant1_init(d1000_roll=900, tiles=["plant10"])
    x = plant1_init(d1000_roll=900, tiles=["plant11"])
    x = plant1_init(d1000_roll=100, tiles=["plant12"])
    x = plant1_init(d1000_roll=100, tiles=["plant13"])
    x = plant1_init(d1000_roll=100, tiles=["plant14"])
    x = plant1_init(d1000_roll=100, tiles=["plant15"])

init()
