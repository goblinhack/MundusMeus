import tp
import mm


def thing_init(t):
    return


def landrock_init(name):
    x = tp.Tp(name)
    x.set_short_name("Snowy outrcop")
    x.set_is_movement_blocking(True)
    x.set_is_landrock_snow(True)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_shadow_caster(True)

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

    x.set_blit_top_off(1)
    x.thing_init = thing_init


def init():
    landrock_init(name="landrock_snow")

init()
