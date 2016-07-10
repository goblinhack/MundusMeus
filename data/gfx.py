import mm;

def init_tex ():

    mm.tex_load(file="data/gfx/sword.tga",      name="sword");
    mm.tex_load(file="data/gfx/title.tga",      name="main_title");
    mm.tex_load(file="data/gfx/map.tga",        name="map");

def init_tiles_hex ():

    mm.tex_load_tiled(
            file="data/gfx/hex.tga", 
            name="hex", 
            width=64, 
            height=64);

    hex_tiles = [
        "grass1.1",
        "forest1.1",
        "forest1.2",
        "forest1.3",
        "forest1.4",
        "forest1.5",
        "forest1.6",
        "forest2.1",
        "forest2.2",
        "forest2.3",
        "forest2.4",
        "forest2.5",
        "forest2.6",
        "grass1.2",
        "grass1.3",
        "grass1.4",
        "grass1.5",
        "grass1.6",
        "grass1.7",
        "grass1.8",
        "grass1.9",
        "grass1.10",
        "grass1.11",
        "grass1.12",
        "grass1.13",
        "grass1.14",
        "grass1.15",
        "grass1.16",
        "grass1.17",
        "grass1.18",
        "grass1.19",
        "grass1.20",
        #
    ];

    mm.tile_load_arr(
            tex_name = "hex",
            tex_name_black_and_white = "",
            width = 64, height = 64,
            arr = hex_tiles);

def init_tiles_chars ():

    mm.tex_load_tiled(
            file="data/gfx/chars.tga", 
            name="chars", 
            width=16, 
            height=16);

    chars_tiles = [
        "player1",
        "player2",
        "player3",
        "player4",
        "player5",
        "player6",
        "player7",
        "player8",
        "player9",
        "player10",
        "player11",
        "player12",
        "player13",
        "player14",
        "player15",
        "player16",
        "player17",
        "player18",
        "player19",
        "player20",
        "player21",
        "player22",
        "player23",
        "player24",
        "player25",
        "player26",
        "player27",
        "player28",
        "player29",
        "player30",
        "player31",
        "player32",
        "player33",
        "player34",
        "player35",
        "player36",
        "player37",
        "player38",
        "player39",
        "player40",
        "player41",
        "player42",
        "player43",
        "player44",
        "player45",
        "player46",
        "player47",
        "player48",
        "player49",
        "player50",
        "player51",
        "player52",
        "player53",
        "player54",
        "player55",
        "player56",
        "player57",
        "player58",
        "player59",
        "player60",
        "player61",
        "player62",
        "player63",
        "player64",
    ];

    mm.tile_load_arr(
            tex_name = "chars",
            tex_name_black_and_white = "",
            width = 16, height = 16,
            arr = chars_tiles);

init_tex();
init_tiles_hex();
init_tiles_chars();
