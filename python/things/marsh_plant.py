import tp
import mm


def thing_init(t):
    return


total_marsh_plant = 0


def marsh_plant1_init(d1000_roll, tiles=[]):

    global total_marsh_plant

    name = "marsh" + str(total_marsh_plant + 1)
    total_marsh_plant = total_marsh_plant + 1

    x = tp.Tp(name=name,
              is_marsh_plant=True,
              d1000_appearing_roll=d1000_roll)

    x.set_short_name("Marsh plant")
    x.set_is_marsh_plant(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_FLOOR2)

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rare, low means common
    marsh_plant1_init(d1000_roll=100, tiles=["marsh_plant1"])
    marsh_plant1_init(d1000_roll=100, tiles=["marsh_plant2"])
    marsh_plant1_init(d1000_roll=100, tiles=["marsh_plant3"])
    marsh_plant1_init(d1000_roll=100, tiles=["marsh_plant4"])

init()
