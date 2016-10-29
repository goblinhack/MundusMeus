import world
import util
import mm
import wid_map
import thing
import wid_popup
import biome_dungeon

global g


class Game:

    def __init__(self):
        self.world = world.World(0)

        (self.width, self.height) = (mm.MAP_WIDTH, mm.MAP_HEIGHT)

        #
        # Create the world
        #
        p = util.Xyz(0, 0, 0)
        self.world.push_level(p)
        self.level = self.world.get_level()
        self.level.set_dim(self.width, self.height)

        self.map_wid_create()

        #
        # And not a dungeon at that point in the world
        #
        self.biome_create(is_dungeon=True, seed=7)

        self.map_center_on_player(level_start=True)
        self.map_center_on_player(level_start=False)

        self.wid_map_summary = None

    def destroy(self):
        self.world.destroy()

    def tick(self):
        self.player_tick()

    #
    # The scrollable map for the level
    #
    def map_wid_create(self):
        self.wid_map = wid_map.WidMap(mm.MAP_WIDTH, mm.MAP_HEIGHT)

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
    def map_mouse_over_tile(self, w, relx, rely, wheelx, wheely):

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

        p = self.player
        path = self.level.dmap_solve(self.player.x, self.player.y, t.x, t.y)
        if (p.x, p.y) in path:
            for o in path:
                (x, y) = o

                t = level.tp_find(x, y, "none")
                if t is not None:
                    t.set_tp("focus2")

        if self.wid_map_summary:
            self.wid_map_summary.destroy()

        self.wid_map_summary = wid_popup.WidPopup(name="wid_map_summary",
                                                  width=1.0)
        w = self.wid_map_summary
        w.add_text(font="vsmall",
                   text="some boring text {0} {1} x ".format(p.x, p.y))
        w.set_color(bg=True, tl=True, br=True, name="blue", alpha=0.2)
        w.update()
        w.move_to_bottom()

    #
    # Move the player to the chosen tile
    #
    def map_mouse_down_tile(self, w, x, y, button):

        level = self.level

        t = w.thing
        t.set_tp("focus1")

        #
        # Set up the player move chain
        #
        p = self.player
        path = level.dmap_solve(self.player.x, self.player.y, t.x, t.y)

        if len(path) < 2:
            return True

        #
        # Only if the destination is in a valid path
        #
        if (p.x, p.y) in path:
            p.path = path

            self.player_tick()

        return True

    def player_tick(self):

        p = self.player

        p.path.pop()
        if len(p.path) < 1:
            return True

        #
        # Move the player. [-1] is the player. [-2] is the adjacent cell.
        #
        x, y = p.path[-1]
        p.move(x, y)

        #
        # Place a light ember so the player can see where they've been
        #
        level = self.level

        t = level.tp_find(x, y, "ember1")
        if t is None:
            t = thing.Thing(self.level, tp_name="ember1")
            t.push(x, y)

        self.map_center_on_player(level_start=False)

    #
    # Create a rendom dungeon
    #
    def biome_create(self, seed, is_land=False, is_dungeon=False):

        self.level.set_biome(is_land=is_land,
                             is_dungeon=is_dungeon)

        for y in range(1, mm.MAP_HEIGHT - 1):
            for x in range(1, mm.MAP_WIDTH - 1):

                t = thing.Thing(self.level, tp_name="none")
                t.push(x, y)
                t.wid.game = self
                t.wid.set_on_m_over_b(game_mouse_over_tile)
                t.wid.set_on_m_down(game_mouse_down_tile)

        if self.level.is_biome_dungeon:
            self.biome_build = biome_dungeon.biome_build
            self.biome_populate = biome_dungeon.biome_populate

        self.biome_build(self, seed=seed)
        self.biome_populate(self)


def game_mouse_over_tile(w, relx, rely, wheelx, wheely):
    if w.game is None:
        return
    w.game.map_mouse_over_tile(w, relx, rely, wheelx, wheely)


def game_mouse_down_tile(w, x, y, button):
    if w.game is None:
        return False
    return w.game.map_mouse_down_tile(w, x, y, button)


g = None


def game_new():
    global g

    g = Game()
