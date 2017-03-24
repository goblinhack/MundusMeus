import random
import mm
import thing
import biome_dungeon_do
import rooms
import tp
import game
import os
import pickle


def biome_build(self, seed=0):
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

    if True:
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

            if m.is_floor_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty,
                                tp_name=l.floor_name)

                t.set_depth(m.depth_map.cells[x][y])
                t.push()

                if random.randint(0, 1000) < 2:
                    if not m.is_wall_at(x, y) and not m.is_cwall_at(x, y):
                        t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="torch1")
                        t.push()

                if random.randint(0, 1000) < 5:
                    if not m.is_wall_at(x, y) \
                       and not m.is_wall_at(x - 1, y) \
                       and not m.is_wall_at(x + 1, y) \
                       and not m.is_wall_at(x, y - 1) \
                       and not m.is_wall_at(x, y + 1) \
                       and not m.is_door_at(x, y) \
                       and not m.is_door_at(x - 1, y) \
                       and not m.is_door_at(x + 1, y) \
                       and not m.is_door_at(x, y - 1) \
                       and not m.is_door_at(x, y + 1) \
                       and not m.is_cwall_at(x, y) \
                       and not m.is_cwall_at(x - 1, y) \
                       and not m.is_cwall_at(x + 1, y) \
                       and not m.is_cwall_at(x, y - 1) \
                       and not m.is_cwall_at(x, y + 1):
                        t = thing.Thing(chunk=c, x=tx, y=ty,
                                        tp_name="stalacmite1")
                        t.push()

            if m.is_dusty_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="dusty1")
                t.set_depth(m.bridge_height[x][y])
                t.push()

                if m.bridge_height[x][y] > 0:
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

                if m.bridge_height[x][y] == 0:
                    if m.is_water_at(x, y + 1) or m.is_water_at(x, y):
                        t.set_depth(0.5)

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

            if m.is_wall_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty,
                                tp_name=l.wall_name)
                t.push()

            if m.is_cwall_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty,
                                tp_name=l.cwall_name)
                t.push()

            if m.is_lava_at(x, y):

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
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                    t.push()

            if m.is_water_at(x, y):

                water = "water1"
                put_treasure = False

                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=water)
                t.push()

                t.set_depth(m.depth_map.cells[x][y])

                if put_treasure:
                    toughness = m.depth_map.cells[x][y] * 2
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                    t.push()

            if m.is_rock_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="rock")
                t.push()

                if random.randint(0, 100) < 5:
                    toughness = m.depth_map.cells[x][y]
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                    t.push()

            if m.is_door_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty,
                                tp_name=tp.get_random_door().name)
                t.push()

            if m.is_key_at(x, y):
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="key1")
                t.push()

                num = len(t.tp.tiles)
                which = m.getr(x, y)
                which += self.biome_seed
                which = which % num
                which += 1

                t.set_tilename("key1." + str(which))

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
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
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

                if random.randint(0, 1000) < 10:
                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                    tp_name="chasm_smoke2")
                    t.push()

                continue


def choose_walls(self):

    l = self.level
    wall = tp.get_random_wall()
    l.wall_name = wall.name

    cwall = tp.get_random_cwall()
    l.cwall_name = cwall.name


def choose_floors(self):

    l = self.level
    floor = tp.get_random_floor()
    l.floor_name = floor.name

    l.corridor = tp.get_random_corridor()
    l.corridor_name = l.corridor.name


def biome_save(self):

    #
    # Only save/load on the last chunk on the level grid
    # as this one chunk holds all the data for the level.
    #
    if self.cx != mm.CHUNK_ACROSS - 1 or \
       self.cy != mm.CHUNK_DOWN - 1:
        return

    self.log("Biome save")

    l = self.level

    with open(os.path.normcase(
                os.path.join(os.environ["APPDATA"],
                             self.biome_name)), 'wb') as f:

        pickle.dump(l.wall_name, f, pickle.HIGHEST_PROTOCOL)
        pickle.dump(l.cwall_name, f, pickle.HIGHEST_PROTOCOL)
        pickle.dump(l.floor_name, f, pickle.HIGHEST_PROTOCOL)
        pickle.dump(l.corridor, f, pickle.HIGHEST_PROTOCOL)
        pickle.dump(l.corridor_name, f, pickle.HIGHEST_PROTOCOL)


def biome_load(self):

    #
    # Only save/load on the last chunk on the level grid
    # as this one chunk holds all the data for the level.
    #
    if self.cx != mm.CHUNK_ACROSS - 1 or \
       self.cy != mm.CHUNK_DOWN - 1:
        return

    self.log("Biome load")

    l = self.level

    with open(os.path.normcase(
                os.path.join(os.environ["APPDATA"],
                             self.biome_name)), 'rb') as f:
        l.wall_name = pickle.load(f)
        l.cwall_name = pickle.load(f)
        l.floor_name = pickle.load(f)
        l.corridor = pickle.load(f)
        l.corridor_name = pickle.load(f)
