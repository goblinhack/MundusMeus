#!/usr/bin/env python3
import copy
import mm

WALL = 999
FLOOR = 998
GOAL = 0


class Dmap:
    def __init__(self, width=80, height=40):

        self.width = width
        self.height = height
        self.cells = [[0 for i in range(self.height)]
                      for j in range(self.width)]

        self.debug = [[0 for i in range(self.height)]
                      for j in range(self.width)]

    def dump(self):
        for y in range(self.height):
            for x in range(self.width):

                c = self.cells[x][y]
                d = self.debug[x][y]

                if c == WALL:
                    if d:
                        c = "X"
                    else:
                        c = "_"
                elif d:
                    c = "*"
                else:
                    c = chr(ord('0') + int(c % 9))

                mm.puts(c)
            mm.puts("\n")

    #
    # An optimized djkstra map that only looks at map areas that change.
    #
    def process(self):

        mm.dmap_process(arr=self.cells)

        #
        # Python version is too slow sadly
        #
        return

        valid = [[1 for i in range(self.height)]
                 for j in range(self.width)]
        orig_valid = [[1 for i in range(self.height)]
                      for j in range(self.width)]

        for y in range(self.height):
            for x in range(self.width):
                if self.cells[x][y] == WALL:
                    valid[x][y] = 0
                    orig_valid[x][y] = 0

        new_cells = [[WALL for i in range(self.height)]
                     for j in range(self.width)]

        changed = True
        while changed is True:
            changed = False
            new_valid = [[0 for i in range(self.height)]
                         for j in range(self.width)]

            for y in range(1, self.height - 1):
                for x in range(1, self.width - 1):

                    new_cells[x][y] = self.cells[x][y]

                    if orig_valid[x][y] == 0:
                        continue

                    if valid[x][y] == 0:
                        continue

                    a = self.cells[x-1][y-1] * 2
                    b = self.cells[x][y-1]
                    c = self.cells[x+1][y-1] * 2
                    d = self.cells[x-1][y]
                    e = self.cells[x][y]
                    f = self.cells[x+1][y]
                    g = self.cells[x-1][y+1] * 2
                    h = self.cells[x][y+1]
                    i = self.cells[x+1][y+1] * 2

                    if a < b:
                        lowest = a
                    else:
                        lowest = b

                    if c < lowest:
                        lowest = c

                    if d < lowest:
                        lowest = d

                    if f < lowest:
                        lowest = f

                    if g < lowest:
                        lowest = g

                    if h < lowest:
                        lowest = h

                    if i < lowest:
                        lowest = i

                    if e - lowest >= 2:
                        new_valid[x-1][y-1] = 1
                        new_valid[x-1][y] = 1
                        new_valid[x-1][y+1] = 1
                        new_valid[x][y-1] = 1
                        new_valid[x][y] = 1
                        new_valid[x][y+1] = 1
                        new_valid[x+1][y-1] = 1
                        new_valid[x+1][y] = 1
                        new_valid[x+1][y+1] = 1

                        new_cells[x][y] = lowest + 1
                        changed = True

            valid = copy.deepcopy(new_valid)
            self.cells = copy.deepcopy(new_cells)

#            os.system("clear")
#            self.dump()
