#!/usr/bin/env python3
from termcolor import colored
import random
import copy
import sys
import os

SPACE = " "
CORRIDOR = "#"
DOOR = "D"
WALL = "x"
FLOOR = "."

charmap = {
    " ": {
        "bg": "on_grey",
        "fg": "grey",
    },
    WALL: {
        "bg": "on_blue",
        "fg": "white",
        "is_wall": True,
    },
    FLOOR: {
        "bg": "on_grey",
        "fg": "white",
        "is_floor": True,
    },
    CORRIDOR: {
        "bg": "on_grey",
        "fg": "yellow",
        "is_corridor": True,
    },
    DOOR: {
        "bg": "on_grey",
        "fg": "red",
        "is_door": True,
    },
    "o": {
        "bg": "on_grey",
        "fg": "yellow",
        "is_obj": True,
    },
}


class Enumeration(object):
    def __init__(self, names):  # or *names, with no .split()
        self.to_name = {}
        for number, name in enumerate(names.split()):
            setattr(self, name, number)
            self.to_name[number] = name

Depth = Enumeration("floor wall obj max")


class Room:
    def __init__(self):
        self.slice = {}
        self.width = 0
        self.height = 0

    def slice_add(self, slice, slice_data):
        w = len(slice_data[0])
        h = len(slice_data)

        if self.width != 0:
            assert w == self.width
        else:
            self.width = w

        if self.height != 0:
            assert h == self.height
        else:
            self.height = h

        self.slice[slice] = slice_data

    def finalize(self):
        fslice = self.slice["floor"]
        wslice = self.slice["wall"]

        self.exits = []

        y = 0
        for x in range(self.width):
            if wslice[y][x] == WALL:
                continue
            if fslice[y][x] == FLOOR:
                self.exits.append((x, y))
        y = self.height - 1
        for x in range(self.width):
            if wslice[y][x] == WALL:
                continue
            if fslice[y][x] == FLOOR:
                self.exits.append((x, y))
        x = 0
        for y in range(self.height):
            if wslice[y][x] == WALL:
                continue
            if fslice[y][x] == FLOOR:
                self.exits.append((x, y))
        x = self.width - 1
        for y in range(self.height):
            if wslice[y][x] == WALL:
                continue
            if fslice[y][x] == FLOOR:
                self.exits.append((x, y))


class Maze:
    def __init__(self, rooms, charmap, width=80, height=40, room_count=20):
        self.width = width
        self.height = height
        self.rooms = rooms
        self.charmap = charmap

        self.corridor_fork_chance = 10

        #
        # Lower, longer corridors
        #
        self.corridor_grow_chance = 10

        self.cells = [[[' ' for d in range(Depth.max)]
                       for i in range(height)]
                      for j in range(width)]

        self.make_random_rooms()
        self.room_count = 0

        loop = 0
        while True:
            roomno = random.randint(0, len(self.rooms) - 1)
            loop += 1
            x = int(width / 2)
            y = int(height / 2)
            room = self.rooms[roomno]
            x -= int(room.width / 2)
            y -= int(room.height / 2)

            if self.room_place(roomno, x, y):
                break

            if loop > 100:
                print("Could not place first room")
                break

        self.corridor_ends = []
        loop = 0
        self.rooms_corridors_create()
        while self.room_count < room_count:
            loop += 1
            if loop > self.room_count * 2:
                print("Too long, made {0} rooms".format(self.room_count))
                break

            if self.rooms_place_corridors_end():
                self.rooms_corridors_create()

        self.rooms_trim_corridors()
        self.rooms_plug_walls()

    def putc(self, x, y, d, c):
        if x >= self.width:
            return
        if y >= self.height:
            return
        if x < 0:
            return
        if y < 0:
            return
        if d < 0:
            return
        if d > Depth.max:
            return
        self.cells[x][y][d] = c

    def getc(self, x, y, d):
        if x >= self.width:
            return None
        if y >= self.height:
            return None
        if x < 0:
            return None
        if y < 0:
            return None
        if d < 0:
            return None
        if d > Depth.max:
            return None
        return self.cells[x][y][d]

    def is_oob(self, x, y):
        if x >= self.width:
            return True
        if y >= self.height:
            return True
        if x < 0:
            return True
        if y < 0:
            return True
        return False

    def is_something_at(self, x, y):
        if self.is_floor_at(x, y):
            return True
        if self.is_corridor_at(x, y):
            return True
        if self.is_wall_at(x, y):
            return True
        if self.is_obj_at(x, y):
            return True

        return False

    def is_floor_at(self, x, y):
        c = self.getc(x, y, Depth.floor)
        if c is not None:
            if "is_floor" in self.charmap[c]:
                return True
        return False

    def is_corridor_at(self, x, y):
        c = self.getc(x, y, Depth.floor)
        if c is not None:
            if "is_corridor" in self.charmap[c]:
                return True
        return False

    def is_corridor_or_floor_at(self, x, y):
        if self.is_corridor_at(x, y):
            return True
        if self.is_floor_at(x, y):
            return True
        return False

    def is_wall_at(self, x, y):
        c = self.getc(x, y, Depth.wall)
        if c is not None:
            if "is_wall" in self.charmap[c]:
                return True
        return False

    def is_obj_at(self, x, y):
        c = self.getc(x, y, Depth.obj)
        if c is not None:
            if "is_obj" in self.charmap[c]:
                return True
        return False

    def room_can_be_placed(self, roomno, x, y):
        room = self.rooms[roomno]

        if x < 1:
            return False
        elif x + room.width >= self.width - 1:
            return False

        if y < 1:
            return False
        elif y + room.height >= self.height - 1:
            return False

        for d in range(Depth.max):
            dname = Depth.to_name[d]
            if dname in room.slice:
                for ry in range(room.height):
                    for rx in range(room.width):
                        if self.is_something_at(x + rx, y + ry):
                            return False
        return True

    def room_place(self, roomno, x, y):
        if not self.room_can_be_placed(roomno, x, y):
            return False

        room = self.rooms[roomno]

        for d in range(Depth.max):
            dname = Depth.to_name[d]
            if dname in room.slice:
                rslice = room.slice[dname]
                for ry in range(room.height):
                    for rx in range(room.width):
                        rchar = rslice[ry][rx]
                        self.putc(x + rx, y + ry, d, rchar)

        self.room_count += 1
        return True

    def corridor_create(self, x, y, dx, dy, clen=0):
        x += dx
        y += dy

        if self.getc(x, y, Depth.floor) is None:
            return

        if self.is_something_at(x, y):
            return

        clen += 1

        self.putc(x, y, Depth.floor, CORRIDOR)

        if random.randint(1, 100) < clen * self.corridor_grow_chance:
            self.corridor_ends.append((x, y))
            return

        if clen % 2 == 0:
            if random.randint(1, 100) < self.corridor_fork_chance:
                self.corridor_create(x, y, 0, - 1, clen)
            if random.randint(1, 100) < self.corridor_fork_chance:
                self.corridor_create(x, y, -1, 0, clen)
            if random.randint(1, 100) < self.corridor_fork_chance:
                self.corridor_create(x, y, 1, 0, clen)
            if random.randint(1, 100) < self.corridor_fork_chance:
                self.corridor_create(x, y, 0, 1, clen)

        self.corridor_create(x, y, dx, dy, clen)

    def rooms_corridors_create(self):
        self.inuse = \
                [[0 for i in range(self.height)] for j in range(self.width)]
        possible_room_exits = []

        for y in range(2, self.height - 2):
            for x in range(2, self.width - 2):

                self.inuse[x][y] = 0

                if not self.is_floor_at(x, y):
                    continue

                if self.is_wall_at(x, y):
                    self.inuse[x][y] = 1
                    continue

                if self.is_obj_at(x, y):
                    self.inuse[x][y] = 1
                    continue

                for dx in range(-1, 2):
                    for dy in range(-1, 2):
                        if self.is_corridor_at(x + dx, y + dy):
                            self.inuse[x][y] = 1
                            continue

                if self.inuse[x][y] == 1:
                    continue

                possible_room_exits.append((x, y))

        for coord in possible_room_exits:
            x, y = coord

            # a b c
            # d * f
            # g h i
            b = self.inuse[x][y-1]
            d = self.inuse[x-1][y]
            f = self.inuse[x+1][y]
            h = self.inuse[x][y+1]

            if not b:
                self.corridor_create(x, y, 0, - 1)
            if not d:
                self.corridor_create(x, y, -1, 0)
            if not f:
                self.corridor_create(x, y, 1, 0)
            if not h:
                self.corridor_create(x, y, 0, 1)

    def rooms_place_corridors_end(self):
        roomno = random.randint(1, len(self.rooms))
        roomno -= 1
        room = self.rooms[roomno]

        rplaced = False

        for coord in self.corridor_ends:
            cx, cy = coord

            for exit in room.exits:
                rx, ry = exit

                x = cx - rx
                y = cy - ry

                if self.room_place(roomno, x - 1, y):
                    rplaced = True
                elif self.room_place(roomno, x + 1, y):
                    rplaced = True
                elif self.room_place(roomno, x, y - 1):
                    rplaced = True
                elif self.room_place(roomno, x, y + 1):
                    rplaced = True

                if rplaced:
                    break

            if rplaced:
                break

        return rplaced

    #
    # Trim dead end corridors
    #
    def rooms_trim_corridors(self):
        trimmed = True
        while trimmed:
            trimmed = False
            for y in range(self.height):
                for x in range(self.width):
                    if not self.is_corridor_at(x, y):
                        continue

                    nbrs = 0
                    if self.is_corridor_or_floor_at(x - 1, y):
                        if not self.is_wall_at(x - 1, y):
                            nbrs += 1
                    if self.is_corridor_or_floor_at(x + 1, y):
                        if not self.is_wall_at(x + 1, y):
                            nbrs += 1
                    if self.is_corridor_or_floor_at(x, y - 1):
                        if not self.is_wall_at(x, y - 1):
                            nbrs += 1
                    if self.is_corridor_or_floor_at(x, y + 1):
                        if not self.is_wall_at(x, y + 1):
                            nbrs += 1
                    if nbrs < 2:
                        self.putc(x, y, Depth.floor, SPACE)
                        trimmed = True

    #
    # Any rooms opening onto nothing, fill them in
    #
    def rooms_plug_walls(self):
        trimmed = True
        while trimmed:
            trimmed = False
            for y in range(self.height):
                for x in range(self.width):
                    if not self.is_floor_at(x, y):
                        continue

                    if self.is_wall_at(x, y):
                        continue

                    for dx in range(-1, 2):
                        for dy in range(-1, 2):
                            if not self.is_something_at(x + dx, y + dy):
                                self.putc(x + dx, y + dy, Depth.wall, WALL)

    def putl(self, start, end, depth, rchar):
        points = get_line(start, end)
        for p in points:
            x, y = p
            self.putc(x, y, depth, rchar)

    def floodfill(self, x, y, depth, rchar):
        s = [(x, y)]
        while len(s) > 0:
            x, y = s.pop()
            if self.is_oob(x, y):
                continue

            if self.is_something_at(x, y):
                continue

            self.putc(x, y, depth, rchar)
            s.append((x + 1, y))
            s.append((x - 1, y))
            s.append((x, y + 1))
            s.append((x, y - 1))

    def floodget(self, x, y, depth, rchar):
        s = [(x, y)]
        r = []
        while len(s) > 0:
            x, y = s.pop()
            if self.is_oob(x, y):
                continue

            if not self.is_something_at(x, y):
                continue

            self.putc(x, y, depth, rchar)
            r.append((x, y))
            s.append((x + 1, y))
            s.append((x - 1, y))
            s.append((x, y + 1))
            s.append((x, y - 1))

        return r

    def make_random_rooms(self):
        cnt = 0
        while cnt < 20:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = random.randint(-10, self.width + 10)
            y2 = random.randint(-10, self.height + 10)
            self.putl((x1, y1), (x2, y2), Depth.floor, FLOOR)
            cnt += 1
        cnt = 0
        while cnt < 20:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = x1 + 100
            y2 = y1
            self.putl((x1, y1), (x2, y2), Depth.floor, FLOOR)
            cnt += 1
        cnt = 0
        while cnt < 20:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = x1
            y2 = y1 + 100
            self.putl((x1, y1), (x2, y2), Depth.floor, FLOOR)
            cnt += 1

        cnt = 0
        while cnt < 20:
            x = random.randint(0, self.width - 1)
            y = random.randint(0, self.height - 1)
            self.floodfill(x, y, Depth.floor, FLOOR)
            cnt += 1

        cnt = 0
        while cnt < 20:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = random.randint(-10, self.width + 10)
            y2 = random.randint(-10, self.height + 10)
            self.putl((x1, y1), (x2, y2), Depth.floor, SPACE)
            cnt += 1
        cnt = 0
        while cnt < 20:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = x1 + 100
            y2 = y1
            self.putl((x1, y1), (x2, y2), Depth.floor, SPACE)
            cnt += 1
        cnt = 0
        while cnt < 20:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = x1
            y2 = y1 + 100
            self.putl((x1, y1), (x2, y2), Depth.floor, SPACE)
            cnt += 1

        cnt = 0
        for y in range(self.height):
            for x in range(self.width):
                if self.is_floor_at(x, y):
                    r = self.floodget(x, y, Depth.floor, SPACE)
                    cnt += 1
                    if len(r) < 20:
                        continue

                    minx = 999
                    maxx = -999
                    miny = 999
                    maxy = -999
                    for p in r:
                        rx, ry = p
                        if rx < minx:
                            minx = rx
                        if ry < miny:
                            miny = ry
                        if rx > maxx:
                            maxx = rx
                        if ry > maxy:
                            maxy = ry

                    rw = maxx - minx + 1
                    rh = maxy - miny + 1
                    rw += 2
                    rh += 2

                    rcells = [[' ' for i in range(rh)] for j in range(rw)]
                    for p in r:
                        rx, ry = p
                        rx -= minx
                        ry -= miny
                        rx += 1
                        ry += 1
                        rcells[rx][ry] = FLOOR

                    for ry in range(rh):
                        for rx in range(rw):
                            if rcells[rx][ry] == FLOOR:
                                if rcells[rx-1][ry] == SPACE:
                                    rcells[rx-1][ry] = WALL
                                if rcells[rx+1][ry] == SPACE:
                                    rcells[rx+1][ry] = WALL
                                if rcells[rx][ry-1] == SPACE:
                                    rcells[rx][ry-1] = WALL
                                if rcells[rx][ry+1] == SPACE:
                                    rcells[rx][ry+1] = WALL

                    floorslice = copy.deepcopy(rcells)
                    wallslice = copy.deepcopy(rcells)
                    objslice = [[' ' for i in range(rh)] for j in range(rw)]

                    for ry in range(0, rh):
                        for rx in range(0, rw):
                            if floorslice[rx][ry] == WALL:
                                floorslice[rx][ry] = FLOOR
                            if wallslice[rx][ry] == FLOOR:
                                wallslice[rx][ry] = SPACE
                            if wallslice[rx][ry] == WALL:
                                if rx % 2 == 0 and ry % 2 == 0:
                                    if random.randint(0, 100) < 50:
                                        wallslice[rx][ry] = SPACE

#                    for ry in range(rh):
#                        for rx in range(rw):
#                            sys.stdout.write(floorslice[rx][ry])
#                        print()
#
#                    for ry in range(rh):
#                        for rx in range(rw):
#                            sys.stdout.write(wallslice[rx][ry])
#                        print()

                    r = Room()
                    r.slice_add("floor", floorslice)
                    r.slice_add("wall", wallslice)
                    r.slice_add("obj", objslice)
                    r.finalize()
                    self.rooms.append(r)

        self.cells = [[[' ' for d in range(Depth.max)]
                       for i in range(height)]
                      for j in range(width)]

    def dump(self):
        os.system('cls' if os.name == 'nt' else 'clear')
        for y in range(self.height):
            for x in range(self.width):
                for d in reversed(range(Depth.max)):
                    c = self.cells[x][y][d]
                    charmap = self.charmap[c]
                    fg = charmap["fg"]
                    bg = charmap["bg"]
                    if c != " ":
                        break

                sys.stdout.write(colored(c, fg, bg))
            print("")


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


rooms = []

r = Room()
r.slice_add("floor", [
                "......",
                "......",
                "......",
                "......",
                "......",
        ])
r.slice_add("wall", [
                "xx.xxx",
                "x     ",
                "     x",
                "x    x",
                "xxx xx",
        ])

r.slice_add("obj", [
                "      ",
                "   o  ",
                "      ",
                "      ",
                "      ",
        ])
r.finalize()
rooms.append(r)

r = Room()
r.slice_add("floor", [
                "..........",
                "..........",
                "..........",
                "..........",
                "..........",
                "..........",
                "..........",
        ])
r.slice_add("wall", [
                "xx xxxxxxx",
                "x  x      ",
                "x  x   xxx",
                "x        x",
                "x        x",
                "x        x",
                "xxxxxxxxxx",
        ])

r.slice_add("obj", [
                "          ",
                "   o      ",
                "          ",
                "          ",
                "          ",
                "          ",
                "          ",
        ])
r.finalize()
rooms.append(r)

r = Room()
r.slice_add("floor", [
                "   .......",
                "   .......",
                "..........",
                "..........",
                "..........",
                ".......   ",
                ".......   ",
        ])
r.slice_add("wall", [
                "   xxxxxxx",
                "   x      ",
                "xxxx     x",
                "         x",
                "x     xxxx",
                "x     x   ",
                "xxxxxxx   ",
        ])

r.slice_add("obj", [
                "          ",
                "          ",
                "          ",
                "          ",
                "          ",
                "          ",
                "          ",
        ])
r.finalize()
rooms.append(r)

r = Room()
r.slice_add("floor", [
                "   ....   ",
                "  ......  ",
                "..........",
                "..........",
                "..........",
                "  ......  ",
                "   ....   ",
        ])
r.slice_add("wall", [
                "   xx.x   ",
                "  xx  xx  ",
                "xxx    xxx",
                "          ",
                "xxx    xxx",
                "  xx  xx  ",
                "   x.xx   ",
        ])

r.slice_add("obj", [
                "          ",
                "          ",
                "          ",
                "          ",
                "          ",
                "          ",
                "          ",
        ])
r.finalize()
rooms.append(r)

r = Room()
r.slice_add("floor", [
                "....  ....   ",
                ".............",
                ".............",
                " ............",
                " ............",
                ".............",
                ".............",
                "............ ",
                "............ ",
                "....   ..... ",
        ])
r.slice_add("wall", [
                "xxxx  xxxx   ",
                "x  xxxx  xxxx",
                "xx          x",
                " x          x",
                " x          x",
                "xx          x",
                "           xx",
                "x          x ",
                "x  xxxxx   x ",
                "xxxx   xxxxx ",
        ])

r.slice_add("obj", [
                "             ",
                "             ",
                "             ",
                "             ",
                "             ",
                "             ",
                "             ",
                "             ",
                "             ",
                "             ",
        ])
r.finalize()
rooms.append(r)

for seed in range(1000):
    width = 64
    height = 64
    random.seed(seed)
    while True:
        maze = Maze(width=width, height=height, rooms=rooms,
                    room_count=20, charmap=charmap)
        if maze.room_count > 10:
            break

    # maze.room_place(roomno=1, x=75, y=20)
    print("Seed {0}".format(seed))
    maze.dump()
