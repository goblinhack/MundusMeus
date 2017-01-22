import random
import mm
import thing
import biome_dungeon_do
import rooms
import tp
import game


def biome_build(self, seed=0):
    self.biome_seed = seed

    #
    # The dungeon takes up all chunks
    #
    if self.cx != mm.CHUNK_ACROSS - 1 or \
       self.cy != mm.CHUNK_DOWN - 1:
        self.biome = None
        return

    seed = self.level.seed + self.level.where.z

    while True:
        fixed_rooms = rooms.create_fixed()
        random.seed(seed)

        mm.log("Build dungeon; seed {0}".format(self.biome_seed))

        self.biome = biome_dungeon_do.Biome(chunk=self,
                                            rooms=fixed_rooms,
                                            rooms_on_chunk=6,
                                            fixed_room_chance=10)

        if not self.biome.generate_failed:
            break

        seed += 1
        seed *= self.biome_seed

        mm.log("Failed to build dungeon; retry")

    if False:
        self.biome.dump_depth()
        self.biome.dump()


def biome_populate(self):
    c = self
    m = self.biome

    if self.biome is None:
        return

    for y in range(0, mm.MAP_HEIGHT):
        for x in range(0, mm.MAP_WIDTH):

            tx = x
            ty = y
            (c, ox, oy) = self.level.xy_to_chunk_xy(tx, ty)

            place_stalactite = False

            if m.is_floor_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="floor1")
                t.set_depth(m.depth_map.cells[x][y])
                t.push()

                if not m.is_wall_at(x, y) and not m.is_cwall_at(x, y):
                    if random.randint(0, 1000) < 2:
                        t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="torch1")
                        t.push()

            if random.randint(0, 1000) < 5:
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="torch1")
                t.push()

            if m.is_dusty_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="dusty1")
                t.set_depth(m.bridge_height[x][y])
                t.push()

                if m.bridge_height[x][y] > 0:
                    t = thing.Thing(chunk=c, x=tx, y=ty + 1,
                                    tp_name="under_dusty1")
                    t.set_depth(m.bridge_height[x][y])
                    t.push()

                    if not m.is_lava_at(x, y) and \
                       not m.is_water_at(x, y):
                        t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="rock")
                        t.push()

                if not m.is_wall_at(x, y) and not m.is_cwall_at(x, y):
                    if random.randint(0, 1000) < 2:
                        t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="torch1")
                        t.push()

            if m.is_corridor_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="corridor1")
                t.set_depth(m.bridge_height[x][y])
                t.push()

                if m.bridge_height[x][y] > 0:
                    t = thing.Thing(chunk=c, x=tx, y=ty + 1,
                                    tp_name="under_corridor1")
                    t.set_depth(m.bridge_height[x][y])
                    t.push()

                if not m.is_wall_at(x, y) and not m.is_cwall_at(x, y):
                    if random.randint(0, 1000) < 2:
                        t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="torch1")
                        t.push()

            if m.is_dungeon_way_up_at(x, y):

                t = thing.Thing(chunk=self, x=tx, y=ty,
                                tp_name="dungeon_way_up1")
                t.set_depth(m.depth_map.cells[x][y])
                t.push()

                if game.g.player is None:
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="player1")
                    t.push()
                    game.g.player = t

            if m.is_dungeon_way_down_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty,
                                tp_name="dungeon_way_down1")
                t.set_depth(m.depth_map.cells[x][y])
                t.push()

                t = thing.Thing(chunk=c, x=tx, y=ty - 1,
                                tp_name="dungeon_way_down1_deco")
                t.set_depth(m.depth_map.cells[x][y])
                t.push()

                t = thing.Thing(chunk=c, x=tx, y=ty,
                                tp_name="chasm_smoke1")
                t.push()

                t = thing.Thing(chunk=c, x=tx, y=ty - 1,
                                tp_name="chasm_smoke1")
                t.push()

            if m.is_wall_at(x, y):
                place_stalactite = True
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="wall1")
                t.push()

                if m.is_wall_at(x, y-1):
                    b = True
                else:
                    b = False

                if m.is_wall_at(x-1, y):
                    d = True
                else:
                    d = False

                if m.is_wall_at(x+1, y):
                    f = True
                else:
                    f = False

                if m.is_wall_at(x, y+1):
                    h = True
                else:
                    h = False

                # a b c
                # d e f
                # g h i
                if b and d and f and h:
                    t.set_tilename("wall1_x")
                elif b and d and f:
                    t.set_tilename("wall1_t180")
                elif b and d and h:
                    t.set_tilename("wall1_t90")
                elif b and f and h:
                    t.set_tilename("wall1_t270")
                elif d and f and h:
                    t.set_tilename("wall1_t")
                elif b and h:
                    t.set_tilename("wall1_up_down")
                elif d and f:
                    t.set_tilename("wall1_left_right")
                elif b and f:
                    t.set_tilename("wall1_l")
                elif h and f:
                    t.set_tilename("wall1_l90")
                elif d and h:
                    t.set_tilename("wall1_l180")
                elif b and d:
                    t.set_tilename("wall1_l270")
                elif b:
                    t.set_tilename("wall1_n180")
                elif f:
                    t.set_tilename("wall1_n270")
                elif h:
                    t.set_tilename("wall1_n")
                elif d:
                    t.set_tilename("wall1_n90")
                else:
                    t.set_tilename("wall1_node")

            if m.is_cwall_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="cwall1")
                t.push()

                if m.is_cwall_at(x, y-1):
                    b = True
                else:
                    b = False

                if m.is_cwall_at(x-1, y):
                    d = True
                else:
                    d = False

                if m.is_cwall_at(x+1, y):
                    f = True
                else:
                    f = False

                if m.is_cwall_at(x, y+1):
                    h = True
                else:
                    h = False

                # a b c
                # d e f
                # g h i
                if b and d and f and h:
                    t.set_tilename("cwall1_x")
                elif b and d and f:
                    t.set_tilename("cwall1_t180")
                elif b and d and h:
                    t.set_tilename("cwall1_t90")
                elif b and f and h:
                    t.set_tilename("cwall1_t270")
                elif d and f and h:
                    t.set_tilename("cwall1_t")
                elif b and h:
                    t.set_tilename("cwall1_up_down")
                elif d and f:
                    t.set_tilename("cwall1_left_right")
                elif b and f:
                    t.set_tilename("cwall1_l")
                elif h and f:
                    t.set_tilename("cwall1_l90")
                elif d and h:
                    t.set_tilename("cwall1_l180")
                elif b and d:
                    t.set_tilename("cwall1_l270")
                elif b:
                    t.set_tilename("cwall1_n180")
                elif f:
                    t.set_tilename("cwall1_n270")
                elif h:
                    t.set_tilename("cwall1_n")
                elif d:
                    t.set_tilename("cwall1_n90")
                else:
                    t.set_tilename("cwall1_node")

            if m.is_lava_at(x, y):

                if m.is_floor_at(x, y) or \
                   m.is_dusty_at(x, y) or \
                   m.is_corridor_at(x, y):
                    #
                    # Underground lava
                    #
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="lava1")
                    t.push()

                elif m.is_wall_at(x, y - 1) or \
                        m.is_rock_at(x, y - 1) or \
                        m.is_cwall_at(x, y - 1):

                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="lava1_top")
                    t.push()

                elif m.is_floor_at(x, y - 1) and not m.is_floor_at(x, y):
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="lava1_top")
                    t.push()
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="lava1")
                    t.push()

                elif m.is_corridor_at(x, y - 1) and \
                        not m.is_corridor_at(x, y):
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="lava1_top")
                    t.push()
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="lava1")
                    t.push()

                elif m.is_dusty_at(x, y - 1) and \
                        not m.is_dusty_at(x, y):
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="lava1_top")
                    t.push()
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="lava1")
                    t.push()

                else:
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="lava1")
                    t.push()

                t.set_depth(m.depth_map.cells[x][y])

                if random.randint(0, 100) < 30:
                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                    tp_name="chasm_smoke2")
                    t.push()

                if random.randint(0, 100) < 5:
                    toughness = m.depth_map.cells[x][y] * 20
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.short_name)
                    t.push()

            if m.is_water_at(x, y):

                water = "water1"
                put_treasure = False

                if random.randint(0, 100) < 5:
                    put_treasure = True
                    water = "water1_trans"

                if m.is_floor_at(x, y) or \
                   m.is_dusty_at(x, y) or \
                   m.is_corridor_at(x, y):
                    #
                    # Underground water
                    #
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=water)
                    t.push()

                elif m.is_wall_at(x, y - 1) or \
                        m.is_rock_at(x, y - 1) or \
                        m.is_cwall_at(x, y - 1):
                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                    tp_name=water + "_top")
                    t.push()

                elif m.is_floor_at(x, y - 1) and not m.is_floor_at(x, y):
                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                    tp_name=water + "_top")
                    t.push()

                elif m.is_corridor_at(x, y - 1) and \
                        not m.is_corridor_at(x, y):
                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                    tp_name=water + "_top")
                    t.push()

                elif m.is_dusty_at(x, y - 1) and \
                        not m.is_dusty_at(x, y):
                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                    tp_name=water + "_top")
                    t.push()

                else:
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=water)
                    t.push()

                t.set_depth(m.depth_map.cells[x][y])

                if put_treasure:
                    toughness = m.depth_map.cells[x][y] * 2
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.short_name)
                    t.push()

                if random.randint(0, 100) < 5:
                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                    tp_name="chasm_smoke2")
                    t.push()

#            if m.is_rock_at(x, y):
#                place_stalactite = True
#                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="rock")
#                t.push()
#
#                if random.randint(0, 100) < 5:
#                    toughness = m.depth_map.cells[x][y]
#                    r = tp.get_random_minable_treasure(toughness=toughness)
#                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.short_name)
#                    t.push()

            if m.is_door_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="door1")
                t.push()

                num = len(t.tp.tiles)
                which = m.getr(x, y)
                which += self.biome_seed
                which = which % num
                which += 1

                t.set_tilename("door1." + str(which))

            if m.is_key_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="key1")
                t.push()

                num = len(t.tp.tiles)
                which = m.getr(x, y)
                which += self.biome_seed
                which = which % num
                which += 1

                t.set_tilename("key1." + str(which))
                t.wid.bounce_to_pct_in(height=0.1, fade=1.0, ms=500,
                                       count=1000)

            if random.randint(0, 100) < 20:
                if m.is_wall_at(x, y) and \
                        m.is_floor_at(x, y + 1) and \
                        not m.is_wall_at(x, y + 1):
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="deco1")
                    t.push()

                    num = len(t.tp.tiles)
                    which = m.getr(x, y)
                    which += self.biome_seed
                    which = which % num
                    which += 1

                    t.set_tilename("deco1." + str(which))

            if m.is_treasure_at(x, y):
                toughness = -1 * self.where.z

                roomno = m.getr(x, y)
                if roomno != -1:
                    toughness += m.roomno_depth[roomno]

                r = tp.get_random_treasure(toughness=toughness)
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.short_name)
                t.push()

            if m.is_chasm_at(x, y):
                if m.is_floor_at(x, y) or \
                        m.is_wall_at(x, y) or \
                        m.is_cwall_at(x, y) or \
                        m.is_corridor_at(x, y):
                    continue

                if m.is_floor_at(x-1, y) or \
                        m.is_wall_at(x-1, y) or \
                        m.is_cwall_at(x-1, y) or \
                        m.is_corridor_at(x-1, y):
                    continue

                if m.is_floor_at(x+1, y) or \
                        m.is_wall_at(x+1, y) or \
                        m.is_cwall_at(x+1, y) or \
                        m.is_corridor_at(x+1, y):
                    continue

                if m.is_floor_at(x, y-1) or \
                        m.is_wall_at(x, y-1) or \
                        m.is_cwall_at(x, y-1) or \
                        m.is_corridor_at(x, y-1):
                    continue

                if m.is_floor_at(x, y+1) or \
                        m.is_wall_at(x, y+1) or \
                        m.is_cwall_at(x, y+1) or \
                        m.is_corridor_at(x, y+1):
                    continue

                if random.randint(0, 100) < 10:
                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                    tp_name="chasm_smoke2")
                    t.push()

                continue

            if place_stalactite:
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="stalactite1")
                t.push()
