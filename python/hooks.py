import mm
import game


def hook_new_game():
    mm.con("New game hook")
    game.game_new()

def hook_destroy_game():
    mm.con("Destroy game hook")

    if game.g is not None:
        game.g.destroy()
