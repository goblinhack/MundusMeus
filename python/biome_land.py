import random
import mm
import thing
import biome_land_do
import tp
import game


def biome_build(self, seed=0):

    while True:
        random.seed(self.biome_seed)

        self.biome_seed += 1
        self.biome_seed *= self.biome_seed

        self.biome = biome_land_do.Biome(chunk=self)

        if not self.biome.generate_failed:
            break

    if False:
        self.biome.dump()


def biome_populate(self):
    c = self
    m = c.biome

    #
    # Get the feel of the chunk for weather effects
    #
    snow_count = 0
    grass_count = 0
    water_count = 0
    poor_quality_ground_count = 0

    for y in range(0, mm.CHUNK_HEIGHT):
        for x in range(0, mm.CHUNK_WIDTH):
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

    threshold = mm.CHUNK_WIDTH * mm.CHUNK_HEIGHT

    c.is_snowy = False
    if snow_count > threshold / 4:
        c.is_snowy = True

    c.is_grassy = False
    if grass_count > threshold / 2:
        c.is_grassy = True

    if poor_quality_ground_count > threshold / 2:
        c.is_grassy = True

    c.is_watery = False
    if water_count > threshold / 2:
        c.is_watery = True

    is_poor_soil = False
    if grass_count < threshold / 2:
        is_poor_soil = True

    if c.is_snowy:
        grass_str = "grass_snow"
        dirt_str = "dirt_snow"
        sand_str = "sand_snow"
    else:
        grass_str = "grass"
        dirt_str = "dirt"
        sand_str = "sand"

    for y in range(0, mm.CHUNK_HEIGHT):
        for x in range(0, mm.CHUNK_WIDTH):

            tx = x + self.base_x
            ty = y + self.base_y

            grass = False
            if m.is_grass_at(x, y):
                grass = True

            dirt = False
            if m.is_dirt_at(x, y):

                if random.randint(0, 10000) < 2:
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="torch1")
                    t.push()

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

            pushed = True

            if m.is_dungeon_at(x, y):
                if c.is_snowy:
                    r = tp.get_random_dungeon_snow()
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                    t.push()
                    pushed = True
                else:
                    r = tp.get_random_dungeon()
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                    t.push()
                    pushed = True

            if m.is_tree_at(x, y):
                if c.is_snowy:
                    r = tp.get_random_tree_snow()
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                    t.push()
                    pushed = True
                elif is_poor_soil:
                    r = tp.get_random_tree_conifer()
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                    t.push()
                    pushed = True
                else:
                    for i in range(1, random.randint(1, 10)):
                        r = tp.get_random_tree()
                        t = thing.Thing(chunk=c, x=tx, y=ty,
                                        tp_name=r.name)
                        t.push()
                        pushed = True

            if grass:
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=grass_str + "")
                t.push()
                pushed = True
                if not c.is_snowy:
                    if m.is_water_at(x - 1, y) or \
                       m.is_water_at(x + 1, y) or \
                       m.is_water_at(x, y - 1) or \
                       m.is_water_at(x, y + 1):

                        r = tp.get_random_marsh_plant()
                        for i in range(1, random.randint(1, 5)):
                            t = thing.Thing(chunk=c, x=tx, y=ty,
                                            tp_name=r.name)
                            t.push()

                        if random.randint(0, 1000) < 10:
                            r = tp.get_random_plant()
                            t = thing.Thing(chunk=c, x=tx, y=ty,
                                            tp_name=r.name)
                            t.push()
                    else:
                        if random.randint(0, 100) < 10:
                            r = tp.get_random_plant()
                            if random.randint(0, 100) < 10:
                                for i in range(1, random.randint(1, 15)):
                                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                                    tp_name=r.name)
                                    t.push()
                            else:
                                for i in range(1, random.randint(1, 5)):
                                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                                    tp_name=r.name)
                                    t.push()

            if dirt:
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=dirt_str + "")
                t.push()
                pushed = True

                if random.randint(0, 200) < 5:
                    r = tp.get_random_plant()
                    for i in range(1, random.randint(1, 5)):
                        t = thing.Thing(chunk=c, x=tx, y=ty,
                                        tp_name=r.name)
                        t.push()

            if sand:
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=sand_str + "")
                t.push()
                pushed = True

                if not grass:
                    if random.randint(0, 1000) < 5:
                        r = tp.get_random_small_rock()
                        t = thing.Thing(chunk=c, x=tx, y=ty,
                                        tp_name=r.name)
                        t.push()

            if gravel:
                if c.is_snowy:
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="gravel_snow")
                    t.push()
                    pushed = True
                else:
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="gravel")
                    t.push()
                    pushed = True

                if random.randint(0, 1000) < 50:
                    r = tp.get_random_small_rock()
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                    t.push()
                    pushed = True

            if road:
                if c.is_snowy:
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="road_snow")
                    t.push()
                    pushed = True
                else:
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="road")
                    t.push()
                    pushed = True

            if ice:
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="ice")
                t.push()
                pushed = True

            if snow:
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="snow")
                t.push()
                pushed = True

                if random.randint(0, 1000) < 50:
                    r = tp.get_random_snow_mound()
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                    t.push()

            if m.is_dungeon_way_up_at(x, y):
                if game.g.player is None:
                    #
                    # Create the player on the active central chunk
                    #
                    if self.cx == 1 and self.cy == 1:
                        t = thing.Thing(chunk=c, x=tx, y=ty,
                                        tp_name="player1")
                        t.push()
                        pushed = True
                        game.g.player = t

            if m.is_rock_at(x, y):
                if c.is_snowy:
                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                    tp_name="landrock_snow")
                    t.push()
                    pushed = True
                else:
                    t = thing.Thing(chunk=c, x=tx, y=ty,
                                    tp_name="landrock")
                    t.push()
                    pushed = True

            if m.is_water_at(x, y):

                water = "water2"
                put_treasure = False

                #
                # Underground water
                #
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=water)
                t.push()
                pushed = True

                if put_treasure:
                    toughness = 1
                    r = tp.get_random_minable_treasure(toughness=toughness)
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                    t.push()

            if m.is_treasure_at(x, y):
                toughness = c.where.z

                r = tp.get_random_treasure(toughness=1)
                t = thing.Thing(chunk=c, x=tx, y=ty, tp_name=r.name)
                t.push()
                pushed = True

            if not pushed:
                if c.is_snowy:
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="gravel_snow")
                    t.push()
                else:
                    t = thing.Thing(chunk=c, x=tx, y=ty, tp_name="gravel")
                    t.push()
