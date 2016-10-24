import tp


def thing_init(t):
    t.set_description("cwall")


def cwall_init(name, short_name):
    x = tp.Tp(name)
    x.set_short_name(short_name)
    x.set_is_movement_blocking(True)
    x.set_is_cwall(True)
    x.set_z_depth("wall")
    x.set_blit_top_off(1)
    x.set_blit_bot_off(1)

    x.set_tile(tile=short_name + "-up-down", is_join_vert=True)
    x.set_tile(tile=short_name + "-left-right", is_join_horiz=True)
    x.set_tile(tile=short_name + "-t180", is_join_t180=True)
    x.set_tile(tile=short_name + "-t270", is_join_t270=True)
    x.set_tile(tile=short_name + "-t", is_join_t=True)
    x.set_tile(tile=short_name + "-t90", is_join_t90=True)
    x.set_tile(tile=short_name + "-x", is_join_x=True)
    x.set_tile(tile=short_name + "-l90", is_join_tl=True)
    x.set_tile(tile=short_name + "-l270", is_join_br=True)
    x.set_tile(tile=short_name + "-l", is_join_bl=True)
    x.set_tile(tile=short_name + "-n180", is_join_bot=True)
    x.set_tile(tile=short_name + "-n270", is_join_left=True)
    x.set_tile(tile=short_name + "-n", is_join_top=True)
    x.set_tile(tile=short_name + "-n90", is_join_right=True)
    x.set_tile(tile=short_name + "-node", is_join_node=True)
    x.set_tile(tile=short_name + "-l180", is_join_tr=True)

    x.thing_init = thing_init


def init():
    cwall_init(name="cwall1",         short_name="cwall1")

init()
