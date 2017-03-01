import tp
import mm


def thing_init(t):
    return


def corridor_init(name, short_name, tiles=[]):
    x = tp.Tp(name, is_corridor=True)
    x.set_short_name(short_name)
    x.set_is_corridor(True)
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

    corridor_init(name="corridor1",
                  short_name="Dirty corridor",
                  tiles=[
                         "corridor1.1", "corridor1.2", "corridor1.3",
                         "corridor1.4", "corridor1.5", "corridor1.6",
                         "corridor1.7", "corridor1.8",
                        ])

    corridor_init(name="corridor2",
                  short_name="Dirty corridor",
                  tiles=[
                         "corridor2.1", "corridor2.2", "corridor2.3",
                         "corridor2.4", "corridor2.5", "corridor2.6",
                         "corridor2.7", "corridor2.8",
                        ])

    corridor_init(name="corridor3",
                  short_name="Dirty corridor",
                  tiles=[
                         "corridor3.1", "corridor3.2", "corridor3.3",
                         "corridor3.4", "corridor3.5", "corridor3.6",
                         "corridor3.7", "corridor3.8",
                        ])

    corridor_init(name="corridor4",
                  short_name="Dirty corridor",
                  tiles=[
                         "corridor4.1", "corridor4.2", "corridor4.3",
                         "corridor4.4", "corridor4.5", "corridor4.6",
                         "corridor4.7", "corridor4.8",
                        ])

    corridor_init(name="corridor5",
                  short_name="Dirty corridor",
                  tiles=[
                         "corridor5.1", "corridor5.2", "corridor5.3",
                         "corridor5.4", "corridor5.5", "corridor5.6",
                         "corridor5.7", "corridor5.8",
                        ])

    corridor_init(name="corridor6",
                  short_name="Dirty corridor",
                  tiles=[
                         "corridor6.1", "corridor6.2", "corridor6.3",
                         "corridor6.4", "corridor6.5", "corridor6.6",
                         "corridor6.7", "corridor6.8",
                        ])

    corridor_init(name="corridor7",
                  short_name="Dirty corridor",
                  tiles=[
                         "corridor7.1", "corridor7.2", "corridor7.3",
                         "corridor7.4", "corridor7.5", "corridor7.6",
                         "corridor7.7", "corridor7.8",
                        ])

    corridor_init(name="corridor8",
                  short_name="Dirty corridor",
                  tiles=[
                         "corridor8.1", "corridor8.2", "corridor8.3",
                         "corridor8.4", "corridor8.5", "corridor8.6",
                         "corridor8.7", "corridor8.8",
                        ])

init()
