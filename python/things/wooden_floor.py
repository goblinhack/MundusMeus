import tp
import mm


def thing_init(t):
    return


def floor_init(name, tiles=[]):
    x = tp.Tp(name, is_floor=True)
    x.set_short_name("Wooden floor")
    x.set_is_floor(True)
    x.set_is_solid_ground(True)
    x.set_is_house_item(True)
    x.set_z_depth(mm.Z_DEPTH_FLOOR)
    x.set_is_house_item(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    floor_init(name="wooden_floor1",
               tiles=[
                      "wooden_floor1.1",
                      "wooden_floor1.2",
                      "wooden_floor1.3",
                      "wooden_floor1.4",
                      "wooden_floor1.5",
                      "wooden_floor1.6",
                      "wooden_floor1.7",
                      "wooden_floor1.8",
                      "wooden_floor1.9",
                      "wooden_floor1.10",
                      "wooden_floor1.11",
                      "wooden_floor1.12",
                      "wooden_floor1.13",
                      "wooden_floor1.14",
                      "wooden_floor1.15",
                      "wooden_floor1.16",
                     ])

    floor_init(name="wooden_floor2",
               tiles=[
                      "wooden_floor2.1",
                      "wooden_floor2.2",
                      "wooden_floor2.3",
                      "wooden_floor2.4",
                      "wooden_floor2.5",
                      "wooden_floor2.6",
                      "wooden_floor2.7",
                      "wooden_floor2.8",
                      "wooden_floor2.9",
                      "wooden_floor2.10",
                      "wooden_floor2.11",
                      "wooden_floor2.12",
                      "wooden_floor2.13",
                      "wooden_floor2.14",
                      "wooden_floor2.15",
                      "wooden_floor2.16",
                     ])

    floor_init(name="wooden_floor3",
               tiles=[
                      "wooden_floor3.1",
                      "wooden_floor3.2",
                      "wooden_floor3.3",
                      "wooden_floor3.4",
                      "wooden_floor3.5",
                      "wooden_floor3.6",
                      "wooden_floor3.7",
                      "wooden_floor3.8",
                      "wooden_floor3.9",
                      "wooden_floor3.10",
                      "wooden_floor3.11",
                      "wooden_floor3.12",
                      "wooden_floor3.13",
                      "wooden_floor3.14",
                      "wooden_floor3.15",
                      "wooden_floor3.16",
                     ])

init()
