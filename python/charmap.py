
SPACE = " "
CORRIDOR = "#"
DOOR = "D"
WALL = "x"
CWALL = "X"
FLOOR = "."
DUSTY = "\""
START = "S"
EXIT = "E"
KEY = "k"
CHASM = "C"
LAVA = "L"
WATER = "_"
ROCK = "r"
TREASURE = "$"
OBJECT = "o"

charmap = {
    " ": {
        "bg": "black",
        "fg": "black",
    },
    WALL: {
        "bg": "magenta",
        "fg": "black",
        "is_wall": True,
        "is_movement_blocking": True,
    },
    CWALL: {
        "bg": "blue",
        "fg": "black",
        "is_cwall": True,
    },
    FLOOR: {
        "bg": "black",
        "fg": "white",
        "is_floor": True,
    },
    DUSTY: {
        "bg": "black",
        "fg": "white",
        "is_dusty": True,
    },
    CORRIDOR: {
        "bg": "black",
        "fg": "yellow",
        "is_corridor": True,
    },
    DOOR: {
        "bg": "green",
        "fg": "green",
        "is_door": True,
        "is_movement_blocking": True,
    },
    START: {
        "bg": "white",
        "fg": "red",
        "is_start": True,
        "is_movement_blocking": True,
    },
    EXIT: {
        "bg": "white",
        "fg": "red",
        "is_exit": True,
        "is_movement_blocking": True,
    },
    KEY: {
        "bg": "white",
        "fg": "yellow",
        "is_key": True,
        "is_movement_blocking": True,
    },
    CHASM: {
        "bg": "black",
        "fg": "black",
        "is_chasm": True,
        "is_dissolves_walls": True,
    },
    LAVA: {
        "bg": "red",
        "fg": "yellow",
        "is_lava": True,
        "is_dissolves_walls": True,
    },
    WATER: {
        "bg": "black",
        "fg": "blue",
        "is_water": True,
        "is_dissolves_walls": True,
    },
    ROCK: {
        "bg": "black",
        "fg": "red",
        "is_rock": True,
    },
    OBJECT: {
        "bg": "black",
        "fg": "yellow",
        "is_obj": True,
        "is_movement_blocking": True,
    },
    TREASURE: {
        "bg": "black",
        "fg": "yellow",
        "is_treasure": True,
    },
}


class Enumeration(object):
    def __init__(self, names):  # or *names, with no .split()
        self.to_name = {}
        for number, name in enumerate(names.split()):
            setattr(self, name, number)
            self.to_name[number] = name

depth = Enumeration("under floor wall obj max")
