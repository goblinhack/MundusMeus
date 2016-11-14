import tp


def thing_init(t):
    return


total_small_rock = 0


def small_rock1_init(d1000_roll, tiles=[]):

    global total_small_rock

    short_name = name = "small_rock" + str(total_small_rock + 1)
    total_small_rock = total_small_rock + 1

    x = tp.Tp(name=name,
              is_small_rock=True,
              d1000_appearing_roll=d1000_roll)

    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_small_rock(True)
    x.set_z_depth("floor2")

    for t in tiles:
        x.set_tile(t, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rate, low means common
    small_rock1_init(d1000_roll=1, tiles=["small_rock1"])
    small_rock1_init(d1000_roll=1, tiles=["small_rock2"])

init()
