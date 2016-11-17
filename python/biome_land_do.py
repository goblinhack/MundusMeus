#!/usr/bin/env python3
import random
import sys
import os
import charmap
import biome
import mm


class Biome(biome.Biome):
    def __init__(self, level=None, width=80, height=40):
        self.width = width
        self.height = height

        #
        # Set if we fail to generate
        #
        self.generate_failed = False

        #
        # The map
        #
        self.cells = [[[' ' for d in range(charmap.depth.max)]
                       for i in range(height)]
                      for j in range(width)]
        self.debug("^^^ init biome land ^^^")

        mx = level.xyz.x
        my = level.xyz.y
        ox = mx * mm.MAP_WIDTH
        oy = my * mm.MAP_HEIGHT

        #
        # Get colors in image
        #
        #        cols = {}
        #        (W, H) = mm.tex_size("topo")
        #        for y in range(H):
        #            for x in range(W):
        #                c = mm.tex_pixel("topo", x, y)
        #                if c not in cols:
        #                    cols[c] = 1
        #                    print(c)

        for y in range(self.height):
            for x in range(self.width):
                ix = ox + x
                iy = oy + y
                r, g, b, a = mm.tex_pixel("topo", ix, iy)

                is_tree = False
                is_rock = False
                is_dirt = False
                is_grass = False
                is_water = False
                is_sand = False
                is_gravel = False
                is_snow = False
                is_ice = False

                if r == 113 and g == 89 and b == 145:
                    # dark purple
                    if random.randint(0, 1000) < 900:
                        is_rock = True
                    else:
                        is_gravel = True

                elif r == 142 and g == 83 and b == 215:
                    # purple, trees
                    is_tree = True

                elif r == 169 and g == 167 and b == 137:
                    # tan
                    is_gravel = True

                elif r == 234 and g == 12 and b == 43:
                    # red
                    is_dirt = True

                elif r == 101 and g == 168 and b == 76:
                    # light green
                    is_grass = True

                elif r == 77 and g == 129 and b == 47:
                    # darker green
                    is_grass = True

                elif r == 43 and g == 128 and b == 172:
                    # darker blue
                    is_water = True

                elif r == 147 and g == 193 and b == 218:
                    # light blue
                    is_water = True

                elif r == 203 and g == 194 and b == 116:
                    # yellowish
                    is_snow = True

                elif r == 240 and g == 243 and b == 237:
                    # white
                    is_snow = True

                elif r == 215 and g == 239 and b == 251:
                    # yellowish
                    is_ice = True

                else:
                    mm.die("unhandled color {0}, {1}, {2}".format(r, g, b))

                if is_grass:
                    if random.randint(0, 1000) < 5:
                        self.putc(x, y, charmap.depth.floor, charmap.TREE)

                    self.putc(x, y, charmap.depth.grass, charmap.GRASS)
                    self.putc(x, y, charmap.depth.dirt, charmap.DIRT)
                    self.putc(x, y, charmap.depth.sand, charmap.SAND)

                if is_tree:
                    if random.randint(0, 100) < 30:
                        self.putc(x, y, charmap.depth.floor, charmap.TREE)
                        self.putc(x, y, charmap.depth.grass, charmap.GRASS)
                        self.putc(x, y, charmap.depth.dirt, charmap.DIRT)
                        self.putc(x, y, charmap.depth.sand, charmap.SAND)
                    else:
                        self.putc(x, y, charmap.depth.grass, charmap.GRASS)
                        self.putc(x, y, charmap.depth.dirt, charmap.DIRT)
                        self.putc(x, y, charmap.depth.sand, charmap.SAND)

                if is_rock:
                    self.putc(x, y, charmap.depth.wall, charmap.ROCK)

                if is_water:
                    self.putc(x, y, charmap.depth.under, charmap.WATER)

                if is_dirt:
                    if random.randint(0, 1000) < 5:
                        self.putc(x, y, charmap.depth.floor, charmap.TREE)

                    self.putc(x, y, charmap.depth.dirt, charmap.DIRT)
                    self.putc(x, y, charmap.depth.sand, charmap.GRAVEL)

                if is_sand:
                    self.putc(x, y, charmap.depth.sand, charmap.SAND)

                if is_gravel:
                    self.putc(x, y, charmap.depth.gravel, charmap.GRAVEL)

                if is_snow:
                    if random.randint(0, 1000) < 5:
                        self.putc(x, y, charmap.depth.wall, charmap.ROCK)

                    self.putc(x, y, charmap.depth.snow, charmap.SNOW)

                    if random.randint(0, 1000) < 5:
                        self.putc(x, y, charmap.depth.sand, charmap.GRAVEL)
                    else:
                        self.putc(x, y, charmap.depth.under, charmap.WATER)

                if is_ice:
                    if random.randint(0, 100) < 35:
                        self.putc(x, y, charmap.depth.ice, charmap.ICE)

                    self.putc(x, y, charmap.depth.under, charmap.WATER)

        #
        # Place start
        #
        if not self.rooms_place_start():
            self.generate_failed = True
            return
        self.debug("^^^ placed start ^^^")

    def debug(self, s):
        return
#        self.dump()

    def rooms_place_start(self):
        tries = 0
        while True:
            tries = tries + 1
            if tries > 100:
                return False

            x = random.randint(1, self.width - 1)
            y = random.randint(1, self.height - 1)

            obstacle = False
            for dx, dy in biome.ALL_DELTAS:
                if self.is_movement_blocking_at(x + dx, y + dy):
                    obstacle = True
                    break

            if obstacle:
                continue

            self.putc(x, y, charmap.depth.wall, charmap.START)
            return True

    def dump(self, max_depth=charmap.depth.max):
        from colored import fg, bg

        os.system('cls' if os.name == 'nt' else 'clear')
        for y in range(self.height):
            for x in range(self.width):
                for d in reversed(range(max_depth)):
                    c = self.cells[x][y][d]
                    cmap = charmap.charmap[c]
                    fg_name = cmap["fg"]
                    bg_name = cmap["bg"]
                    if c != " ":
                        break

                color = fg(fg_name) + bg(bg_name)

                sys.stdout.write(color + c)
            print("")
