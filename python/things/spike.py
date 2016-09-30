import tp


def thing_init(t):
    t.set_description("spike")


def spike_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_rock(True)
    x.set_map_depth("floor")

    x.set_light_radius(7.0)
    x.set_light_tint("purple")

    x.thing_init = thing_init

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=150)
    else:
        x.set_tile(tile=name, delay_ms=150)

    return x


def init():
    x = spike_init(name="spike1",         short_name="spike1",
                   tiles=[
                          "spike1.1",
                          "spike1.2",
                          "spike1.3",
                          "spike1.4",
                          "spike1.5",
                          "spike1.6",
                          "spike1.7",
                          "spike1.8",
                          "spike1.9",
                          "spike1.10",
                          "spike1.11",
                          "spike1.12",
                          "spike1.13",
                          "spike1.14",
                      ])
    x.set_blit_bot_off(1)

init()
