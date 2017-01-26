import util
import mm
import level
import wid_map
import thing
import wid_popup
import time_of_day
import pickle
import os.path
import wid_console
import game

global g


class Game:

    def __init__(self):

        #
        # Max thing ID in use in any level. This grows forever.
        #
        self.wid_map = None
        self.wid_player_location = None
        self.save_file = "save_file"
        self.player = None

        self.nexthops = None
        self.saved_nexthops = []
        self.level_stack = []
        self.last_level_seed = None
        wid_console.create()
        self.last_scroll_px = 0.5
        self.last_scroll_py = 0.5

    def new_game(self):

        self.sdl_delay = 2
        self.move_count = 0
        self.moves_per_day = 1000
        self.seed = 10
        self.where = util.Xyz(74*3, 70*3, -1)
        self.load_level(self.seed)

    def load_level(self, seed):

        self.map_wid_create()
        self.level = level.Level(xyz=self.where, seed=seed)
        mm.game_map_add_selection_buttons()

    def load_level_finalize(self):

        l = self.level
        mm.game_set_move_count(self.move_count)
        mm.game_set_moves_per_day(self.moves_per_day)
        mm.game_set_sdl_delay(self.sdl_delay)

        time_of_day.set_lighting(self,
                                 move=self.move_count,
                                 moves_per_day=self.moves_per_day)

        mm.biome_set_is_land(value=l.chunk[0][0].is_biome_land)
        mm.biome_set_is_dungeon(value=l.chunk[0][0].is_biome_dungeon)

        mm.game_map_fixup()

    def save(self):
        l = self.level
        s = os.path.normcase(
                os.path.join(os.environ["APPDATA"], self.save_file))

        with open(s, 'wb') as f:
            pickle.dump(self.seed, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.sdl_delay, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.where, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.move_count, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.moves_per_day, f, pickle.HIGHEST_PROTOCOL)
            pickle.dump(self.level_stack, f, pickle.HIGHEST_PROTOCOL)

            self.last_level_seed = l.seed
            pickle.dump(self.last_level_seed, f, pickle.HIGHEST_PROTOCOL)

            l.save(f)
            mm.con("Game saved @ chunk {0} to {1}".format(str(l), s))

    def load(self):

        s = os.path.normcase(os.path.join(os.environ["APPDATA"],
                                          self.save_file))
        with open(s, 'rb') as f:
            mm.log("Game loading from {0}".format(s))

            self.seed = pickle.load(f)
            self.sdl_delay = pickle.load(f)
            self.where = pickle.load(f)
            self.move_count = pickle.load(f)
            self.moves_per_day = pickle.load(f)
            self.level_stack = pickle.load(f)
            self.last_level_seed = pickle.load(f)

            self.load_level(self.last_level_seed)
            mm.con("Game loaded @ chunk {0} to {1}".format(str(self.level), s))

    def destroy(self):
        l = self.level
        l.destroy()

    #
    # Player has moved.
    #
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

    #
    # Player is pressing '.'
    #
    def time_waste(self):
        mm.game_map_time_step()

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

        text += "%%fg=white$Lat %%fg=green${0}%%fg=reset$ ".format(
                l.where.x)

        text += "%%fg=white$Long %%fg=green${0}%%fg=reset$ ".format(
                l.where.y)

        text += "%%fg=white$Move %%fg=green${0}%%fg=reset$ ".format(
                self.move_count)

        if l.where.z < 0:
            text += "Depth %%fg=green${0} feet%%fg=reset ".format(- l.where.z
                                                                  * 10)

#        player = self.player
#        text += "@ %%fg=green${0},{1}%%fg=reset ".format(player.x, player.y)
#        text += "offset %%fg=green${0},{1}%%fg=reset ".format(player.offset_x,
#                                                              player.offset_y)
#        text += "chunk %%fg=green${0}%%fg=reset ".format(player.chunk)
#
        w.add_text(font="vsmall", text=text)

        w.set_color(bg=True, tl=True, br=True, name="blue", alpha=0.2)
        w.update()
        w.move_to_bottom()
        w.set_movable(value=False)

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

    #
    # Get rid of the path indicators where the player will move
    #
    def map_clear_focus(self):
        mm.game_map_clear_selection_buttons()

    #
    # Mouse is over a map tile; show the route back to the player
    #
    def map_mouse_over(self, w, x, y, wheelx, wheely):

        #
        # Want to scroll without the focus moving
        #
        if wheelx != 0 or wheely != 0:
            return False

        l = self.level

        self.map_clear_focus()

        mm.game_map_set_selection_buttons(x, y, "focus1")

        #
        # Check we can get back from the chosen point to the player.
        #
        player = self.player
        nexthops = l.dmap_solve(self.player.x, self.player.y, x, y)
        self.saved_nexthops = nexthops
        self.saved_nexthops.append((x, y))

        if (player.x, player.y) in nexthops:
            for o in nexthops:
                (x, y) = o

                mm.game_map_set_selection_buttons(x, y, "focus2")

    #
    # Move the player to the chosen tile
    #
    def map_mouse_down(self, w, x, y, button):

        l = self.level

        #
        # Set up the player move chain
        #
        player = self.player
        nexthops = l.dmap_solve(self.player.x, self.player.y, x, y)

        if len(nexthops) < 2:
            nexthops.append((x, y))

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

        self.map_clear_focus()

        if sym == mm.SDLK_PERIOD:
            self.tick()
            self.time_waste()
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
        if l.chunk[0][0].is_biome_dungeon:
            t = l.tp_find(x, y, "ember1")
            if t is None:
                t = thing.Thing(level=l, tp_name="ember1", x=x, y=y)
                t.push()

        level_dx = 0
        level_dy = 0
        level_change = False

        if x <= mm.CHUNK_WIDTH - 1:
            level_dx = -1
            level_change = True

        elif x >= mm.CHUNK_WIDTH * (mm.CHUNK_ACROSS - 1):
            level_dx = 1
            level_change = True

        if y <= mm.CHUNK_HEIGHT - 1:
            level_dy = -1
            level_change = True

        elif y >= mm.CHUNK_HEIGHT * (mm.CHUNK_DOWN - 1):
            level_dy = 1
            level_change = True

        if l.tp_is(x, y, "is_dungeon"):
            xyz = l.where
            xyz.z -= 1

            new_level_seed = l.seed
            new_level_seed *= xyz.y * mm.WORLD_WIDTH
            new_level_seed *= xyz.x
            new_level_seed *= self.player.x
            new_level_seed += self.player.y

            l.jump(xyz, seed=new_level_seed, backtracking=False)

        if l.tp_is(x, y, "is_dungeon_way_down"):
            xyz = l.where
            xyz.z -= 1

            l.jump(xyz, seed=l.seed, backtracking=False)

        if l.tp_is(x, y, "is_dungeon_way_up"):
            xyz = l.where
            xyz.z += 1

            if xyz.z == 0:
                l.jump(xyz, seed=game.g.seed, backtracking=True)
            else:
                l.jump(xyz, seed=l.seed, backtracking=True)

        if level_change:
            l.scroll(level_dx, level_dy)


def map_mouse_over(w, relx, rely, wheelx, wheely):
    g.map_mouse_over(w, relx, rely, wheelx, wheely)


def map_mouse_down(w, x, y, button):
    return g.map_mouse_down(w, x, y, button)


def map_key_down(w, sym, mod):
    return g.map_key_down(w, sym, mod)


g = None


def game_new():
    global g

    game_dir = os.path.join(os.environ["APPDATA"], "mundusmeus")

    mm.log("Appdata dir is " + game_dir)

    g = Game()
    s = os.path.normcase(os.path.join(os.environ["APPDATA"], g.save_file))
    if os.path.isfile(s):
        g.load()
    else:
        g.new_game()

    g.load_level_finalize()
    g.level.tick()
    g.player_location_update()
