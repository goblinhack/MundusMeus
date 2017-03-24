#!/usr/bin/env python3
import charmap


class Room:
    def __init__(self):
        self.vert_slice = {}
        self.width = 0
        self.height = 0
        self.can_be_placed_as_first_room = False
        self.can_be_placed_as_start = False
        self.can_be_placed_as_exit = False

    #
    # Rooms are made out of stacks of vertical slices
    #
    def vert_slice_add(self, vert_slice, vert_slice_data):
        #
        # Work our the size of this slice. Make sure it's the
        # same size as the other slices.
        #
        tiles_across = len(vert_slice_data[0])
        tiles_down = len(vert_slice_data)

        if self.width != 0:
            assert tiles_down == self.width
        else:
            self.width = tiles_down

        if self.height != 0:
            assert tiles_across == self.height
        else:
            self.height = tiles_across

        self.vert_slice[vert_slice] = vert_slice_data

    #
    # Find the floor tiles at the edge of the room. We choose
    # these for corridor starts.
    #
    def find_edge_exits(self):
        vert_floor_slice = self.vert_slice["floor"]
        vert_wall_slice = self.vert_slice["wall"]
        self.edge_exits = []

        y = 0
        for x in range(self.width):
            if vert_wall_slice[x][y] == charmap.WALL:
                continue
            if vert_floor_slice[x][y] == charmap.FLOOR:
                self.edge_exits.append((x, y))

        y = self.height - 1
        for x in range(self.width):
            if vert_wall_slice[x][y] == charmap.WALL:
                continue
            if vert_floor_slice[x][y] == charmap.FLOOR:
                self.edge_exits.append((x, y))

        x = 0
        for y in range(self.height):
            if vert_wall_slice[x][y] == charmap.WALL:
                continue
            if vert_floor_slice[x][y] == charmap.FLOOR:
                self.edge_exits.append((x, y))

        x = self.width - 1
        for y in range(self.height):
            if vert_wall_slice[x][y] == charmap.WALL:
                continue
            if vert_floor_slice[x][y] == charmap.FLOOR:
                self.edge_exits.append((x, y))

        for x in range(self.width):
            for y in range(self.height):
                if vert_wall_slice[x][y] == charmap.DOOR:
                    self.edge_exits.append((x, y))

    def finalize(self):
        self.find_edge_exits()
