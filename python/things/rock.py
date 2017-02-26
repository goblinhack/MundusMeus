import tp
import mm


def thing_init(t):
    return


def rock_init(name, short_name, tiles=[]):
    x = tp.Tp(name)

    x.set_is_shadow_caster_soft(True)
    x.set_is_shadow_caster(True)
    x.set_short_name(short_name)
    x.set_is_movement_blocking(True)
    x.set_is_rock(True)
    x.set_z_depth(mm.Z_DEPTH_ROCK)
    x.set_is_solid_ground(True)
    x.set_blit_top_off(1)
    x.set_blit_bot_off(1)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t, delay_ms=150)
    else:
        x.set_tile(tile=name, delay_ms=150)

    x.thing_init = thing_init


def init():
    rock_init(name="rock",
              short_name="A boring rock",
              tiles=[
                     "rock.1",
                     "rock.2",
                     "rock.3",
                     "rock.4",
                     "rock.5",
                     "rock.6",
                     "rock.7",
                     "rock.8",
                     "rock.9",
                     "rock.10",
                     "rock.11",
                     "rock.12",
                     "rock.13",
                     "rock.14",
                     "rock.15",
                     "rock.16",
                     "rock.17",
                     "rock.18",
                     "rock.19",
                     "rock.20",
                     "rock.21",
                     "rock.22",
                     "rock.23",
                     "rock.24",
                     "rock.25",
                     "rock.26",
                     "rock.27",
                     "rock.28",
                     "rock.29",
                     "rock.30",
                     "rock.31",
                     "rock.32",
                    ])

init()
