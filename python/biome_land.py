import random
import mm
import thing
import biome_land_do
import tp


def biome_build(self, seed):
    self.biome_seed = seed

    while True:
        random.seed(self.biome_seed)

        self.biome_seed += 1
        self.biome_seed *= self.biome_seed

        self.level.biome = biome_land_do.Biome(width=self.width,
                                               height=self.height)

        if not self.level.biome.generate_failed:
            break

    self.level.biome.dump(biome_land_do.charmap.depth.floor)
    self.level.biome.dump()


def biome_populate(self):
    m = self.level.biome

    for y in range(1, mm.MAP_HEIGHT - 1):
        for x in range(1, mm.MAP_WIDTH - 1):

            grass = False

            if m.is_grass_at(x, y):
                grass = True

            if m.is_tree_at(x, y):
                grass = True

                r = tp.get_random_tree()
                t = thing.Thing(self.level, tp_name=r.short_name)
                t.push(x, y)

                if m.is_grass_at(x, y):

                    if random.randint(0, 100) < 5:
                        r = tp.get_random_plant()
                        t = thing.Thing(self.level, tp_name=r.short_name)
                        t.push(x, y)

            if grass:
                which = None
                if m.is_water_at(x + 1, y):
                    which = "grass1-left"
                if m.is_water_at(x - 1, y):
                    which = "grass1-right"
                if m.is_water_at(x, y + 1):
                    which = "grass1-top"
                if m.is_water_at(x, y - 1):
                    which = "grass1-bot"
                if m.is_water_at(x + 1, y + 1):
                    which = "grass1-tl"
                if m.is_water_at(x - 1, y + 1):
                    which = "grass1-tr"
                if m.is_water_at(x + 1, y - 1):
                    which = "grass1-bl"
                if m.is_water_at(x - 1, y - 1):
                    which = "grass1-br"

                if which is not None:
                    t = thing.Thing(self.level, tp_name="grass1")
                    t.push(x, y)
                    t.set_tilename(which)

            if m.is_start_at(x, y):
                t = thing.Thing(self.level, tp_name="start1")
                t.push(x, y)

                t = thing.Thing(self.level, tp_name="player1")
                t.push(x, y)
                self.player = t

            if m.is_wall_at(x, y):
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

            if m.is_water_at(x, y):

                water = "water1"
                put_treasure = False

                if random.randint(0, 100) < 5:
                    put_treasure = True
                    water = "water1-trans"

                #
                # Underground water
                #
                t = thing.Thing(self.level, tp_name=water)
                t.push(x, y)

                if put_treasure:
                    toughness = 1
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(self.level, tp_name=r.short_name)
                    t.push(x, y)

            if m.is_rock_at(x, y):
                t = thing.Thing(self.level, tp_name="rock1")
                t.push(x, y)

                if random.randint(0, 100) < 5:
                    toughness = 1
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(self.level, tp_name=r.short_name)
                    t.push(x, y)

            if m.is_treasure_at(x, y):
                toughness = self.level.xyz.z

                r = tp.get_random_treasure(toughness=1)
                t = thing.Thing(self.level, tp_name=r.short_name)
                t.push(x, y)
