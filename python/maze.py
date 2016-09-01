#!/usr/bin/env python3
from termcolor import colored
import sys

rooms = [
    {
        "floor": [
            "xxxxxx",
            "x.....",
            "x....x",
            "xxxxxx",
        ],
        "obj": [
            "      ",
            "   o  ",
            "      ",
            "      ",
        ]
    },
    {
        "floor": [
            "xxxxxx",
            "......",
            "x.....",
            "x....x",
            "xxxxxx",
        ],
    },
]

charmap = {
    " ": {
        "bg": "on_grey",
        "fg": "grey",
    },
    "x": {
        "bg": "on_blue",
        "fg": "white",
    },
    ".": {
        "bg": "on_grey",
        "fg": "white",
    },
    "o": {
        "bg": "on_grey",
        "fg": "yellow",
    },
}


class Enumeration(object):
        def __init__(self, names):  # or *names, with no .split()
            self.to_name = {}
            for number, name in enumerate(names.split()):
                setattr(self, name, number)
                self.to_name[number] = name

Depth = Enumeration("floor obj max")


class Maze:
    def __init__(self, mwidth, mheight, rooms, charmap):
        self.mwidth = mwidth
        self.mheight = mheight
        self.rooms = rooms
        self.charmap = charmap
        self.cells = [[[' ' for d in range(Depth.max)]
                       for i in range(mheight)]
                      for j in range(mwidth)]

    def putc(self, x, y, d, c):
        if x >= self.mwidth:
            return
        if y >= self.mheight:
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

    def place_room(self, roomno, x, y):
        room = self.rooms[roomno]

        for d in range(Depth.max):
            dname = Depth.to_name[d]
            if dname in room:
                floordata = room[dname]
                rwidth = len(floordata[0])
                rheight = len(floordata)

                for ry in range(rheight):
                    for rx in range(rwidth):
                        rc = floordata[ry][rx]
                        self.putc(x + rx, y + ry, d, rc)

    def dump(self):
        for y in range(self.mheight):
            for x in range(self.mwidth):
                for d in reversed(range(Depth.max)):
                    c = self.cells[x][y][d]
                    m = self.charmap[c]
                    fg = m["fg"]
                    bg = m["bg"]
                    if c != " ":
                        break

                sys.stdout.write(colored(c, fg, bg))
            print("")

m = Maze(mwidth=80, mheight=40, rooms=rooms, charmap=charmap)
m.place_room(roomno=0, x=10, y=10)
m.place_room(roomno=1, x=75, y=20)

m.dump()
