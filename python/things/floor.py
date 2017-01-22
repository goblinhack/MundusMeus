import tp


def thing_init(t):
    return


def floor_init(name, short_name, long_name, tiles=[]):
    x = tp.Tp(name, is_floor=True)
    x.set_long_name(short_name)
    x.set_short_name(short_name)
    x.set_is_floor(True)
    x.set_z_depth("floor")

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init


def init():
    floor_init(name="floor1",
               short_name="dirty floor",
               long_name="dirty floor",
               tiles=[
                      "floor1.1", "floor1.2", "floor1.3", "floor1.4",
                      "floor1.5", "floor1.6", "floor1.7", "floor1.8",
                     ])

    floor_init(name="floor2",
               short_name="dirty floor",
               long_name="dirty floor",
               tiles=[
                      "floor2.1", "floor2.2", "floor2.3", "floor2.4",
                      "floor2.5", "floor2.6", "floor2.7", "floor2.8",
                     ])

    floor_init(name="floor3",
               short_name="dirty floor",
               long_name="dirty floor",
               tiles=[
                      "floor3.1", "floor3.2", "floor3.3", "floor3.4",
                      "floor3.5", "floor3.6", "floor3.7", "floor3.8",
                     ])

    floor_init(name="floor4",
               short_name="dirty floor",
               long_name="dirty floor",
               tiles=[
                      "floor4.1", "floor4.2", "floor4.3", "floor4.4",
                      "floor4.5", "floor4.6",
                     ])

    floor_init(name="floor5",
               short_name="dirty floor",
               long_name="dirty floor",
               tiles=[
                      "floor5.1", "floor5.2", "floor5.3", "floor5.4",
                      "floor5.5", "floor5.6", "floor5.7", "floor5.8",
                     ])

    floor_init(name="floor6",
               short_name="dirty floor",
               long_name="dirty floor",
               tiles=[
                      "floor6.1", "floor6.2", "floor6.3", "floor6.4",
                      "floor6.5", "floor6.6", "floor6.7", "floor6.8",
                     ])

    floor_init(name="floor7",
               short_name="dirty floor",
               long_name="dirty floor",
               tiles=[
                      "floor7.1", "floor7.2", "floor7.3", "floor7.4",
                      "floor7.5", "floor7.6", "floor7.7", "floor7.8",
                     ])

    floor_init(name="floor8",
               short_name="dirty floor",
               long_name="dirty floor",
               tiles=[
                      "floor8.1", "floor8.2", "floor8.3", "floor8.4",
                      "floor8.5", "floor8.6",
                     ])

init()
