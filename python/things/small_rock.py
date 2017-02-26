import tp
import mm


def thing_init(t):
    return


total_small_rock = 0


def small_rock_init(d1000_roll, tiles=[]):

    global total_small_rock

    name = "small_rock" + str(total_small_rock + 1)
    total_small_rock = total_small_rock + 1

    x = tp.Tp(name=name,
              is_small_rock=True,
              d1000_appearing_roll=d1000_roll)

    x.set_short_name("Small rock")
    x.set_is_small_rock(True)
    x.set_is_world_item(True)
    x.set_z_depth(mm.Z_DEPTH_FLOOR2)

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rare, low means common
    small_rock_init(d1000_roll=1, tiles=["small_rock"])
    small_rock_init(d1000_roll=1, tiles=["small_rock2"])
    small_rock_init(d1000_roll=1, tiles=["small_rock3"])
    small_rock_init(d1000_roll=1, tiles=["small_rock4"])
    small_rock_init(d1000_roll=1, tiles=["small_rock5"])
    small_rock_init(d1000_roll=1, tiles=["small_rock6"])

init()
