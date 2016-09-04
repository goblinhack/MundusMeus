#!/usr/bin/env python3
from termcolor import colored
import random
import sys

rooms = [
    {
        "floor": [
            "......",
            "......",
            "......",
            "......",
        ],
        "wall": [
            "xxxxxx",
            "x    D",
            "x    x",
            "xxxxxx",
        ],
        "obj": [
            "      ",
            "   o  ",
            "      ",
            "      ",
        ]
    },
]

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
        "is_floor": True,
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


class Maze:
    def __init__(self, mw, mh, rooms, charmap):
        self.mw = mw
        self.mh = mh
        self.rooms = rooms
        self.charmap = charmap
        self.corridor_fork_chance = 20
        self.corridor_grow_chance = 5
        self.cells = [[[' ' for d in range(Depth.max)]
                       for i in range(mh)]
                      for j in range(mw)]

    def putc(self, x, y, d, c):
        if x >= self.mw:
            return
        if y >= self.mh:
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
        if x >= self.mw:
            return None
        if y >= self.mh:
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

    def is_something_at(self, x, y):
        if self.is_floor_at(x, y):
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

    def place_room(self, roomno, x, y):
        room = self.rooms[roomno]

        for d in range(Depth.max):
            dname = Depth.to_name[d]
            if dname in room:
                rdata = room[dname]
                rwidth = len(rdata[0])
                rheight = len(rdata)

                for ry in range(rheight):
                    for rx in range(rwidth):
                        rc = rdata[ry][rx]
                        self.putc(x + rx, y + ry, d, rc)

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

    def find_doors(self):
        self.doors_n = [[0 for i in range(self.mh)] for j in range(self.mw)]
        self.doors_e = [[0 for i in range(self.mh)] for j in range(self.mw)]
        self.doors_s = [[0 for i in range(self.mh)] for j in range(self.mw)]
        self.doors_w = [[0 for i in range(self.mh)] for j in range(self.mw)]
        self.inuse = [[0 for i in range(self.mh)] for j in range(self.mw)]
        possible_doors = []

        for y in range(2, self.mh - 2):
            for x in range(2, self.mw - 2):

                self.inuse[x][y] = 0

                if not self.is_floor_at(x, y):
                    continue

                if self.is_wall_at(x, y):
                    continue

                if self.is_obj_at(x, y):
                    continue

                self.inuse[x][y] = 1
                possible_doors.append((x, y))

        self.doorlist = []
        for coord in possible_doors:
            x, y = coord

#            a = self.inuse[x-1][y-1]
            b = self.inuse[x][y-1]
#            c = self.inuse[x+1][y-1]
            d = self.inuse[x-1][y]
            f = self.inuse[x+1][y]
#            g = self.inuse[x-1][y+1]
            h = self.inuse[x][y+1]
#            i = self.inuse[x+1][y+1]

            if not b:
                self.corridor_create(x, y, 0, - 1)
            if not d:
                self.corridor_create(x, y, -1, 0)
            if not f:
                self.corridor_create(x, y, 1, 0)
            if not h:
                self.corridor_create(x, y, 0, 1)

    def dump(self):
        for y in range(self.mh):
            for x in range(self.mw):
                for d in reversed(range(Depth.max)):
                    c = self.cells[x][y][d]
                    charmap = self.charmap[c]
                    fg = charmap["fg"]
                    bg = charmap["bg"]
                    if c != " ":
                        break

                sys.stdout.write(colored(c, fg, bg))
            print("")

maze = Maze(mw=80, mh=40, rooms=rooms, charmap=charmap)
maze.place_room(roomno=0, x=10, y=10)
# maze.place_room(roomno=1, x=75, y=20)
maze.find_doors()

maze.dump()
