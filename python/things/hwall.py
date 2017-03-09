import tp
import mm


def thing_init(t):
    return


def hwall_init(name):
    x = tp.Tp(name, is_hwall=True)
    x.set_short_name("House wall")
    x.set_is_movement_blocking(True)
    x.set_is_hwall(True)
    x.set_z_depth(mm.Z_DEPTH_WALL)
    x.set_is_house_item(True)

#    x.set_is_shadow_caster_soft(True)
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

    x.thing_init = thing_init

    return x


def init():
    x = hwall_init(name="hwall1")
    x.set_blit_top_off(1)

init()
