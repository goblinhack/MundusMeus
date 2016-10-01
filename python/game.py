import world
import util
import mm
import wid_map
import thing
import maze
import rooms
import random

global g


class Game:

    def __init__(self):
        self.world = world.World(0)

        (self.width, self.height) = (mm.MAP_WIDTH, mm.MAP_HEIGHT)

        #
        # Create the world
        #
        p = util.Xyz(0, 0, 0)
        self.world.push_level(p)
        self.level = self.world.get_level()
        self.level.set_dim(self.width, self.height)

        #
        # And not a maze at that point in the world
        #
        self.maze_create(4)

        self.map_wid_create()

        self.maze_populate_level()

    def destroy(self):
        self.world.destroy()

    #
    # The scrollable map for the level
    #
    def map_wid_create(self):
        self.wid_map = wid_map.WidMap(mm.MAP_WIDTH, mm.MAP_HEIGHT)

    #
    # Create a rendom maze
    #
    def maze_create(self, seed):
        self.maze_seed = seed

        while True:
            fixed_rooms = rooms.create_fixed()
            random.seed(self.maze_seed)

            self.maze_seed += 1
            self.maze_seed *= self.maze_seed

            self.level.maze = maze.Maze(width=self.width,
                                        height=self.height,
                                        rooms=fixed_rooms,
                                        rooms_on_level=15,
                                        fixed_room_chance=10)

            if not self.level.maze.generate_failed:
                break

        self.level.maze.dump()

    #
    # For the current maze, put tiles on the map
    #
    def maze_populate_level(self):
        m = self.level.maze

        for y in range(1, mm.MAP_HEIGHT - 1):
            for x in range(1, mm.MAP_WIDTH - 1):

                nothing_placed_here = True
                place_stalactite = False

                if m.is_floor_at(x, y):
                    place_stalactite = True
                    nothing_placed_here = False
                    t = thing.Thing(self.level, tp_name="floor1")
                    t.push(x, y)

                    if not m.is_wall_at(x, y):
                        if random.randint(0, 100) < 5:
                            t = thing.Thing(self.level, tp_name="jellycube1")
                            t.push(x, y)

                if m.is_start_at(x, y):
                    t = thing.Thing(self.level, tp_name="player1")
                    t.push(x, y)
                    nothing_placed_here = False

                if m.is_wall_at(x, y):
                    place_stalactite = True
                    nothing_placed_here = False
                    t = thing.Thing(self.level, tp_name="wall1")
                    t.push(x, y)

                    if m.is_wall_at(x, y-1):
                        b = True
                    else:
                        b = False

                    if m.is_wall_at(x-1, y):
                        d = True
                    else:
                        d = False

                    if m.is_wall_at(x+1, y):
                        f = True
                    else:
                        f = False

                    if m.is_wall_at(x, y+1):
                        h = True
                    else:
                        h = False

                    # a b c
                    # d e f
                    # g h i
                    if b and d and f and h:
                        t.set_tilename("wall1-x")
                    elif b and d and f:
                        t.set_tilename("wall1-t180")
                    elif b and d and h:
                        t.set_tilename("wall1-t90")
                    elif b and f and h:
                        t.set_tilename("wall1-t270")
                    elif d and f and h:
                        t.set_tilename("wall1-t")
                    elif b and h:
                        t.set_tilename("wall1-up-down")
                    elif d and f:
                        t.set_tilename("wall1-left-right")
                    elif b and f:
                        t.set_tilename("wall1-l")
                    elif h and f:
                        t.set_tilename("wall1-l90")
                    elif d and h:
                        t.set_tilename("wall1-l180")
                    elif b and d:
                        t.set_tilename("wall1-l270")
                    elif b:
                        t.set_tilename("wall1-n180")
                    elif f:
                        t.set_tilename("wall1-n270")
                    elif h:
                        t.set_tilename("wall1-n")
                    elif d:
                        t.set_tilename("wall1-n90")
                    else:
                        t.set_tilename("wall1-node")

                if m.is_cwall_at(x, y):
                    t = thing.Thing(self.level, tp_name="cwall1")
                    t.push(x, y)

                    if m.is_cwall_at(x, y-1):
                        b = True
                    else:
                        b = False

                    if m.is_cwall_at(x-1, y):
                        d = True
                    else:
                        d = False

                    if m.is_cwall_at(x+1, y):
                        f = True
                    else:
                        f = False

                    if m.is_cwall_at(x, y+1):
                        h = True
                    else:
                        h = False

                    # a b c
                    # d e f
                    # g h i
                    if b and d and f and h:
                        t.set_tilename("cwall1-x")
                    elif b and d and f:
                        t.set_tilename("cwall1-t180")
                    elif b and d and h:
                        t.set_tilename("cwall1-t90")
                    elif b and f and h:
                        t.set_tilename("cwall1-t270")
                    elif d and f and h:
                        t.set_tilename("cwall1-t")
                    elif b and h:
                        t.set_tilename("cwall1-up-down")
                    elif d and f:
                        t.set_tilename("cwall1-left-right")
                    elif b and f:
                        t.set_tilename("cwall1-l")
                    elif h and f:
                        t.set_tilename("cwall1-l90")
                    elif d and h:
                        t.set_tilename("cwall1-l180")
                    elif b and d:
                        t.set_tilename("cwall1-l270")
                    elif b:
                        t.set_tilename("cwall1-n180")
                    elif f:
                        t.set_tilename("cwall1-n270")
                    elif h:
                        t.set_tilename("cwall1-n")
                    elif d:
                        t.set_tilename("cwall1-n90")
                    else:
                        t.set_tilename("cwall1-node")
                    nothing_placed_here = False

                if m.is_corridor_at(x, y):
                    place_stalactite = True
                    nothing_placed_here = False
                    t = thing.Thing(self.level, tp_name="corridor1")
                    t.set_depth(m.bridge_height[x][y])
                    t.push(x, y)

                if m.is_water_at(x, y):
                    nothing_placed_here = False

                    if m.is_floor_at(x, y) or \
                       m.is_corridor_at(x, y):
                        #
                        # Underground water
                        #
                        t = thing.Thing(self.level, tp_name="water1")
                        t.push(x, y)

                    elif m.is_wall_at(x, y - 1) or m.is_rock_at(x, y - 1):
                        t = thing.Thing(self.level, tp_name="water1-top")
                        t.push(x, y)

                    elif m.is_corridor_at(x, y - 1) or m.is_cwall_at(x, y - 1):
                        t = thing.Thing(self.level, tp_name="water1-top")
                        t.push(x, y)

                    else:
                        t = thing.Thing(self.level, tp_name="water1")
                        t.push(x, y)

                    t.set_depth(m.depth_map.cells[x][y])

                if m.is_lava_at(x, y):
                    nothing_placed_here = False

                    if m.is_floor_at(x, y) or \
                       m.is_corridor_at(x, y):
                        #
                        # Underground lava
                        #
                        t = thing.Thing(self.level, tp_name="lava1")
                        t.push(x, y)

                    elif m.is_wall_at(x, y - 1) or m.is_rock_at(x, y - 1):
                        t = thing.Thing(self.level, tp_name="lava1-top")
                        t.push(x, y)

                    elif m.is_corridor_at(x, y - 1) or m.is_cwall_at(x, y - 1):
                        t = thing.Thing(self.level, tp_name="lava1-top")
                        t.push(x, y)

                    else:
                        t = thing.Thing(self.level, tp_name="lava1")
                        t.push(x, y)

                    t.set_depth(m.depth_map.cells[x][y])

                if m.is_rock_at(x, y):
                    nothing_placed_here = False
                    place_stalactite = True
                    t = thing.Thing(self.level, tp_name="rock1")
                    t.push(x, y)

                if m.is_chasm_at(x, y):
                    if nothing_placed_here:
                        if m.is_chasm_at(x - 1, y - 1) and \
                           m.is_chasm_at(x, y - 1) and \
                           m.is_chasm_at(x + 1, y - 1) and \
                           m.is_chasm_at(x - 1, y) and \
                           m.is_chasm_at(x + 1, y) and \
                           m.is_chasm_at(x - 1, y + 1) and \
                           m.is_chasm_at(x, y + 1) and \
                           m.is_chasm_at(x + 1, y + 1):
                            if random.randint(0, 100) < 5:
                                t = thing.Thing(self.level, tp_name="floor1")
                                t.push(x, y)
                                nothing_placed_here = False
                                place_stalactite = True
                            else:
                                continue
                        else:
                            continue
                    else:
                        continue

                if place_stalactite:
                    t = thing.Thing(self.level, tp_name="stalactite1")
                    t.push(x, y)

g = None


def game_new():
    global g

    g = Game()
