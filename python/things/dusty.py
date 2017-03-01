import tp
import mm


def thing_init(t):
    return


def dusty_init(name, short_name, tiles=[]):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_dusty(True)
    x.set_is_solid_ground(True)
    x.set_z_depth(mm.Z_DEPTH_FLOOR)
    x.set_is_dungeon_item(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init
    x.set_blit_bot_off(2)


def init():
    dusty_init(name="dusty1",
               short_name="Rarely trodden passage",
               tiles=[
                      "dusty1.1",
                      "dusty1.2",
                      "dusty1.3",
                      "dusty1.4",
                      "dusty1.5",
                      "dusty1.6",
                      "dusty1.7",
                      "dusty1.8",
                      "dusty1.9",
                      "dusty1.10",
                      "dusty1.11",
                      "dusty1.12",
                      "dusty1.13",
                      "dusty1.14",
                      "dusty1.15",
                      "dusty1.16",
                      "dusty1.17",
                      "dusty1.18",
                      "dusty1.19",
                      "dusty1.20",
                      "dusty1.21",
                      "dusty1.22",
                      "dusty1.23",
                      "dusty1.24",
                      "dusty1.25",
                      "dusty1.26",
                      "dusty1.27",
                      "dusty1.28",
                      "dusty1.29",
                      "dusty1.30",
                      "dusty1.31",
                      "dusty1.32",
                     ])

init()
