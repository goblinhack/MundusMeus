import util
import mm
import level
import wid_map
import thing
import wid_popup
import biome_dungeon
import biome_land
import time_of_day
import pickle
import os.path
import copy

global g


class Game:

    def __init__(self):

        #
        # Max thing ID in use in any level. This grows forever.
        #
        (self.width, self.height) = (mm.MAP_WIDTH, mm.MAP_HEIGHT)
        self.wid_map = None
        self.save_file = ".save_file"
        self.player = None

    def load_empty_level(self):
        self.level = level.Level(game=self,
                                 xyz=self.where,
                                 width=self.width,
                                 height=self.height)
        self.map_wid_create()

    def game_load_failed_init_new_game(self):

        self.seed = 0
        self.move_count = 0
        self.moves_per_day = 1000
        self.max_thing_id = 1
        self.seed = 9
        self.where = util.Xyz(24, 37, 0)
        self.load_level()

    def load_level(self):

        self.load_empty_level()
        l = self.level

        mm.con("Loading level @ {0}".format(str(l)))

        if os.path.isfile(str(l)):
            with open(str(l), 'rb') as f:
                self.level = pickle.load(f)
                l = self.level

                mm.con("Depickled level @ {0}".format(str(l)))

                for thing_id in l.all_things:
                    t = l.all_things[thing_id]
                    mm.thing_new(t, thing_id, t.tp_name)
                    t.on_map = False

                mm.con("Pushing things on level @ {0}".format(str(l)))

                for thing_id in l.all_things:
                    t = l.all_things[thing_id]
                    t.push(t.x, t.y)

                    tp = t.tp
                    if tp.is_player:
                        self.player = t

            if self.player is None:
                mm.die("No player found on level")
        else:
            mm.con("Did not find level @ {0}, create it".format(str(l)))
            self.biome_create(is_land=True, seed=self.seed)
#            self.biome_create(is_dungeon=True, seed=self.seed)

        mm.con("Loaded level @ {0}".format(str(l)))

        l = self.level
        mm.biome_set_is_land(value=l.is_biome_land)
        mm.biome_set_is_dungeon(value=l.is_biome_dungeon)

    def load_level_finalize(self):

        l = self.level
        mm.game_set_move_count(self.move_count)
        mm.game_set_moves_per_day(self.moves_per_day)

        time_of_day.set_lighting(self,
                                 move=self.move_count,
                                 moves_per_day=self.moves_per_day)

        for y in range(0, mm.MAP_HEIGHT):
            for x in range(0, mm.MAP_WIDTH):

                t = l.tp_find(x, y, "focus1")
                if t is not None:
                    t.set_tp("none")
                else:
                    t = l.tp_find(x, y, "focus2")
                    if t is not None:
                        t.set_tp("none")

                t = l.tp_find(x, y, "none")
                if t is None:
                    t = thing.Thing(l, tp_name="none")
                    t.push(x, y)

                t.wid.game = self
                t.wid.set_on_m_over_b(game_map_mouse_over)
                t.wid.set_on_m_down(game_map_mouse_down)
                t.wid.set_on_key_down(game_key_down)

        self.map_center_on_player(level_start=True)
        self.map_center_on_player(level_start=False)

        self.wid_player_location = None
        self.player_location_update()
        self.save()

    def save(self):
        l = self.level

        mm.con("Saving game @ {0}".format(str(l)))

        with open(self.save_file, 'wb') as f:
            pickle.dump(self.width, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.height, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.seed, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.max_thing_id, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.where, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.move_count, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.moves_per_day, f, pickle.HIGHEST_PROTOCOL)

        l.save()

    def load(self):
        mm.con("Loading game")

        with open(self.save_file, 'rb') as f:
            self.width = pickle.load(f)
            self.height = pickle.load(f)
            self.seed = pickle.load(f)
            self.max_thing_id = pickle.load(f)
            self.where = pickle.load(f)
            self.move_count = pickle.load(f)
            self.moves_per_day = pickle.load(f)
            self.load_level()

    def destroy(self):
        l = self.level
        l.destroy()

    def tick(self):

        l = self.level
        self.move_count += 1
        mm.game_set_move_count(self.move_count)
        time_of_day.set_lighting(self,
                                 move=self.move_count,
                                 moves_per_day=self.moves_per_day)

        l.tick()
        self.player_location_update()
        self.player_get_next_move()

    def player_location_update(self):

        l = self.level
        if self.wid_player_location:
            self.wid_player_location.destroy()

        self.wid_player_location = wid_popup.WidPopup(
                name="wid_player_location",
                width=1.0)
        w = self.wid_player_location

        text = ""

        text += "%%fg=white$Hour %%fg=green${0}%%fg=reset$ ".format(
                self.hour_str)

        text += "%%fg=white$Day %%fg=green${0}%%fg=reset$ ".format(
                self.day)

        text += "%%fg=white$World %%fg=green${0},{1}%%fg=reset$ ".format(
                l.xyz.x, l.xyz.y)

        text += "%%fg=white$Move %%fg=green${0}%%fg=reset$ ".format(
                self.move_count)

        if l.xyz.z > 0:
            text += "Depth %%fg=green${0} feet%%fg=reset ".format(l.xyz.z
                                                                  * 10)

#        player = self.player
#        text += "@ %%fg=green${0},{1}%%fg=reset ".format(player.x, player.y)

        w.add_text(font="vsmall", text=text)

        w.set_color(bg=True, tl=True, br=True, name="blue", alpha=0.2)
        w.update()
        w.move_to_bottom()

    #
    # The scrollable map for the level
    #
    def map_wid_create(self):
        self.map_wid_destroy()
        self.wid_map = wid_map.WidMap(mm.MAP_WIDTH, mm.MAP_HEIGHT)

    def map_wid_destroy(self):
        if self.wid_map is not None:
            self.wid_map.destroy()
            self.wid_map = None

    def map_center_on_player(self, level_start):
        px = self.player.x / mm.MAP_WIDTH
        py = self.player.y / mm.MAP_HEIGHT

        tm = self.wid_map.TILES_SCREEN_WIDTH / 2
        th = self.wid_map.TILES_SCREEN_HEIGHT / 2

        dx = 1.0 / mm.MAP_WIDTH
        dy = 1.0 / mm.MAP_HEIGHT

        px = px - dx * tm
        py = py - dy * th

        if level_start:
            self.wid_map.wid_vert_scroll.move_to_vert_pct(pct=px)
            self.wid_map.wid_horiz_scroll.move_to_horiz_pct(pct=py)
            self.wid_map.wid_vert_scroll.move_to_vert_pct(pct=px)
            self.wid_map.wid_horiz_scroll.move_to_horiz_pct(pct=py)
        else:
            self.wid_map.wid_vert_scroll.move_to_vert_pct_in(pct=py, delay=500)
            self.wid_map.wid_horiz_scroll.move_to_horiz_pct_in(pct=px,
                                                               delay=500)

    #
    # Get rid of the path indicators where the player will move
    #
    def map_clear_focus(self):

        l = self.level
        for x in range(0, l.width):
            for y in range(0, l.height):
                t = l.tp_find(x, y, "focus2")
                if t is not None:
                    t.set_tp("none")

                t = l.tp_find(x, y, "focus1")
                if t is not None:
                    t.set_tp("none")

    #
    # Mouse is over a map tile; show the route back to the player
    #
    def map_mouse_over(self, w, relx, rely, wheelx, wheely):

        if len(self.player.nexthops) > 0:
            return False

        #
        # Want to scroll without the focus moving
        #
        if wheelx != 0 or wheely != 0:
            return False

        #
        # When the map is scrolling, no focus change
        #
        if relx == 0 and rely == 0:
            return False

        l = self.level

        self.map_clear_focus()

        t = w.thing
        t.set_tp("focus2")

        #
        # Check we can get back from the chosen point to the player.
        #
        player = self.player
        nexthops = l.dmap_solve(self.player.x, self.player.y, t.x, t.y)
        if (player.x, player.y) in nexthops:
            for o in nexthops:
                (x, y) = o

                t = l.tp_find(x, y, "none")
                if t is not None:
                    t.set_tp("focus2")

    #
    # Move the player to the chosen tile
    #
    def map_mouse_down(self, w, x, y, button):

        l = self.level

        t = w.thing
        t.set_tp("focus1")

        #
        # Set up the player move chain
        #
        player = self.player
        nexthops = l.dmap_solve(self.player.x, self.player.y, t.x, t.y)

        if len(nexthops) < 2:
            return True

        #
        # Only if the destination is in a valid nexthops
        #
        if (player.x, player.y) in nexthops:
            player.nexthops = nexthops

            self.player_get_next_move()

        self.map_clear_focus()

        return True

    #
    # Player input
    #
    def map_key_down(self, w, sym, mod):

        self.map_clear_focus()

        if sym == mm.SDLK_PERIOD:
            self.tick()
            return True

        if sym == mm.SDLK_s:
            self.save()

        return False

    def player_get_next_move(self):

        player = self.player
        if len(player.nexthops) == 0:
            return True

        player.nexthops.pop()
        if len(player.nexthops) < 1:
            return True

        #
        # Move the player. [-1] is the player. [-2] is the adjacent cell.
        #
        x, y = player.nexthops[-1]
        player.move(x, y)

        #
        # Place a light ember so the player can see where they've been
        #
        l = self.level

        #
        # If in a dungeon place a trail of breadcrumbs
        #
        if l.is_biome_dungeon:
            t = l.tp_find(x, y, "ember1")
            if t is None:
                t = thing.Thing(l, tp_name="ember1")
                t.push(x, y)

        if x == 0 or x == mm.MAP_WIDTH - 1 or y == 0 or y == mm.MAP_HEIGHT - 1:
            mm.con("Level move")
            player.pop()
            c = copy.copy(player)

            mm.con("Destroy player")
            player.destroy()
            player = None

            mm.con("Save before changing level @ {0}".format(str(l)))
            self.save()

            mm.con("Destroying old level @ {0}".format(str(l)))
            l.destroy()
            self.map_wid_destroy()
            mm.con("Destroyed old level @ {0}".format(str(l)))

            if self.wid_player_location:
                self.wid_player_location.destroy()
                self.wid_player_location = None

            if x == 0:
                x = mm.MAP_WIDTH - 1
                self.where.x -= 1
                if self.where.x < 0:
                    self.where.x = mm.WORLD_WIDTH - 1

            elif x == mm.MAP_WIDTH - 1:
                x = 0
                self.where.x += 1
                if self.where.x >= mm.WORLD_WIDTH:
                    self.where.x = 0

            elif y == 0:
                y = mm.MAP_HEIGHT - 1
                self.where.y -= 1
                if self.where.y < 0:
                    self.where.y = mm.WORLD_HEIGHT - 1

            elif y == mm.MAP_HEIGHT - 1:
                y = 0
                self.where.y += 1
                if self.where.y >= mm.WORLD_HEIGHT:
                    self.where.y = 0

            self.load_level()
            l = self.level

            player = thing.Thing(l, c.tp_name)
            player.push(x, y)
            self.player = player

            self.load_level_finalize()
            mm.con("Loaded next level @ {0}".format(str(l)))

            self.save()

        self.map_center_on_player(level_start=False)

    #
    # Create a random dungeon
    #
    def biome_create(self, seed, is_land=False, is_dungeon=False):

        l = self.level
        l.set_biome(is_land=is_land, is_dungeon=is_dungeon)

        if l.is_biome_dungeon:
            self.biome_build = biome_dungeon.biome_build
            self.biome_populate = biome_dungeon.biome_populate

        if l.is_biome_land:
            self.biome_build = biome_land.biome_build
            self.biome_populate = biome_land.biome_populate

        self.biome_build(self, l, seed=seed)
        self.biome_populate(self)


def game_map_mouse_over(w, relx, rely, wheelx, wheely):
    if w.game is None:
        return
    w.game.map_mouse_over(w, relx, rely, wheelx, wheely)


def game_map_mouse_down(w, x, y, button):
    if w.game is None:
        return False
    return w.game.map_mouse_down(w, x, y, button)


def game_key_down(w, sym, mod):
    if w.game is None:
        return False
    return w.game.map_key_down(w, sym, mod)

g = None


def game_new():
    global g

    g = Game()
    if os.path.isfile(g.save_file):
        try:
            g.load()
        except Exception as inst:
            mm.con("Loading failed, init new game, error [{0}]".format(inst))
            g.map_wid_destroy()

            g = Game()
            g.game_load_failed_init_new_game()
    else:
        g.game_load_failed_init_new_game()

    g.load_level_finalize()
