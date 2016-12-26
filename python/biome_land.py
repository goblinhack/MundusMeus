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

    if False:
        self.level.biome.dump()


def biome_populate(self):
    l = self.level
    m = l.biome

    #
    # Get the feel of the level for weather effects
    #
    snow_count = 0
    grass_count = 0
    water_count = 0
    poor_quality_ground_count = 0

    for y in range(0, mm.MAP_HEIGHT):
        for x in range(0, mm.MAP_WIDTH):
            if m.is_snow_at(x, y):
                snow_count += 1

            if m.is_ice_at(x, y):
                snow_count += 1

            if m.is_grass_at(x, y):
                grass_count += 1

            if m.is_water_at(x, y):
                grass_count += 1

            if m.is_dirt_at(x, y):
                poor_quality_ground_count += 1

            if m.is_gravel_at(x, y):
                poor_quality_ground_count += 1

    threshold = mm.MAP_WIDTH * mm.MAP_HEIGHT

    l.is_snowy = False
    if snow_count > threshold / 4:
        l.is_snowy = True

    l.is_grassy = False
    if grass_count > threshold / 2:
        l.is_grassy = True

    if poor_quality_ground_count > threshold / 2:
        l.is_grassy = True

    l.is_watery = False
    if water_count > threshold / 2:
        l.is_watery = True

    is_poor_soil = False
    if grass_count < threshold / 2:
        is_poor_soil = True

    if l.is_snowy:
        grass_str = "grass_snow"
        dirt_str = "dirt_snow"
        sand_str = "sand_snow"
        landrock_str = "landrock_snow"
    else:
        grass_str = "grass"
        dirt_str = "dirt"
        sand_str = "sand"
        landrock_str = "landrock"

    for y in range(0, mm.MAP_HEIGHT):
        for x in range(0, mm.MAP_WIDTH):

            grass = False
            if m.is_grass_at(x, y):
                grass = True

            dirt = False
            if m.is_dirt_at(x, y):

                if random.randint(0, 1000) < 2:
                    t = thing.Thing(l, tp_name="torch1")
                    t.push(x, y)

                dirt = True

            sand = False
            if m.is_sand_at(x, y):
                sand = True

            gravel = False
            if m.is_gravel_at(x, y):
                gravel = True

            snow = False
            if m.is_snow_at(x, y):
                snow = True

            ice = False
            if m.is_ice_at(x, y):
                ice = True

            road = False
            if m.is_road_at(x, y):
                road = True

            if m.is_dungeon_at(x, y):
                if l.is_snowy:
                    r = tp.get_random_dungeon_snow()
                    t = thing.Thing(l, tp_name=r.short_name)
                    t.push(x, y)
                else:
                    r = tp.get_random_dungeon()
                    t = thing.Thing(l, tp_name=r.short_name)
                    t.push(x, y)

            if m.is_tree_at(x, y):
                if l.is_snowy:
                    r = tp.get_random_tree_snow()
                    t = thing.Thing(l, tp_name=r.short_name)
                    t.push(x, y)
                elif is_poor_soil:
                    r = tp.get_random_tree_conifer()
                    t = thing.Thing(l, tp_name=r.short_name)
                    t.push(x, y)
                else:
                    for i in range(1, random.randint(1, 10)):
                        r = tp.get_random_tree()
                        t = thing.Thing(l, tp_name=r.short_name)
                        t.push(x, y)

            if grass:
                t = thing.Thing(l, tp_name=grass_str + "")
                t.push(x, y)
                if not l.is_snowy:
                    if m.is_water_at(x - 1, y) or \
                       m.is_water_at(x + 1, y) or \
                       m.is_water_at(x, y - 1) or \
                       m.is_water_at(x, y + 1):

                        r = tp.get_random_marsh_plant()
                        for i in range(1, random.randint(1, 5)):
                            t = thing.Thing(l, tp_name=r.short_name)
                            t.push(x, y)

                        if random.randint(0, 1000) < 10:
                            r = tp.get_random_plant()
                            t = thing.Thing(l, tp_name=r.short_name)
                            t.push(x, y)
                    else:
                        if random.randint(0, 100) < 10:
                            r = tp.get_random_plant()
                            for i in range(1, random.randint(1, 5)):
                                t = thing.Thing(l, tp_name=r.short_name)
                                t.push(x, y)

            if dirt:
                t = thing.Thing(l, tp_name=dirt_str + "")
                t.push(x, y)

                if random.randint(0, 200) < 5:
                    for i in range(1, random.randint(1, 5)):
                        r = tp.get_random_plant()
                        t = thing.Thing(l, tp_name=r.short_name)
                        t.push(x, y)

            if sand:
                t = thing.Thing(l, tp_name=sand_str + "")
                t.push(x, y)

                if not grass:
                    if random.randint(0, 1000) < 5:
                        r = tp.get_random_small_rock()
                        t = thing.Thing(l, tp_name=r.short_name)
                        t.push(x, y)

            if gravel:
                if l.is_snowy:
                    t = thing.Thing(l, tp_name="gravel_snow")
                    t.push(x, y)
                else:
                    t = thing.Thing(l, tp_name="gravel")
                    t.push(x, y)

                if random.randint(0, 1000) < 50:
                    r = tp.get_random_small_rock()
                    t = thing.Thing(l, tp_name=r.short_name)
                    t.push(x, y)

            if road:
                if l.is_snowy:
                    t = thing.Thing(l, tp_name="road_snow")
                    t.push(x, y)
                else:
                    t = thing.Thing(l, tp_name="road")
                    t.push(x, y)

            if ice:
                t = thing.Thing(l, tp_name="ice")
                t.push(x, y)

            if snow:
                t = thing.Thing(l, tp_name="snow")
                t.push(x, y)

                if random.randint(0, 1000) < 50:
                    r = tp.get_random_snow_mound()
                    t = thing.Thing(l, tp_name=r.short_name)
                    t.push(x, y)

            if not grass:
                if m.is_grass_at(x - 1, y):
                    t = thing.Thing(l, tp_name=grass_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(grass_str + "-right")
                if m.is_grass_at(x + 1, y):
                    t = thing.Thing(l, tp_name=grass_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(grass_str + "-left")
                if m.is_grass_at(x, y - 1):
                    t = thing.Thing(l, tp_name=grass_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(grass_str + "-bot")
                if m.is_grass_at(x, y + 1):
                    t = thing.Thing(l, tp_name=grass_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(grass_str + "-top")
                if m.is_grass_at(x - 1, y - 1):
                    t = thing.Thing(l, tp_name=grass_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(grass_str + "-br")
                if m.is_grass_at(x + 1, y - 1):
                    t = thing.Thing(l, tp_name=grass_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(grass_str + "-bl")
                if m.is_grass_at(x - 1, y + 1):
                    t = thing.Thing(l, tp_name=grass_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(grass_str + "-tr")
                if m.is_grass_at(x + 1, y + 1):
                    t = thing.Thing(l, tp_name=grass_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(grass_str + "-tl")

            if not dirt:
                if m.is_dirt_at(x - 1, y):
                    t = thing.Thing(l, tp_name=dirt_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(dirt_str + "-right")
                if m.is_dirt_at(x + 1, y):
                    t = thing.Thing(l, tp_name=dirt_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(dirt_str + "-left")
                if m.is_dirt_at(x, y - 1):
                    t = thing.Thing(l, tp_name=dirt_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(dirt_str + "-bot")
                if m.is_dirt_at(x, y + 1):
                    t = thing.Thing(l, tp_name=dirt_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(dirt_str + "-top")
                if m.is_dirt_at(x - 1, y - 1):
                    t = thing.Thing(l, tp_name=dirt_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(dirt_str + "-br")
                if m.is_dirt_at(x + 1, y - 1):
                    t = thing.Thing(l, tp_name=dirt_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(dirt_str + "-bl")
                if m.is_dirt_at(x - 1, y + 1):
                    t = thing.Thing(l, tp_name=dirt_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(dirt_str + "-tr")
                if m.is_dirt_at(x + 1, y + 1):
                    t = thing.Thing(l, tp_name=dirt_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(dirt_str + "-tl")

            if not sand:
                if m.is_sand_at(x - 1, y):
                    t = thing.Thing(l, tp_name=sand_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(sand_str + "-right")
                if m.is_sand_at(x + 1, y):
                    t = thing.Thing(l, tp_name=sand_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(sand_str + "-left")
                if m.is_sand_at(x, y - 1):
                    t = thing.Thing(l, tp_name=sand_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(sand_str + "-bot")
                if m.is_sand_at(x, y + 1):
                    t = thing.Thing(l, tp_name=sand_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(sand_str + "-top")
                if m.is_sand_at(x - 1, y - 1):
                    t = thing.Thing(l, tp_name=sand_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(sand_str + "-br")
                if m.is_sand_at(x + 1, y - 1):
                    t = thing.Thing(l, tp_name=sand_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(sand_str + "-bl")
                if m.is_sand_at(x - 1, y + 1):
                    t = thing.Thing(l, tp_name=sand_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(sand_str + "-tr")
                if m.is_sand_at(x + 1, y + 1):
                    t = thing.Thing(l, tp_name=sand_str + "_deco")
                    t.push(x, y)
                    t.set_tilename(sand_str + "-tl")

            if not gravel:
                if m.is_gravel_at(x - 1, y):
                    t = thing.Thing(l, tp_name="gravel_deco")
                    t.push(x, y)
                    t.set_tilename("gravel-right")
                if m.is_gravel_at(x + 1, y):
                    t = thing.Thing(l, tp_name="gravel_deco")
                    t.push(x, y)
                    t.set_tilename("gravel-left")
                if m.is_gravel_at(x, y - 1):
                    t = thing.Thing(l, tp_name="gravel_deco")
                    t.push(x, y)
                    t.set_tilename("gravel-bot")
                if m.is_gravel_at(x, y + 1):
                    t = thing.Thing(l, tp_name="gravel_deco")
                    t.push(x, y)
                    t.set_tilename("gravel-top")
                if m.is_gravel_at(x - 1, y - 1):
                    t = thing.Thing(l, tp_name="gravel_deco")
                    t.push(x, y)
                    t.set_tilename("gravel-br")
                if m.is_gravel_at(x + 1, y - 1):
                    t = thing.Thing(l, tp_name="gravel_deco")
                    t.push(x, y)
                    t.set_tilename("gravel-bl")
                if m.is_gravel_at(x - 1, y + 1):
                    t = thing.Thing(l, tp_name="gravel_deco")
                    t.push(x, y)
                    t.set_tilename("gravel-tr")
                if m.is_gravel_at(x + 1, y + 1):
                    t = thing.Thing(l, tp_name="gravel_deco")
                    t.push(x, y)
                    t.set_tilename("gravel-tl")

            if not snow:
                if m.is_snow_at(x - 1, y):
                    t = thing.Thing(l, tp_name="snow_deco")
                    t.push(x, y)
                    t.set_tilename("snow_right")
                if m.is_snow_at(x + 1, y):
                    t = thing.Thing(l, tp_name="snow_deco")
                    t.push(x, y)
                    t.set_tilename("snow_left")
                if m.is_snow_at(x, y - 1):
                    t = thing.Thing(l, tp_name="snow_deco")
                    t.push(x, y)
                    t.set_tilename("snow_bot")
                if m.is_snow_at(x, y + 1):
                    t = thing.Thing(l, tp_name="snow_deco")
                    t.push(x, y)
                    t.set_tilename("snow_top")
                if m.is_snow_at(x - 1, y - 1):
                    t = thing.Thing(l, tp_name="snow_deco")
                    t.push(x, y)
                    t.set_tilename("snow_br")
                if m.is_snow_at(x + 1, y - 1):
                    t = thing.Thing(l, tp_name="snow_deco")
                    t.push(x, y)
                    t.set_tilename("snow_bl")
                if m.is_snow_at(x - 1, y + 1):
                    t = thing.Thing(l, tp_name="snow_deco")
                    t.push(x, y)
                    t.set_tilename("snow_tr")
                if m.is_snow_at(x + 1, y + 1):
                    t = thing.Thing(l, tp_name="snow_deco")
                    t.push(x, y)
                    t.set_tilename("snow_tl")

            if m.is_start_at(x, y):
                if self.player is None:
                    t = thing.Thing(l, tp_name="player1")
                    t.push(x, y)
                    self.player = t

            if m.is_rock_at(x, y):
                if l.is_snowy:
                    t = thing.Thing(l, tp_name="landrock_snow")
                    t.push(x, y)
                else:
                    t = thing.Thing(l, tp_name="landrock")
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
                    t.set_tilename(landrock_str + "-x")
                elif b and d and f:
                    t.set_tilename(landrock_str + "-t180")
                elif b and d and h:
                    t.set_tilename(landrock_str + "-t90")
                elif b and f and h:
                    t.set_tilename(landrock_str + "-t270")
                elif d and f and h:
                    t.set_tilename(landrock_str + "-t")
                elif b and h:
                    t.set_tilename(landrock_str + "-up-down")
                elif d and f:
                    t.set_tilename(landrock_str + "-left-right")
                elif b and f:
                    t.set_tilename(landrock_str + "-l")
                elif h and f:
                    t.set_tilename(landrock_str + "-l90")
                elif d and h:
                    t.set_tilename(landrock_str + "-l180")
                elif b and d:
                    t.set_tilename(landrock_str + "-l270")
                elif b:
                    t.set_tilename(landrock_str + "-n180")
                elif f:
                    t.set_tilename(landrock_str + "-n270")
                elif h:
                    t.set_tilename(landrock_str + "-n")
                elif d:
                    t.set_tilename(landrock_str + "-n90")
                else:
                    t.set_tilename(landrock_str + "-node")

            if m.is_water_at(x, y):

                water = "water2"
                put_treasure = False

                #
                # Underground water
                #
                t = thing.Thing(l, tp_name=water)
                t.push(x, y)

                if put_treasure:
                    toughness = 1
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(l, tp_name=r.short_name)
                    t.push(x, y)

            if m.is_treasure_at(x, y):
                toughness = l.xyz.z

                r = tp.get_random_treasure(toughness=1)
                t = thing.Thing(l, tp_name=r.short_name)
                t.push(x, y)
