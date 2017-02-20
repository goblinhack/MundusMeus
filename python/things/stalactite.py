import tp


def thing_init(t):
    return


def stalactite_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_movement_blocking(True)
    x.set_is_rock(True)
    x.set_z_depth("stalactite")
    x.set_is_dungeon_item(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=150)
    else:
        x.set_tile(tile=name, delay_ms=150)

    x.thing_init = thing_init

    return x


def init():
    x = stalactite_init(name="stalactite1",
                        short_name="stalactite",
                        tiles=[
                               "stalactite1.1",
                               "stalactite1.2",
                               "stalactite1.3",
                               "stalactite1.4",
                               "stalactite1.5",
                               "stalactite1.6",
                               "stalactite1.7",
                               "stalactite1.8",
                               "stalactite1.9",
                               "stalactite1.10",
                               "stalactite1.11",
                               "stalactite1.12",
                               "stalactite1.13",
                               "stalactite1.14",
                               "stalactite1.15",
                               "stalactite1.16",
                               "stalactite1.17",
                               "stalactite1.18",
                               "stalactite1.19",
                               "stalactite1.20",
                               "stalactite1.21",
                               "stalactite1.22",
                               "stalactite1.23",
                               "stalactite1.24",
                               "stalactite1.25",
                               "stalactite1.26",
                               "stalactite1.27",
                               "stalactite1.28",
                               "stalactite1.29",
                               "stalactite1.30",
                               "stalactite1.31",
                               "stalactite1.32",
                           ])
    x.set_blit_bot_off(2)

init()
