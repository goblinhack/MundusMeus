import tp
import mm


def thing_init(t):
    return


def chasm_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_FLOOR)
    x.set_is_chasm_smoke(True)
    x.set_is_animated(True)
    x.set_is_dungeon_item(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=50)
    else:
        x.set_tile(tile=name, delay_ms=50)

    x.thing_init = thing_init

    return x


def init():
    x = chasm_init(name="chasm_smoke1",
                   short_name="Whisps of smoke",
                   tiles=[
                          "chasm_smoke1.1",
                          "chasm_smoke1.2",
                          "chasm_smoke1.3",
                          "chasm_smoke1.4",
                          "chasm_smoke1.5",
                          "chasm_smoke1.6",
                          "chasm_smoke1.7",
                          "chasm_smoke1.8",
                          "chasm_smoke1.9",
                          "chasm_smoke1.10",
                          "chasm_smoke1.11",
                          "chasm_smoke1.12",
                          "chasm_smoke1.13",
                          "chasm_smoke1.14",
                          "chasm_smoke1.15",
                          "chasm_smoke1.16",
                          "chasm_smoke1.17",
                          "chasm_smoke1.18",
                          "chasm_smoke1.19",
                          "chasm_smoke1.20",
                          "chasm_smoke1.21",
                          "chasm_smoke1.22",
                          "chasm_smoke1.23",
                          "chasm_smoke1.24",
                          "chasm_smoke1.25",
                          "chasm_smoke1.26",
                          "chasm_smoke1.27",
                          "chasm_smoke1.28",
                          "chasm_smoke1.29",
                          "chasm_smoke1.30",
                          "chasm_smoke1.31",
                          "chasm_smoke1.32",
                     ])

    x.set_blit_top_off(1)

    x = chasm_init(name="chasm_smoke2",
                   short_name="Whisps of smoke",
                   tiles=[
                          "chasm_smoke1.1",
                          "chasm_smoke1.2",
                          "chasm_smoke1.3",
                          "chasm_smoke1.4",
                          "chasm_smoke1.5",
                          "chasm_smoke1.6",
                          "chasm_smoke1.7",
                          "chasm_smoke1.8",
                          "chasm_smoke1.9",
                          "chasm_smoke1.10",
                          "chasm_smoke1.11",
                          "chasm_smoke1.12",
                          "chasm_smoke1.13",
                          "chasm_smoke1.14",
                          "chasm_smoke1.15",
                          "chasm_smoke1.16",
                          "chasm_smoke1.17",
                          "chasm_smoke1.18",
                          "chasm_smoke1.19",
                          "chasm_smoke1.20",
                          "chasm_smoke1.21",
                          "chasm_smoke1.22",
                          "chasm_smoke1.23",
                          "chasm_smoke1.24",
                          "chasm_smoke1.25",
                          "chasm_smoke1.26",
                          "chasm_smoke1.27",
                          "chasm_smoke1.28",
                          "chasm_smoke1.29",
                          "chasm_smoke1.30",
                          "chasm_smoke1.31",
                          "chasm_smoke1.32",
                     ])

    x.set_blit_top_off(1)

    x.set_light_radius(1.1)
    x.set_light_tint("gray30")
    x.set_light_pulse_amount(0.002)
    x.set_is_candle_light(True)


init()
