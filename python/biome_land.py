import random
import mm
import thing
import biome_land_do
import tp


def biome_build(self, level=None, seed=0):
    self.biome_seed = seed

    while True:
        random.seed(self.biome_seed)

        self.biome_seed += 1
        self.biome_seed *= self.biome_seed

        self.level.biome = biome_land_do.Biome(level=level,
                                               width=self.width,
                                               height=self.height)

        if not self.level.biome.generate_failed:
            break

    self.level.biome.dump()


def biome_populate(self):
    m = self.level.biome

    snow_count = 0
    poor_soil_count = 0

    for y in range(0, mm.MAP_HEIGHT):
        for x in range(0, mm.MAP_WIDTH):
            if m.is_snow_at(x, y):
                snow_count += 1

            if not m.is_grass_at(x, y):
                poor_soil_count += 1

    threshold = mm.MAP_WIDTH * mm.MAP_HEIGHT

    is_snowy = False
    if snow_count > 0:
        is_snowy = True

    is_poor_soil = False
    if poor_soil_count > threshold / 2:
        is_poor_soil = True

    for y in range(0, mm.MAP_HEIGHT):
        for x in range(0, mm.MAP_WIDTH):

            grass1 = False
            if m.is_grass_at(x, y):
                grass1 = True

            dirt1 = False
            if m.is_dirt_at(x, y):
                dirt1 = True

            sand1 = False
            if m.is_sand_at(x, y):
                sand1 = True

            gravel1 = False
            if m.is_gravel_at(x, y):
                gravel1 = True

            snow1 = False
            if m.is_snow_at(x, y):
                snow1 = True

            ice1 = False
            if m.is_ice_at(x, y):
                ice1 = True

            if m.is_tree_at(x, y):
                if is_snowy:
                    r = tp.get_random_tree_snow()
                    t = thing.Thing(self.level, tp_name=r.short_name)
                    t.push(x, y)
                elif is_poor_soil:
                    r = tp.get_random_tree_conifer()
                    t = thing.Thing(self.level, tp_name=r.short_name)
                    t.push(x, y)
                else:
                    r = tp.get_random_tree()
                    t = thing.Thing(self.level, tp_name=r.short_name)
                    t.push(x, y)

            if grass1:
                t = thing.Thing(self.level, tp_name="grass1")
                t.push(x, y)

                if m.is_water_at(x - 1, y) or \
                   m.is_water_at(x + 1, y) or \
                   m.is_water_at(x, y - 1) or \
                   m.is_water_at(x, y + 1):

                    r = tp.get_random_marsh_plant()
                    for i in range(1, random.randint(1, 10)):
                        t = thing.Thing(self.level, tp_name=r.short_name)
                        t.push(x, y)
                    if random.randint(0, 100) < 10:
                        r = tp.get_random_plant()
                        t = thing.Thing(self.level, tp_name=r.short_name)
                        t.push(x, y)
                else:
                    if random.randint(0, 100) < 10:
                        r = tp.get_random_plant()
                        for i in range(1, random.randint(1, 10)):
                            t = thing.Thing(self.level, tp_name=r.short_name)
                            t.push(x, y)

            if dirt1:
                t = thing.Thing(self.level, tp_name="dirt1")
                t.push(x, y)

                if random.randint(0, 100) < 5:
                    for i in range(1, random.randint(1, 10)):
                        r = tp.get_random_plant()
                        t = thing.Thing(self.level, tp_name=r.short_name)
                        t.push(x, y)

            if sand1:
                t = thing.Thing(self.level, tp_name="sand1")
                t.push(x, y)

                if not grass1:
                    if random.randint(0, 1000) < 5:
                        r = tp.get_random_small_rock()
                        t = thing.Thing(self.level, tp_name=r.short_name)
                        t.push(x, y)

            if gravel1:
                t = thing.Thing(self.level, tp_name="gravel1")
                t.push(x, y)

                if random.randint(0, 1000) < 50:
                    r = tp.get_random_small_rock()
                    t = thing.Thing(self.level, tp_name=r.short_name)
                    t.push(x, y)

            if ice1:
                t = thing.Thing(self.level, tp_name="ice1")
                t.push(x, y)

            if snow1:
                t = thing.Thing(self.level, tp_name="snow1")
                t.push(x, y)

                if random.randint(0, 1000) < 50:
                    r = tp.get_random_snow_mound()
                    t = thing.Thing(self.level, tp_name=r.short_name)
                    t.push(x, y)

            if not grass1:
                if m.is_grass_at(x - 1, y):
                    t = thing.Thing(self.level, tp_name="grass1_deco")
                    t.push(x, y)
                    t.set_tilename("grass1-right")
                if m.is_grass_at(x + 1, y):
                    t = thing.Thing(self.level, tp_name="grass1_deco")
                    t.push(x, y)
                    t.set_tilename("grass1-left")
                if m.is_grass_at(x, y - 1):
                    t = thing.Thing(self.level, tp_name="grass1_deco")
                    t.push(x, y)
                    t.set_tilename("grass1-bot")
                if m.is_grass_at(x, y + 1):
                    t = thing.Thing(self.level, tp_name="grass1_deco")
                    t.push(x, y)
                    t.set_tilename("grass1-top")
                if m.is_grass_at(x - 1, y - 1):
                    t = thing.Thing(self.level, tp_name="grass1_deco")
                    t.push(x, y)
                    t.set_tilename("grass1-br")
                if m.is_grass_at(x + 1, y - 1):
                    t = thing.Thing(self.level, tp_name="grass1_deco")
                    t.push(x, y)
                    t.set_tilename("grass1-bl")
                if m.is_grass_at(x - 1, y + 1):
                    t = thing.Thing(self.level, tp_name="grass1_deco")
                    t.push(x, y)
                    t.set_tilename("grass1-tr")
                if m.is_grass_at(x + 1, y + 1):
                    t = thing.Thing(self.level, tp_name="grass1_deco")
                    t.push(x, y)
                    t.set_tilename("grass1-tl")

            if not dirt1:
                if m.is_dirt_at(x - 1, y):
                    t = thing.Thing(self.level, tp_name="dirt1_deco")
                    t.push(x, y)
                    t.set_tilename("dirt1-right")
                if m.is_dirt_at(x + 1, y):
                    t = thing.Thing(self.level, tp_name="dirt1_deco")
                    t.push(x, y)
                    t.set_tilename("dirt1-left")
                if m.is_dirt_at(x, y - 1):
                    t = thing.Thing(self.level, tp_name="dirt1_deco")
                    t.push(x, y)
                    t.set_tilename("dirt1-bot")
                if m.is_dirt_at(x, y + 1):
                    t = thing.Thing(self.level, tp_name="dirt1_deco")
                    t.push(x, y)
                    t.set_tilename("dirt1-top")
                if m.is_dirt_at(x - 1, y - 1):
                    t = thing.Thing(self.level, tp_name="dirt1_deco")
                    t.push(x, y)
                    t.set_tilename("dirt1-br")
                if m.is_dirt_at(x + 1, y - 1):
                    t = thing.Thing(self.level, tp_name="dirt1_deco")
                    t.push(x, y)
                    t.set_tilename("dirt1-bl")
                if m.is_dirt_at(x - 1, y + 1):
                    t = thing.Thing(self.level, tp_name="dirt1_deco")
                    t.push(x, y)
                    t.set_tilename("dirt1-tr")
                if m.is_dirt_at(x + 1, y + 1):
                    t = thing.Thing(self.level, tp_name="dirt1_deco")
                    t.push(x, y)
                    t.set_tilename("dirt1-tl")

            if not sand1:
                if m.is_sand_at(x - 1, y):
                    t = thing.Thing(self.level, tp_name="sand1_deco")
                    t.push(x, y)
                    t.set_tilename("sand1-right")
                if m.is_sand_at(x + 1, y):
                    t = thing.Thing(self.level, tp_name="sand1_deco")
                    t.push(x, y)
                    t.set_tilename("sand1-left")
                if m.is_sand_at(x, y - 1):
                    t = thing.Thing(self.level, tp_name="sand1_deco")
                    t.push(x, y)
                    t.set_tilename("sand1-bot")
                if m.is_sand_at(x, y + 1):
                    t = thing.Thing(self.level, tp_name="sand1_deco")
                    t.push(x, y)
                    t.set_tilename("sand1-top")
                if m.is_sand_at(x - 1, y - 1):
                    t = thing.Thing(self.level, tp_name="sand1_deco")
                    t.push(x, y)
                    t.set_tilename("sand1-br")
                if m.is_sand_at(x + 1, y - 1):
                    t = thing.Thing(self.level, tp_name="sand1_deco")
                    t.push(x, y)
                    t.set_tilename("sand1-bl")
                if m.is_sand_at(x - 1, y + 1):
                    t = thing.Thing(self.level, tp_name="sand1_deco")
                    t.push(x, y)
                    t.set_tilename("sand1-tr")
                if m.is_sand_at(x + 1, y + 1):
                    t = thing.Thing(self.level, tp_name="sand1_deco")
                    t.push(x, y)
                    t.set_tilename("sand1-tl")

            if not gravel1:
                if m.is_gravel_at(x - 1, y):
                    t = thing.Thing(self.level, tp_name="gravel1_deco")
                    t.push(x, y)
                    t.set_tilename("gravel1-right")
                if m.is_gravel_at(x + 1, y):
                    t = thing.Thing(self.level, tp_name="gravel1_deco")
                    t.push(x, y)
                    t.set_tilename("gravel1-left")
                if m.is_gravel_at(x, y - 1):
                    t = thing.Thing(self.level, tp_name="gravel1_deco")
                    t.push(x, y)
                    t.set_tilename("gravel1-bot")
                if m.is_gravel_at(x, y + 1):
                    t = thing.Thing(self.level, tp_name="gravel1_deco")
                    t.push(x, y)
                    t.set_tilename("gravel1-top")
                if m.is_gravel_at(x - 1, y - 1):
                    t = thing.Thing(self.level, tp_name="gravel1_deco")
                    t.push(x, y)
                    t.set_tilename("gravel1-br")
                if m.is_gravel_at(x + 1, y - 1):
                    t = thing.Thing(self.level, tp_name="gravel1_deco")
                    t.push(x, y)
                    t.set_tilename("gravel1-bl")
                if m.is_gravel_at(x - 1, y + 1):
                    t = thing.Thing(self.level, tp_name="gravel1_deco")
                    t.push(x, y)
                    t.set_tilename("gravel1-tr")
                if m.is_gravel_at(x + 1, y + 1):
                    t = thing.Thing(self.level, tp_name="gravel1_deco")
                    t.push(x, y)
                    t.set_tilename("gravel1-tl")

            if not snow1:
                if m.is_snow_at(x - 1, y):
                    t = thing.Thing(self.level, tp_name="snow1_deco")
                    t.push(x, y)
                    t.set_tilename("snow1_right")
                if m.is_snow_at(x + 1, y):
                    t = thing.Thing(self.level, tp_name="snow1_deco")
                    t.push(x, y)
                    t.set_tilename("snow1_left")
                if m.is_snow_at(x, y - 1):
                    t = thing.Thing(self.level, tp_name="snow1_deco")
                    t.push(x, y)
                    t.set_tilename("snow1_bot")
                if m.is_snow_at(x, y + 1):
                    t = thing.Thing(self.level, tp_name="snow1_deco")
                    t.push(x, y)
                    t.set_tilename("snow1_top")
                if m.is_snow_at(x - 1, y - 1):
                    t = thing.Thing(self.level, tp_name="snow1_deco")
                    t.push(x, y)
                    t.set_tilename("snow1_br")
                if m.is_snow_at(x + 1, y - 1):
                    t = thing.Thing(self.level, tp_name="snow1_deco")
                    t.push(x, y)
                    t.set_tilename("snow1_bl")
                if m.is_snow_at(x - 1, y + 1):
                    t = thing.Thing(self.level, tp_name="snow1_deco")
                    t.push(x, y)
                    t.set_tilename("snow1_tr")
                if m.is_snow_at(x + 1, y + 1):
                    t = thing.Thing(self.level, tp_name="snow1_deco")
                    t.push(x, y)
                    t.set_tilename("snow1_tl")

            if m.is_start_at(x, y):
                t = thing.Thing(self.level, tp_name="start1")
                t.push(x, y)

                t = thing.Thing(self.level, tp_name="player1")
                t.push(x, y)
                self.player = t

            if m.is_rock_at(x, y):
                t = thing.Thing(self.level, tp_name="landrock1")
                t.push(x, y)

                if m.is_rock_at(x, y-1):
                    b = True
                else:
                    b = False

                if m.is_rock_at(x-1, y):
                    d = True
                else:
                    d = False

                if m.is_rock_at(x+1, y):
                    f = True
                else:
                    f = False

                if m.is_rock_at(x, y+1):
                    h = True
                else:
                    h = False

                # a b c
                # d e f
                # g h i
                if b and d and f and h:
                    t.set_tilename("landrock1-x")
                elif b and d and f:
                    t.set_tilename("landrock1-t180")
                elif b and d and h:
                    t.set_tilename("landrock1-t90")
                elif b and f and h:
                    t.set_tilename("landrock1-t270")
                elif d and f and h:
                    t.set_tilename("landrock1-t")
                elif b and h:
                    t.set_tilename("landrock1-up-down")
                elif d and f:
                    t.set_tilename("landrock1-left-right")
                elif b and f:
                    t.set_tilename("landrock1-l")
                elif h and f:
                    t.set_tilename("landrock1-l90")
                elif d and h:
                    t.set_tilename("landrock1-l180")
                elif b and d:
                    t.set_tilename("landrock1-l270")
                elif b:
                    t.set_tilename("landrock1-n180")
                elif f:
                    t.set_tilename("landrock1-n270")
                elif h:
                    t.set_tilename("landrock1-n")
                elif d:
                    t.set_tilename("landrock1-n90")
                else:
                    t.set_tilename("landrock1-node")

            if m.is_water_at(x, y):

                water = "water2"
                put_treasure = False

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

            if m.is_treasure_at(x, y):
                toughness = self.level.xyz.z

                r = tp.get_random_treasure(toughness=1)
                t = thing.Thing(self.level, tp_name=r.short_name)
                t.push(x, y)
