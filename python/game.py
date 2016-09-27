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
        self.maze_create(3955)

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

        for y in range(0, mm.MAP_HEIGHT):
            for x in range(0, mm.MAP_WIDTH):

                if m.is_floor_at(x, y):
                    t = thing.Thing(self.level, tp_name="floor1")
                    t.push(x, y)

                    if m.is_wall_at(x, y):
                        t = thing.Thing(self.level, tp_name="wall1")
                        t.push(x, y)
                    else:
                        if random.randint(0, 100) < 5:
                            t = thing.Thing(self.level, tp_name="jellycube1")
                            t.push(x, y)

                elif m.is_cwall_at(x, y):
                    t = thing.Thing(self.level, tp_name="cwall1")
                    t.push(x, y)

                elif m.is_corridor_at(x, y):
                    t = thing.Thing(self.level, tp_name="corridor1")
                    t.push(x, y)

#                else:
#                    c = m.getc(x, y, maze.Depth.under)
#                    if c == maze.CHASM:
#                        t = thing.Thing(self.level, tp_name="chasm1")
#                        t.push(x, y)
#                    elif c == maze.LAVA:
#                        pass
#                    elif c == maze.WATER:
#                        pass
#                    else:
#                        t = thing.Thing(self.level, tp_name="dirt1")
#                        t.push(x, y)

        t = thing.Thing(self.level, tp_name="player1")
        t.push(30, 30)

g = None


def game_new():
    global g

    g = Game()
