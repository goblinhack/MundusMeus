import pickle
import traceback
import mm
import dmap


class Level:

    def __init__(self, game, xyz):
        self.game = game
        self.xyz = xyz
        self.all_things = {}
        self.is_biome_land = False
        self.is_biome_dungeon = False

    def __str__(self):
        return "l{0}".format(str(self.xyz))

    def destroy(self):
        self.log("Destroying level {")
        for key, value in self.all_things.items():
            value.destroy()

        self.all_things = {}
        self.log("} Destroyed level")
        del self

    def log(self, msg):
        mm.log("p-level: {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.err("p-level: {0}: ERROR: {1}".format(str(self), msg))
        traceback.print_stack()

    def dump(self):
        for i in self.all_things:
            self.all_things[i].dump()

    def save(self):
        self.log("Save level")

        with open(str(self), 'wb') as f:
            pickle.dump(self, f, pickle.HIGHEST_PROTOCOL)

    def set_dim(self, width, height):
        self.width = width
        self.height = height

        self.on_map = [[[] for x in range(width)] for y in range(height)]
        self.dmaps = [[None for x in range(width)] for y in range(height)]

    def set_biome(self, is_land=False, is_dungeon=False):
        self.is_biome_land = is_land
        self.is_biome_dungeon = is_dungeon

    def tp_find(self, x, y, tp_name):
        if x >= self.width or y >= self.height or x < 0 or y < 0:
            return None

        for t in self.on_map[x][y]:
            if t.tp.name == tp_name:
                return t

        return None

    def is_movement_blocking_at(self, x, y):
        if x >= self.width or y >= self.height or x < 0 or y < 0:
            return False

        for t in self.on_map[x][y]:
            if t.tp.is_movement_blocking:
                return True

        return False

    def thing_push(self, x, y, t):
        if x >= self.width or y >= self.height or x < 0 or y < 0:
            mm.err("thing_push: map oob {0} {1}".format(x, y))
            return

        self.on_map[x][y].append(t)

    def dmap_create(self, px, py):

        d = dmap.Dmap(width=self.width, height=self.height)

        self.dmaps[px][py] = d

        for y in range(self.height):
            for x in range(self.width):
                d.cells[x][y] = dmap.WALL

        for y in range(self.height):
            for x in range(self.width):
                skip = False
                for t in self.on_map[x][y]:
                    if t.tp.is_wall or \
                       t.tp.is_rock or \
                       t.tp.is_door or \
                       t.tp.is_landrock or \
                       t.tp.is_cwall:
                        skip = True
                        break

                if skip:
                    continue

                for t in self.on_map[x][y]:
                    if t.tp.is_floor or \
                       t.tp.is_grass or \
                       t.tp.is_dirt or \
                       t.tp.is_snow or \
                       t.tp.is_ice or \
                       t.tp.is_gravel or \
                       t.tp.is_gravel_snow or \
                       t.tp.is_snow or \
                       t.tp.is_sand or \
                       t.tp.is_corridor or \
                       t.tp.is_water:
                        d.cells[x][y] = dmap.FLOOR
                        break

        d.cells[px][py] = 0
        d.process()

    def dmap_solve(self, ex, ey, sx, sy):

        if self.dmaps[ex][ey] is None:
            self.dmap_create(ex, ey)

        walked = [[0 for i in range(self.height)]
                  for j in range(self.width)]

        out_path = []
        x = sx
        y = sy
        d = self.dmaps[ex][ey]
        cells = d.cells
        out_path.append((x, y))

        while True:
            ALL_DELTAS = [(-1, -1, 1.5),
                          (0, -1, 1.0),
                          (1, -1, 1.5),
                          (-1, 0, 1.0),
                          (1, 0, 1.0),
                          (-1, 1, 1.5),
                          (0, 1, 1.0),
                          (1, 1, 1.5)]

            lowest = cells[x][y]
            got = False
            bx = -1
            by = -1

            if cells[x][y] == dmap.WALL:
                return out_path

            for dx, dy, cost in ALL_DELTAS:
                tx = x + dx
                ty = y + dy

                if tx >= self.width or ty >= self.height or tx < 0 or ty < 0:
                    continue

                if walked[tx][ty]:
                    continue

                if cells[tx][ty] == dmap.WALL:
                    continue

                c = cells[tx][ty] * cost
                if c <= lowest:
                    got = True
                    bx = tx
                    by = ty
                    lowest = c

            if not got:
                return self.dmap_path_optimize(out_path)

            out_path.append((bx, by))
            x = bx
            y = by
            walked[bx][by] = 1

    def dmap_path_debug(self, d, path):

        d.debug = [[0 for i in range(self.height)]
                   for j in range(self.width)]

        for p in path:
            x, y = p
            d.debug[x][y] = 1

        d.dump()

    #
    # Make L shaped moves into diagonal ones
    #
    def dmap_path_optimize(self, path):

        while True:
            modified = False
            i = 0
            while True:
                if i + 2 >= len(path):
                    break

                px, py = path[i]
                nx, ny = n = path[i + 1]
                mx, my = path[i + 2]

                i = i + 1

                if px - 1 == mx and py + 1 == my and \
                   px == nx and py + 1 == ny and \
                   not self.is_movement_blocking_at(px - 1, py):
                    path.remove(n)
                    modified = True
                    break

                if px - 1 == mx and py + 1 == my and \
                   px - 1 == nx and py == ny and \
                   not self.is_movement_blocking_at(px, py + 1):
                    path.remove(n)
                    modified = True
                    break

                if px + 1 == mx and py + 1 == my and \
                   px == nx and py + 1 == ny and \
                   not self.is_movement_blocking_at(px + 1, py):
                    path.remove(n)
                    modified = True
                    break

                if px + 1 == mx and py + 1 == my and \
                   px + 1 == nx and py == ny and \
                   not self.is_movement_blocking_at(px, py + 1):
                    path.remove(n)
                    modified = True
                    break

                if px - 1 == mx and py - 1 == my and \
                   px == nx and py - 1 == ny and \
                   not self.is_movement_blocking_at(px - 1, py):
                    path.remove(n)
                    modified = True
                    break

                if px - 1 == mx and py - 1 == my and \
                   px - 1 == nx and py == ny and \
                   not self.is_movement_blocking_at(px, py - 1):
                    path.remove(n)
                    modified = True
                    break

                if px + 1 == mx and py - 1 == my and \
                   px == nx and py - 1 == ny and \
                   not self.is_movement_blocking_at(px + 1, py):
                    path.remove(n)
                    modified = True
                    break

                if px + 1 == mx and py - 1 == my and \
                   px + 1 == nx and py == ny and \
                   not self.is_movement_blocking_at(px, py - 1):
                    path.remove(n)
                    modified = True
                    break

            if not modified:
                break

        return (path)
