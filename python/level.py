import traceback
import mm
import dmap
import math
import game
import chunk
import util


class Level:

    def __init__(self, xyz):
        self.xyz = xyz

        #
        # This is where the active chunk is. The active chunk is surrounded
        # by other chunks that although things can move in them, it will not
        # contain the player
        #
        where = util.Xyz(0, 0, 0)

        #
        # All things in all chunks. We have per chunk all_things too.
        #
        self.all_things = {}

        #
        # Create all chunks
        #
        self.chunk = [[None for x in range(mm.CHUNK_WIDTH)]
                      for y in range(mm.CHUNK_HEIGHT)]

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                where.x = xyz.x - 1 + cx
                where.y = xyz.y - 1 + cy
                where.z = xyz.z
                self.chunk[cx][cy] = chunk.Chunk(self, where, cx, cy)

        self.active_chunk = self.chunk[1][1]

        #
        # The dmap spans all chunks so we can have things move between chunks
        #
        self.dmaps = [[None for x in range(mm.MAP_WIDTH)]
                      for y in range(mm.MAP_HEIGHT)]

    def scroll(self, dx, dy, dz):

        mm.con("Level scroll dx {0} dy {1} dz {2}".format(dx, dy, dz))

        game.g.map_clear_focus()

        #
        # Get rid of chunks that fall off the map
        #
        odx = dx
        ody = dy

        cdx = -1 * dx
        cdy = -1 * dy

        new_chunk = [[None for x in range(mm.CHUNK_WIDTH)]
                     for y in range(mm.CHUNK_HEIGHT)]

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                x = cx + cdx
                y = cy + cdy
                if x < 0 or y < 0 or \
                        x >= mm.CHUNK_ACROSS or \
                        y >= mm.CHUNK_DOWN:

                    c = self.chunk[cx][cy]
                    mm.con("Chunk {0} scrolled off".format(c))
                    c.save()
                    c.destroy()
                else:
                    new_chunk[x][y] = self.chunk[cx][cy]

        self.chunk = new_chunk

        self.active_chunk = self.chunk[1][1]

        mm.game_scroll_chunk(dx, dy)

        #
        # Move things so they will align with where their chunk moved
        #
        dx *= mm.CHUNK_WIDTH
        dy *= mm.CHUNK_HEIGHT
        dx *= -1
        dy *= -1

        for thing_id in list(self.all_things.keys()):
            t = self.all_things[thing_id]
            if t is None:
                mm.die("thing id {0} not found during scrolling".format(
                    thing_id))

            t.update_pos(t.x + dx, t.y + dy)

        #
        # Add new chunks that scrolled into the map
        #
        self.xyz.x += odx
        self.xyz.y += ody

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                if self.chunk[cx][cy] is None:
                    where = util.Xyz(0, 0, 0)
                    where.x = self.xyz.x - 1 + cx
                    where.y = self.xyz.y - 1 + cy
                    where.z = self.xyz.z
                    self.chunk[cx][cy] = chunk.Chunk(self, where, cx, cy)
                    c = self.chunk[cx][cy]
                    mm.con("Chunk {0} scrolled on".format(c))

                c = self.chunk[cx][cy]
                c.cx = cx
                c.cy = cy
                c.base_x = cx * mm.CHUNK_WIDTH
                c.base_y = cy * mm.CHUNK_HEIGHT

        for place, item in enumerate(game.g.player.nexthops):
            (x, y) = item
            x += dx
            y += dy
            item = (x, y)
            game.g.player.nexthops[place] = item

        for place, item in enumerate(game.g.saved_nexthops):
            (x, y) = item
            x += dx
            y += dy
            item = (x, y)
            game.g.saved_nexthops[place] = item

        game.g.load_level_finalize()
        game.g.player_location_update()

    #
    # Convert from co-ordinates that are the width of all chunks to chunk
    # local co-ords
    #
    def xy_to_chunk_xy(self, x, y):

        cx = (int)(x / mm.CHUNK_WIDTH)
        cy = (int)(y / mm.CHUNK_HEIGHT)
        offset_x = (int)(x % mm.CHUNK_WIDTH)
        offset_y = (int)(y % mm.CHUNK_HEIGHT)

        return (self.chunk[cx][cy], offset_x, offset_y)

    #
    # Convert from co-ordinates that are the chunc-local
    #
    def chunk_xy_to_xy(self, cx, cy, x, y):

        return (x + cx * mm.CHUNK_WIDTH, y + cy * mm.CHUNK_HEIGHT)

    def __str__(self):
        return "l{0}".format(str(self.xyz))

    def destroy(self):
        self.debug("Destroying level {")

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                self.chunk[cx][cy].destroy()

        self.debug("} Destroyed level")
        del self

    def tick(self):

        s = math.sin(game.g.move_count / (math.pi * 11))
        if self.active_chunk.is_snowy:
            if s > 0:
                mm.game_set_snow_amount(int(s * 100))
            else:
                mm.game_set_snow_amount(0)

        if self.active_chunk.is_grassy or self.active_chunk.is_watery:
            if s > 0:
                mm.game_set_rain_amount(int(s * 100))
            else:
                mm.game_set_rain_amount(0)

    def log(self, msg):
        mm.log("p-level: {0}: {1}".format(str(self), msg))

    def debug(self, msg):
        return
        mm.log("p-level: {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.err("p-level: {0}: ERROR: {1}".format(str(self), msg))
        traceback.print_stack()

    def dump(self):
        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                self.chunk[cx][cy].dump()

    def save(self):
        self.debug("Save level")

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                self.chunk[cx][cy].save()

    def load(self):
        self.debug("Load level")

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                self.chunk[cx][cy].save()

    def tp_find(self, x, y, tp_name):
        if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or x < 0 or y < 0:
            return None

        (chunk, x, y) = self.xy_to_chunk_xy(x, y)
        return chunk.tp_find(x, y, tp_name)

    def tp_is(self, x, y, value):
        if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or x < 0 or y < 0:
            return None

        (chunk, x, y) = self.xy_to_chunk_xy(x, y)
        return chunk.tp_is(x, y, value)

    def tp_is_where(self, value):
        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                where = self.chunk[cx][cy].is_where(value)
                if where is not None:
                    (where.x, where.y) = \
                        self.chunk_xy_to_xy(cx, cy, where.x, where.y)
                    return where
        return None

    def is_movement_blocking_at(self, x, y):
        (chunk, ox, oy) = self.xy_to_chunk_xy(x, y)
        if chunk.is_movement_blocking_at(ox, oy):
            return True
        return False

    def dmap_create(self, px, py):

        d = dmap.Dmap(width=mm.MAP_WIDTH, height=mm.MAP_HEIGHT)

        self.dmaps[px][py] = d

        for y in range(mm.MAP_HEIGHT):
            for x in range(mm.MAP_WIDTH):
                d.cells[x][y] = dmap.WALL

        for y in range(mm.MAP_HEIGHT):
            for x in range(mm.MAP_WIDTH):
                (chunk, ox, oy) = self.xy_to_chunk_xy(x, y)

                skip = False
                for t in chunk.on_map[ox][oy]:
                    if t.tp.is_wall or \
                       t.tp.is_rock or \
                       t.tp.is_door or \
                       t.tp.is_landrock or \
                       t.tp.is_cwall:
                        skip = True
                        break

                if skip:
                    continue

                for t in chunk.on_map[ox][oy]:
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

        walked = [[0 for i in range(mm.MAP_HEIGHT)]
                  for j in range(mm.MAP_WIDTH)]

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

                if tx >= mm.MAP_WIDTH or ty >= mm.MAP_HEIGHT or \
                   tx < 0 or ty < 0:
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

        d.debug = [[0 for i in range(mm.MAP_HEIGHT)]
                   for j in range(mm.MAP_WIDTH)]

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
