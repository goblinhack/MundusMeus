import world
import level
import thing
import util
import mm
import random

global g

class Game:
    def __init__ (self):
        w = self.world = world.World(0)

        (width, height) = mm.tex_size("map")

        p = util.Xyz(0,0,0)
        w.push_level(p)
        l = w.get_level()
        l.set_dim(width, height)
        
        p2 = util.Xyz(0,0,1)
        w.push_level(p2)
        l = w.get_level()
        l.set_dim(width, height)

        for y in range(0, height):
            for x in range(0, width):

                (r, g, b, a) = mm.tex_pixel("map", x, y)

                is_snow         = False
                is_grass        = False
                is_castle       = False
                is_sea          = False
                is_settlement   = False
                is_forest       = False

                if r == 0 and g == 0 and b == 0: 
                    is_snow = True
                elif r == 0 and g == 0 and b == 128: 
                    is_sea = True
                elif r == 0 and g == 0 and b == 255: 
                    is_sea = True
                elif r == 0 and g == 128 and b == 128: 
                    is_sand = True
                elif r == 0 and g == 128 and b == 0: 
                    is_grass = True
                elif r == 0 and g == 128 and b == 255: 
                    is_grass = True
                elif r == 0 and g == 255 and b == 0: 
                    is_grass = True
                elif r == 0 and g == 255 and b == 128: 
                    is_settlement = True
                elif r == 0 and g == 255 and b == 255: 
                    is_sea = True
                elif r == 128 and g == 0 and b == 0: 
                    is_sand = True
                elif r == 128 and g == 0 and b == 128: 
                    is_forest = True
                elif r == 128 and g == 0 and b == 255: 
                    is_sand = True
                elif r == 128 and g == 128 and b == 0: 
                    is_sand = True
                elif r == 128 and g == 128 and b == 128: 
                    is_road = True
                elif r == 128 and g == 128 and b == 255: 
                    is_sand = True
                elif r == 128 and g == 255 and b == 0: 
                    is_castle = True
                elif r == 128 and g == 255 and b == 128: 
                    is_snow = True
                elif r == 128 and g == 255 and b == 255: 
                    is_snow = True
                elif r == 255 and g == 128 and b == 0: 
                    is_grass = True
                elif r == 255 and g == 128 and b == 128: 
                    is_grass = True
                elif r == 255 and g == 128 and b == 255: 
                    is_grass = True
                elif r == 255 and g == 255 and b == 0: 
                    is_grass = True
                elif r == 255 and g == 255 and b == 128: 
                    is_sand = True
                elif r == 255 and g == 255 and b == 255: 
                    is_snow = True

                if (x % 9) == 0 and (y % 9) == 0:
                    t = thing.Thing(level=l, tp_name="sea1")
                    t.push(x, y)
                    
                if x == width / 2 and y == height / 2:
                    t = thing.Thing(level=l, tp_name="bigmap")
                    t.push(x, y)

                if  random.randint(1, 1000) < 50:
                    t = thing.Thing(level=l, tp_name="player1")
                    t.push(x, y)
    #        w.destroy()
        print("done")

def game_new():
    g = Game()
