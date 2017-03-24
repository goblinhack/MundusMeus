import tp
import mm


def thing_init(t):
    return


def floor_init(name, tiles=[]):
    x = tp.Tp(name, is_floor=True)
    x.set_short_name("Dirty floor")
    x.set_is_floor(True)
    x.set_is_solid_ground(True)
    x.set_z_depth(mm.Z_DEPTH_FLOOR)
    x.set_is_dungeon_item(True)

    if tiles is not None:
        for t in tiles:
            x.set_tile(t)
    else:
        x.set_tile(tile=name)

    x.thing_init = thing_init
    x.set_blit_bot_off(1)
    x.set_has_shadow(True)


def init():
    floor_init(name="floor1",
               tiles=[
                      "floor1.1", "floor1.2", "floor1.3", "floor1.4",
                      "floor1.5", "floor1.6", "floor1.7", "floor1.8",
                     ])

    floor_init(name="floor2",
               tiles=[
                      "floor2.1", "floor2.2", "floor2.3", "floor2.4",
                      "floor2.5", "floor2.6", "floor2.7", "floor2.8",
                     ])

    floor_init(name="floor3",
               tiles=[
                      "floor3.1", "floor3.2", "floor3.3", "floor3.4",
                      "floor3.5", "floor3.6", "floor3.7", "floor3.8",
                     ])

    floor_init(name="floor4",
               tiles=[
                      "floor4.1", "floor4.2", "floor4.3", "floor4.4",
                      "floor4.5", "floor4.6",
                     ])

    floor_init(name="floor5",
               tiles=[
                      "floor5.1", "floor5.2", "floor5.3", "floor5.4",
                      "floor5.5", "floor5.6", "floor5.7", "floor5.8",
                     ])

    floor_init(name="floor6",
               tiles=[
                      "floor6.1", "floor6.2", "floor6.3", "floor6.4",
                      "floor6.5", "floor6.6", "floor6.7", "floor6.8",
                     ])

    floor_init(name="floor7",
               tiles=[
                      "floor7.1", "floor7.2", "floor7.3", "floor7.4",
                      "floor7.5", "floor7.6", "floor7.7", "floor7.8",
                     ])

    floor_init(name="floor8",
               tiles=[
                      "floor8.1", "floor8.2", "floor8.3", "floor8.4",
                      "floor8.5", "floor8.6",
                     ])

    floor_init(name="floor9",
               tiles=[
                      "floor9.1", "floor9.2", "floor9.3", "floor9.4",
                      "floor9.5", "floor9.6", "floor9.7", "floor9.8",
                     ])

    floor_init(name="floor10",
               tiles=[
                      "floor10.1", "floor10.2", "floor10.3", "floor10.4",
                      "floor10.5", "floor10.6", "floor10.7", "floor10.8",
                     ])

    floor_init(name="floor11",
               tiles=[
                      "floor11.1", "floor11.2", "floor11.3", "floor11.4",
                      "floor11.5", "floor11.6", "floor11.7", "floor11.8",
                     ])

    floor_init(name="floor12",
               tiles=[
                      "floor12.1", "floor12.2", "floor12.3", "floor12.4",
                      "floor12.5", "floor12.6", "floor12.7", "floor12.8",
                     ])

    floor_init(name="floor13",
               tiles=[
                      "floor13.1", "floor13.2", "floor13.3", "floor13.4",
                      "floor13.5", "floor13.6", "floor13.7", "floor13.8",
                     ])

    floor_init(name="floor14",
               tiles=[
                      "floor14.1", "floor14.2", "floor14.3", "floor14.4",
                      "floor14.5", "floor14.6", "floor14.7", "floor14.8",
                     ])

    floor_init(name="floor15",
               tiles=[
                      "floor15.1", "floor15.2", "floor15.3", "floor15.4",
                      "floor15.5", "floor15.6", "floor15.7", "floor15.8",
                     ])

    floor_init(name="floor16",
               tiles=[
                      "floor16.1", "floor16.2", "floor16.3", "floor16.4",
                      "floor16.5", "floor16.6", "floor16.7", "floor16.8",
                     ])

    floor_init(name="floor17",
               tiles=[
                      "floor17.1", "floor17.2", "floor17.3", "floor17.4",
                      "floor17.5", "floor17.6", "floor17.7", "floor17.8",
                     ])

    floor_init(name="floor18",
               tiles=[
                      "floor18.1", "floor18.2", "floor18.3", "floor18.4",
                      "floor18.5", "floor18.6", "floor18.7", "floor18.8",
                     ])

    floor_init(name="floor19",
               tiles=[
                      "floor19.1", "floor19.2", "floor19.3", "floor19.4",
                      "floor19.5", "floor19.6", "floor19.7", "floor19.8",
                     ])

    floor_init(name="floor20",
               tiles=[
                      "floor20.1", "floor20.2", "floor20.3", "floor20.4",
                      "floor20.5", "floor20.6", "floor20.7", "floor20.8",
                     ])

    floor_init(name="floor21",
               tiles=[
                      "floor21.1", "floor21.2", "floor21.3", "floor21.4",
                      "floor21.5", "floor21.6", "floor21.7", "floor21.8",
                     ])

    floor_init(name="floor22",
               tiles=[
                      "floor22.1", "floor22.2", "floor22.3", "floor22.4",
                      "floor22.5", "floor22.6", "floor22.7", "floor22.8",
                     ])

    floor_init(name="floor23",
               tiles=[
                      "floor23.1", "floor23.2", "floor23.3", "floor23.4",
                      "floor23.5", "floor23.6", "floor23.7", "floor23.8",
                     ])

    floor_init(name="floor24",
               tiles=[
                      "floor24.1", "floor24.2", "floor24.3", "floor24.4",
                      "floor24.5", "floor24.6", "floor24.7", "floor24.8",
                     ])

    floor_init(name="floor25",
               tiles=[
                      "floor25.1", "floor25.2", "floor25.3", "floor25.4",
                      "floor25.5", "floor25.6", "floor25.7", "floor25.8",
                     ])

    floor_init(name="floor26",
               tiles=[
                      "floor26.1", "floor26.2", "floor26.3", "floor26.4",
                      "floor26.5", "floor26.6", "floor26.7", "floor26.8",
                     ])

    floor_init(name="floor27",
               tiles=[
                      "floor27.1", "floor27.2", "floor27.3", "floor27.4",
                      "floor27.5", "floor27.6", "floor27.7", "floor27.8",
                     ])

    floor_init(name="floor28",
               tiles=[
                      "floor28.1", "floor28.2", "floor28.3", "floor28.4",
                      "floor28.5", "floor28.6", "floor28.7", "floor28.8",
                     ])

    floor_init(name="floor29",
               tiles=[
                      "floor29.1", "floor29.2", "floor29.3", "floor29.4",
                      "floor29.5", "floor29.6", "floor29.7", "floor29.8",
                     ])

    floor_init(name="floor30",
               tiles=[
                      "floor30.1", "floor30.2", "floor30.3", "floor30.4",
                      "floor30.5", "floor30.6", "floor30.7", "floor30.8",
                     ])

    floor_init(name="floor31",
               tiles=[
                      "floor31.1", "floor31.2", "floor31.3", "floor31.4",
                      "floor31.5", "floor31.6", "floor31.7", "floor31.8",
                     ])

    floor_init(name="floor32",
               tiles=[
                      "floor32.1", "floor32.2", "floor32.3", "floor32.4",
                      "floor32.5", "floor32.6", "floor32.7", "floor32.8",
                     ])

    floor_init(name="floor33",
               tiles=[
                      "floor33.1", "floor33.2", "floor33.3", "floor33.4",
                      "floor33.5", "floor33.6", "floor33.7", "floor33.8",
                     ])

    floor_init(name="floor34",
               tiles=[
                      "floor34.1", "floor34.2", "floor34.3", "floor34.4",
                      "floor34.5", "floor34.6", "floor34.7", "floor34.8",
                     ])

    floor_init(name="floor35",
               tiles=[
                      "floor35.1", "floor35.2", "floor35.3", "floor35.4",
                      "floor35.5", "floor35.6", "floor35.7", "floor35.8",
                     ])

    floor_init(name="floor36",
               tiles=[
                      "floor36.1", "floor36.2", "floor36.3", "floor36.4",
                      "floor36.5", "floor36.6", "floor36.7", "floor36.8",
                     ])

    floor_init(name="floor37",
               tiles=[
                      "floor37.1", "floor37.2", "floor37.3", "floor37.4",
                      "floor37.5", "floor37.6", "floor37.7", "floor37.8",
                     ])

    floor_init(name="floor38",
               tiles=[
                      "floor38.1", "floor38.2", "floor38.3", "floor38.4",
                      "floor38.5", "floor38.6", "floor38.7", "floor38.8",
                     ])

    floor_init(name="floor39",
               tiles=[
                      "floor39.1", "floor39.2", "floor39.3", "floor39.4",
                      "floor39.5", "floor39.6", "floor39.7", "floor39.8",
                     ])

    floor_init(name="floor40",
               tiles=[
                      "floor40.1", "floor40.2", "floor40.3", "floor40.4",
                      "floor40.5", "floor40.6", "floor40.7", "floor40.8",
                     ])

    floor_init(name="floor41",
               tiles=[
                      "floor41.1", "floor41.2", "floor41.3", "floor41.4",
                      "floor41.5", "floor41.6", "floor41.7", "floor41.8",
                     ])

    floor_init(name="floor42",
               tiles=[
                      "floor42.1", "floor42.2", "floor42.3", "floor42.4",
                      "floor42.5", "floor42.6", "floor42.7", "floor42.8",
                     ])

    floor_init(name="floor43",
               tiles=[
                      "floor43.1", "floor43.2", "floor43.3", "floor43.4",
                      "floor43.5", "floor43.6", "floor43.7", "floor43.8",
                     ])

    floor_init(name="floor44",
               tiles=[
                      "floor44.1", "floor44.2", "floor44.3", "floor44.4",
                      "floor44.5", "floor44.6", "floor44.7", "floor44.8",
                     ])

    floor_init(name="floor45",
               tiles=[
                      "floor45.1", "floor45.2", "floor45.3", "floor45.4",
                      "floor45.5", "floor45.6", "floor45.7", "floor45.8",
                     ])

    floor_init(name="floor46",
               tiles=[
                      "floor46.1", "floor46.2", "floor46.3", "floor46.4",
                      "floor46.5", "floor46.6", "floor46.7", "floor46.8",
                     ])

    floor_init(name="floor47",
               tiles=[
                      "floor47.1", "floor47.2", "floor47.3", "floor47.4",
                      "floor47.5", "floor47.6", "floor47.7", "floor47.8",
                     ])

    floor_init(name="floor48",
               tiles=[
                      "floor48.1", "floor48.2", "floor48.3", "floor48.4",
                      "floor48.5", "floor48.6", "floor48.7", "floor48.8",
                     ])

    floor_init(name="floor49",
               tiles=[
                      "floor49.1", "floor49.2", "floor49.3", "floor49.4",
                      "floor49.5", "floor49.6", "floor49.7", "floor49.8",
                     ])

    floor_init(name="floor50",
               tiles=[
                      "floor50.1", "floor50.2", "floor50.3", "floor50.4",
                      "floor50.5", "floor50.6", "floor50.7", "floor50.8",
                     ])

    floor_init(name="floor51",
               tiles=[
                      "floor51.1", "floor51.2", "floor51.3", "floor51.4",
                      "floor51.5", "floor51.6", "floor51.7", "floor51.8",
                     ])

    floor_init(name="floor52",
               tiles=[
                      "floor52.1", "floor52.2", "floor52.3", "floor52.4",
                      "floor52.5", "floor52.6", "floor52.7", "floor52.8",
                     ])

    floor_init(name="floor53",
               tiles=[
                      "floor53.1", "floor53.2", "floor53.3", "floor53.4",
                      "floor53.5", "floor53.6", "floor53.7", "floor53.8",
                     ])

    floor_init(name="floor54",
               tiles=[
                      "floor54.1", "floor54.2", "floor54.3", "floor54.4",
                      "floor54.5", "floor54.6", "floor54.7", "floor54.8",
                     ])

    floor_init(name="floor55",
               tiles=[
                      "floor55.1", "floor55.2", "floor55.3", "floor55.4",
                      "floor55.5", "floor55.6", "floor55.7", "floor55.8",
                     ])

    floor_init(name="floor56",
               tiles=[
                      "floor56.1", "floor56.2", "floor56.3", "floor56.4",
                      "floor56.5", "floor56.6", "floor56.7", "floor56.8",
                     ])

    floor_init(name="floor57",
               tiles=[
                      "floor57.1", "floor57.2", "floor57.3", "floor57.4",
                      "floor57.5", "floor57.6", "floor57.7", "floor57.8",
                     ])

    floor_init(name="floor58",
               tiles=[
                      "floor58.1", "floor58.2", "floor58.3", "floor58.4",
                      "floor58.5", "floor58.6", "floor58.7", "floor58.8",
                     ])

    floor_init(name="floor59",
               tiles=[
                      "floor59.1", "floor59.2", "floor59.3", "floor59.4",
                      "floor59.5", "floor59.6", "floor59.7", "floor59.8",
                     ])

    floor_init(name="floor60",
               tiles=[
                      "floor60.1", "floor60.2", "floor60.3", "floor60.4",
                      "floor60.5", "floor60.6", "floor60.7", "floor60.8",
                     ])

    floor_init(name="floor61",
               tiles=[
                      "floor61.1", "floor61.2", "floor61.3", "floor61.4",
                      "floor61.5", "floor61.6", "floor61.7", "floor61.8",
                     ])

    floor_init(name="floor62",
               tiles=[
                      "floor62.1", "floor62.2", "floor62.3", "floor62.4",
                      "floor62.5", "floor62.6", "floor62.7", "floor62.8",
                     ])

    floor_init(name="floor63",
               tiles=[
                      "floor63.1", "floor63.2", "floor63.3", "floor63.4",
                      "floor63.5", "floor63.6", "floor63.7", "floor63.8",
                     ])

    floor_init(name="floor64",
               tiles=[
                      "floor64.1", "floor64.2", "floor64.3", "floor64.4",
                      "floor64.5", "floor64.6", "floor64.7", "floor64.8",
                     ])

    floor_init(name="floor65",
               tiles=[
                      "floor65.1", "floor65.2", "floor65.3", "floor65.4",
                      "floor65.5", "floor65.6", "floor65.7", "floor65.8",
                     ])

    floor_init(name="floor66",
               tiles=[
                      "floor66.1", "floor66.2", "floor66.3", "floor66.4",
                      "floor66.5", "floor66.6", "floor66.7", "floor66.8",
                     ])

    floor_init(name="floor67",
               tiles=[
                      "floor67.1", "floor67.2", "floor67.3", "floor67.4",
                      "floor67.5", "floor67.6", "floor67.7", "floor67.8",
                     ])

    floor_init(name="floor68",
               tiles=[
                      "floor68.1", "floor68.2", "floor68.3", "floor68.4",
                      "floor68.5", "floor68.6", "floor68.7", "floor68.8",
                     ])

    floor_init(name="floor69",
               tiles=[
                      "floor69.1", "floor69.2", "floor69.3", "floor69.4",
                      "floor69.5", "floor69.6", "floor69.7", "floor69.8",
                     ])

    floor_init(name="floor70",
               tiles=[
                      "floor70.1", "floor70.2", "floor70.3", "floor70.4",
                      "floor70.5", "floor70.6", "floor70.7", "floor70.8",
                     ])

    floor_init(name="floor71",
               tiles=[
                      "floor71.1", "floor71.2", "floor71.3", "floor71.4",
                      "floor71.5", "floor71.6", "floor71.7", "floor71.8",
                     ])

    floor_init(name="floor72",
               tiles=[
                      "floor72.1", "floor72.2", "floor72.3", "floor72.4",
                      "floor72.5", "floor72.6", "floor72.7", "floor72.8",
                     ])

    floor_init(name="floor73",
               tiles=[
                      "floor73.1", "floor73.2", "floor73.3", "floor73.4",
                      "floor73.5", "floor73.6", "floor73.7", "floor73.8",
                     ])

    floor_init(name="floor74",
               tiles=[
                      "floor74.1", "floor74.2", "floor74.3", "floor74.4",
                      "floor74.5", "floor74.6", "floor74.7", "floor74.8",
                     ])

    floor_init(name="floor75",
               tiles=[
                      "floor75.1", "floor75.2", "floor75.3", "floor75.4",
                      "floor75.5", "floor75.6", "floor75.7", "floor75.8",
                     ])

    floor_init(name="floor76",
               tiles=[
                      "floor76.1", "floor76.2", "floor76.3", "floor76.4",
                      "floor76.5", "floor76.6", "floor76.7", "floor76.8",
                     ])

    floor_init(name="floor77",
               tiles=[
                      "floor77.1", "floor77.2", "floor77.3", "floor77.4",
                      "floor77.5", "floor77.6", "floor77.7", "floor77.8",
                     ])

    floor_init(name="floor78",
               tiles=[
                      "floor78.1", "floor78.2", "floor78.3", "floor78.4",
                      "floor78.5", "floor78.6", "floor78.7", "floor78.8",
                     ])

    floor_init(name="floor79",
               tiles=[
                      "floor79.1", "floor79.2", "floor79.3", "floor79.4",
                      "floor79.5", "floor79.6", "floor79.7", "floor79.8",
                     ])

    floor_init(name="floor80",
               tiles=[
                      "floor80.1", "floor80.2", "floor80.3", "floor80.4",
                      "floor80.5", "floor80.6", "floor80.7", "floor80.8",
                     ])

    floor_init(name="floor81",
               tiles=[
                      "floor81.1", "floor81.2", "floor81.3", "floor81.4",
                      "floor81.5", "floor81.6", "floor81.7", "floor81.8",
                     ])

    floor_init(name="floor82",
               tiles=[
                      "floor82.1", "floor82.2", "floor82.3", "floor82.4",
                      "floor82.5", "floor82.6", "floor82.7", "floor82.8",
                     ])

    floor_init(name="floor83",
               tiles=[
                      "floor83.1", "floor83.2", "floor83.3", "floor83.4",
                      "floor83.5", "floor83.6", "floor83.7", "floor83.8",
                     ])

    floor_init(name="floor84",
               tiles=[
                      "floor84.1", "floor84.2", "floor84.3", "floor84.4",
                      "floor84.5", "floor84.6", "floor84.7", "floor84.8",
                     ])


init()
