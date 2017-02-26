import tp
import mm


def thing_init(t):
    return


total_snow_mound = 0


def snow_mound1_init(d1000_roll, tiles=[]):

    global total_snow_mound

    name = "snow_mound" + str(total_snow_mound + 1)
    total_snow_mound = total_snow_mound + 1

    x = tp.Tp(name=name,
              is_snow_mound=True,
              d1000_appearing_roll=d1000_roll)

    x.set_short_name("Snow mound")
    x.set_is_snow_mound(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_FLOOR2)
    x.set_blit_top_off(1)
    x.set_blit_left_off(1)
    x.set_blit_right_off(1)

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rare, low means common
    snow_mound1_init(d1000_roll=1, tiles=["snow_mound1"])
    snow_mound1_init(d1000_roll=1, tiles=["snow_mound2"])
    snow_mound1_init(d1000_roll=1, tiles=["snow_mound3"])
    snow_mound1_init(d1000_roll=1, tiles=["snow_mound4"])
    snow_mound1_init(d1000_roll=1, tiles=["snow_mound5"])

init()
