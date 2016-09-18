#!/usr/bin/env python3
# exit
# place keys
# water
# lava
# chasms
# bridges
# treasure based on depth
import random
import copy
import sys
import os

SPACE = " "
CORRIDOR = "#"
DOOR = "D"
WALL = "x"
FLOOR = "."
START = "S"
EXIT = "E"

charmap = {
    " ": {
        "bg": "black",
        "fg": "black",
    },
    WALL: {
        "bg": "blue",
        "fg": "white",
        "is_wall": True,
    },
    FLOOR: {
        "bg": "black",
        "fg": "white",
        "is_floor": True,
    },
    CORRIDOR: {
        "bg": "black",
        "fg": "yellow",
        "is_corridor": True,
    },
    DOOR: {
        "bg": "black",
        "fg": "red",
        "is_door": True,
    },
    START: {
        "bg": "black",
        "fg": "red",
        "is_start": True,
    },
    EXIT: {
        "bg": "black",
        "fg": "red",
        "is_exit": True,
    },
    "o": {
        "bg": "black",
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

XY_DELTAS = [(-1, 0), (1, 0), (0, -1), (0, 1)]
ALL_DELTAS = [(-1, -1), (0, -1), (1, -1),
              (-1, 0), (0, 0), (1, 0),
              (-1, 1), (0, 1), (1, 1)]


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
            if vert_wall_slice[x][y] == WALL:
                continue
            if vert_floor_slice[x][y] == FLOOR:
                self.edge_exits.append((x, y))

        y = self.height - 1
        for x in range(self.width):
            if vert_wall_slice[x][y] == WALL:
                continue
            if vert_floor_slice[x][y] == FLOOR:
                self.edge_exits.append((x, y))

        x = 0
        for y in range(self.height):
            if vert_wall_slice[x][y] == WALL:
                continue
            if vert_floor_slice[x][y] == FLOOR:
                self.edge_exits.append((x, y))

        x = self.width - 1
        for y in range(self.height):
            if vert_wall_slice[x][y] == WALL:
                continue
            if vert_floor_slice[x][y] == FLOOR:
                self.edge_exits.append((x, y))

        for x in range(self.width):
            for y in range(self.height):
                if vert_wall_slice[x][y] == DOOR:
                    self.edge_exits.append((x, y))

    def finalize(self):
        self.find_edge_exits()


class Maze:
    def __init__(self, rooms, charmap, width=80, height=40,
                 rooms_on_level=20,
                 fixed_room_chance=10):

        self.width = width
        self.height = height
        self.charmap = charmap

        #
        # Set if we fail to generate
        #
        self.generate_failed = False

        #
        # All possible rooms we will choose from. Initially these are fixed
        # rooms and we add more random ones onto this list.
        #
        self.rooms = rooms

        #
        # First range of rooms are fixed; rest randomly generated
        #
        self.fixed_room_count = len(self.rooms)

        #
        # How many rooms on the level.
        #
        self.rooms_on_level = 0

        #
        # Chance of a corridor splitting
        #
        self.corridor_fork_chance = 35

        #
        # Lower, longer corridors
        #
        self.corridor_grow_chance = 2

        #
        # How often a random room is locked
        #
        self.room_locked_chance = 5

        #
        # How close corridors should be to each other
        #
        self.corridor_spacing = 3

        #
        # What chance for fixed versus random rooms
        #
        self.fixed_room_chance = fixed_room_chance

        #
        # For random shape rooms, how large?
        #
        self.min_room_size = 10

        #
        # Depth is how many rooms from the start room we are
        #
        self.roomno_depth = {}

        #
        # Rooms that have all doors locked
        #
        self.roomno_locked = {}

        #
        # Exits from each room
        #
        self.room_exits = {}

        #
        # The tiles of a each romm
        #
        self.room_occupiable_tiles = {}

        #
        # The map
        #
        self.cells = [[[' ' for d in range(Depth.max)]
                       for i in range(height)]
                      for j in range(width)]
        self.roomno_cells = [[-1 for i in range(height)]
                             for j in range(width)]

        #
        # Create all randomly shaped rooms.
        #
        for count in range(0, 2):
            self.rooms_all_create_random_shapes()

        #
        # Total of fixed and random room
        #
        self.fixed_roomno_list = list(range(0, self.fixed_room_count))
        random.shuffle(self.fixed_roomno_list)

        self.random_roomno_list = list(range(self.fixed_room_count,
                                             len(self.rooms)))
        random.shuffle(self.random_roomno_list)

        #
        # First room goes in the center. The rest hang off of its
        # corridors.
        #
        if not self.rooms_place_all(rooms_on_level):
            self.generate_failed = True
            return

        self.debug("^^^ placed all rooms ^^^")

        #
        # Remove dangling corridors that go nowhere.
        #
        self.rooms_trim_corridors()
        self.debug("^^^ trimmed all corridors ^^^")

        #
        # Remove corridors that go nowhere.
        #
        self.rooms_trim_looped_corridors()
        self.debug("^^^ trimmed all looped corridors ^^^")

        #
        # How far from the start is each room?
        #
        self.rooms_set_depth()
        self.debug("^^^ calculated depth ^^^")

#        self.rooms_dump_info()

        #
        # Randomly lock some rooms
        #
        self.rooms_randomly_lock()
        self.debug("^^^ locked some rooms ^^^")

        #
        # Redo the depth. Rooms that are locked add extra depth points.
        #
        self.rooms_set_depth()
        self.debug("^^^ calculated depth ^^^")

        #
        # Plug gaps in the wall that go nowhere.
        #
        self.rooms_plug_walls()
        self.debug("^^^ plugged all walls ^^^")

        #
        # Any dead end doors with no corridor, zap em
        #
        self.rooms_plug_doors()
        self.debug("^^^ removed dead end doors ^^^")

        self.rooms_find_occupiable_tiles()

        self.rooms_place_start()
        self.debug("^^^ placed start ^^^")

        self.rooms_place_exit()
        self.debug("^^^ placed exit ^^^")

    def debug(self, s):
        return
        self.dump()
        print(s)

    #
    # Puts a tile on the map
    #
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

    #
    # Gets a tile oof the map or None
    #
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

    #
    # Set a tile with a given roomno
    #
    def putr(self, x, y, r):
        if x >= self.width:
            return
        if y >= self.height:
            return
        if x < 0:
            return
        if y < 0:
            return
        self.roomno_cells[x][y] = r

    #
    # Get the roomno in this tile
    #
    def getr(self, x, y):
        if x >= self.width:
            return None
        if y >= self.height:
            return None
        if x < 0:
            return None
        if y < 0:
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

            if self.is_something_at(x, y):
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
    def flood_erase(self, x, y, depth, rchar):
        walked = [[0 for i in range(height)]
                  for j in range(width)]

        stack = [(x, y)]
        r = []
        while len(stack) > 0:
            x, y = stack.pop(0)
            if self.is_oob(x, y):
                continue

            if walked[x][y]:
                continue

            walked[x][y] = 1

            if not self.is_something_at(x, y):
                continue

            self.putc(x, y, depth, rchar)
            r.append((x, y))
            stack.append((x + 1, y))
            stack.append((x - 1, y))
            stack.append((x, y + 1))
            stack.append((x, y - 1))

        return r

    #
    # Find all adjacent characters of the same type.
    #
    def flood_find(self, x, y, func):

        walked = [[0 for i in range(height)]
                  for j in range(width)]

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

        walked = [[0 for i in range(height)]
                  for j in range(width)]

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

    def is_corridor_or_door_at(self, x, y):
        if self.is_corridor_at(x, y):
            return True
        if self.is_door_at(x, y):
            return True
        return False

    def is_wall_at(self, x, y):
        c = self.getc(x, y, Depth.wall)
        if c is not None:
            if "is_wall" in self.charmap[c]:
                return True
        return False

    def is_door_at(self, x, y):
        c = self.getc(x, y, Depth.wall)
        if c is not None:
            if "is_door" in self.charmap[c]:
                return True
        return False

    def is_obj_at(self, x, y):
        c = self.getc(x, y, Depth.obj)
        if c is not None:
            if "is_obj" in self.charmap[c]:
                return True
        return False

    #
    # Check for room overlaps
    #
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

        vert_floor_slice = room.vert_slice["floor"]

        for ry in range(room.height):
            for rx in range(room.width):
                if vert_floor_slice[rx][ry] == FLOOR:
                    if self.is_something_at(x + rx, y + ry):
                        return False
        return True

    #
    # Dump a room onto the level. No checks
    #
    def room_place(self, roomno, x, y):
        room = self.rooms[roomno]

        for d in range(Depth.max):
            dname = Depth.to_name[d]
            if dname in room.vert_slice:
                rvert_slice = room.vert_slice[dname]
                for ry in range(room.height):
                    for rx in range(room.width):
                        rchar = rvert_slice[rx][ry]
                        tx = x + rx
                        ty = y + ry
                        if rchar != SPACE:
                            self.putc(tx, ty, d, rchar)
                            self.putr(tx, ty, roomno)
                        if rchar == DOOR:
                            self.roomno_locked[roomno] = True

        self.rooms_on_level += 1

        #
        # Keeo track of what rooms we've added. We'll work out what joins
        # onto what later.
        #
        self.room_connection[roomno] = set()
        self.roomnos.add(roomno)

    #
    # Try to push a room on the level
    #
    def room_place_if_no_overlaps(self, roomno, x, y):
        if not self.room_can_be_placed(roomno, x, y):
            return False

        self.room_place(roomno, x, y)
        return True

    #
    # Grow a corridor in the given direction
    #
    def room_corridor_draw(self, x, y, dx, dy, clen=0, fork_count=0):
        x += dx
        y += dy

        if self.getc(x, y, Depth.floor) is None:
            return

        if self.is_something_at(x, y):
            return

        clen += 1

        self.putc(x, y, Depth.floor, CORRIDOR)

        #
        # Reached the end of a corridor?
        #
        if random.randint(1, 100) < clen * self.corridor_grow_chance:
            self.corridor_ends.append((x, y))
            return

        #
        # Stopped growing. Fork the corridor.
        # Don't do corridors forks adjacent to each other.
        #
        if fork_count < 3 and clen % 2 == 0:
            if random.randint(1, 100) < self.corridor_fork_chance:
                self.room_corridor_draw(x, y, dy, dx, clen, fork_count + 1)

            if random.randint(1, 100) < self.corridor_fork_chance:
                self.room_corridor_draw(x, y, -dy, -dx, clen, fork_count + 1)

        #
        # Keep on growing
        #
        self.room_corridor_draw(x, y, dx, dy, clen, fork_count + 1)

    #
    # Search the whole level for possible room exits
    #
    def rooms_find_all_exits(self):
        self.inuse = [[0 for i in range(self.height)]
                      for j in range(self.width)]
        cand = [[0 for i in range(self.height)]
                for j in range(self.width)]

        #
        # First pass find all the places we could place a corridor
        #
        border = self.corridor_spacing
        for y in range(border + 1, self.height - (border + 1)):
            for x in range(border + 1, self.width - (border + 1)):

                if not self.is_floor_at(x, y):
                    continue

                if self.is_wall_at(x, y):
                    self.inuse[x][y] = 1
                    continue

                if self.is_corridor_at(x, y):
                    self.inuse[x][y] = 1
                    continue

                if self.is_obj_at(x, y):
                    self.inuse[x][y] = 1
                    continue

                if not self.is_something_at(x + 1, y):
                    cand[x][y] = 1
                elif not self.is_something_at(x - 1, y):
                    cand[x][y] = 1
                elif not self.is_something_at(x, y - 1):
                    cand[x][y] = 1
                elif not self.is_something_at(x, y + 1):
                    cand[x][y] = 1

        possible_new_corridors = []

        #
        # Next pass filter all places we could start that are too near
        # to other corridors that already exist, or are candidates to
        # create.
        #
        for y in range(border + 1, self.height - (border + 1)):
            for x in range(border + 1, self.width - (border + 1)):

                if not cand[x][y]:
                    continue

                c = - self.corridor_spacing
                d = self.corridor_spacing + 1

                #
                # Check no corridor adjacent to any other one nearby. Or
                # any new one we plan.
                #
                skip = False
                for dx in range(c, d):
                    for dy in range(c, d):
                        if dx == 0 and dy == 0:
                            continue

                        if cand[x + dx][y + dy]:
                            self.inuse[x + dx][y + dy] = 1
                            skip = True
                            break

                        if self.is_corridor_at(x + dx, y + dy):
                            self.inuse[x + dx][y + dy] = 1
                            skip = True
                            break
                    if skip:
                        break
                if skip:
                    continue

                possible_new_corridors.append((x, y))

        #
        # Return the final list of corridor starts
        #
        return possible_new_corridors

    #
    # For each room exit (and we search the whole room) grow corridors
    #
    def rooms_all_grow_new_corridors(self):

        possible_new_corridors = self.rooms_find_all_exits()

        #
        # For each possible direction of a corridor, sprout one.
        #
        for coord in possible_new_corridors:
            x, y = coord

            # a b c
            # d * f
            # g h i
            b = self.inuse[x][y-1]
            d = self.inuse[x-1][y]
            f = self.inuse[x+1][y]
            h = self.inuse[x][y+1]

            if not b:
                self.room_corridor_draw(x, y, 0, - 1)
            if not d:
                self.room_corridor_draw(x, y, -1, 0)
            if not f:
                self.room_corridor_draw(x, y, 1, 0)
            if not h:
                self.room_corridor_draw(x, y, 0, 1)

    #
    # From a fixed list of random roomnos, return the next one. This
    # ensures no room will ever appear more than once.
    #
    def rooms_get_next_roomno(self):
        while True:
            if random.randint(0, 100) <= self.fixed_room_chance:
                roomno = self.fixed_roomno_list.pop(0)
                self.fixed_roomno_list.append(roomno)
            else:
                roomno = self.random_roomno_list.pop(0)
                self.random_roomno_list.append(roomno)

            #
            # Make sure we never place the same room twice.
            #
            if roomno not in self.roomnos:
                return roomno

        return roomno

    #
    # Search for corridor end points and try to dump rooms there.
    #
    def rooms_all_try_to_place_at_end_of_corridors(self):

        roomno = self.rooms_get_next_roomno()
        room = self.rooms[roomno]
        placed_a_room = False

        #
        # For all corridor end points.
        #
        for coord in self.corridor_ends:
            cx, cy = coord

            #
            # Try to attach room only by it's edges. This is a bit quicker
            # than searching the room for exits.
            #
            for edge in room.edge_exits:
                rx, ry = edge

                x = cx - rx
                y = cy - ry

                if self.room_place_if_no_overlaps(roomno, x - 1, y):
                    placed_a_room = True
                elif self.room_place_if_no_overlaps(roomno, x + 1, y):
                    placed_a_room = True
                elif self.room_place_if_no_overlaps(roomno, x, y - 1):
                    placed_a_room = True
                elif self.room_place_if_no_overlaps(roomno, x, y + 1):
                    placed_a_room = True

                if placed_a_room:
                    break

            if placed_a_room:
                break

        #
        # Placed at least one?
        #
        return placed_a_room

    #
    # Place the first room in a level, in the center ish. First room should
    # not be a fixed room.
    #
    def room_place_first(self):
        self.room_connection = {}

        room_place_tries = 0
        while True:
            room_place_tries += 1

            #
            # First room should not be fixed.
            #
            roomno = self.rooms_get_next_roomno()
            if roomno < self.fixed_room_count:
                room = self.rooms[roomno]
                if room.can_be_placed_as_first_room is not True:
                    if room_place_tries < 100:
                        continue

            room = self.rooms[roomno]
            x = int(width / 2)
            y = int(height / 2)
            room = self.rooms[roomno]
            x -= int(room.width / 2)
            y -= int(room.height / 2)

            if self.room_place_if_no_overlaps(roomno, x, y):
                self.roomno_first = roomno

                room = self.rooms[roomno]
                room.can_be_placed_as_start = True
                room.can_be_placed_as_exit = False
                return True

            if room_place_tries > 1000:
                print("Could not place first room")
                return False

    #
    # Place remaining rooms hanging off of the corridors of the last.
    #
    def rooms_place_remaining(self, rooms_on_level):
        self.corridor_ends = []
        self.rooms_all_grow_new_corridors()

        room_place_tries = 0
        while self.rooms_on_level < rooms_on_level:
            room_place_tries += 1
            if room_place_tries > rooms_on_level * 2:
                print("Tried to place rooms for too long, made {0} rooms".
                      format(self.rooms_on_level))
                return False

            #
            # If we place at least one new room, we will have new corridors
            # to grow.
            #
            if self.rooms_all_try_to_place_at_end_of_corridors():
                self.rooms_all_grow_new_corridors()

        return True

    #
    # Place all rooms
    #
    def rooms_place_all(self, rooms_on_level):
        self.roomnos = set()
        if not self.room_place_first():
            return False
        if not self.rooms_place_remaining(rooms_on_level):
            return False
        self.roomnos = sorted(self.roomnos)
        return True

    #
    # Remove dangling corridors that go nowhere.
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
    # Remove dangling corridors that go nowhere.
    #
    def rooms_trim_looped_corridors(self):

        walked = [[0 for i in range(height)]
                  for j in range(width)]

        for y in range(1, self.height - 1):
            for x in range(1, self.width - 1):
                if walked[x][y]:
                    continue

                if not self.is_corridor_at(x, y) and \
                   not self.is_door_at(x, y):
                    continue

                roomno = None
                corridor = self.flood_find(x, y, self.is_corridor_or_door_at)

                #
                # We've found all tiles in this corridor. Now find all rooms
                # adjacent to the corridor and connect many to many
                #
                rooms_adjoining_this_corridor = set()
                for c in corridor:
                    cx, cy = c

                    walked[cx][cy] = 1

                    for dx, dy in XY_DELTAS:
                        if self.is_wall_at(cx + dx, cy + dy):
                            continue

                        fx = cx + dx
                        fy = cy + dy

                        if not self.is_floor_at(fx, fy) and \
                           not self.is_door_at(fx, fy):
                            continue

                        #
                        # Keep track of all room exits so we can use them
                        # later to place doors perhaps
                        #
                        roomno = self.getr(fx, fy)
                        if roomno not in self.room_exits:
                            self.room_exits[roomno] = set()
                        self.room_exits[roomno].add((fx, fy))

                        #
                        # Add this room to the list of adjoining rooms
                        #
                        rooms_adjoining_this_corridor.add(roomno)

                #
                # Connect each room to each other to build a graph
                #
                for r in rooms_adjoining_this_corridor:
                    for n in rooms_adjoining_this_corridor:
                        self.room_connection[n].add(r)
                        self.room_connection[r].add(n)

                if len(rooms_adjoining_this_corridor) < 2:
                    self.flood_replace(x, y, Depth.floor, CORRIDOR, SPACE)

    #
    # Display room connectivity info
    #
    def rooms_dump_info(self):

        for r in self.roomnos:
            print("room {0} --> neighbor {1}".format(r,
                                                     self.room_connection[r]))
            for e in self.room_exits[r]:
                print("    exit at {0}".format(e))

    #
    # Lock some rooms - not the first room
    #
    def rooms_randomly_lock(self):

        for r in self.roomnos:
            if self.roomno_depth[r] > 0:
                if random.randint(0, 100) < self.room_locked_chance:
                    self.roomno_locked[r] = True
                    for e in self.room_exits[r]:
                        ex, ey = e
                        self.putc(ex, ey, Depth.wall, DOOR)

    #
    # Starting from the first room, do a breadth first search to find out
    # the depth of all rooms from the first.
    #
    def rooms_set_depth(self):

        roomno = self.roomno_first
        stack = [roomno]

        self.roomno_depth = {}
        self.roomno_depth[roomno] = 0

        while len(stack) > 0:
            roomno = stack.pop(0)

            #
            # Rooms are harder to get to
            #
            if roomno in self.roomno_locked:
                if self.roomno_locked[roomno]:
                    self.roomno_depth[roomno] = self.roomno_depth[roomno] + 2

            for neb in self.room_connection[roomno]:
                if neb not in self.roomno_depth:
                    stack.append(neb)
                    self.roomno_depth[neb] = self.roomno_depth[roomno] + 1

    #
    # Any rooms opening onto nothing, fill them in
    #
    def rooms_plug_walls(self):
        for y in range(self.height):
            for x in range(self.width):
                if not self.is_floor_at(x, y):
                    continue

                if self.is_door_at(x, y):
                    continue

                if self.is_wall_at(x, y):
                    continue

                for dx in range(-1, 2):
                    for dy in range(-1, 2):
                        if not self.is_something_at(x + dx, y + dy):
                            self.putc(x + dx, y + dy, Depth.wall, WALL)

    #
    # Any dead end doors with no corridor, zap em
    #
    def rooms_plug_doors(self):
        for y in range(self.height):
            for x in range(self.width):
                if not self.is_door_at(x, y):
                    continue

                ok = False
                for dx, dy in XY_DELTAS:
                    if self.is_corridor_at(x + dx, y + dy):
                        ok = True
                        break

                if not ok:
                    self.putc(x, y, Depth.wall, WALL)

    #
    # Find all tiles where we can place objects
    #
    def rooms_find_occupiable_tiles(self):
        for roomno in self.roomnos:
            self.room_occupiable_tiles[roomno] = []

        for y in range(self.height):
            for x in range(self.width):
                if not self.is_floor_at(x, y):
                    continue

                if self.is_wall_at(x, y):
                    continue

                roomno = self.getr(x, y)

                self.room_occupiable_tiles[roomno].append((x, y))

    #
    # Any dead end doors with no corridor, zap em
    #
    def rooms_place_start(self):
        while True:
            while True:
                roomno = random.choice(self.roomnos)
                room = self.rooms[roomno]

                if room.can_be_placed_as_start:
                    break

            x, y = random.choice(self.room_occupiable_tiles[roomno])

            obstacle = False
            for dx, dy in ALL_DELTAS:
                if self.is_wall_at(x + dx, y + dy):
                    obstacle = True
                    break

            if obstacle:
                continue

            self.putc(x, y, Depth.wall, START)
            break

    #
    # Any dead end doors with no corridor, zap em
    #
    def rooms_place_exit(self):
        while True:
            while True:
                roomno = random.choice(self.roomnos)
                room = self.rooms[roomno]

                if room.can_be_placed_as_exit:
                    break

            x, y = random.choice(self.room_occupiable_tiles[roomno])

            obstacle = False
            for dx, dy in ALL_DELTAS:
                if self.is_wall_at(x + dx, y + dy):
                    obstacle = True
                    break

            if obstacle:
                continue

            self.putc(x, y, Depth.wall, EXIT)
            break

    #
    # Make randomly shaped rooms
    #
    # We use the map as a scratchpad for creating the room.
    #
    def rooms_all_create_random_shapes(self):
        cnt = 0

        #
        # First draw some random lines
        #
        while cnt < 10:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = random.randint(-10, self.width + 10)
            y2 = random.randint(-10, self.height + 10)
            self.line_draw((x1, y1), (x2, y2), Depth.floor, FLOOR)
            cnt += 1
        #
        # Next draw straight across lines.
        #
        cnt = 0
        while cnt < 10:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = x1 + 100
            y2 = y1
            self.line_draw((x1, y1), (x2, y2), Depth.floor, FLOOR)
            cnt += 1
        #
        # Next draw straight down lines.
        #
        cnt = 0
        while cnt < 10:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = x1
            y2 = y1 + 100
            self.line_draw((x1, y1), (x2, y2), Depth.floor, FLOOR)
            cnt += 1
        #
        # Next randomly fill in with floor tiles. This will make large
        # patches of connected floor tiles.
        #
        cnt = 0
        while cnt < 10:
            x = random.randint(0, self.width - 1)
            y = random.randint(0, self.height - 1)
            self.flood_fill(x, y, Depth.floor, FLOOR)
            cnt += 1
        #
        # Now carve out some empty regions. We could just do smaller
        # regions above, but somehow this looks better.
        #
        cnt = 0
        while cnt < 10:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = random.randint(-10, self.width + 10)
            y2 = random.randint(-10, self.height + 10)
            self.line_draw((x1, y1), (x2, y2), Depth.floor, SPACE)
            cnt += 1
        cnt = 0
        while cnt < 10:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = x1 + 100
            y2 = y1
            self.line_draw((x1, y1), (x2, y2), Depth.floor, SPACE)
            cnt += 1
        cnt = 0
        while cnt < 10:
            x1 = random.randint(-10, self.width + 10)
            y1 = random.randint(-10, self.height + 10)
            x2 = x1
            y2 = y1 + 100
            self.line_draw((x1, y1), (x2, y2), Depth.floor, SPACE)
            cnt += 1
        #
        # Now pull each room out of the level with a kind of inverse
        # flood fill.
        #
        cnt = 0
        for y in range(self.height):
            for x in range(self.width):
                if self.is_floor_at(x, y):
                    r = self.flood_erase(x, y, Depth.floor, SPACE)
                    cnt += 1
                    #
                    # Filter to rooms of a certain size.
                    #
                    if len(r) < self.min_room_size:
                        continue

                    #
                    # Find the size of this random room.
                    #
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

                    #
                    # Now we need to create the floor and wall vertical
                    # room slices.
                    #
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

                    vert_floor_slice = copy.deepcopy(rcells)
                    vert_wall_slice = copy.deepcopy(rcells)
                    vert_obj_slice = [[' ' for i in range(rh)]
                                      for j in range(rw)]

                    for ry in range(0, rh):
                        for rx in range(0, rw):
                            if vert_floor_slice[rx][ry] == WALL:
                                vert_floor_slice[rx][ry] = FLOOR
                            if vert_wall_slice[rx][ry] == FLOOR:
                                vert_wall_slice[rx][ry] = SPACE
                            if vert_wall_slice[rx][ry] == WALL:
                                if rx % 2 == 0 and ry % 2 == 0:
                                    if random.randint(0, 100) < 50:
                                        vert_wall_slice[rx][ry] = SPACE
                    #
                    # To dump the room:
                    #
                    if False:
                        for ry in range(rh):
                            for rx in range(rw):
                                sys.stdout.write(vert_floor_slice[rx][ry])
                            print()

                        for ry in range(rh):
                            for rx in range(rw):
                                sys.stdout.write(vert_wall_slice[rx][ry])
                            print()

                    #
                    # Add to the rooms list.
                    #
                    r = Room()
                    r.vert_slice_add("floor", vert_floor_slice)
                    r.vert_slice_add("wall", vert_wall_slice)
                    r.vert_slice_add("obj", vert_obj_slice)
                    r.finalize()
                    r.can_be_placed_as_exit = True
                    self.rooms.append(r)

        #
        # Zero out the map as we were lazy and used it for a scratchpad
        # when creating rooms.
        #
        self.cells = [[[' ' for d in range(Depth.max)]
                       for i in range(height)]
                      for j in range(width)]

    def dump(self):
        from colored import fg, bg, attr

        os.system('cls' if os.name == 'nt' else 'clear')
        for y in range(self.height):
            for x in range(self.width):
                for d in reversed(range(Depth.max)):
                    c = self.cells[x][y][d]
                    charmap = self.charmap[c]
                    fg_name = charmap["fg"]
                    bg_name = charmap["bg"]
                    if c != " ":
                        break

                res = attr('reset')
                if c == FLOOR:
                    r = self.roomno_cells[x][y]
                    if r == -1:
                        c = "!"
                        color = fg("white") + bg("red")
                    else:
                        color = fg(r % 255) + bg(0)

                    r = self.getr(x, y)
                    if r is None:
                        print("No room depth at {0},{1}".format(x, y))
                    else:
                        if r in self.roomno_depth:
                            d = self.roomno_depth[r]
                            c = chr(ord('0') + d)
                else:
                    color = fg(fg_name) + bg(bg_name)
                sys.stdout.write(color + c + res)
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


def maze_create_fixed_rooms():
    rooms = []

    r = Room()
    r.vert_slice_add("floor", [
                    "......",
                    "......",
                    "......",
                    "......",
                    "......",
            ])
    r.vert_slice_add("wall", [
                    "xxDxxx",
                    "x    D",
                    "D    x",
                    "x    x",
                    "xxxDxx",
            ])

    r.vert_slice_add("obj", [
                    "      ",
                    "   o  ",
                    "      ",
                    "      ",
                    "      ",
            ])
    r.finalize()
    rooms.append(r)

    r = Room()
    r.vert_slice_add("floor", [
                    "..........",
                    "..........",
                    "..........",
                    "..........",
                    "..........",
                    "..........",
                    "..........",
            ])
    r.vert_slice_add("wall", [
                    "xxDxxxxxxx",
                    "x  x     D",
                    "x  x   xxx",
                    "x        x",
                    "x        x",
                    "x        x",
                    "xxxxxxxxxx",
            ])

    r.vert_slice_add("obj", [
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
    r.vert_slice_add("floor", [
                    "   .......",
                    "   .......",
                    "..........",
                    "..........",
                    "..........",
                    ".......   ",
                    ".......   ",
            ])
    r.vert_slice_add("wall", [
                    "   xxxxxxx",
                    "   x     D",
                    "xxxx     x",
                    "D        x",
                    "x     xxxx",
                    "x     x   ",
                    "xxxxxxx   ",
            ])

    r.vert_slice_add("obj", [
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
    r.vert_slice_add("floor", [
                    "  ....  ",
                    " ...... ",
                    "........",
                    "........",
                    "........",
                    " .......",
                    "  ......",
            ])
    r.vert_slice_add("wall", [
                    "  xxDx  ",
                    " xx  xx ",
                    "xx    xx",
                    "D      D",
                    "xx     x",
                    " xx    x",
                    "  xDxxxx",
            ])

    r.vert_slice_add("obj", [
                    "        ",
                    "        ",
                    "        ",
                    "        ",
                    "        ",
                    "        ",
                    "        ",
            ])
    r.finalize()
    rooms.append(r)

    r = Room()
    r.vert_slice_add("floor", [
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
    r.vert_slice_add("wall", [
                    "xxDx  xxxx   ",
                    "x  xxxx  xxxx",
                    "xx          D",
                    " x          x",
                    " x          x",
                    "xx          x",
                    "D          xx",
                    "x          x ",
                    "x  xxxxx   x ",
                    "xxxx   xxxDx ",
            ])

    r.vert_slice_add("obj", [
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

    r = Room()
    r.vert_slice_add("floor", [
                    ".....   .....",
                    ".............",
                    ".............",
                    ".............",
                    "   .......   ",
                    "   .......   ",
                    "   .......   ",
                    ".............",
                    ".............",
                    ".............",
                    ".....   .....",
            ])
    r.vert_slice_add("wall", [
                    "xxxxx   xxxxx",
                    "x   xxDxx   x",
                    "x           x",
                    "xxxx     xxxx",
                    "   x     x   ",
                    "   D     D   ",
                    "   x     x   ",
                    "xxxx     xxxx",
                    "x           x",
                    "x   xxDxx   x",
                    "xxxxx   xxxxx",
            ])

    r.vert_slice_add("obj", [
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
                    "             ",
            ])
    r.can_be_placed_as_first_room = True
    r.finalize()
    rooms.append(r)

    return rooms

for seed in range(1, 1000):
    width = 64
    height = 64

    maze_seed = seed

    while True:
        fixed_rooms = maze_create_fixed_rooms()
        random.seed(maze_seed)
#        random.seed(1)

        maze = Maze(width=width, height=height, rooms=fixed_rooms,
                    rooms_on_level=10,
                    charmap=charmap,
                    fixed_room_chance=10)
        if not maze.generate_failed:
            break

        maze_seed += 1
        maze_seed *= maze_seed

    print("Seed {0}".format(seed))
    maze.dump()
#    break
