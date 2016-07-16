import mm
import game


def hook_new_game():
    mm.con("New game hook")
    game.game_new()
