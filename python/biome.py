#!/usr/bin/env python3
import charmap

XY_DELTAS = [(-1, 0), (1, 0), (0, -1), (0, 1)]

ALL_DELTAS = [(-1, -1), (0, -1), (1, -1),
              (-1, 0), (0, 0), (1, 0),
              (-1, 1), (0, 1), (1, 1)]

ALL_DELTAS_BAR_MID = [(-1, -1), (0, -1), (1, -1),
                      (-1, 0), (1, 0),
                      (-1, 1), (0, 1), (1, 1)]


class Biome:

    #
    # Puts a tile on the map
    #
    def putc(self, x, y, d, c):
        if not 0 <= x < self.width:
            return None
        if not 0 <= y < self.height:
            return None
        self.cells[x][y][d] = c

    #
    # Gets a tile of the map or None
    #
    def getc(self, x, y, d):
        if not 0 <= x < self.width:
            return None
        if not 0 <= y < self.height:
            return None
        return self.cells[x][y][d]

    def getc_fast(self, x, y, d):
        return self.cells[x][y][d]

    #
    # Set a tile with a given roomno
    #
    def putr(self, x, y, r):
        if not 0 <= x < self.width:
            return None
        if not 0 <= y < self.height:
            return None
        self.roomno_cells[x][y] = r

    #
    # Get the roomno in this tile
    #
    def getr(self, x, y):
        if not 0 <= x < self.width:
            return None
        if not 0 <= y < self.height:
            return None
        return self.roomno_cells[x][y]

    #
    # Line between points
    #
    def line_draw(self, start, end, depth, rchar):
        points = get_line(start, end)
        for p in points:
            x, y = p
            self.putc(x, y, depth, rchar)

    #
    # Flood fill empty space.
    #
    def flood_fill(self, x, y, depth, rchar):
        stack = [(x, y)]
        while len(stack) > 0:
            x, y = stack.pop(0)
            if self.is_oob(x, y):
                continue

            if self.is_any_floor_at(x, y):
                continue

            self.putc(x, y, depth, rchar)
            stack.append((x + 1, y))
            stack.append((x - 1, y))
            stack.append((x, y + 1))
            stack.append((x, y - 1))

    #
    # Flood fill empty space and return the points.
    # Used to get all the tiles in a room.
    #
    def flood_erase(self, x, y):
        walked = [[0 for i in range(self.height)]
                  for j in range(self.width)]

        stack = [(x, y)]
        r = []

        while len(stack) > 0:
            x, y = stack.pop(0)
            if x >= self.width or y >= self.height or x < 0 or y < 0:
                continue

            if walked[x][y]:
                continue

            walked[x][y] = 1

            c = self.cells[x][y][charmap.depth.floor]
            if c != charmap.FLOOR:
                continue

            self.cells[x][y][charmap.depth.floor] = charmap.SPACE
            r.append((x, y))

            #
            # To limit room size
            #
            if len(r) > 100:
                return r

            if x < self.width - 1 and not walked[x + 1][y]:
                stack.append((x + 1, y))
            if x > 1 and not walked[x - 1][y]:
                stack.append((x - 1, y))
            if y < self.height - 1 and not walked[x][y + 1]:
                stack.append((x, y + 1))
            if y > 1 and not walked[x][y - 1]:
                stack.append((x, y - 1))

        return r

    #
    # Find all adjacent characters of the same type.
    #
    def flood_find(self, x, y, func):

        walked = [[0 for i in range(self.height)]
                  for j in range(self.width)]

        stack = [(x, y)]
        r = []
        while len(stack) > 0:
            x, y = stack.pop(0)
            if self.is_oob(x, y):
                continue

            if walked[x][y]:
                continue

            walked[x][y] = 1

            if not func(x, y):
                continue

            r.append((x, y))
            stack.append((x + 1, y))
            stack.append((x - 1, y))
            stack.append((x, y + 1))
            stack.append((x, y - 1))

        return r

    #
    # Find all adjacent characters of the same type.
    #
    def flood_replace(self, x, y, depth, old, new):

        walked = [[0 for i in range(self.height)]
                  for j in range(self.width)]

        stack = [(x, y)]
        r = []
        while len(stack) > 0:
            x, y = stack.pop(0)
            if self.is_oob(x, y):
                continue

            if walked[x][y]:
                continue

            walked[x][y] = 1

            c = self.getc(x, y, depth)
            if c != old:
                continue

            self.putc(x, y, depth, new)
            r.append((x, y))
            stack.append((x + 1, y))
            stack.append((x - 1, y))
            stack.append((x, y + 1))
            stack.append((x, y - 1))

        return r

    #
    # Is out of bounds?
    #
    def is_oob(self, x, y):
        if x >= self.width or y >= self.height or x < 0 or y < 0:
            return True
        return False

    def is_anything_at(self, x, y):
        for d in range(charmap.depth.max):
            c = self.getc(x, y, d)
            if c != charmap.SPACE:
                return True
        return False

    def is_any_floor_at(self, x, y):
        c = self.getc(x, y, charmap.depth.floor)
        if c is None:
            return False
        if "is_floor" in charmap.charmap[c] or \
           "is_corridor" in charmap.charmap[c]:
            return True
        return False

    def is_movement_blocking_at(self, x, y):
        c = self.getc(x, y, charmap.depth.wall)
        if c is None:
            return False
        if "is_wall" in charmap.charmap[c] or \
           "is_door" in charmap.charmap[c] or \
           "is_obj" in charmap.charmap[c] or \
           "is_dungeon_way_up" in charmap.charmap[c] or \
           "is_dungeon_way_down" in charmap.charmap[c] or \
           "is_key" in charmap.charmap[c]:
            return True
        return False

    def is_floor_at(self, x, y):
        c = self.getc(x, y, charmap.depth.floor)
        if c is not None:
            if "is_floor" in charmap.charmap[c]:
                return True
        return False

    def is_dusty_at(self, x, y):
        c = self.getc(x, y, charmap.depth.floor)
        if c is not None:
            if "is_dusty" in charmap.charmap[c]:
                return True
        return False

    def is_floor_at_fast(self, x, y):
        c = self.cells[x][y][charmap.depth.floor]
        if c == charmap.FLOOR:
            return True
        return False

    def is_corridor_at(self, x, y):
        c = self.getc(x, y, charmap.depth.floor)
        if c is not None:
            if "is_corridor" in charmap.charmap[c]:
                return True
        return False

    def is_corridor_or_floor_at(self, x, y):
        if self.is_corridor_at(x, y):
            return True
        if self.is_floor_at(x, y):
            return True
        return False

    def is_corridor_or_door_at(self, x, y):
        if self.is_corridor_at(x, y):
            return True
        if self.is_door_at(x, y):
            return True
        return False

    def is_wall_at(self, x, y):
        c = self.getc(x, y, charmap.depth.wall)
        if c is not None:
            if "is_wall" in charmap.charmap[c]:
                return True
        return False

    def is_cwall_at(self, x, y):
        c = self.getc(x, y, charmap.depth.wall)
        if c is not None:
            if "is_cwall" in charmap.charmap[c]:
                return True
        return False

    def is_door_at(self, x, y):
        c = self.getc(x, y, charmap.depth.wall)
        if c is not None:
            if "is_door" in charmap.charmap[c]:
                return True
        return False

    def is_obj_at(self, x, y):
        c = self.getc(x, y, charmap.depth.obj)
        if c is not None:
            if "is_obj" in charmap.charmap[c]:
                return True
        return False

    def is_dungeon_way_up_at(self, x, y):
        c = self.getc(x, y, charmap.depth.wall)
        if c is not None:
            if "is_dungeon_way_up" in charmap.charmap[c]:
                return True
        return False

    def is_dungeon_way_down_at(self, x, y):
        c = self.getc(x, y, charmap.depth.wall)
        if c is not None:
            if "is_dungeon_way_down" in charmap.charmap[c]:
                return True
        return False

    def is_lava_at(self, x, y):
        c = self.getc(x, y, charmap.depth.under)
        if c is not None:
            if "is_lava" in charmap.charmap[c]:
                return True
        return False

    def is_treasure_at(self, x, y):
        c = self.getc(x, y, charmap.depth.obj)
        if c is not None:
            if "is_treasure" in charmap.charmap[c]:
                return True
        return False

    def is_water_at(self, x, y):
        c = self.getc(x, y, charmap.depth.under)
        if c is not None:
            if "is_water" in charmap.charmap[c]:
                return True
        return False

    def is_chasm_at(self, x, y):
        c = self.getc(x, y, charmap.depth.under)
        if c is not None:
            if "is_chasm" in charmap.charmap[c]:
                return True
        return False

    def is_rock_at(self, x, y):
        c = self.getc(x, y, charmap.depth.wall)
        if c is not None:
            if "is_rock" in charmap.charmap[c]:
                return True
        return False

    def is_key_at(self, x, y):
        c = self.getc(x, y, charmap.depth.wall)
        if c is not None:
            if "is_key" in charmap.charmap[c]:
                return True
        return False

    def is_dissolves_walls_at(self, x, y):
        c = self.getc(x, y, charmap.depth.under)
        if c is not None:
            if "is_dissolves_walls" in charmap.charmap[c]:
                return True
        return False

    def is_grass_at(self, x, y):
        c = self.getc(x, y, charmap.depth.grass)
        if c is not None:
            if "is_grass" in charmap.charmap[c]:
                return True
        return False

    def is_gravel_at(self, x, y):
        c = self.getc(x, y, charmap.depth.gravel)
        if c is not None:
            if "is_gravel" in charmap.charmap[c]:
                return True
        return False

    def is_road_at(self, x, y):
        c = self.getc(x, y, charmap.depth.road)
        if c is not None:
            if "is_road" in charmap.charmap[c]:
                return True
        return False

    def is_dungeon_at(self, x, y):
        c = self.getc(x, y, charmap.depth.floor)
        if c is not None:
            if "is_dungeon" in charmap.charmap[c]:
                return True
        return False

    def is_snow_at(self, x, y):
        c = self.getc(x, y, charmap.depth.snow)
        if c is not None:
            if "is_snow" in charmap.charmap[c]:
                return True
        return False

    def is_ice_at(self, x, y):
        c = self.getc(x, y, charmap.depth.ice)
        if c is not None:
            if "is_ice" in charmap.charmap[c]:
                return True
        return False

    def is_dirt_at(self, x, y):
        c = self.getc(x, y, charmap.depth.dirt)
        if c is not None:
            if "is_dirt" in charmap.charmap[c]:
                return True
        return False

    def is_sand_at(self, x, y):
        c = self.getc(x, y, charmap.depth.sand)
        if c is not None:
            if "is_sand" in charmap.charmap[c]:
                return True
        return False

    def is_tree_at(self, x, y):
        c = self.getc(x, y, charmap.depth.floor)
        if c is not None:
            if "is_tree" in charmap.charmap[c]:
                return True
        return False


def get_line(start, end):
    # Setup initial conditions
    x1, y1 = start
    x2, y2 = end
    dx = x2 - x1
    dy = y2 - y1

    # Determine how steep the line is
    is_steep = abs(dy) > abs(dx)

    # Rotate line
    if is_steep:
        x1, y1 = y1, x1
        x2, y2 = y2, x2

    # Swap start and end points if necessary and store swap state
    swapped = False
    if x1 > x2:
        x1, x2 = x2, x1
        y1, y2 = y2, y1
        swapped = True

    # Recalculate differentials
    dx = x2 - x1
    dy = y2 - y1

    # Calculate error
    error = int(dx / 2.0)
    ystep = 1 if y1 < y2 else -1

    # Iterate over bounding box generating points between start and end
    y = y1
    points = []
    for x in range(x1, x2 + 1):
        coord = (y, x) if is_steep else (x, y)
        points.append(coord)
        error -= abs(dy)
        if error < 0:
            y += ystep
            error += dx

    # Reverse the list if the coordinates were swapped
    if swapped:
        points.reverse()
    return points
