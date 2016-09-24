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

    def map_wid_create(self):
        self.wid_map = wid_map.WidMap(mm.MAP_WIDTH, mm.MAP_HEIGHT)

        for y in range(0, mm.MAP_HEIGHT, 5):
            for x in range(0, mm.MAP_WIDTH, 5):
                t = thing.Thing(self.level, tp_name="floor1")
                t.push(x, y)
        t = thing.Thing(self.level, tp_name="player1")
        t.push(10, 10)

    def destroy(self):
        self.world.destroy()

g = None


def game_new():
    global g

    g = Game()
