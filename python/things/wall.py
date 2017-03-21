import tp
import mm


def thing_init(t):
    return


def wall_init(name):
    x = tp.Tp(name, is_wall=True)
    x.set_short_name(name)
    x.set_is_movement_blocking(True)
    x.set_is_shadow_caster(True)
    x.set_is_wall(True)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_dungeon_item(True)

    x.set_tile(tile=name + "_up_down", is_join_vert=True)
    x.set_tile(tile=name + "_left_right", is_join_horiz=True)
    x.set_tile(tile=name + "_t180", is_join_t180=True)
    x.set_tile(tile=name + "_t270", is_join_t270=True)
    x.set_tile(tile=name + "_t", is_join_t=True)
    x.set_tile(tile=name + "_t90", is_join_t90=True)
    x.set_tile(tile=name + "_x", is_join_x=True)
    x.set_tile(tile=name + "_l90", is_join_tl=True)
    x.set_tile(tile=name + "_l270", is_join_br=True)
    x.set_tile(tile=name + "_l", is_join_bl=True)
    x.set_tile(tile=name + "_n180", is_join_bot=True)
    x.set_tile(tile=name + "_n270", is_join_left=True)
    x.set_tile(tile=name + "_n", is_join_top=True)
    x.set_tile(tile=name + "_n90", is_join_right=True)
    x.set_tile(tile=name + "_node", is_join_node=True)
    x.set_tile(tile=name + "_l180", is_join_tr=True)

    x.thing_init = thing_init

    return x


def init():
    x = wall_init(name="wall1")
    x.set_blit_top_off(1)

    wall_init(name="wall2")
    wall_init(name="wall3")
    wall_init(name="wall4")
    wall_init(name="wall5")
    wall_init(name="wall6")
    wall_init(name="wall7")
    wall_init(name="wall8")
    wall_init(name="wall9")
    wall_init(name="wall10")
    wall_init(name="wall11")
    wall_init(name="wall12")
    wall_init(name="wall13")
    wall_init(name="wall14")
    wall_init(name="wall15")
    wall_init(name="wall16")
    wall_init(name="wall17")
    x = wall_init(name="wall18")
    x.set_is_shadow_caster(False)

    wall_init(name="wall19")
    wall_init(name="wall20")
    wall_init(name="wall21")
    wall_init(name="wall22")
    wall_init(name="wall23")
    wall_init(name="wall24")

init()
