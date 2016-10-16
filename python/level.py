import pickle
import traceback
import mm
import dmap


class Level:

    def __init__(self, world, xyz):
        self.world = world
        self.xyz = xyz
        self.all_things = {}

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

        with open(str(self.world) + str(self), 'wb') as f:
            pickle.dump(self, f, pickle.HIGHEST_PROTOCOL)

    def set_dim(self, width, height):
        self.width = width
        self.height = height

        self.on_map = [[[] for x in range(width)] for y in range(height)]
        self.dmaps = [[None for x in range(width)] for y in range(height)]

    def tp_find(self, x, y, tp_name):
        if x >= self.width or y >= self.height or x < 0 or y < 0:
            return None

        for t in self.on_map[x][y]:
            if t.tp.name == tp_name:
                return t

        return None

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
                       t.tp.is_cwall:
                        skip = True
                        break

                if skip:
                    continue

                for t in self.on_map[x][y]:
                    if t.tp.is_floor or \
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

            for dx, dy, cost in ALL_DELTAS:
                tx = x + dx
                ty = y + dy

                if walked[tx][ty]:
                    continue

                c = cells[tx][ty] * cost
                if c <= lowest:
                    got = True
                    bx = tx
                    by = ty
                    lowest = c

            if not got:
                self.dmap_path_debug(d, out_path)
                return out_path

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
