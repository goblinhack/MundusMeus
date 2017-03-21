import traceback
import mm
import dmap
import math
import game
import chunk
import util
import thing
import sys


class Level:

    def __init__(self, xyz, seed):

        self.where = xyz

        #
        # This is where the active chunk is. The active chunk is surrounded
        # by other chunks that although things can move in them, it will not
        # contain the player
        #
        where = util.Xyz(0, 0, 0)
        self.seed = seed

        #
        # All things in all chunks. We have per chunk all_things too.
        #
        self.all_things = {}

        #
        # Create all active chunks. These are the ones displayed on the map
        # all the time.
        #
        self.chunk = [[None for x in range(mm.CHUNK_ACROSS)]
                      for y in range(mm.CHUNK_DOWN)]
        #
        # We try to avoid saving chunks when they scroll off the map for speed
        # and so store them in a cache.
        #
        self.chunk_cache = {}

        #
        # Create all active chunks.
        #
        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                where.x = xyz.x - 1 + cx
                where.y = xyz.y - 1 + cy
                where.z = xyz.z
                self.chunk[cx][cy] = chunk.Chunk(self, where,
                                                 self.seed,
                                                 cx, cy)

        #
        # The dmap spans all chunks so we can have things move between chunks
        #
        self.dmap_reset()

        if game.g.player is None:
            raise NameError("No player found on any chunk")

    def scroll(self, dx, dy):

        #
        # Scroll only on the land
        #
        if not self.chunk[0][0].is_biome_land:
            return

        self.log("Scroll dx {0}, dy {1}".format(dx, dy))

        game.g.map_clear_focus()

        #
        # Get rid of chunks that fall off the map
        #
        odx = dx
        ody = dy

        cdx = -1 * dx
        cdy = -1 * dy

        new_chunk = [[None for x in range(mm.CHUNK_ACROSS)]
                     for y in range(mm.CHUNK_DOWN)]

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                x = cx + cdx
                y = cy + cdy

                if x < 0 or y < 0 or \
                        x >= mm.CHUNK_ACROSS or \
                        y >= mm.CHUNK_DOWN:

                    c = self.chunk[cx][cy]
                    self.debug("Chunk {0}: Scrolled off of map".format(c))
                    c.scrolled_off()
                else:
                    new_chunk[x][y] = self.chunk[cx][cy]

        self.chunk = new_chunk

        mm.game_scroll_chunk(dx, dy)

        #
        # Move things so they will align with where their chunk moved
        #
        dx *= mm.CHUNK_WIDTH
        dy *= mm.CHUNK_HEIGHT
        dx *= -1
        dy *= -1

        for thing_id, t in self.all_things.items():
            t.update_pos(t.x + dx, t.y + dy)

        #
        # Add new chunks that scrolled into the map
        #
        self.where.x += odx
        self.where.y += ody
        self.where.x %= mm.WORLD_WIDTH
        self.where.y %= mm.WORLD_HEIGHT

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                if self.chunk[cx][cy] is None:
                    where = util.Xyz(0, 0, 0)
                    where.x = self.where.x - 1 + cx
                    where.y = self.where.y - 1 + cy
                    where.z = self.where.z

                    where.x %= mm.WORLD_WIDTH
                    where.y %= mm.WORLD_HEIGHT

                    chunk_name = "level.{0}.seed.{1}".format(str(where),
                                                             self.seed)
                    c = self.chunk_cache.get(chunk_name)
                    if c is None:
                        self.chunk[cx][cy] = chunk.Chunk(self, where,
                                                         self.seed,
                                                         cx, cy)
                        c = self.chunk[cx][cy]
                    else:
                        self.chunk[cx][cy] = c
                        c.load(cx, cy)

                    self.debug("Chunk {0}: Scrolled onto map".format(c))

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

        self.dmap_reset()

    def jump(self, to, seed, backtracking=False):

        self.log("Jump to {0}".format(to))
        self.seed = seed

        game.g.map_clear_focus()

        #
        # Preserve the player
        #
        player = game.g.player
        game.g.player.pop()

        #
        # Get rid of all chunks
        #
        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                c = self.chunk[cx][cy]
                self.log("Chunk {0}: Jumped off of map".format(c))
                c.scrolled_off()

        self.chunk = [[None for x in range(mm.CHUNK_WIDTH)]
                      for y in range(mm.CHUNK_HEIGHT)]
        #
        # Add new chunks
        #
        self.where = to

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                if self.chunk[cx][cy] is None:
                    where = util.Xyz(0, 0, 0)
                    where.x = self.where.x - 1 + cx
                    where.y = self.where.y - 1 + cy
                    where.z = self.where.z

                    chunk_name = "level.{0}.seed.{1}".format(str(where), seed)
                    c = self.chunk_cache.get(chunk_name)
                    if c is None:
                        self.chunk[cx][cy] = chunk.Chunk(self, where,
                                                         seed,
                                                         cx, cy)
                        c = self.chunk[cx][cy]
                    else:
                        self.chunk[cx][cy] = c
                        c.load(cx, cy)

                    self.debug("Chunk {0}: Jumped onto map".format(c))

                c = self.chunk[cx][cy]
                c.cx = cx
                c.cy = cy
                c.base_x = cx * mm.CHUNK_WIDTH
                c.base_y = cy * mm.CHUNK_HEIGHT

        new_pos = None
        if backtracking:
            #
            # Heading back to the surface. Trace our steps back.
            #
            if len(game.g.level_stack) > 0:
                new_pos = game.g.level_stack.pop()
            else:
                self.err("No level stack to backtrack to")
        else:
            #
            # Not backtracking and going into the unknown.
            # Try to find a suitable starting point.
            #
            entrance = self.tp_is_where("is_dungeon_way_up")
            if entrance is not None:
                new_pos = entrance

            game.g.level_stack.append((player.x, player.y))

        if new_pos is not None:
            (x, y) = new_pos
            game.g.player.push(x, y)
        else:
            self.err("Don't know where to put the player on new level")

        game.g.player.nexthops = []
        game.g.saved_nexthops = []

        game.g.load_level_finalize()
        game.g.player_location_update()

        self.dmap_reset()

    #
    # Convert from co-ordinates that are the width of all chunks to chunk
    # local co-ords
    #
    def xy_to_chunk_xy(self, x, y):

        cx = (int)(x / mm.CHUNK_WIDTH)
        cy = (int)(y / mm.CHUNK_HEIGHT)
        offset_x = (int)(x % mm.CHUNK_WIDTH)
        offset_y = (int)(y % mm.CHUNK_HEIGHT)

        if self.chunk[cx][cy] is None:
            self.die("Co-ord {0} {1} maps to no known chunk".format(
                     x, y))

        return (self.chunk[cx][cy], offset_x, offset_y)

    #
    # Convert from co-ordinates that are the chunc_local
    #
    def chunk_xy_to_xy(self, cx, cy, x, y):

        return (x + cx * mm.CHUNK_WIDTH, y + cy * mm.CHUNK_HEIGHT)

    def log(self, msg):
        mm.log("Level {0}: {1}".format(str(self), msg))

    def con(self, msg):
        mm.con("Level {0}: {1}".format(str(self), msg))

    def debug(self, msg):
        return
        mm.log("Level {0}: {1}".format(str(self), msg))

    def err(self, msg):
        mm.con("".join(traceback.format_stack()))
        mm.err("Level {0}: ERROR: {1}".format(self, msg))

    def die(self, msg):
        mm.con("".join(traceback.format_stack()))
        mm.die("Level {0}: FATAL ERROR: {1}".format(self, msg))

    def __str__(self):
        return "level.{0}".format(str(self.where))

    def destroy(self):
        self.log("Destroying level {")

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                self.chunk[cx][cy].destroy()

        self.log("Destroy cached levels")
        for chunk_name, c in list(self.chunk_cache.items()):
            c.destroy()

        self.log("} Destroyed level")
        del self

    def tick(self):

        s = math.sin(game.g.move_count / (math.pi * 11))
        if game.g.player.chunk.is_snowy:
            if s > 0:
                mm.game_set_snow_amount(int(s * 100))
            else:
                mm.game_set_snow_amount(0)

        if game.g.player.chunk.is_grassy or game.g.player.chunk.is_watery:
            if s > 0:
                mm.game_set_rain_amount(int(s * 100))
            else:
                mm.game_set_rain_amount(0)

    def dump(self):

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                self.chunk[cx][cy].dump()

    def save(self, f):

        self.log("Save level")

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                self.chunk[cx][cy].save()

        self.log("Save cached levels")
        for chunk_name, c in self.chunk_cache.items():
            c.save()

    def thing_find(self, x, y, tp_name):

        if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or x < 0 or y < 0:
            return None

        (chunk, x, y) = self.xy_to_chunk_xy(x, y)
        return chunk.thing_find(x, y, tp_name)

    def thing_find_same_type(self, x, y, tp_name):

        if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or x < 0 or y < 0:
            return None

        (chunk, x, y) = self.xy_to_chunk_xy(x, y)
        return chunk.thing_find_same_type(x, y, tp_name)

    def thing_top(self, x, y):

        if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or x < 0 or y < 0:
            return None

        (chunk, x, y) = self.xy_to_chunk_xy(x, y)
        return chunk.thing_top(x, y)

    def things_at(self, x, y):

        if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or x < 0 or y < 0:
            return []

        (chunk, x, y) = self.xy_to_chunk_xy(x, y)
        return chunk.things_at(x, y)

    def tp_is(self, x, y, value):

        if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or x < 0 or y < 0:
            return None

        (chunk, x, y) = self.xy_to_chunk_xy(x, y)
        return chunk.tp_is(x, y, value)

    def tp_is_where(self, value):

        for cx in range(0, mm.CHUNK_ACROSS):
            for cy in range(0, mm.CHUNK_DOWN):
                found = self.chunk[cx][cy].tp_is_where(value)
                if found is not None:
                    (x, y) = found
                    return (self.chunk_xy_to_xy(cx, cy, x, y))
        return None

    def things_remove_all_except_player(self):
        for y in range(mm.MAP_HEIGHT):
            for x in range(mm.MAP_WIDTH):
                r = self.things_at(x, y)
                for t in r:
                    if t.tp.is_player:
                        continue

                    t.destroy("via things remove all")

    def things_flood_fill_(self, x, y, tp, walked):

        sys.setrecursionlimit(mm.MAP_WIDTH * mm.MAP_HEIGHT)

        if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or x < 0 or y < 0:
            return

        if walked[x][y] == 1:
            return

        walked[x][y] = 1

        (chunk, ox, oy) = self.xy_to_chunk_xy(x, y)

        for f in chunk.things_on_chunk[ox][oy]:
            other_tp = f.tp

            if not tp.is_wall and \
               not tp.is_landrock and \
               not tp.is_landrock_snow and \
               not tp.is_cwall and \
               not tp.is_hwall:
                if other_tp.is_wall or \
                   other_tp.is_landrock or \
                   other_tp.is_landrock_snow or \
                   other_tp.is_hwall or \
                   other_tp.is_cwall:
                    return

            if tp.is_wall or \
               tp.is_door or \
               tp.is_landrock or \
               tp.is_landrock_snow or \
               tp.is_cwall or \
               tp.is_hwall or \
               tp.is_bridge or \
               tp.is_water:
                if other_tp.is_wall or \
                   other_tp.is_door or \
                   other_tp.is_landrock or \
                   other_tp.is_landrock_snow or \
                   other_tp.is_cwall or \
                   other_tp.is_hwall or \
                   other_tp.is_bridge or \
                   other_tp.is_water:
                    return

            if tp.is_door:
                if other_tp.is_wall or \
                   other_tp.is_cwall or \
                   other_tp.is_hwall or \
                   other_tp.is_water:
                    return

            if tp.is_dirt or \
               tp.is_grass or \
               tp.is_lawn or \
               tp.is_snow or \
               tp.is_ice or \
               tp.is_gravel or \
               tp.is_gravel_snow or \
               tp.is_snow or \
               tp.is_sand or \
               tp.is_water:
                if other_tp.is_dirt or \
                   other_tp.is_grass or \
                   other_tp.is_lawn or \
                   other_tp.is_snow or \
                   other_tp.is_ice or \
                   other_tp.is_gravel or \
                   other_tp.is_gravel_snow or \
                   other_tp.is_snow or \
                   other_tp.is_sand or \
                   other_tp.is_water:
                    return

        t = self.thing_find_same_type(x, y, tp.name)
        if t is not None:
            t.destroy("via flood fill")

        t = thing.Thing(level=self,
                        tp_name=tp.name,
                        x=x, y=y)
        t.push()

        self.things_flood_fill_(x - 1, y, tp, walked)
        self.things_flood_fill_(x + 1, y, tp, walked)
        self.things_flood_fill_(x, y - 1, tp, walked)
        self.things_flood_fill_(x, y + 1, tp, walked)

    def things_flood_fill(self, x, y, tp):

        if tp is None:
            return

        walked = [[0 for i in range(mm.MAP_HEIGHT)]
                  for j in range(mm.MAP_WIDTH)]

        self.things_flood_fill_(x, y, tp, walked)

    def is_movement_blocking_at(self, x, y):

        (chunk, ox, oy) = self.xy_to_chunk_xy(x, y)
        if chunk.is_movement_blocking_at(ox, oy):
            return True
        return False

    def describe_position(self, x, y):

        (chunk, ox, oy) = self.xy_to_chunk_xy(x, y)
        return chunk.describe_position(ox, oy)

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
                for t in chunk.things_on_chunk[ox][oy]:
                    if t.tp.is_wall or \
                       t.tp.is_rock or \
                       t.tp.is_door or \
                       t.tp.is_landrock or \
                       t.tp.is_landrock_snow or \
                       t.tp.is_hwall or \
                       t.tp.is_cwall:
                        skip = True
                        break

                if skip:
                    continue

                for t in chunk.things_on_chunk[ox][oy]:
                    if t.tp.is_floor or \
                       t.tp.is_grass or \
                       t.tp.is_carpet or \
                       t.tp.is_lawn or \
                       t.tp.is_dirt or \
                       t.tp.is_snow or \
                       t.tp.is_ice or \
                       t.tp.is_gravel or \
                       t.tp.is_gravel_snow or \
                       t.tp.is_snow or \
                       t.tp.is_sand or \
                       t.tp.is_corridor or \
                       t.tp.is_bridge or \
                       t.tp.is_dusty or \
                       t.tp.is_water:
                        d.cells[x][y] = dmap.FLOOR
                        break

#        mm.con("dmap")
#
#        for y in range(mm.MAP_HEIGHT):
#            for x in range(mm.MAP_WIDTH):
#                if x == game.g.player.x and y == game.g.player.y:
#                    mm.puts("@")
#                else:
#                    if d.cells[x][y] == dmap.FLOOR:
#                        mm.puts(".")
#                    else:
#                        mm.puts("#")
#            mm.puts("\n")
#        mm.puts("\n")

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
            ALL_DELTAS = [
                          (0, -1, 1.0),
                          (-1, 0, 1.0),
                          (1, 0, 1.0),
                          (0, 1, 1.0),
                          ]

            if x >= mm.MAP_WIDTH or y >= mm.MAP_HEIGHT or \
               x < 0 or y < 0:
                return out_path

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

        #        mm.con("path before optimize")
        #        debug = [[' ' for x in range(mm.MAP_WIDTH)]
        #                 for y in range(mm.MAP_HEIGHT)]
        #
        #        for (x, y) in path:
        #            debug[x][y] = "."
        #
        #        for y in range(mm.MAP_HEIGHT):
        #            for x in range(mm.MAP_WIDTH):
        #                mm.puts(debug[x][y])
        #            mm.puts("\n")
        #        mm.puts("\n")

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

#        mm.con("path after optimize")
#        debug = [[' ' for x in range(mm.MAP_WIDTH)]
#                 for y in range(mm.MAP_HEIGHT)]
#
#        for (x, y) in path:
#            debug[x][y] = "."
#
#        for y in range(mm.MAP_HEIGHT):
#            for x in range(mm.MAP_WIDTH):
#                mm.puts(debug[x][y])
#            mm.puts("\n")
#        mm.puts("\n")

        return (path)

    def dmap_reset(self):

        self.dmaps = [[None for x in range(mm.MAP_WIDTH)]
                      for y in range(mm.MAP_HEIGHT)]
