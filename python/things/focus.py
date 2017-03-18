import tp
import mm


def thing_init(t):
    return


def focus_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_z_depth(mm.Z_DEPTH_PLAYER)
    x.set_is_animated(True)
    x.set_is_focus(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=25)
    else:
        x.set_tile(tile=name, delay_ms=25)

    x.thing_init = thing_init

    return x


def init():
    focus_init(name="focus1",
               short_name="Mouse focus",
               tiles=[
                      "focus1.1",
                      "focus1.2",
                      "focus1.3",
                      "focus1.4",
                      "focus1.5",
                      "focus1.6",
                      "focus1.7",
                      "focus1.8",
                      "focus1.9",
                      "focus1.10",
                      "focus1.11",
                      "focus1.12",
                      "focus1.13",
                      "focus1.14",
                      "focus1.15",
                      "focus1.16",
                      "focus1.17",
                      "focus1.18",
                      "focus1.19",
                      "focus1.20",
                      "focus1.21",
                      "focus1.22",
                      "focus1.23",
                      "focus1.24",
                      "focus1.25",
                      "focus1.26",
                      "focus1.27",
                      "focus1.28",
                      "focus1.29",
                      "focus1.30",
                      "focus1.31",
                      "focus1.32",
                 ])

    focus_init(name="focus2",
               short_name="Mouse focus",
               tiles=[
                      "focus2.1",
                      "focus2.2",
                      "focus2.3",
                      "focus2.4",
                      "focus2.5",
                      "focus2.6",
                      "focus2.7",
                      "focus2.8",
                      "focus2.9",
                      "focus2.10",
                      "focus2.11",
                      "focus2.12",
                      "focus2.13",
                      "focus2.14",
                      "focus2.15",
                      "focus2.16",
                      "focus2.17",
                      "focus2.18",
                      "focus2.19",
                      "focus2.20",
                      "focus2.21",
                      "focus2.22",
                      "focus2.23",
                      "focus2.24",
                      "focus2.25",
                      "focus2.26",
                      "focus2.27",
                      "focus2.28",
                      "focus2.29",
                      "focus2.30",
                      "focus2.31",
                      "focus2.32",
                 ])

init()
