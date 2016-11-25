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

global g


class Game:

    def __init__(self):

        #
        # Max thing ID in use in any level. This grows forever.
        #
        (self.width, self.height) = (mm.MAP_WIDTH, mm.MAP_HEIGHT)

    def load_failed_init_new_game(self):

        self.seed = 0
        self.move_count = 0
        self.moves_per_day = 1000
        self.rain_amount = 0
        self.snow_amount = 0
        self.max_thing_id = 1
        self.seed = 9
        self.where = util.Xyz(28, 40, 0)
        self.level = level.Level(game=self, xyz=self.where)
        self.level.set_dim(self.width, self.height)

        self.map_wid_create()

    def post_load_init(self):

        mm.game_set_move_count(self.move_count)
        mm.game_set_moves_per_day(self.moves_per_day)
        mm.game_set_snow_amount(self.snow_amount)
        mm.game_set_rain_amount(self.rain_amount)

        time_of_day.set_lighting(self,
                                 move=self.move_count,
                                 moves_per_day=self.moves_per_day)

        if not self.was_loaded:
            self.biome_create(is_land=True, seed=self.seed)
#        self.biome_create(is_dungeon=True, seed=self.seed)

        #
        # Create or recreate the focus widgets
        #
        level = self.level

        for y in range(0, mm.MAP_HEIGHT):
            for x in range(0, mm.MAP_WIDTH):

                t = level.tp_find(x, y, "focus1")
                if t is not None:
                    t.set_tp("none")
                else:
                    t = level.tp_find(x, y, "focus2")
                    if t is not None:
                        t.set_tp("none")

                t = level.tp_find(x, y, "none")
                if t is None:
                    t = thing.Thing(self.level, tp_name="none")
                    t.push(x, y)

                t.wid.game = self
                t.wid.set_on_m_over_b(game_map_mouse_over)
                t.wid.set_on_m_down(game_map_mouse_down)
                t.wid.set_on_key_down(game_key_down)

        self.map_center_on_player(level_start=True)
        self.map_center_on_player(level_start=False)

        self.wid_map_summary = None
        self.player_location_update()

    def save(self):
        mm.con("Saving game @ {0}".format(str(self.level)))

        with open(".current", 'wb') as f:
            pickle.dump(self.width, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.height, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.seed, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.max_thing_id, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.where, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.move_count, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.moves_per_day, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.snow_amount, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.rain_amount, f, pickle.HIGHEST_PROTOCOL)

        self.level.save()

    def load(self):
        mm.con("Loading game")

        with open(".current", 'rb') as f:
            self.width = pickle.load(f)
            self.height = pickle.load(f)
            self.seed = pickle.load(f)
            self.max_thing_id = pickle.load(f)
            self.where = pickle.load(f)
            self.move_count = pickle.load(f)
            self.moves_per_day = pickle.load(f)
            self.snow_amount = pickle.load(f)
            self.rain_amount = pickle.load(f)

            self.level = level.Level(game=self, xyz=self.where)

            mm.con("Loading level @ {0}".format(str(self.level)))

            self.map_wid_create()

            with open(str(self.level), 'rb') as f:
                self.level = pickle.load(f)

                for thing_id in self.level.all_things:
                    t = self.level.all_things[thing_id]
                    mm.thing_new(t, thing_id, t.tp_name)
                    t.on_map = False
                    t.push(t.x, t.y)

                    tp = t.tp
                    if tp.is_player:
                        self.player = t

            mm.con("Loaded level @ {0}".format(str(self.level)))

    def destroy(self):
        self.level.destroy()

    def tick(self):
        self.move_count += 1
        mm.game_set_move_count(self.move_count)
        time_of_day.set_lighting(self,
                                 move=self.move_count,
                                 moves_per_day=self.moves_per_day)

        self.rain_amount = 0
        mm.game_set_rain_amount(0)

        self.snow_amount = 0
        mm.game_set_snow_amount(0)

        self.player_location_update()
        self.player_get_next_move()

    def player_location_update(self):

        level = self.level
        player = self.player

        if self.wid_map_summary:
            self.wid_map_summary.destroy()

        self.wid_map_summary = wid_popup.WidPopup(name="wid_map_summary",
                                                  width=1.0)
        w = self.wid_map_summary
        text = "Move %%fg=green${0}%%fg=reset$ ".format(self.move_count)
        text += "World %%fg=green${0},{1}%%fg=reset$ ".format(level.xyz.x,
                                                              level.xyz.y)
        if level.xyz.z > 0:
            text += "Depth %%fg=green${2} feet%%fg=reset ".format(level.xyz.z
                                                                  * 10)

        text += "@ %%fg=green${0},{1}%%fg=reset ".format(player.x, player.y)

        text += "Hour %%fg=green${0} Day {1}%%fg=reset ".format(self.hour_str,
                                                                self.day)
        w.add_text(font="vsmall", text=text)

        w.set_color(bg=True, tl=True, br=True, name="blue", alpha=0.2)
        w.update()
        w.move_to_bottom()

    #
    # The scrollable map for the level
    #
    def map_wid_create(self):
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
    # Mouse is over a map tile; show the route back to the player
    #
    def map_mouse_over(self, w, relx, rely, wheelx, wheely):

        level = self.level
        for x in range(0, level.width):
            for y in range(0, level.height):
                t = level.tp_find(x, y, "focus2")
                if t is not None:
                    t.set_tp("none")

                t = level.tp_find(x, y, "focus1")
                if t is not None:
                    t.set_tp("none")

        t = w.thing
        t.set_tp("focus2")

        #
        # Check we can get back from the chosen point to the player.
        #
        player = self.player
        nexthops = self.level.dmap_solve(self.player.x, self.player.y,
                                         t.x, t.y)
        if (player.x, player.y) in nexthops:
            for o in nexthops:
                (x, y) = o

                t = level.tp_find(x, y, "none")
                if t is not None:
                    t.set_tp("focus2")

    #
    # Move the player to the chosen tile
    #
    def map_mouse_down(self, w, x, y, button):

        level = self.level

        t = w.thing
        t.set_tp("focus1")

        #
        # Set up the player move chain
        #
        player = self.player
        nexthops = level.dmap_solve(self.player.x, self.player.y, t.x, t.y)

        if len(nexthops) < 2:
            return True

        #
        # Only if the destination is in a valid nexthops
        #
        if (player.x, player.y) in nexthops:
            player.nexthops = nexthops

            self.player_get_next_move()

        return True

    #
    # Player input
    #
    def map_key_down(self, w, sym, mod):

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
        level = self.level

        #
        # If in a dungeon place a trail of breadcrumbs
        #
        if level.is_biome_dungeon:
            t = level.tp_find(x, y, "ember1")
            if t is None:
                t = thing.Thing(self.level, tp_name="ember1")
                t.push(x, y)

        self.map_center_on_player(level_start=False)

    #
    # Create a random dungeon
    #
    def biome_create(self, seed, is_land=False, is_dungeon=False):

        self.level.set_biome(is_land=is_land,
                             is_dungeon=is_dungeon)

        if self.level.is_biome_dungeon:
            self.biome_build = biome_dungeon.biome_build
            self.biome_populate = biome_dungeon.biome_populate

        if self.level.is_biome_land:
            self.biome_build = biome_land.biome_build
            self.biome_populate = biome_land.biome_populate

        self.biome_build(self, level=self.level, seed=seed)
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

    try:
        g.load()
        g.was_loaded = True
    except Exception as inst:
        mm.con("Loading failed, init new game, error [{0}]".format(inst))
        g.map_wid_destroy()

        try:
            g = Game()
            g.was_loaded = False
            g.load_failed_init_new_game()
        except Exception as inst:
            mm.err("Failed to make a new level, error [{0}]".format(inst))

    try:
        g.post_load_init()
    except Exception as inst:
        mm.err("Failed to finalize level, error [{0}]".format(inst))
