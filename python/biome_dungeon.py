import random
import mm
import thing
import biome_dungeon_do
import rooms
import tp
import game


def biome_build(self, seed=0):
    self.biome_seed = seed
    l = self.level

    #
    # The dungeon takes up all chunks
    #
    if self.cx != mm.CHUNK_ACROSS - 1 or \
       self.cy != mm.CHUNK_DOWN - 1:
        self.biome = None
        return

    seed = l.seed + l.where.z

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
    l = self.level

    if self.biome is None:
        return

    if self.where.z == -1:
        choose_walls(self)
    choose_floors(self)

    for y in range(0, mm.MAP_HEIGHT):
        for x in range(0, mm.MAP_WIDTH):

            tx = x
            ty = y
            (c, ox, oy) = l.xy_to_chunk_xy(tx, ty)

            place_stalactite = False

            if m.is_floor_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty,
                                tp_name=l.floor_name)
                t.set_depth(m.depth_map.cells[x][y])
                t.push()
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="under_floor1")
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
                t = thing.Thing(chunk=c, x=tx, y=ty,
                                tp_name=l.corridor_name)
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
                t = thing.Thing(chunk=c, x=tx, y=ty,
                                tp_name=l.wall_name)
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
                    t.set_tilename(l.wall_x_name)
                elif b and d and f:
                    t.set_tilename(l.wall_t180_name)
                elif b and d and h:
                    t.set_tilename(l.wall_t90_name)
                elif b and f and h:
                    t.set_tilename(l.wall_t270_name)
                elif d and f and h:
                    t.set_tilename(l.wall_t_name)
                elif b and h:
                    t.set_tilename(l.wall_up_down_name)
                elif d and f:
                    t.set_tilename(l.wall_left_right_name)
                elif b and f:
                    t.set_tilename(l.wall_l_name)
                elif h and f:
                    t.set_tilename(l.wall_l90_name)
                elif d and h:
                    t.set_tilename(l.wall_l180_name)
                elif b and d:
                    t.set_tilename(l.wall_l270_name)
                elif b:
                    t.set_tilename(l.wall_n180_name)
                elif f:
                    t.set_tilename(l.wall_n270_name)
                elif h:
                    t.set_tilename(l.wall_n_name)
                elif d:
                    t.set_tilename(l.wall_n90_name)
                else:
                    t.set_tilename(l.wall_node_name)

                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="under_floor1")
                t.push()

            if m.is_cwall_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty,
                                tp_name=l.cwall_name)
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
                    t.set_tilename(l.cwall_x_name)
                elif b and d and f:
                    t.set_tilename(l.cwall_t180_name)
                elif b and d and h:
                    t.set_tilename(l.cwall_t90_name)
                elif b and f and h:
                    t.set_tilename(l.cwall_t270_name)
                elif d and f and h:
                    t.set_tilename(l.cwall_t_name)
                elif b and h:
                    t.set_tilename(l.cwall_up_down_name)
                elif d and f:
                    t.set_tilename(l.cwall_left_right_name)
                elif b and f:
                    t.set_tilename(l.cwall_l_name)
                elif h and f:
                    t.set_tilename(l.cwall_l90_name)
                elif d and h:
                    t.set_tilename(l.cwall_l180_name)
                elif b and d:
                    t.set_tilename(l.cwall_l270_name)
                elif b:
                    t.set_tilename(l.cwall_n180_name)
                elif f:
                    t.set_tilename(l.cwall_n270_name)
                elif h:
                    t.set_tilename(l.cwall_n_name)
                elif d:
                    t.set_tilename(l.cwall_n90_name)
                else:
                    t.set_tilename(l.cwall_node_name)

                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="under_floor1")
                t.push()

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

            if m.is_rock_at(x, y):
                place_stalactite = True
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="rock")
                t.push()

                if random.randint(0, 100) < 5:
                    toughness = m.depth_map.cells[x][y]
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.short_name)
                    t.push()

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


def choose_walls(self):

    l = self.level
    l.wall = tp.get_random_wall()
    l.wall_name = l.wall.name

    l.wall_x_name = l.wall_name + "_x"
    l.wall_t180_name = l.wall_name + "_t180"
    l.wall_t90_name = l.wall_name + "_t90"
    l.wall_t270_name = l.wall_name + "_t270"
    l.wall_t_name = l.wall_name + "_t"
    l.wall_up_down_name = l.wall_name + "_up_down"
    l.wall_left_right_name = l.wall_name + "_left_right"
    l.wall_l_name = l.wall_name + "_l"
    l.wall_l90_name = l.wall_name + "_l90"
    l.wall_l180_name = l.wall_name + "_l180"
    l.wall_l270_name = l.wall_name + "_l270"
    l.wall_n180_name = l.wall_name + "_n180"
    l.wall_n270_name = l.wall_name + "_n270"
    l.wall_n_name = l.wall_name + "_n"
    l.wall_n90_name = l.wall_name + "_n90"
    l.wall_node_name = l.wall_name + "_node"

    l.cwall = tp.get_random_cwall()
    l.cwall_name = l.cwall.name

    l.cwall_x_name = l.cwall_name + "_x"
    l.cwall_t180_name = l.cwall_name + "_t180"
    l.cwall_t90_name = l.cwall_name + "_t90"
    l.cwall_t270_name = l.cwall_name + "_t270"
    l.cwall_t_name = l.cwall_name + "_t"
    l.cwall_up_down_name = l.cwall_name + "_up_down"
    l.cwall_left_right_name = l.cwall_name + "_left_right"
    l.cwall_l_name = l.cwall_name + "_l"
    l.cwall_l90_name = l.cwall_name + "_l90"
    l.cwall_l180_name = l.cwall_name + "_l180"
    l.cwall_l270_name = l.cwall_name + "_l270"
    l.cwall_n180_name = l.cwall_name + "_n180"
    l.cwall_n270_name = l.cwall_name + "_n270"
    l.cwall_n_name = l.cwall_name + "_n"
    l.cwall_n90_name = l.cwall_name + "_n90"
    l.cwall_node_name = l.cwall_name + "_node"


def choose_floors(self):

    l = self.level
    l.floor = tp.get_random_floor()
    l.floor_name = l.floor.name

    l.corridor = tp.get_random_corridor()
    l.corridor_name = l.corridor.name
