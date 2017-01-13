import tp


def thing_init(t):
    return


def cwall_init(name):
    x = tp.Tp(name, is_cwall=True)
    x.set_long_name(name)
    x.set_short_name(name)
    x.set_is_movement_blocking(True)
    x.set_is_cwall(True)
    x.set_z_depth("wall")
    x.set_blit_top_off(1)
    x.set_blit_bot_off(1)

#    x.set_is_shadow_caster_soft(True)
    x.set_is_shadow_caster(True)
    x.set_tile(tile=name + "-up-down", is_join_vert=True)
    x.set_tile(tile=name + "-left-right", is_join_horiz=True)
    x.set_tile(tile=name + "-t180", is_join_t180=True)
    x.set_tile(tile=name + "-t270", is_join_t270=True)
    x.set_tile(tile=name + "-t", is_join_t=True)
    x.set_tile(tile=name + "-t90", is_join_t90=True)
    x.set_tile(tile=name + "-x", is_join_x=True)
    x.set_tile(tile=name + "-l90", is_join_tl=True)
    x.set_tile(tile=name + "-l270", is_join_br=True)
    x.set_tile(tile=name + "-l", is_join_bl=True)
    x.set_tile(tile=name + "-n180", is_join_bot=True)
    x.set_tile(tile=name + "-n270", is_join_left=True)
    x.set_tile(tile=name + "-n", is_join_top=True)
    x.set_tile(tile=name + "-n90", is_join_right=True)
    x.set_tile(tile=name + "-node", is_join_node=True)
    x.set_tile(tile=name + "-l180", is_join_tr=True)

    x.thing_init = thing_init


def init():
    cwall_init(name="cwall1")

init()
