import world
import mm

global g

class Game:
    def __init__ (self):
        self.world = world.World(0)

def game_new():
    mm.con("new game")
    g = Game()
