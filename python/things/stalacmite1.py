import tp
import mm


def thing_init(t):
    return


def stalacmite_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_movement_blocking(True)
    x.set_is_shadow_caster(True)
    x.set_is_rock(True)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_dungeon_item(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=150)
    else:
        x.set_tile(tile=name, delay_ms=150)

    x.thing_init = thing_init

    return x


def init():
    x = stalacmite_init(name="stalacmite1",
                        short_name="Stalactite",
                        tiles=[
                               "stalacmite1.1",
                               "stalacmite1.2",
                               "stalacmite1.3",
                               "stalacmite1.4",
                               "stalacmite1.5",
                               "stalacmite1.6",
                               "stalacmite1.7",
                               "stalacmite1.8",
                               "stalacmite1.9",
                               "stalacmite1.10",
                               "stalacmite1.11",
                               "stalacmite1.12",
                               "stalacmite1.13",
                               "stalacmite1.14",
                               "stalacmite1.15",
                               "stalacmite1.16",
                               "stalacmite1.17",
                               "stalacmite1.18",
                               "stalacmite1.19",
                               "stalacmite1.20",
                               "stalacmite1.21",
                               "stalacmite1.22",
                               "stalacmite1.23",
                               "stalacmite1.24",
                               "stalacmite1.25",
                               "stalacmite1.26",
                               "stalacmite1.27",
                               "stalacmite1.28",
                               "stalacmite1.29",
                               "stalacmite1.30",
                               "stalacmite1.31",
                               "stalacmite1.32",
                           ])
    x.set_blit_top_off(2)

init()
