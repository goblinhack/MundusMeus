import random
import mm
import thing
import biome_dungeon_do
import rooms
import tp
import game


def biome_build(self, chunk=None, seed=0):
    self.biome_seed = seed

    while True:
        fixed_rooms = rooms.create_fixed()
        random.seed(self.biome_seed)

        self.biome_seed += 1
        self.biome_seed *= self.biome_seed

        self.chunk.biome = biome_dungeon_do.Biome(chunk=chunk,
                                                  rooms=fixed_rooms,
                                                  rooms_on_chunk=15,
                                                  fixed_room_chance=10)

        if not self.chunk.biome.generate_failed:
            break

#    self.chunk.biome.dump_depth()
#    self.chunk.biome.dump(biome_dungeon_do.charmap.depth.floor)
#    self.chunk.biome.dump()


def biome_populate(self):
    c = self
    m = self.biome

    for y in range(0, mm.CHUNK_HEIGHT):
        for x in range(0, mm.CHUNK_WIDTH):

            tx = x + self.base_x
            ty = y + self.base_y

            place_stalactite = False

            if m.is_floor_at(x, y):
                t = thing.Thing(chunk=c, tp_name="floor1")
                t.set_depth(m.depth_map.cells[x][y])
                t.push(tx, ty)

                if not m.is_wall_at(x, y) and not m.is_cwall_at(x, y):
                    if random.randint(0, 1000) < 2:
                        t = thing.Thing(chunk=c, tp_name="torch1")
                        t.push(tx, ty)

            if random.randint(0, 1000) < 5:
                t = thing.Thing(chunk=c, tp_name="torch1")
                t.push(tx, ty)

            if m.is_dusty_at(x, y):
                t = thing.Thing(chunk=c, tp_name="dusty1")
                t.set_depth(m.bridge_height[x][y])
                t.push(tx, ty)

                if m.bridge_height[x][y] > 0:
                    t = thing.Thing(chunk=c, tp_name="under_dusty1")
                    t.set_depth(m.bridge_height[x][y])
                    t.push(tx, ty + 1)

                    if not m.is_lava_at(x, y) and \
                       not m.is_water_at(x, y):
                        t = thing.Thing(chunk=c, tp_name="rock")
                        t.push(tx, ty)

                if not m.is_wall_at(x, y) and not m.is_cwall_at(x, y):
                    if random.randint(0, 1000) < 2:
                        t = thing.Thing(chunk=c, tp_name="torch1")
                        t.push(tx, ty)

            if m.is_corridor_at(x, y):
                t = thing.Thing(chunk=c, tp_name="corridor1")
                t.set_depth(m.bridge_height[x][y])
                t.push(tx, ty)

                if m.bridge_height[x][y] > 0:
                    t = thing.Thing(chunk=c,
                                    tp_name="under_corridor1")
                    t.set_depth(m.bridge_height[x][y])
                    t.push(tx, ty + 1)

                if not m.is_wall_at(x, y) and not m.is_cwall_at(x, y):
                    if random.randint(0, 1000) < 2:
                        t = thing.Thing(chunk=c, tp_name="torch1")
                        t.push(tx, ty)

            if m.is_start_at(x, y):

                t = thing.Thing(chunk=self, tp_name="start1")
                t.set_depth(m.depth_map.cells[x][y])
                t.push(tx, ty)

                if game.g.player is None:
                    #
                    # Create the player on the active central chunk
                    #
                    if self.cx == 1 and self.cy == 1:
                        t = thing.Thing(chunk=c, tp_name="player1")
                        t.push(tx, ty)
                        game.g.player = t

            if m.is_exit_at(x, y):
                t = thing.Thing(chunk=c, tp_name="exit1")
                t.set_depth(m.depth_map.cells[x][y])
                t.push(tx, ty)

                t = thing.Thing(chunk=c, tp_name="exit1_deco")
                t.set_depth(m.depth_map.cells[x][y])
                t.push(tx, ty - 1)

                t = thing.Thing(chunk=c, tp_name="chasm_smoke1")
                t.push(tx, ty)

                t = thing.Thing(chunk=c, tp_name="chasm_smoke1")
                t.push(tx, ty - 1)

            if m.is_wall_at(x, y):
                place_stalactite = True
                t = thing.Thing(chunk=c, tp_name="wall1")
                t.push(tx, ty)

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
                    t.set_tilename("wall1-x")
                elif b and d and f:
                    t.set_tilename("wall1-t180")
                elif b and d and h:
                    t.set_tilename("wall1-t90")
                elif b and f and h:
                    t.set_tilename("wall1-t270")
                elif d and f and h:
                    t.set_tilename("wall1-t")
                elif b and h:
                    t.set_tilename("wall1-up-down")
                elif d and f:
                    t.set_tilename("wall1-left-right")
                elif b and f:
                    t.set_tilename("wall1-l")
                elif h and f:
                    t.set_tilename("wall1-l90")
                elif d and h:
                    t.set_tilename("wall1-l180")
                elif b and d:
                    t.set_tilename("wall1-l270")
                elif b:
                    t.set_tilename("wall1-n180")
                elif f:
                    t.set_tilename("wall1-n270")
                elif h:
                    t.set_tilename("wall1-n")
                elif d:
                    t.set_tilename("wall1-n90")
                else:
                    t.set_tilename("wall1-node")

            if m.is_cwall_at(x, y):
                t = thing.Thing(chunk=c, tp_name="cwall1")
                t.push(tx, ty)

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
                    t.set_tilename("cwall1-x")
                elif b and d and f:
                    t.set_tilename("cwall1-t180")
                elif b and d and h:
                    t.set_tilename("cwall1-t90")
                elif b and f and h:
                    t.set_tilename("cwall1-t270")
                elif d and f and h:
                    t.set_tilename("cwall1-t")
                elif b and h:
                    t.set_tilename("cwall1-up-down")
                elif d and f:
                    t.set_tilename("cwall1-left-right")
                elif b and f:
                    t.set_tilename("cwall1-l")
                elif h and f:
                    t.set_tilename("cwall1-l90")
                elif d and h:
                    t.set_tilename("cwall1-l180")
                elif b and d:
                    t.set_tilename("cwall1-l270")
                elif b:
                    t.set_tilename("cwall1-n180")
                elif f:
                    t.set_tilename("cwall1-n270")
                elif h:
                    t.set_tilename("cwall1-n")
                elif d:
                    t.set_tilename("cwall1-n90")
                else:
                    t.set_tilename("cwall1-node")

            if m.is_lava_at(x, y):

                if m.is_floor_at(x, y) or \
                   m.is_dusty_at(x, y) or \
                   m.is_corridor_at(x, y):
                    #
                    # Underground lava
                    #
                    t = thing.Thing(chunk=c, tp_name="lava1")
                    t.push(tx, ty)

                elif m.is_wall_at(x, y - 1) or \
                        m.is_rock_at(x, y - 1) or \
                        m.is_cwall_at(x, y - 1):

                    t = thing.Thing(chunk=c, tp_name="lava1-top")
                    t.push(tx, ty)

                elif m.is_floor_at(x, y - 1) and not m.is_floor_at(x, y):
                    t = thing.Thing(chunk=c, tp_name="lava1-top")
                    t.push(tx, ty)
                    t = thing.Thing(chunk=c, tp_name="lava1")
                    t.push(tx, ty)

                elif m.is_corridor_at(x, y - 1) and \
                        not m.is_corridor_at(x, y):
                    t = thing.Thing(chunk=c, tp_name="lava1-top")
                    t.push(tx, ty)
                    t = thing.Thing(chunk=c, tp_name="lava1")
                    t.push(tx, ty)

                elif m.is_dusty_at(x, y - 1) and \
                        not m.is_dusty_at(x, y):
                    t = thing.Thing(chunk=c, tp_name="lava1-top")
                    t.push(tx, ty)
                    t = thing.Thing(chunk=c, tp_name="lava1")
                    t.push(tx, ty)

                else:
                    t = thing.Thing(chunk=c, tp_name="lava1")
                    t.push(tx, ty)

                t.set_depth(m.depth_map.cells[x][y])

                if random.randint(0, 100) < 30:
                    t = thing.Thing(chunk=c, tp_name="chasm_smoke2")
                    t.push(tx, ty)

                if random.randint(0, 100) < 5:
                    toughness = m.depth_map.cells[x][y] * 20
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(chunk=c, tp_name=r.short_name)
                    t.push(tx, ty)

            if m.is_water_at(x, y):

                water = "water1"
                put_treasure = False

                if random.randint(0, 100) < 5:
                    put_treasure = True
                    water = "water1-trans"

                if m.is_floor_at(x, y) or \
                   m.is_dusty_at(x, y) or \
                   m.is_corridor_at(x, y):
                    #
                    # Underground water
                    #
                    t = thing.Thing(chunk=c, tp_name=water)
                    t.push(tx, ty)

                elif m.is_wall_at(x, y - 1) or \
                        m.is_rock_at(x, y - 1) or \
                        m.is_cwall_at(x, y - 1):
                    t = thing.Thing(chunk=c, tp_name=water + "-top")
                    t.push(tx, ty)

                elif m.is_floor_at(x, y - 1) and not m.is_floor_at(x, y):
                    t = thing.Thing(chunk=c, tp_name=water + "-top")
                    t.push(tx, ty)

                elif m.is_corridor_at(x, y - 1) and \
                        not m.is_corridor_at(x, y):
                    t = thing.Thing(chunk=c, tp_name=water + "-top")
                    t.push(tx, ty)

                elif m.is_dusty_at(x, y - 1) and \
                        not m.is_dusty_at(x, y):
                    t = thing.Thing(chunk=c, tp_name=water + "-top")
                    t.push(tx, ty)

                else:
                    t = thing.Thing(chunk=c, tp_name=water)
                    t.push(tx, ty)

                t.set_depth(m.depth_map.cells[x][y])

                if put_treasure:
                    toughness = m.depth_map.cells[x][y] * 2
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(chunk=c, tp_name=r.short_name)
                    t.push(tx, ty)

                if random.randint(0, 100) < 5:
                    t = thing.Thing(chunk=c, tp_name="chasm_smoke2")
                    t.push(tx, ty)

            if m.is_rock_at(x, y):
                place_stalactite = True
                t = thing.Thing(chunk=c, tp_name="rock")
                t.push(tx, ty)

                if random.randint(0, 100) < 5:
                    toughness = m.depth_map.cells[x][y]
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(chunk=c, tp_name=r.short_name)
                    t.push(tx, ty)

            if m.is_door_at(x, y):
                t = thing.Thing(chunk=c, tp_name="door1")
                t.push(tx, ty)

                num = len(t.tp.tiles)
                which = m.getr(x, y)
                which += self.biome_seed
                which = which % num
                which += 1

                t.set_tilename("door1." + str(which))

            if m.is_key_at(x, y):
                t = thing.Thing(chunk=c, tp_name="key1")
                t.push(tx, ty)

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
                    t = thing.Thing(chunk=c, tp_name="deco1")
                    t.push(tx, ty)

                    num = len(t.tp.tiles)
                    which = m.getr(x, y)
                    which += self.biome_seed
                    which = which % num
                    which += 1

                    t.set_tilename("deco1." + str(which))

            if m.is_treasure_at(x, y):
                toughness = self.chunk.xyz.z

                roomno = m.getr(x, y)
                if roomno != -1:
                    toughness += m.roomno_depth[roomno]

                r = tp.get_random_treasure(toughness=toughness)
                t = thing.Thing(chunk=c, tp_name=r.short_name)
                t.push(tx, ty)

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
                    t = thing.Thing(chunk=c, tp_name="chasm_smoke2")
                    t.push(tx, ty)

                continue

            if place_stalactite:
                t = thing.Thing(chunk=c, tp_name="stalactite1")
                t.push(tx, ty)
