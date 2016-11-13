import tp


def thing_init(t):
    return


total_treasure = 0


def treasure1_init(d1000_roll,
                   is_minable=False,
                   tiles=[]):

    global total_treasure

    short_name = name = "treasure" + str(total_treasure + 1)
    total_treasure = total_treasure + 1

    x = tp.Tp(name=name,
              is_treasure=True,
              d1000_appearing_roll=d1000_roll)

    x.set_long_name(short_name)
    x.set_short_name(short_name)

    if is_minable:
        x.set_z_depth("buried")
    else:
        x.set_z_depth("player")

    x.set_is_treasure(True)
    x.set_is_minable(is_minable)

#    x.set_light_radius(1)
#    x.set_light_tint("gray20")
#    x.set_is_candle_light(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    # high numbers means rate, low means common
    x = treasure1_init(d1000_roll=950, tiles=["gem1.1", "gem1.2", "gem1.3", ])
    x.set_scale(0.3)

    x = treasure1_init(d1000_roll=950, tiles=["gem2.1", "gem2.2", "gem2.3", ])
    x.set_scale(0.3)

    x = treasure1_init(d1000_roll=950, tiles=["gem3.1", "gem3.2", "gem3.3", ])
    x.set_scale(0.3)

    x = treasure1_init(d1000_roll=950, tiles=["gem4.1", ])
    x.set_scale(0.3)

    x = treasure1_init(d1000_roll=500, tiles=["chest1.1", ])
    x.set_scale(0.5)

    x = treasure1_init(d1000_roll=200, tiles=["bagofgold1.1", ])
    x.set_scale(0.5)

    treasure1_init(d1000_roll=100,
                   is_minable=True,
                   tiles=[
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.2", "gold1.2", "gold1.2", "gold1.2",
                          "gold1.3",
                          "gold1.2", "gold1.2", "gold1.2", "gold1.2",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.1", "gold1.1", "gold1.1", "gold1.1",
                          "gold1.2", "gold1.2", "gold1.2", "gold1.2",
                          "gold1.3",
                          "gold1.2", "gold1.2", "gold1.2", "gold1.2",
                         ])

    treasure1_init(d1000_roll=100,
                   is_minable=True,
                   tiles=[
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.2", "gold2.2", "gold2.2", "gold2.2",
                          "gold2.3",
                          "gold2.2", "gold2.2", "gold2.2", "gold2.2",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.1", "gold2.1", "gold2.1", "gold2.1",
                          "gold2.2", "gold2.2", "gold2.2", "gold2.2",
                          "gold2.3",
                          "gold2.2", "gold2.2", "gold2.2", "gold2.2",
                         ])

    treasure1_init(d1000_roll=100,
                   is_minable=True,
                   tiles=[
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.2", "gold3.2", "gold3.2", "gold3.2",
                          "gold3.3",
                          "gold3.2", "gold3.2", "gold3.2", "gold3.2",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.1", "gold3.1", "gold3.1", "gold3.1",
                          "gold3.2", "gold3.2", "gold3.2", "gold3.2",
                          "gold3.3",
                          "gold3.2", "gold3.2", "gold3.2", "gold3.2",
                         ])

init()
