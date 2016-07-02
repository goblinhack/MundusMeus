import world
import level
import thing
import util
import mm

global g

class Game:
    def __init__ (self):
        w = self.world = world.World(0)

        p = util.Xyz(0,0,0)
        w.push_level(p)
        l = w.get_level()
        l.set_dim(256, 256)
        
        p2 = util.Xyz(0,0,1)
        w.push_level(p2)
        l = w.get_level()
        l.set_dim(256, 256)
        
        for i in range(0,3):
            t = thing.Thing(level=l, tp_name="grass1")
            t.push(10 + 1, 10 + 1)

def game_new():
    mm.con("new game")
    g = Game()
